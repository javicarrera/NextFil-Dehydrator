#include <Arduino.h>
#include "HX711.h" //Para las básculas

#define PESO_DATA2_PIN 8
#define PESO_SCK2_PIN 9  //pwm capable
#define PESO_SCK_PIN 10  //pwm capable, SPI-SS
#define PESO_DATA_PIN 11 //pwm capable, SPI-MOSI

// BASCULAS Y PESOS
// ------------------------------------------------

// Variables comunes modificables
float WeightReadingRefreshTime = 1.5; // (s)entre lecturas de peso y temp. exterior (min. aprox 1s) si da error de sensor no conectado aumentar!
const uint8_t umbral_display = 5;   // en decigramos el umbral para la salida por pantalla 5=0.5g,.
const uint8_t umbral_EMA = 1;       // en decigramos el umbral para la media dinamica 1=0.1g,.
const int EMA_K = 2;            // variable K de la media dinamica exponencial regula la agresividad del filtro.K=2 se ajusta mejor al cambio
const uint8_t AVG_FACT = 10;        // cuantas lecturas se hacen y mandan al EMA en un ciclo 
 
// Bascula 1
// ------------------------------------------------

// Variables modificables:
int32_t zero_offset1 = -251726;
float calibration_factor1 = 270.21;

int32_t new_dif_EMA1 = 0;    // Diferencia entre la nueva lectura y la media del EMA
int32_t last_dif_EMA1 = 0;   // Diferencia entre la ultima lectura y la media del
int32_t new_dif_last1 = 0;   // Diferencia entre la anterior lectura y la actual
int32_t tara1_actual_g = 0;
int32_t tara1_actual_offset = 0;
static uint32_t prevMicros1 = 0;      // tiempo de la ultima lectura - en us
volatile float PESO1_f = 0.00f;
volatile int32_t PESO1 = 0;
volatile int32_t ult_PESO1 = 0;
volatile int32_t ult_PESO1_DISPLAY = 0;
static uint32_t filteredValue1 = 0;
bool update_output1 = false;       // si se actualiza o no el peso en pantalla.
bool bascula1_conectada = true;   //por defecto conectada
bool tara1_cambio = false;        //cuando cuambias de tara (rollo de filamento distinto)
bool bascula1_calibrada = false;

// Iniciar el sensor HX711 de la bascula 1
HX711 bascula1;                 

// Bascula 2
// ------------------------------------------------

// Variables modificables:
int32_t zero_offset2 = -253970;
float calibration_factor2 = 270;

int32_t new_dif_EMA2 = 0;    // Diferencia entre la nueva lectura y la media del EMA
int32_t last_dif_EMA2 = 0;   // Diferencia entre la ultima lectura y la media del
int32_t new_dif_last2 = 0;   // Diferencia entre la anterior lectura y la actual
int32_t tara2_actual_g = 0;
int32_t tara2_actual_offset = 0;
static uint32_t prevMicros2 = 0;      // tiempo de la ultima lectura - en us
volatile float PESO2_f = 0.00f;
volatile int32_t PESO2 = 0;
volatile int32_t ult_PESO2 = 0;
volatile int32_t ult_PESO2_DISPLAY = 0;
static uint32_t filteredValue2 = 0;
bool update_output2 = false;       // si se actualiza o no el peso en pantalla.
bool bascula2_conectada = true;   //por defecto conectada
bool tara2_cambio = false;        //cuando cuambias de tara (rollo de filamento distinto)
bool bascula2_calibrada = false;

// Iniciar el sensor HX711 de la bascula 1
HX711 bascula2;                 

// Taras
// ------------------------------------------------
char name_tara1[10], name_tara2[10], name_tara3[10], name_tara4[10], name_tara5[10];
uint16_t peso_tara1, peso_tara2, peso_tara3, peso_tara4, peso_tara5;
uint16_t ultima_tara1, ultima_tara2;

// SERIAL DEBUG
// ------------------------------------------------
bool SERIAL_DEBUG = true; // variable que determina si tenemos serial o no
uint32_t serialTime;      // determina cada cuanto sacamos un loop de información serial
char received;            // para la funcion SerialReceive
static char input[64] = {};           // para recibir del serial
bool inputReadingCompleted = false;

//***************************************************
//                 INIT SERIAL
//***************************************************
void init_serial()
{
  Serial.begin(9600);
  Serial.setTimeout(2000); //Tiempo de espera para recibir en serial en ms
  while(!Serial);  // wait for serial to come online
  Serial.println("Serial preparado");
}

//***************************************************
//        Exponential moving average filter EMA
//***************************************************

/**
 * @brief   Exponential moving average filter.
 * 
 * Fast integer EMA implementation where the weight factor is a power of two.
 * 
 * The factor alpha in the difference equation of the Exponential Moving Average filter is a number between zero and one.
 * If alpha=1 the output is just equal to the input, and no filtering takes place. 
 * 
 * There are two main ways to implement this multiplication by alpha: Either we use floating point numbers and calculate
 * the multiplication directly, or we use integers, and express the multiplication as a division by 1/alpha > 1.
 * We can choose the value for alpha in such a way that 1/alpha = 2^k, k=numero natural
 * This is useful, because a division by a power of two can be replaced by a very fast right bitshift: 
 * alpha·x = x / 2^k =     x >> k
 * 
 * The difference equation of an exponential moving average filter: 
 * y[n] = alpha·x[n]+(1-alpha)·y[n-1] 
 * 
 * We can now rewrite the difference equation of the EMA with this optimization in mind:  
 * y[n] = y[n-1] + (x[n]- y[n-1]) >> k
 *  
 * Where x is the input sequence, and y is the output sequence.
 * 
 * Rounding. A final improvement we can make to our division algorithm is to round the result to the nearest integer, instead of truncating it towards zero
 * Consider the rounded result of the division a/b We can then express it as a flooring of the result plus one half: 
 * a/b = a/b + 1/2 = (a + b/2) / b
 * When b is a power of two, is equivalent to:
 * [a/2^k] = (a+1 << (k-1) >> k)
 *
 * [An in-depth explanation of the EMA filter](https://tttapa.github.io/Pages/Mathematics/Systems-and-Control-Theory/Digital-filters/Exponential%20Moving%20Average/)
 *
 * @tparam  K
 *          The amount of bits to shift by. This determines the location
 *          of the pole in the EMA transfer function, and therefore the
 *          cut-off frequency.  
 *          The higher this number, the more filtering takes place.  
 *          The pole location is @f$ 1 - 2^{-K} @f$.
 * @tparam  input_t
 *          The integer type to use for the input and output of the filter. 
 *          Can be signed or unsigned.
 * @tparam  state_t
 *          The unsigned integer type to use for the internal state of the
 *          filter. A fixed-point representation with @f$ K @f$ fractional
 *          bits is used, so this type should be at least @f$ M + K @f$ bits
 *          wide, where @f$ M @f$ is the maximum number of bits of the input.
 *
 * Some examples of different combinations of template parameters:
 * 
 * 1. Filtering the result of `analogRead`: analogRead returns an integer
 *    between 0 and 1023, which can be represented using 10 bits, so 
 *    @f$ M = 10 @f$. If `input_t` and `output_t` are both `uint16_t`,
 *    the maximum shift factor `K` is @f$ 16 - M = 6 @f$. If `state_t`
 *    is increased to `uint32_t`, the maximum shift factor `K` is 
 *    @f$ 32 - M = 22 @f$.
 * 2. Filtering a signed integer between -32768 and 32767: this can be 
 *    represented using a 16-bit signed integer, so `input_t` is `int16_t`,
 *    and @f$ M = 16 @f$. (2¹⁵ = 32768)
 *    Let's say the shift factor `K` is 1, then the minimum width of 
 *    `state_t` should be @f$ M + K = 17 @f$ bits, so `uint32_t` would be 
 *    a sensible choice.
 */

template <uint8_t K, class uint_t = uint32_t>
class EMA
{
public:
  /// Update the filter with the given input and return the filtered output.
  uint_t operator()(bool restart, uint_t input)
  {
    //if (restart)
    //  state = 0; // reinicia la EMA
    state += input;
    uint_t output = (state + half) >> K;
    state -= output;
    return output;
  }

  static_assert(
      uint_t(0) < uint_t(-1), // Check that `uint_t` is an unsigned type
      "The `uint_t` type should be an unsigned integer, otherwise, "
      "the division using bit shifts is invalid.");

  /// Fixed point representation of one half, used for rounding.
  constexpr static uint_t half = 1 << (K - 1);

private:
  uint_t state = 0;
};

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
//                      flushInput
//***********************************************************
void flushInput()
{
  for (int i = 0; i < 64; i++)
  {
    input[i] = {};
  }
}
//***********************************************************
//                      serialEvent()
//***********************************************************
void serialEvent()
{
  int i = 0;
  flushInput(); // al principio vaciamos el array para recibir nuevos datos
  while (Serial.available() < 1) // esperar a presionar una tecla
    delay(100);
  while (Serial.available() > 0)
  {
    input[i] = Serial.read();
    delay(2);
    i++;
    inputReadingCompleted = !(Serial.available());
  }

  // printing the input
  if (inputReadingCompleted)
  { // if input
    Serial.print("Serial input = ");
    Serial.println(input);
    inputReadingCompleted = false;
  }
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
    inputWeight = atoi(input); //convierte char -> int
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
  while (input[0] != '*')
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
    //input[0] = Serial.read(); //Serial.read no para la ejecución, pero solo nos da tiempo a 1 caracter.
    float f = atof(input); //convierte char -> float
    Serial.println(f);
    if (input[0] == '+')
      calibration_factor1 += 100;
    else if (input[0] == '-' && input[1] == 0 && input[2] != '.')  // contemplamos que es posible -0.xxxx
      calibration_factor1 -= 100;
    else
      calibration_factor1 += f;
  }
}


//***************************************************
//                 SETUP
//***************************************************
void setup()
{
  SERIAL_DEBUG = true; //!BORRAR PARA PRODUCCION

  if (SERIAL_DEBUG)
  {  
    init_serial();
    flushInput();
  }
  // Iniciar bascula 1
  bascula1.begin(PESO_DATA_PIN, PESO_SCK_PIN);
  bascula1.set_scale(calibration_factor1);
  bascula1.set_offset(zero_offset1);

  // Iniciar bascula 2
  bascula2.begin(PESO_DATA2_PIN, PESO_SCK2_PIN);
  bascula2.set_scale(calibration_factor2);
  bascula2.set_offset(zero_offset2);
  delay(100); //le damos un tiempo a que se inicie la bascula



  // Iniciar temporizador de las basculas
  prevMicros1 = micros();              // iniciar temporizador para las bascula 1
  prevMicros2 = micros();              // iniciar temporizador para las bascula 2
  WeightReadingRefreshTime *= 1000000; // convertir a µs
}

void loop()
{
  //calibrar_peso1();
  //calibrar_peso2();
  calibrar_factor1();
  leerBascula1();
  leerBascula2();
} 