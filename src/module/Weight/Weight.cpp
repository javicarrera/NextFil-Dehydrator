/*!
 * Bascula.h - Funciones para el uso con la pantalla Nextion
 * 2021 Javier Carrera Arias  
 * All rights reserved under the library's licence
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 */

/*! Archivo separado < pageRefresh.cpp > para las funciones de RefreshCurrentPage(), 
 *  para no hacer demasiado largo el EasyNextionLibrary.cpp. Hay tantas funciones como
 *  número de páginas en el programa de Nextion. Así es más sencillo para editar.
 */
#ifndef Weight_h
  #include "Weight.h"
#endif

// Variables importadas de serial_debug.h
static char inputchar[64] = {};           // para recibir del serial

//***********************************************************
//                       LEER HX711
//***********************************************************
//
// Utilizado como referencia:
// https://www.analog.com/en/analog-dialogue/articles/a-reference-design-for-weigh-scales.html
// https://www.analog.com/media/en/analog-dialogue/volume-39/number-4/articles/a-reference-design-for-weigh-scales.pdf
//
// Se determinan cuantas lecturas se ponderan (M), se crean 2 arrays A[M] y B[M-2]
// Se sigue el siguiente algoritmo:
//
// NUEVA LECTURA DEL ADC -> A[DATA1 - DATA 2 ... DATA M]  (ARRAY DINAMICO DE LECTURAS)
//
// Las lecturas son continuas hasta que no se llene el array por primera vez.
// Despues cada nueva lectura se guarda en la ulima posicion del array A[M], todos lo valores existentes se corren
// una posición hacia la izquierda. El valor mas antiguo se elimina. La ventaja es que salvo en la lectura inicial, las siguientes
// lecturas son prácticamente en tiempo real (respetando los tiempos del sensor), ya que sólo se añade una lectura más a la media y no M lecturas de nuevo.
//
// ALGORITMO PARA INCREMENTAR EL TIEMPO DE RESPUESTA A CAMBIOS DE PESO (SE APLICA A CADA NUEVA LECTURA DEL ADC)
// -------------------------------
// Nueva lectura ADC -> Diferencia entre la nueva   -SI-> Diferencia entre lalectura    -SI->   Reemplazar todas las lecturas
//                      lectura y la media en el          anterior, con la                      del filtro por 6 nuevas lecturas
//                      ciclo es mayor que el umbral      media es mayor que el umbral          |
//                      |                                 |                                     variable: restartEMA=true
//                      new_dif_EMA= PESO1-filteredValue  last_dif_EMA= ult_PESO1-filteredValue
//                      var: new_dif_EMA > umbral_EMA?    var: last_dif_EMA > umbral_EMA?
//                      |                                 |
//                      |                                 |
//                      |                                 |
//                      |NO                               |NO
//                      |-> Añadir lectura a la media     |-> Ignorar lectura, ruido
//
//
// ALGORITMO DE PONDERACIÓN DINAMICA:
// -------------------------------
// BORRAR VALORES MIN Y MAX DEL ARRAY A = B[DATA1 - DATA 2 ... DATA M-2] --> MEDIA --> VALOR DE SALIDA
//
// ALGORITMO PARA ELIMINAR PARPADEOS ENTRE DOS LECTURAS/DIGITOS CONTIGUOS:
// -------------------------------
// NEW_DISPLAY = OLD DISPLAY?  -NO->  |NEW CODE-OLD CODE|>UMBRAL -SI-> OLD CODE=NEW CODE
//          |YES                                        |NO            OLD_DISP=NEW_DISP
//          |                                           |                     |
//          |                                           V                     V
//          |---> OLD CODE = NEW CODE   -----------------------------  DISPLAY OLD DISP

//***********************************************************
//                     LEER BASCULA 1
//***********************************************************
void leerBascula1()
{
  bool negative = false;                                  // variable que determina si la lectura es negativa
  float output_weight1 = 0;                               // salida en float
  update_output1 = false;                                 // por defecto no se actualiza la lectura
  if (micros() - prevMicros1 >= WeightReadingRefreshTime) // temporizador
  {
    static EMA<EMA_K> filter; // Iniciamos <EMA_K> es la variable K, que regula la agresividad de la EMA. A static variable remains in memory while the program is running

    //----------------------------------------------------------------
    // HX711 CONECTADO?
    //----------------------------------------------------------------
    if (bascula1.wait_ready_retry(10)) // comprueba que este conectado el HX711
    {
      bascula1_conectada = true;
      for (int i = 0; i < AVG_FACT; i++) //hacemos AVG_FACT lecturas.
      {
        // NUEVA LECTURA HX711:
        //--------------------------------- -------------------------------
        // Nueva lectura ADC -> 1 lecturas
        // la EMA no permite numeros negativos, asi que lo añadimos al final
        // multiplicamos por 10 para obtener 1 decimal en tipo integer
        // convertimos el float a integer sin valores negativos, (incluido 1 decimal)
        //----------------------------------------------------------------
        PESO1_f = bascula1.get_units(1) * 10;
        if (PESO1_f < 0)
          negative = true;
        PESO1 = abs((int)PESO1_f);
        new_dif_EMA1 = PESO1 - filteredValue1;

        // LECTURA INICIAL HX711
        //----------------------------------------------------------------
        // 2ª LECTURA - INICIO - En el paso anterior hemos realizado la 1ª lectura, así que iniciamos el proceso con la una 2ª lectura
        // El 1er valor PESO1 se guarda como ultimo para el próximo ciclo (ult_PESO1), se toma la 2ª lectura:
        // Nueva lectura ADC -> 1 lecturas, se define de nuevo PESO1, ahora ya podemos empezar el proceso.
        // multiplicamos por 10 para obtener 1 decimal en tipo integer y convertimos a int sin valores negativos, (includo 1 decimal)
        //----------------------------------------------------------------
        if (ult_PESO1 == 0)
        {
          ult_PESO1 = PESO1;
          last_dif_EMA1 = 0;
          new_dif_EMA1 = 0;
          PESO1_f = bascula1.get_units(1) * 10;
          PESO1 = abs((int)PESO1_f);
        }

        // FILTRO EMA - MEDIA MOVIL EXPONENCIAL -filter()-
        // ALGORITMO PARA INCREMENTAR EL TIEMPO DE RESPUESTA A CAMBIOS DE PESO (SE APLICA A CADA NUEVA LECTURA DEL ADC)
        //----------------------------------------------------------------

        if (abs(new_dif_EMA1) > (umbral_EMA))     //1º Diferencia entre la nueva lectura y la media en el ciclo es mayor que el umbral definido: umbral_EMA
        {                                         //1º SI
          if (abs(last_dif_EMA1) > (umbral_EMA))  //2º Diferencia entre la nueva lectura y la ultima lectura con el media, es mayor que el umbral definido: umbral_EMA
            filteredValue1 = filter(true, PESO1); //2ª SI Reemplazar todas las lecturas del filtro - REINICIAR EMA
        }                                         //2º NO. Ignorar lectura, ruido.
        else
          filteredValue1 = filter(false, PESO1);  //1º NO: Añadir lectura a la media movil exponencial
        last_dif_EMA1 = new_dif_EMA1;

        //----------------------------------------------------------------
        // ALGORITMO PARA ELIMINAR PARPADEOS ENTRE DOS LECTURAS/DIGITOS CONTIGUOS:
        //----------------------------------------------------------------
        // Compara la ultima lectura sacada a pantalla y la actual
        // Si la diferencia entre la lectura actual y la anterior es mayor que el umbral definido: umbral_EMA
        // SI, es mayor, cambiar el valor de la pantalla
        //   Utilizamos una variable para la pantalla porque se puede ir corriendo la lectura despacio y nunca llega al
        //   umbral aunque lo haya sobrepasado.
        // El nuevo valor se guarda como ultimo para el próximo ciclo
        // convertimos a float con 1 decimal
        //----------------------------------------------------------------
        new_dif_last1 = filteredValue1 - ult_PESO1_DISPLAY; // ultima salida del filtro y la ultima lectura en pantalla
        new_dif_last1 = abs(new_dif_last1); 
       
        if (new_dif_last1 > umbral_display)
        {
          update_output1 = true;
          if (i==AVG_FACT-1) ult_PESO1_DISPLAY = filteredValue1; //solo sacamos el utimo valor a display
        }
        ult_PESO1 = filteredValue1;
        output_weight1 = (float)filteredValue1 / 10;


        //----------------------------------------------------------------
        // TRATAMOS LA SALIDA (peso negativo o cercano a 0):
        //----------------------------------------------------------------

        //Convertimos a lectura negativa si es preciso
        if (negative)
          output_weight1 = -output_weight1;

        //Si estamos en los limites del umbral alrededor de la tara, hacemos que muestre 0
        int16_t b = 0 - ult_PESO1;
        b = abs(b);
        if (b <= umbral_display)
          output_weight1 = 0;
      } // for AVG_FACT lecturas.
    } // if bascula1_conectada
    else 
    //----------------------------------------------------------------
    // SENSOR NO CONECTADO:
    //----------------------------------------------------------------
    {
      if (SERIAL_DEBUG)
        Serial.println(F("ERROR:Sensor HX711(1) no conectado"));
      //TODO señal a Nextion de error
      bascula1_conectada = false;
      return;
    } //else - sensor NO conectado 

    //----------------------------------------------------------------
    // SALIDA:
    //----------------------------------------------------------------
    if (bascula1_conectada  && update_output1)
    {
      if (SERIAL_DEBUG){
      Serial.print(F("PESO FINAL1: "));
      Serial.println(output_weight1, 1); //VALOR FINAL PARA NEXTION!!
      }
    }
    prevMicros1 += WeightReadingRefreshTime; // correr la marca temporal del temporizador
    //TODO enviar datos a Nextion
  } //temporizador

}

//***********************************************************
//                     LEER BASCULA 2
//***********************************************************
void leerBascula2()
{
  bool negative = false;                                  // variable que determina si la lectura es negativa
  float output_weight2 = 0;                               // salida en float
  update_output2 = false;                                 // por defecto no se actualiza la lectura
  if (micros() - prevMicros2 >= WeightReadingRefreshTime) // temporizador
  {
    static EMA<EMA_K> filter; // Iniciamos <EMA_K> es la variable K, que regula la agresividad de la EMA. A static variable remains in memory while the program is running

    //----------------------------------------------------------------
    // HX711 CONECTADO?
    //----------------------------------------------------------------
    if (bascula2.wait_ready_retry(10)) // comprueba que este conectado el HX711
    {
      bascula2_conectada = true;
      for (int i = 0; i < AVG_FACT; i++) //hacemos AVG_FACT lecturas.
      {
        // NUEVA LECTURA HX711:
        //--------------------------------- -------------------------------
        // Nueva lectura ADC -> 1 lecturas
        // la EMA no permite numeros negativos, asi que lo añadimos al final
        // multiplicamos por 10 para obtener 1 decimal en tipo integer
        // convertimos el float a integer sin valores negativos, (incluido 1 decimal)
        //----------------------------------------------------------------
        PESO2_f = bascula2.get_units(1) * 10;
        if (PESO2_f < 0)
          negative = true;
        PESO2 = abs((int)PESO2_f);
        new_dif_EMA2 = PESO2 - filteredValue2;

        // LECTURA INICIAL HX711
        //----------------------------------------------------------------
        // 2ª LECTURA - INICIO - En el paso anterior hemos realizado la 1ª lectura, así que iniciamos el proceso con la una 2ª lectura
        // El 1er valor PESO1 se guarda como ultimo para el próximo ciclo (ult_PESO1), se toma la 2ª lectura:
        // Nueva lectura ADC -> 1 lecturas, se define de nuevo PESO1, ahora ya podemos empezar el proceso.
        // multiplicamos por 10 para obtener 1 decimal en tipo integer y convertimos a int sin valores negativos, (includo 1 decimal)
        //----------------------------------------------------------------
        if (ult_PESO2 == 0)
        {
          ult_PESO2 = PESO2;
          last_dif_EMA2 = 0;
          new_dif_EMA2 = 0;
          PESO2_f = bascula2.get_units(1) * 10;
          PESO2 = abs((int)PESO2_f);
        }

        // FILTRO EMA - MEDIA MOVIL EXPONENCIAL -filter()-
        // ALGORITMO PARA INCREMENTAR EL TIEMPO DE RESPUESTA A CAMBIOS DE PESO (SE APLICA A CADA NUEVA LECTURA DEL ADC)
        //----------------------------------------------------------------

        if (abs(new_dif_EMA2) > (umbral_EMA))      //1º Diferencia entre la nueva lectura y la media en el ciclo es mayor que el umbral definido: umbral_EMA
        {                                         //1º SI
          if (abs(last_dif_EMA2) > (umbral_EMA))   //2º Diferencia entre la nueva lectura y la ultima lectura con el media, es mayor que el umbral definido: umbral_EMA
            filteredValue2 = filter(true, PESO2); //2ª SI Reemplazar todas las lecturas del filtro - REINICIAR EMA
        }                                         //2º NO. Ignorar lectura, ruido.
        else
          filteredValue2 = filter(false, PESO2); //1º NO: Añadir lectura a la media movil exponencial
        last_dif_EMA2 = new_dif_EMA2;

        //----------------------------------------------------------------
        // ALGORITMO PARA ELIMINAR PARPADEOS ENTRE DOS LECTURAS/DIGITOS CONTIGUOS:
        //----------------------------------------------------------------
        // Compara la ultima lectura sacada a pantalla y la actual
        // Si la diferencia entre la lectura actual y la anterior es mayor que el umbral definido: umbral_EMA
        // SI, es mayor, cambiar el valor de la pantalla
        //   Utilizamos una variable para la pantalla porque se puede ir corriendo la lectura despacio y nunca llega al
        //   umbral aunque lo haya sobrepasado.
        // El nuevo valor se guarda como ultimo para el próximo ciclo
        // convertimos a float con 1 decimal
        //----------------------------------------------------------------
        new_dif_last2 = filteredValue2 - ult_PESO2_DISPLAY; // ultima salida del filtro y la ultima lectura en pantalla
        new_dif_last2 = abs(new_dif_last2); 
       
        if (new_dif_last2 > umbral_display)
        {
          update_output2 = true;
          if (i==AVG_FACT-1) ult_PESO2_DISPLAY = filteredValue2; //solo sacamos el utimo valor a display
        }
        ult_PESO2 = filteredValue2;
        output_weight2 = (float)filteredValue2 / 10;

        //----------------------------------------------------------------
        // TRATAMOS LA SALIDA (peso negativo o cercano a 0):
        //----------------------------------------------------------------

        //Convertimos a lectura negativa si es preciso
        if (negative)
          output_weight2 = -output_weight2;

        //Si estamos en los limites del umbral alrededor de la tara, hacemos que muestre 0
        int16_t b = 0 - ult_PESO2;
        b = abs(b);
        if (b <= umbral_display)
          output_weight2 = 0;
      } // for AVG_FACT lecturas.
    } // if bascula2_conectada
    else
    //----------------------------------------------------------------
    // SENSOR NO CONECTADO:
    //----------------------------------------------------------------
    {
      if (SERIAL_DEBUG)
        Serial.println(F("ERROR:Sensor HX711(2) no conectado"));
      //TODO señal a Nextion de error
      bascula2_conectada = false;
      return;
    } //else - sensor NO conectado 

    //----------------------------------------------------------------
    // SALIDA:
    //----------------------------------------------------------------
    if (bascula2_conectada  && update_output2)
    {
      if (SERIAL_DEBUG){
      Serial.print(F("PESO FINAL2: "));
      Serial.println(output_weight2, 1); //VALOR FINAL PARA NEXTION!!
      }
    }
    prevMicros2 += WeightReadingRefreshTime; // correr la marca temporal del temporizador
    //TODO enviar datos a Nextion
  } //temporizador
}

//***********************************************************
//                      CAMBIO DE TARA
//***********************************************************

// Se controla con las variables tara1_cambio y tara2_cambio
//TODO cambiar estas variables con la Nextion

void cambioTara()
{
  if (tara1_cambio)
  {
    //TODO  tara1_actual_g = ; //en gramos, LEER DE  nextion
    tara1_actual_offset = tara1_actual_g * calibration_factor1;
    bascula1.set_offset(zero_offset1 + tara1_actual_offset); //Zero out the scale using a previously known zero_factor
    tara1_cambio = false;
    //TODO enviar datos a Nextion
  }
  /*if (tara2_cambio)
  {
    //TODO tara2_actual_g = ; //en gramosLEER DE  nextion
    tara2_actual_offset = tara2_actual_g * calibration_factor2;
    bascula2.set_offset(zero_offset2 + tara2_actual_offset); //Zero out the scale using a previously known zero_factor
    tara2_cambio = false;
    //TODO enviar datos a Nextion
  }
  */
  if (SERIAL_DEBUG)
  {
    Serial.println(F("CAMBIO DE TARA: "));
    Serial.print(F("zero_offset1: "));
    Serial.print(zero_offset1);
    Serial.print(F(" tara1_actual_g: "));
    Serial.print(tara1_actual_g);
    Serial.print(F(" tara1_actual_offset: "));
    Serial.print(tara1_actual_offset);
    Serial.print(F(" bascula get offset: "));
    Serial.print(bascula1.get_offset());
    Serial.print(F(" calibration_factor1: "));
    Serial.println(bascula1.get_scale());
  }
}


//***************************************************
//Calibración colocando un peso conocido - OK PROBADO
//                 CALIBRAR_PESO 1
//***************************************************
void calibrar_peso1()
{
  //TODO PASAR ESTA FUNCIONALIDAD A NEXTION
  if (!bascula1_conectada)
  {
    if (SERIAL_DEBUG)
      Serial.println("calibrar peso _ Sensor HX711 (1) no conectado");
    return;
  }
  uint16_t inputWeight = 0;
  if (SERIAL_DEBUG)
  { //DESCOMENTAR
    Serial.println(F("\nQuita cualquier peso de la báscula, pulsa una tecla para continuar \n"));
    serialEvent();
  }
  bascula1.tare(AVG_FACT);
  zero_offset1 = bascula1.get_offset(); //despues de tarar, tenemos un nuevo zero offset
  if (SERIAL_DEBUG)
  {
    Serial.println(F("TARA: "));
    Serial.print(bascula1.get_units(AVG_FACT));
    Serial.print(F(" g"));
    Serial.print(F("¿Peso 1 calibrado? (g) "));
    serialEvent();
    inputWeight = atoi(inputchar); //convierte char -> int
    Serial.print(F("\nPon el peso de "));
    Serial.print(inputWeight);
    Serial.print(F(" g en la báscula, pulsa una tecla para continuar \n"));
    serialEvent();
  }

  bascula1.callibrate_scale(inputWeight, AVG_FACT);
  calibration_factor1 = bascula1.get_scale();

  tara1_cambio = true; //Para actualizar calibration_factor
  cambioTara();        //Para actualizar calibration_factor
  bascula1_calibrada = true;

  //falta escribir en EEPROM calibration_factor
  if (SERIAL_DEBUG)
  {
    Serial.print(F("PESO CALIBRADO: "));
    Serial.print(bascula1.get_units(AVG_FACT),2); //2 decimales
    Serial.println(F(" g"));
  }
}


//***********************************************************
//     Calibración cambiando el factor de calibración
//                  CALIBRAR_FACTOR 1
//***********************************************************

//TODO cambiar serial output a nextion!!
//TODO PASAR ESTA FUNCIONALIDAD A NEXTION

void calibrar_factor1()
{
  if (!bascula1_conectada)
  {
    if (SERIAL_DEBUG)
      Serial.println(F("ERROR:Sensor HX711(1) no conectado"));
    //TODO señal a Nextion de error
    return;
  }

  if (SERIAL_DEBUG)
  {
    Serial.println(F("CALIBRACIÓN 1"));
    Serial.println(F("Retirar peso de la báscula, presiona cualquier tecla"));
    serialEvent();
  }

  bascula1.tare(AVG_FACT);
  zero_offset1 = bascula1.read_average(); //Get a baseline reading

  if (SERIAL_DEBUG)
  {
    Serial.print(F("Factor de tara1: "));
    Serial.println(zero_offset1);
    Serial.println(F("+ y ENTER para factor de calibración +100"));
    Serial.println(F("- y ENTER para factor de calibración -100"));
    Serial.println(F("o numero con decimales y ENTER"));
  }

  bascula1.tare(AVG_FACT); //AVG_FACT determina cuantas lecturas se van a ponderar
  //solo necesitamos 1 caracter, ingoramos el resto
  while (inputchar[0] != '*')
  {
    bascula1.set_scale(calibration_factor1); // Ajustar el factor de calibracion
    //delay(300);
    if (SERIAL_DEBUG)
    {
      Serial.print(bascula1.get_units(AVG_FACT), 1); //1 posicion decimal
      Serial.print(F(" g"));
      Serial.print(F("\t calibration_factor1: "));
      Serial.print(calibration_factor1);
      Serial.print(F("\t zero_offset1: "));
      Serial.println(zero_offset1);
    }
    serialEvent();              //detiene la ejecucion hasta que no pulsemos ENTER
    //inputchar[0] = Serial.read(); //Serial.read no para la ejecución, pero solo nos da tiempo a 1 caracter.
    float f = atof(inputchar); //convierte char -> float
    Serial.println(f);
    if (inputchar[0] == '+')
      calibration_factor1 += 100;
    else if (inputchar[0] == '-' && inputchar[1] == 0 && inputchar[2] != '.')  // contemplamos que es posible -0.xxxx
      calibration_factor1 -= 100;
    else
      calibration_factor1 += f;
  }
}


//***************************************************
//Calibración colocando un peso conocido - OK PROBADO
//                 CALIBRAR_PESO 2
//***************************************************
void calibrar_peso2()
{
  //TODO PASAR ESTA FUNCIONALIDAD A NEXTION
  if (!bascula2_conectada)
  {
    if (SERIAL_DEBUG)
      Serial.println("calibrar peso _ Sensor HX711 (2) no conectado");
    return;
  }
  uint16_t inputWeight = 0;
  if (SERIAL_DEBUG)
  { //DESCOMENTAR
    Serial.println(F("\nQuita cualquier peso de la báscula 2, pulsa una tecla para continuar \n"));
    serialEvent();
  }
  bascula2.tare(AVG_FACT);
  zero_offset2 = bascula2.get_offset(); //despues de tarar, tenemos un nuevo zero offset
  if (SERIAL_DEBUG)
  {
    Serial.println(F("TARA: "));
    Serial.print(bascula2.get_units(AVG_FACT));
    Serial.print(F(" g"));
    Serial.print(F("¿Peso 2 calibrado? (g) "));
    serialEvent();
    inputWeight = atoi(inputchar); //convierte char -> int
    Serial.print(F("\nPon el peso de "));
    Serial.print(inputWeight);
    Serial.print(F(" g en la báscula 2, pulsa una tecla para continuar \n"));
    serialEvent();
  }

  bascula2.callibrate_scale(inputWeight, AVG_FACT);
  calibration_factor2 = bascula2.get_scale();

  tara2_cambio = true; //Para actualizar calibration_factor
  cambioTara();        //Para actualizar calibration_factor
  bascula2_calibrada = true;

  //falta escribir en EEPROM calibration_factor
  if (SERIAL_DEBUG)
  {
    Serial.print(F("PESO CALIBRADO: "));
    Serial.print(bascula2.get_units(AVG_FACT),2); //2 decimales
    Serial.println(F(" g"));
  }
}


//***********************************************************
//     Calibración cambiando el factor de calibración
//                  CALIBRAR_FACTOR 2
//***********************************************************

//TODO cambiar serial output a nextion!!
//TODO PASAR ESTA FUNCIONALIDAD A NEXTION

void calibrar_factor2()
{
  if (!bascula2_conectada)
  {
    if (SERIAL_DEBUG)
      Serial.println(F("ERROR:Sensor HX711(2) no conectado"));
    //TODO señal a Nextion de error
    return;
  }

  if (SERIAL_DEBUG)
  {
    Serial.println(F("CALIBRACIÓN 2"));
    Serial.println(F("Retirar peso de la báscula, presiona cualquier tecla"));
    serialEvent();
  }

  bascula2.tare(AVG_FACT);
  zero_offset2 = bascula2.read_average(); //Get a baseline reading

  if (SERIAL_DEBUG)
  {
    Serial.print(F("Factor de tara2: "));
    Serial.println(zero_offset2);
    Serial.println(F("+ y ENTER para factor de calibración +100"));
    Serial.println(F("- y ENTER para factor de calibración -100"));
    Serial.println(F("o numero con decimales y ENTER"));
  }

  bascula2.tare(AVG_FACT); //AVG_FACT determina cuantas lecturas se van a ponderar
  //solo necesitamos 1 caracter, ingoramos el resto
  while (inputchar[0] != '*')
  {
    bascula2.set_scale(calibration_factor2); // Ajustar el factor de calibracion
    //delay(300);
    if (SERIAL_DEBUG)
    {
      Serial.print(bascula2.get_units(AVG_FACT), 1); //1 posicion decimal
      Serial.print(F(" g"));
      Serial.print(F("\t calibration_factor2: "));
      Serial.print(calibration_factor2);
      Serial.print(F("\t zero_offset2: "));
      Serial.println(zero_offset2);
    }
    serialEvent();              //detiene la ejecucion hasta que no pulsemos ENTER
    //inputchar[0] = Serial.read(); //Serial.read no para la ejecución, pero solo nos da tiempo a 1 caracter.
    float f = atof(inputchar); //convierte char -> float
    Serial.println(f);
    if (inputchar[0] == '+')
      calibration_factor2 += 100;
    else if (inputchar[0] == '-' && inputchar[1] == 0 && inputchar[2] != '.')  // contemplamos que es posible -0.xxxx
      calibration_factor2 -= 100;
    else
      calibration_factor2 += f;
  }
}
