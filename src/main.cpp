/*
 * Bascula.h - Funciones para el uso con la pantalla Nextion
 * 2021 Javier Carrera Arias  
 * All rights reserved under the library's licence
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 */

#include "inc/NextFilConfig.h"
#include <Arduino.h>

#include "HX711.h"  //Para las básculas
#include <DHT.h>    // Sensores de temperatura y humedad
#include "PID_v2.h" //https://github.com/gelraen/Arduino-PID-Library/
#include <EEPROM.h>
#include "lcd/PageRefresh/PageRefresh.h"  //incluir settings->.h y .cpp
#include "Settings/Settings.h"     //incluir settings->.h y .cpp
#include "module/SerialDebug/SerialDebug.h"     //incluir Serial_debug->.h y .cpp
#include "module/Weight/Weight.h"     //incluir Pesos->.h y .cpp

// ACTIVACIÓN DE FUNCIONES
// ------------------------------------------------
//#define ARDUINO_RTC  //comentar para desactivar en caso de usar el de nextion
#define NEXTION      //comentar si se usa otra pantalla (habria que programarlo)
#define NEOPIXEL_LED //comentar si no hay tira LED

// NEXTION
// ------------------------------------------------
#ifdef NEXTION
  #include <SoftwareSerial.h>     //Para no usar el mismo SERIAL que el programador del arduino usamos SoftwareSerial
  #include "EasyNextionLibrary.h" //https://www.arduino.cc/reference/en/libraries/easy-nextion-library/
  #include "trigger.h"            //EasyNextionLibrary

  /*Arduino mini pro 7 -> nextion TX   13-> Nextion RX*/
  SoftwareSerial nxSerial(NEXTION_TX_PIN, NEXTION_RX_PIN);
  EasyNex myNex(nxSerial);              // Create an object of EasyNex class with the name < myNex >
                                        // Set as parameter the Hardware Serial you are going to use
  uint32_t DATA_REFRESH_RATE = 1000;    // The time between each Data refresh of the page
                                        // Depending on the needs of the project, the DATA_REFRESH_RATE can be set
                                        // to 50ms or 100ms without a problem. In this example, we use 1000ms,
                                        // as DHT sensor is a slow sensor and gives measurements every 2 seconds
  uint32_t pageRefreshTimer = millis(); // Timer for DATA_REFRESH_RATE
  bool newPageLoaded = false;           // true when the page is first loaded ( lastCurrentPageId != currentPageId )
#endif

// RGB LED Support for Adafruit NeoPixel LED driver
// ------------------------------------------------
#ifdef NEOPIXEL_LED
  #include <Adafruit_NeoPixel.h>
#endif

// SETTINGS (valores por defecto)
// ------------------------------------------------
// Define la estructura defaults y la guarda en la memoria de programa no en la dinamica!
// Estos valores son constantes y no se pueden cambiar, solo en el sketch. Asi nos ahorramos mucha memoria dinámica

//DEFINICIÓN DE DEFAULTS - SE GUARDAN EN PROGMEM PARA AHORRAR RAM 
const SettingsType defaults PROGMEM = {
    true,    //bool SERIAL_DEBUG; //0 off / 1 on PONER A 0 para producto final
    "PLA",   //char name_prog1[6];
    "PETG",  //char name_prog2[6];
    "ABS",   //char name_prog3[6];
    "NYL",   //char name_prog4[6];
    "FLEX",  //char name_prog5[6];
    10,      //byte temp_prog1 ºC
    40,      //byte temp_prog2 ºC
    50,      //byte temp_prog3 ºC
    60,      //byte temp_prog4 ºC
    70,      //byte temp_prog5 ºC
    true,    //bool timer - si se activa por defecto el temporizador
    180,     //unsigned int set_time_min;
    true,    //bool timer_beep;
    true,    //bool vent_auto_off;
    1,       //byte VentMinDutyCycle; % Porcentaje de duty cycle en el que se apaga el ventilador
    270.00f, //float calibration_factor1; // con la calibracion
    -253970, //long zero_offset1; // con la calibracion de factor se obtiene, cuando se sabe el peso sin carga
    false,   //bascula1_calibrada);
    0,       //float calibration_factor2;
    0,       //long zero_offset1;
    false,   //bascula2_calibrada);
    100.0,   //float vaggKp; //float very aggressive Kp
    4.0,     //float vaggKi; //float very aggressive Ki
    20.0,    //float vaggKd; //float very aggressive Kd
    20.0,    //float aggKp; //float aggressive Kp
    1.0,     //float aggKi; //float aggressive Ki
    5.0,     //float aggKd; //float aggressive Kd
    1.0,     //float consKp; //float conservative Kp
    0.05,    //float consKi; //float conservative Ki
    0.25,    //float consKd; //float conservative Kd
    5000,    //unsigned int WindowSize intervalos en ms 100% duty cycle = WindowSize. Time proportioning control,  It's a really slow version of PWM
    3.0,     //float dht_T_ext_correccion;
    0.0,     //float dht_T_int_correccion;
    19.7,    //float dht_H_ext_correccion;
    0.0,     //float dht_H_int_correccion;
    120,     //unsigned int time_prog1; 2h en min
    180,     //unsigned int time_prog2; 3h en min
    240,     //unsigned int time_prog3; 4h en min
    360,     //unsigned int time_prog4; 6h en min
    600,     //unsigned int time_prog5; 10h  en min
    5000,    //unsigned int (ms) WeightReadingRefreshTime cada cuanto se refresca el peso en ms
    5,       //byte margen_temp del termostato, setpoint - margen_temp = se enciende de nuevo la resistencia
    60,      //byte setpoint ºC
    82,      //ledType HEX 0x52 = int 82 = NEO_GBR + KHZ800 abajo la tabla:
    7,       //byte neopixel_pixels
    true,    //bool ledStatus
    "TIPO1", //char name_tara1[10];
    "TIPO2", //char name_tara2[10];
    "TIPO3", //char name_tara3[10];
    "TIPO4", //char name_tara4[10];
    "TIPO5", //char name_tara5[10];
    0,       //int peso_tara1;
    0,       //int peso_tara2;
    0,       //int peso_tara3;
    0,       //int peso_tara4;
    0,       //int peso_tara5;
    0,       //int ultima_tara1; la configuracion usada por ultima vez
    0,       //int ultima_tara2; la configuracion usada por ultima vez
    SETTING_VER};

// tenemos 3 copias de la estructura de configuracion.
// 1. MEMPROG - defaults, constante definido en sketch
// 2. EEPROM - configuración de usuario, que se puede modificar
// 3. RAM - donde se carga la configuración seleccionada 1 o 2.

// actSettings es la instancia de la estructura en RAM, los valores se leen directamente de la estructura
// sin necesidad de declarar variables. Los valores en esta estructura son los seleccionados por el usuario
// Dos opciones. Por defecto que se guardan en la PROGMEM y definidos por usuario

Settings *settings; //Iniciamos solo el pointer para poder iniciar en setup, si no no medeja leer la MEMPROG antes de iniciarlo

// RGB LED Support for Adafruit NeoPixel LED driver
// ------------------------------------------------

/* RGB NeoPixel permutations
Offset:     HEX    DEC
NEO_RGB     0x06     6     
NEO_RBG     0x09     9
NEO_GRB     0x52    82
NEO_GBR     0xA1   161  
NEO_BRG     0x58    88
NEO_BGR     0xA4   164
NEO_KHZ800  0x0000   0
NEO_KHZ400  0x0100 256 */

#ifdef NEOPIXEL_LED
#define NEOPIXEL_IS_SEQUENTIAL    // Sequential display for temperature change - LED by LED. Disable to change all LEDs at once.
#define NEOPIXEL_BRIGHTNESS 127   // Initial brightness (0-255)
#define NEOPIXEL_STARTUP_TEST     // Cycle through colors at startup
#define NEOPIXEL_BKGD_LED_INDEX 0 // Index of the LED to use
#define NEOPIXEL_BKGD_COLOR \
  {                         \
    55, 255, 255, 0         \
  }

// Rather than declaring the whole NeoPixel object here, we just create
// a pointer for one, which we'll then allocate later
// It's helpful if you don't know NeoPixel settings->at compile time or
// just want to store this settings->in EEPROM or a file on an SD card....
Adafruit_NeoPixel *neo;
// variables NeoPixel
uint8_t NEOPIXEL_PIXELS;
uint32_t NEOPIXEL_TYPE;
uint32_t ledTime;
uint32_t ledTimeInterval = 300; //Tiempo de cambio de los LED ms
bool ledStatus;
bool ledFlip = false;

// Color helpers and presets
// --------------------------

/*uint32_t colorOff = neo->Color(0, 0, 0);
uint32_t colorRed = neo->Color(255, 0, 0);
uint32_t colorOrange = neo->Color(255, 80, 0);
uint32_t colorYellow = neo->Color(255, 255, 0);
uint32_t colorGreen = neo->Color(0, 255, 0);
uint32_t colorBlue = neo->Color(0, 0, 255);
uint32_t colorIndigo = neo->Color(0, 255, 255);
uint32_t colorViolet = neo->Color(255, 0, 255);
uint32_t colorWhite = neo->Color(255, 255, 255);*/
//uint16_t hue = 65536 / 3; //Color Verde
#endif

// ARDUINO RTC CLOCK
// ------------------------------------------------
#ifdef ARDUINO_RTC
#include "RTClib.h" //Date and time functions using a DS1307 RTC connected via I2C and Wire lib
RTC_DS1307 rtc;
//char daysOfTheWeek[7][12] = {"Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado", "Domingo"};
byte hora_actual;
byte minuto_actual;
#endif

// PID
// ------------------------------------------------
uint8_t setpoint;             // temperatura objetivo
uint16_t WindowSize;          // tamaño entre dos pases del PID - PWM 100% = activado WindoSize en cada pase.
uint32_t windowStartTime;     // variable temporal que guarda el inicio de cada pase
float vaggKp, vaggKi, vaggKd; // parametros PID "muy agresivos"
float aggKp, aggKi, aggKd;    // parametros PID "agresivos"
float consKp, consKi, consKd; // parametros PID "conservadores"
float input, output;          // input=temperatura actual, output traduce a tiempo efectivo de PWM (salida del PID)
bool HeatingDONE = false;     // verdadero cuando se llegue al objetivo de temperatura
bool keepWARM = false;        // controla la resistencia cuando ya hemos llegado a la temp. final en este programa (mantener caliente)
//Iniciar PID con parametros iniciales
PID_v2 myPID(consKp, consKi, consKd, PID::Direct);

// BASCULAS Y PESOS
// ------------------------------------------------
// Variables comunes modificables
float WeightReadingRefreshTime = 1.5; // (s)entre lecturas de peso y temp. exterior (min. aprox 1s) si da error de sensor no conectado aumentar!
const uint8_t umbral_display = 5;   // en decigramos el umbral para la salida por pantalla 5=0.5g,.
const uint8_t umbral_EMA = 1;       // en decigramos el umbral para la media dinamica 1=0.1g,.
//constexpr int EMA_K = 2;            // variable K de la media dinamica exponencial regula la agresividad del filtro.K=2 se ajusta mejor al cambio
// ESTA VARIABLE ESTA EN PESOS.H
// TODO: NO SE COMO HACER QUE ESTA VARIABLE ESTE DISPONIBLE EN MAIN
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
uint32_t prevMicros1 = 0;      // tiempo de la ultima lectura - en us
uint32_t filteredValue1 = 0;
volatile int32_t PESO1 = 0;
volatile int32_t ult_PESO1 = 0;
volatile int32_t ult_PESO1_DISPLAY = 0;
volatile float PESO1_f = 0.00f;
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
uint32_t prevMicros2 = 0;      // tiempo de la ultima lectura - en us
uint32_t filteredValue2 = 0;
volatile int32_t PESO2 = 0;
volatile int32_t ult_PESO2 = 0;
volatile int32_t ult_PESO2_DISPLAY = 0;
volatile float PESO2_f = 0.00f;
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

// SENSORES DE TEMPERATURA Y HUMEDAD
// ------------------------------------------------
// sensor exterior
float dht_T_ext_correccion;
float dht_H_ext_correccion;
float T_ext;
float H_ext;
DHT dht_ext(SENSOR_EXT_PIN, DHT11); // Initialize DHT sensor DHT 11.

// sensor interior
float dht_T_int_correccion; //el sensor AM2301 es mas preciso, el otro se corrige
float dht_H_int_correccion; //el sensor AM2301 es mas preciso, el otro se corrige
float T_int;
float H_int;
DHT dht_int(SENSOR_INT_PIN, DHT21); // Initialize DHT sensor DHT 21 (AM2301).

float start_temp;
float target_temp;
float current_temp;

uint32_t tempTime;
uint32_t tempTimeInterval = 4000; //Tiempo entre lecturas del sensor de temperatura interior

// temperaturas programadas
uint8_t temp_prog1, temp_prog2, temp_prog3, temp_prog4, temp_prog5;
uint8_t margen_temp = 5; // margen de temperatura (ºC) admisible antes de volver a calentar si se ajusta la T - objetivo a 60ºC
                         // el margen del termostato a 5 no se activa hasta que no baje de 55ºC

// VENTILADOR
// ------------------------------------------------
bool vent_auto_off;
uint8_t VentMinDutyCycle; //cuando el dutyCycle de la resistencia es menor que este numero el ventilador se apaga

// TEMPORIZADOR, TIEMPOS Y PROGRAMAS PERSONALIZADOS
// ------------------------------------------------
uint16_t set_time_min; //Temporizador en minutos
bool timer;            //activar/desactivar temporizador
bool timer_beep;       //señal acustica de fin de programa - temporizador

uint32_t now = 0, startTime = 0, totalTime = 0; // en ms
bool temporizador_end = false;                  //Para saber si el programa /temporizador ha terminado
bool empezar_programa = false;                  //Cuando empieza el temporizador para iniciar variables iniciales

// temporizadores programados
uint16_t time_prog1, time_prog2, time_prog3, time_prog4, time_prog5;

// nombre de los programas;
char name_prog1[6], name_prog2[6], name_prog3[6], name_prog4[6], name_prog5[6];

// SERIAL DEBUG
// ------------------------------------------------
bool SERIAL_DEBUG = true; // variable que determina si tenemos serial o no
uint32_t serialTime;      // determina cada cuanto sacamos un loop de información serial


//***************************************************
//                 BEEP - ok probado
//***************************************************

void beep(int HZ, int on, int off)
{
  tone(ALARMA_PIN, HZ); // Enviar señal de sonido...
  delay(on);            // ...tiempo encendido
  noTone(ALARMA_PIN);   // Se para la señal...
  delay(off);           // ...tiempo apagado
}

//***************************************************
//               ALARMA - ok probado
//***************************************************
// Podemos añadir más modos

void alarma(int modo)
{
  //MODO 0 = BIENVENIDA - ENCENDIDO
  //MODO 1 = ERROR CRITICO SENSOR DE TEMPERATURA
  //MODO 2 = PROGRAMA TERMINADO
  //MODO 3 = RTC no conectado o sin batería

  switch (modo)
  {
  case 0:
    beep(500, 100, 300);
    beep(2000, 300, 1000);
    break;
  case 1:
    for (;;) //loop infinito
    {
      beep(2000, 300, 200);
      T_int = dht_int.readTemperature();
      if (!isnan(T_int))
      {
        break;
      }
    }
    break;
  case 2:
    for (int i = 0; i < 3; i++)
    { //señal acústica de fin de programa 3x3pitidos
      beep(500, 100, 100);
      beep(1000, 200, 100);
      beep(2000, 300, 100);
    }
    break;
  case 3:
    beep(500, 300, 300);
    beep(500, 300, 300);
    break;
  }
}

//***************************************************
//               FLOAT MAP - ok probado
//***************************************************
// funcion map en formato float! Convierte un rango en otro en formato float
// se utiliza para convertir el rango entre temperatura inicial y final en
// un porcentaje o en una escala de color por ejemplo para no tener saltos tan
// grandes en la escala de temperatura y que el cambio de color sea mas gradual

float fmap(float sensorValue, float sensorMin, float sensorMax, float outMin, float outMax)
{
  return (sensorValue - sensorMin) * (outMax - outMin) / (sensorMax - sensorMin) + outMin;
}

//***************************************************
//                   NEOPIXEL LED
//***************************************************

inline uint16_t convert_currentTemp_HUE(const float &start, const float &current, const float &target)
{
  if (SERIAL_DEBUG) // Comprobar que la conversion recibe los datos correctos
  {
    Serial.print(F("LED currentTemp->HUE start:"));
    Serial.println(start);
    Serial.print(F("LED CurrentTemp->HUE current:"));
    Serial.println(current);
    Serial.print(F("LED CurrentTemp->HUE target:"));
    Serial.println(target);
  }
  //Funcion fmap (float map) para no tener saltos tan grandes y que el cambio de color sea mas gradual
  return (float)fmap(current, start, target, 65536 / 3, 0.0); // HUE:Verde (65536/3)  -> Rojo (0)
}

inline void set_hue(uint16_t hue, bool flip, bool HeatingDONE)
{
  //HUE - Neopixel
  // Cian:     65536/2
  // Verde:    65536/3
  // Amarillo: 65536/6
  // Rojo:     0
  // Magenta:  65536x5/6
  // Azul:     65536x2/3

  uint32_t rgbcolor = neo->ColorHSV(hue);
  uint32_t rgbcolor_alt = neo->ColorHSV(hue - 3000);

  if (SERIAL_DEBUG)
  {
    Serial.print(F("HUE "));
    Serial.println(hue);
    Serial.print(F("NEOPIXEL_PIXELS "));
    Serial.println(NEOPIXEL_PIXELS);
    Serial.print(F("now "));
    Serial.println(now);
    Serial.print(F("serialTime "));
    Serial.println(serialTime);
    Serial.print(F("ledFlip "));
    Serial.println(ledFlip);
    Serial.print(F("rgbcolor "));
    Serial.println(rgbcolor);
    Serial.print(F("rgbcolor_alt "));
    Serial.println(rgbcolor_alt);
  }

  if (now > serialTime)
  { // Set tiempo
    if (!HeatingDONE)
    { //modo de cambio de color
      for (int i = 0; i < NEOPIXEL_PIXELS; i++)
      {
        if (flip)
        {                             //Para que cambien de color par/impar - como calentado en proceso
          ledTime += ledTimeInterval; //Tiempo de cambio ms
          if ((i % 2) == 0)
            neo->setPixelColor(i, rgbcolor); //Pixel par
          else
            neo->setPixelColor(i, rgbcolor_alt); //Pixel impar
        }
        else
        {
          ledTime += ledTimeInterval; //Tiempo de cambio ms
          if ((i % 2) == 0)
            neo->setPixelColor(i, rgbcolor_alt); //Pixel impar
          else
            neo->setPixelColor(i, rgbcolor); //Pixel par
        }
      }
      ledFlip = !ledFlip;
    }
    else
    { //Calentado terminado - color fijo, dimming..
      for (int e = 0; e < 3; e++)
      { //repetir 3 veces
        uint8_t value = 155;
        for (value = 155; value > 0; value--)
        { //fade out
          rgbcolor = neo->ColorHSV(hue, 255, value);
          neo->fill(rgbcolor, 0, NEOPIXEL_PIXELS);
          delay(10);
          neo->show();
        }
      }
    }
  }
}

//***********************************************************
//                       PROGRAMAS LED
//***********************************************************

void LEDonHeating(const float &start, const float &current, const float &target)
{
  uint16_t HueActual = convert_currentTemp_HUE(start, current, target);
  if (SERIAL_DEBUG) // Mostrar el HUE actual
  {
    Serial.print(F("currentTemp->HUE HUE"));
    Serial.println(HueActual);
  }
  set_hue(HueActual, ledFlip, HeatingDONE);
  neo->show();
}

void LEDBienvenida()
{
  uint32_t hue_;
  uint32_t rgbcolor;
  for (hue_ = 0; hue_ < 65500; hue_ += 20)
  { //fade out
    rgbcolor = neo->ColorHSV(hue_);
    neo->fill(rgbcolor, 0, NEOPIXEL_PIXELS);
    neo->show();
  }
  for (hue_ = 65535; hue_ > 51; hue_ -= 20)
  { //fade out
    rgbcolor = neo->ColorHSV(hue_);
    neo->fill(rgbcolor, 0, NEOPIXEL_PIXELS);
    neo->show();
  }
}

/*
//interrupt functions for the buttons
void buttonPressed1()
{
  button_zero = 1;
}
void buttonPressed2()
{
  button_hold = 1;
}
*/

//***************************************************
//                  OK PROBADO
//            LEER TEMPERATURA INTERIOR
//***************************************************

// Atención: la lectura de temperatura o humedad tarda aprox. 250 ms
// Algunos sensores pueden tardar hasta 2 seg.

void leer_temp_int()
{
  //Se lee la temperatura en intervalos determinados por tempTimeInterval (en ms)
  if (now > tempTime)
  {
    H_int = dht_int.readHumidity() + dht_H_int_correccion;
    T_int = dht_int.readTemperature() + dht_T_int_correccion;
    // Comprueba si alguna lectura ha fallado y sale para probar de nuevo
    if (isnan(H_int) || isnan(T_int))
    {
      if (SERIAL_DEBUG)
        Serial.println(F("ERROR: ¡No se obtienen lecturas del sensor DHT interior!"));
      //TODO señal a Nextion de error
      digitalWrite(RELE_RESISTENCIA_PIN, LOW); // apagar la resistencia por precaucion
      alarma(1);                               // Alarma: error crítico sensor de temperatura
      return;
    }
    //TODO enviar datos a Nextion
    if (SERIAL_DEBUG) //Muestra temperatura y humedad
    {
      Serial.print(F("Humedad int: "));
      Serial.print(H_int);
      Serial.print(F("% Temp. int: "));
      Serial.print(T_int);
      Serial.println(F("°C"));
    }
    tempTime += tempTimeInterval; //añade otro intervalo al tiempo final de la próxima lecturas (ms)
  }
}

//***********************************************************
//               OK PROBADO
//                LEER TEMPERATURA EXTERIOR
//***********************************************************

//? De momento se controla la lectura desde el loop, ponemos un temporizador
//? como para la temperatura interior???

void leer_temp_ext()
{
  H_ext = dht_ext.readHumidity() + dht_H_ext_correccion;
  T_ext = dht_ext.readTemperature() + dht_T_ext_correccion;
  // Comprueba si alguna lectura ha fallado y sale para probar de nuevo
  if (isnan(H_ext) || isnan(T_ext))
  {
    if (SERIAL_DEBUG)
      Serial.println(F("ERROR: ¡No se obtienen lecturas del sensor DHT exterior!"));
    //TODO señal a Nextion de error
    digitalWrite(RELE_RESISTENCIA_PIN, LOW); // apagar la resistencia por precaucion
    alarma(1);                               // Alarma: error crítico sensor de temperatura
    return;
  }
  //TODO enviar datos a Nextion
  if (SERIAL_DEBUG)
  {
    Serial.print(F("Humedad ext: "));
    Serial.print(H_ext);
    Serial.print(F("% Temp. ext: "));
    Serial.print(T_ext);
    Serial.println(F("°C"));
  }
}

//***************************************************
//                 TEMPORIZADOR
//***************************************************

// comprueba si el tiempo programado se ha cumplido

bool end_time()
{
  totalTime = now - startTime;
  if ((set_time_min * 60) > (totalTime / 1000)) //cálculo en segundos
    return false;
  else
    return true;
}

//***************************************************
//             SERIAL SEND - PARA PID
//***************************************************

void SerialSend()
{
  Serial.print(F("setpoint: "));
  Serial.print(setpoint);
  Serial.print(F("/input: "));
  Serial.print(input);
  Serial.print(F("/output: "));
  Serial.println(output);
  Serial.print(F("HeatingDONE? "));
  Serial.println(HeatingDONE);
  delay(1500);

#ifdef NEOPIXEL_LED
  Serial.print(F("Neop.Temp.inicial "));
  Serial.println(start_temp);
  Serial.print(F("Neop.Temp.actual "));
  Serial.println(T_int);
  Serial.print(F("Neop.Temp.final "));
  Serial.println(setpoint);
#endif
}

//***************************************************
//            RESET todas las variables
//***************************************************
//? COMPROBAR QUE VARIABLES HAY QUE RESETEAR

void reset_var()
{
  prevMicros1 = prevMicros2 = totalTime = startTime = now = 0; //ms
  temporizador_end = true;
  timer = empezar_programa = false;
}

//***************************************************
//                CRC CHECK EEPROM
//***************************************************
//https://www.arduino.cc/en/Tutorial/LibraryExamples/EEPROMCrc

uint32_t eeprom_crc()
{
  uint32_t crc_table[16] = {
      0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
      0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
      0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
      0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c};
  uint32_t crc = ~0L;
  for (uint16_t index = 0; index < EEPROM.length(); ++index)
  {
    crc = crc_table[(crc ^ EEPROM[index]) & 0x0f] ^ (crc >> 4);
    crc = crc_table[(crc ^ (EEPROM[index] >> 4)) & 0x0f] ^ (crc >> 4);
    crc = ~crc;
  }
  return crc;
}

//***************************************************
//                       NEXTION
//***************************************************

/*Function trigger();
Description: This is the most important function of the library. And this is because,
it gives you the ability to use the predefined functions and run your code from there. 
These predefined functions are named trigger0(), trigger1(), trigger2()... up to trigger50().
You can use them as a simple void function out of the loop, in which you will have written
a block of code to run every time it is called. You can call those trigger() functions and 
run the code they contain anytime by simply writing in a Nextion Event the command:
printh 23 02 54 XX , where XX the id for the triggerXX() in HEX. For example in a button's 
Touch Release Event, WRITE:

Command   Function
printh 23 02 54 00  trigger0()
printh 23 02 54 01  trigger1()
...   ...
printh 23 02 54 0A  trigger10()
... up to   ... up to
printh 23 02 54 32  trigger50()
*/

/*
 * Manage Variables You can read/write the variables as any other component.

Use readNumber() to read the value of a numeric variable.
Example: myNex.readNumber("va0.val");
BUT: myNex.readNumber("sys0");

Use writeNum() to change the value of a numeric variable.
Example: myNex.writeNum("va0.val", 255);
BUT: myNex.readNumber("sys0", 375);

Use readStr() to read the text of a String variable.
Example: myNex.readStr("va0.txt");

Use writeStr() to change the text of a String variable.
Example: myNex.writeStr("va0.txt", "Hello World"); For this to happen, the variables you want to read/write must be at the page you are currently on.
Otherwise, if the variables are of global scope, you will need to use a prefix with the page name that the variables are at.
Example:
myNex.readNumber("page0.va0.val"); // If the variable is at page0
The same goes for the other functions as well.
 */

void trigger0()
{ // printh 23 02 54 00
  /* Write in the Touch Release Event of the button this command: printh 23 02 54 00
   * Every time the button is pressed, the trigger0() function will run and the code inside will be executed once */

  /*digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); //If LED_BUILTIN is ON, turn it OFF, or the opposite
  if(digitalRead(LED_BUILTIN) == HIGH){
    myNex.writeNum("b0.bco", 2016); // Set button b0 background color to GREEN (color code: 2016)
    myNex.writeStr("b0.txt", "ON"); // Set button b0 text to "ON"
  }
  else if(digitalRead(LED_BUILTIN) == LOW){
    myNex.writeNum("b0.bco", 63488); // Set button b0 background color to RED (color code: 63488)
    myNex.writeStr("b0.txt", "OFF"); // Set button b0 text to "ON"
  }*/
}

void trigger1()
{ // printh 23 02 54 01
}
void trigger2()
{ // printh 23 02 54 02
}
void trigger3()
{ // printh 23 02 54 03
}
void trigger4()
{ // printh 23 02 54 04
}
void trigger5()
{ // printh 23 02 54 05
}
void trigger6()
{ // printh 23 02 54 06
}
void trigger7()
{ // printh 23 02 54 07
}
void trigger8()
{ // printh 23 02 54 08
}
void trigger9()
{ // printh 23 02 54 09
}
void trigger10()
{ // printh 23 02 54 0A
}
void trigger11()
{ // printh 23 02 54 0B
}
void trigger12()
{ // printh 23 02 54 0C
}
void trigger13()
{ // printh 23 02 54 0D
}
void trigger14()
{ // printh 23 02 54 0E
}
void trigger15()
{ // printh 23 02 54 0F
}
void trigger16()
{ // printh 23 02 54 10
}
void trigger17()
{ // printh 23 02 54 11
}
void trigger18()
{ // printh 23 02 54 12
}
void trigger19()
{ // printh 23 02 54 13
}
void trigger20()
{ // printh 23 02 54 14
}
void trigger21()
{ // printh 23 02 54 15
}
void trigger22()
{ // printh 23 02 54 16
}
void trigger23()
{ // printh 23 02 54 17
}
void trigger24()
{ // printh 23 02 54 18
}
void trigger25()
{ // printh 23 02 54 19
}
void trigger26()
{ // printh 23 02 54 1A
}
void trigger27()
{ // printh 23 02 54 1B
}
void trigger28()
{ // printh 23 02 54 1C
}
void trigger29()
{ // printh 23 02 54 1D
}
void trigger30()
{ // printh 23 02 54 1E
}
void trigger31()
{ // printh 23 02 54 1F
}
void trigger32()
{ // printh 23 02 54 20
}
void trigger33()
{ // printh 23 02 54 21
}
void trigger34()
{ // printh 23 02 54 22
}
void trigger35()
{ // printh 23 02 54 23
}
void trigger36()
{ // printh 23 02 54 24
}
void trigger37()
{ // printh 23 02 54 25
}
void trigger38()
{ // printh 23 02 54 26
}
void trigger39()
{ // printh 23 02 54 27
}
void trigger40()
{ // printh 23 02 54 28
}
void trigger41()
{ // printh 23 02 54 29
}
void trigger42()
{ // printh 23 02 54 2A
}
void trigger43()
{ // printh 23 02 54 2B
}
void trigger44()
{ // printh 23 02 54 2C
}
void trigger45()
{ // printh 23 02 54 2D
}
void trigger46()
{ // printh 23 02 54 2E
}
void trigger47()
{ // printh 23 02 54 2F
}
void trigger48()
{ // printh 23 02 54 30
}
void trigger49()
{ // printh 23 02 54 31
}
void trigger50()
{ // printh 23 02 54 32
}

/*
 * Manage Variables You can read/write the variables as any other component.

Use readNumber() to read the value of a numeric variable.
Example: myNex.readNumber("va0.val");
BUT: myNex.readNumber("sys0");

Use writeNum() to change the value of a numeric variable.
Example: myNex.writeNum("va0.val", 255);
BUT: myNex.readNumber("sys0", 375);

Use readStr() to read the text of a String variable.
Example: myNex.readStr("va0.txt");

Use writeStr() to change the text of a String variable.
Example: myNex.writeStr("va0.txt", "Hello World"); For this to happen, the variables you want to read/write must be at the page you are currently on.
Otherwise, if the variables are of global scope, you will need to use a prefix with the page name that the variables are at.
Example:
myNex.readNumber("page0.va0.val"); // If the variable is at page0
The same goes for the other functions as well.
 */

void refreshPage0() // página: main
{
  //  myNex.writeStr("t0.txt","3");
  //uint16_t tempInt = 3;
  //  myNex.writeNum("x0.val",tempInt);
}

void refreshPage1() // página: hora
{
}

void refreshPage2() // página: setTiempo
{
}

void refreshPage3() // página: setTemp
{
}

void refreshPage4() // página: configA
{
}

void refreshPage5() // página: configB
{
}

void refreshPage6() // página: configC
{
}

void refreshPage7() // página: configD
{
}

void refreshPage8() // página: configE
{
}

void refreshPage9() // página: configF
{
}

void refreshPage10() // página: Teclado
{
}

void refreshPage11() // página: numpad
{
}

void refreshPage12() // página: peso1Met1
{
}

void refreshPage13() // página: peso1Met2
{
}

void refreshPage14() // página: configDHTtemp
{
}

void refreshPage15() // página: configDHThum
{
}

void refreshPage16() // página: peso2Met1
{
}

void refreshPage17() // página: peso2Met2
{
}

void refreshPage18() // página: graficaTemp
{
}

void refreshPage19() // página: graficaHum
{
}

void refreshPage20() // página: graficaPeso1
{
}

void refreshPage21() // página: graficaPeso2
{
}

/*
#############################################
# Methods and Functions
#############################################

NextionListen (loop)
RefreshCurrentPage
writeStr 
writeNum  
readNumber 
currentPageId 
lastCurrentPageId 
readNumberFromSerial  */

//***************************************************
//         AGRESIVIDAD DE LOS FACTORES PID
//***************************************************
// Cambia la agresividad del PID dependiendo de lo lejos que estemos del objetivo:
// La razón es que se ponga al máximo enseguida porque la resistencia es lenta
// -------------------------------------------------------------------------------
void factores_PID()
{
  float gap = abs(setpoint - input); //distance away from setpoint
  if (gap <= 5)
  {
    myPID.SetTunings(consKp, consKi, consKd);
  } //we're 5º close to setpoint, use conservative tuning parameters
  else if (gap > 5 && gap < 10)
  {
    myPID.SetTunings(aggKp, aggKi, aggKd);
  } //we're between 5 and 10º far from setpoint, use aggressive tuning parameters
  else
  {
    myPID.SetTunings(vaggKp, vaggKi, vaggKd);
  } //> 10º far from setpoint, very aggressive tuning
}

/*----------------------------------------------------------------------------------*/
/*----------------------------------- SETUP ----------------------------------------*/
/*----------------------------------------------------------------------------------*/
void setup()
{

  // INICIAR PINES
  // ------------------------------------------------
  pinMode(ALARMA_PIN, OUTPUT);           // Set buzzer
  pinMode(RELE_RESISTENCIA_PIN, OUTPUT); //rele de la resistencia
  pinMode(RELE_VENTILADOR_PIN, OUTPUT);  //rele del ventilador

  // SERIAL DEBUG
  // ------------------------------------------------
  EEPROM.get(0, SERIAL_DEBUG); //leer la variable bool SERIAL_DEBUG que se almacena en la EEPROM 1er byte
  SERIAL_DEBUG = true;         //!BORRAR PARA PRODUCCION

  if (SERIAL_DEBUG)
  {  
    init_serial();
    flushInput();
  }

  // COPIAR ESTRUCTURA defaults - PROGMEM A RAM
  // ------------------------------------------------
  /*
   * Carga la estructura defaults de la PROGMEM a RAM: struct actSettings y la pasa al constructor
   * porque no se puede leer directamente de la PROGMEM dentro de la clase
   *
   * Se crea una instancia de settings-> y se pasan los valores defaults
   * En la clase, si la version default y la version EEPROM coinciden, se sobreescribe con la lectura de la EEPROM
   * Si la EEPROM esta corrupta, no se sobreescribe con la lectura de la EEPROM y se dejan los valores de defaults
   */

  SettingsType actSettings = defaults;          // copiar los contenidos de defaults a una nueva estructura actSettings
  PROGMEM_readAnything(&defaults, actSettings); // método para leer la estructura en PROGMEM y copiarla a RAM actSettings

  if (SERIAL_DEBUG)                             // Para comprobar que se lee la estructura de la PROGMEM correctamente
  {
    Serial.println();
    Serial.print(F("(Setup) comprobar MEMPROG defaults: name.prog1_ "));
    Serial.print(actSettings.name_prog1);
    Serial.print(F(" / temp_prog1_ "));
    Serial.print(actSettings.temp_prog1);
    Serial.print(F(" / version_ "));
    Serial.println(actSettings.ver);
  }
  // Constructor de settings - definimos el pointer
  settings = new Settings(actSettings, SERIAL_DEBUG);

  if (SERIAL_DEBUG) // Y comprobamos que se lee la estructura de la RAM correctamente
  {
    Serial.println("Configuracion actual");
    settings->printSerial(); // muestra toda los datos guardados de configuración en RAM - actSettings
  }

  // EEPROM
  // ------------------------------------------------
  if (SERIAL_DEBUG)
  {
    Serial.print(F("Longitud EEPROM: "));
    Serial.println(EEPROM.length()); // Longitud de la EEPROM.
    Serial.print(F("CRC32 of EEPROM data: 0x"));
    Serial.println(eeprom_crc(), HEX); // Mostrar el resultado del método eeprom_crc()
    Serial.print(F("\nCRC Terminado!\n"));
  }

// NEXTION
// ------------------------------------------------
#ifdef NEXTION
  myNex.begin(115200);      // starting the serial port at 115200. NEXTION MUST HAVE THE SAME RATE. For this we write at
                            // first page to the preinitialize event the command @  baud=115200  @
                            // NOTE "bauds" will change the default baud rate off 9600 until it changed again
  uint8_t nxPagina = 0;     // pagina inicial 0
  delay(500);               // le damos 500ms a Nextion para iniciar
  myNex.writeStr("page 0"); // ir a la pagina inicial en caso de que Arduino se resetee
  delay(50);
  myNex.lastCurrentPageId = 1;
#endif

  // NEOPIXEL RGB
  // ------------------------------------------------
  // Aqui ya podemos leer 'NEOPIXEL_PIN', 'NEOPIXEL_PIXELS' y'NEOPIXEL_TYPE' de la EEPROM

#ifdef NEOPIXEL_LED
  // leer de la EEPROM
  NEOPIXEL_TYPE = settings->parameters->neopixel_type;
  NEOPIXEL_PIXELS = settings->parameters->neopixel_pixels;
  ledStatus = settings->parameters->ledStatus;
  //ledStatus = true;
  //NEOPIXEL_PIXELS = 7;
  // Creamos el objeto NeoPixel (neo) dinamicamente con los valores leidos
  neo = new Adafruit_NeoPixel(NEOPIXEL_PIXELS, NEOPIXEL_PIN, NEOPIXEL_TYPE); //crear instancia ya reservada con el pointer *neo
  neo->begin();                                                              // INITIALIZE NeoPixel strip object (REQUIRED)
  neo->clear();                                                              // /Todos los pixeles apagados
  LEDBienvenida();                                                           // Animación LED de bienvenida
#endif

  // LEER PARÁMETROS DE LA CONFIGURACIÓN GUARDADA
  // los char array se modifican con strcpy
  // ------------------------------------------------
  SERIAL_DEBUG = settings->parameters->SERIAL_DEBUG;
  strcpy(name_prog1, settings->parameters->name_prog1);
  strcpy(name_prog2, settings->parameters->name_prog2);
  strcpy(name_prog3, settings->parameters->name_prog3);
  strcpy(name_prog4, settings->parameters->name_prog4);
  strcpy(name_prog5, settings->parameters->name_prog5);
  temp_prog1 = settings->parameters->temp_prog1;
  temp_prog2 = settings->parameters->temp_prog2;
  temp_prog3 = settings->parameters->temp_prog3;
  temp_prog4 = settings->parameters->temp_prog4;
  temp_prog5 = settings->parameters->temp_prog5;
  timer = settings->parameters->timer;
  set_time_min = settings->parameters->set_time_min;
  timer_beep = settings->parameters->timer_beep;
  vent_auto_off = settings->parameters->vent_auto_off;
  VentMinDutyCycle = settings->parameters->VentMinDutyCycle;
  calibration_factor1 = settings->parameters->calibration_factor1;
  zero_offset1 = settings->parameters->zero_offset1;
  bascula1_calibrada = settings->parameters->bascula1_calibrada;
  calibration_factor2 = settings->parameters->calibration_factor2;
  zero_offset2 = settings->parameters->zero_offset2;
  bascula2_calibrada = settings->parameters->bascula2_calibrada;
  vaggKp = settings->parameters->vaggKp;
  vaggKi = settings->parameters->vaggKi;
  vaggKd = settings->parameters->vaggKd;
  aggKp = settings->parameters->aggKp;
  aggKi = settings->parameters->aggKi;
  aggKd = settings->parameters->aggKd;
  consKp = settings->parameters->consKp;
  consKi = settings->parameters->consKi;
  consKd = settings->parameters->consKd;
  WindowSize = settings->parameters->WindowSize;
  dht_T_ext_correccion = settings->parameters->dht_T_ext_correccion;
  dht_T_int_correccion = settings->parameters->dht_T_int_correccion;
  dht_H_ext_correccion = settings->parameters->dht_H_ext_correccion;
  dht_H_int_correccion = settings->parameters->dht_H_int_correccion;
  time_prog1 = settings->parameters->time_prog1;
  time_prog2 = settings->parameters->time_prog2;
  time_prog3 = settings->parameters->time_prog3;
  time_prog4 = settings->parameters->time_prog4;
  time_prog5 = settings->parameters->time_prog5;
  WeightReadingRefreshTime = settings->parameters->WeightReadingRefreshTime;
  setpoint = settings->parameters->setpoint;
  margen_temp = settings->parameters->margen_temp;
  strcpy(name_tara1, settings->parameters->name_tara1);
  strcpy(name_tara2, settings->parameters->name_tara2);
  strcpy(name_tara3, settings->parameters->name_tara3);
  strcpy(name_tara4, settings->parameters->name_tara4);
  strcpy(name_tara5, settings->parameters->name_tara5);
  peso_tara1 = settings->parameters->peso_tara1;
  peso_tara2 = settings->parameters->peso_tara2;
  peso_tara3 = settings->parameters->peso_tara3;
  peso_tara4 = settings->parameters->peso_tara4;
  peso_tara5 = settings->parameters->peso_tara5;
  ultima_tara1 = settings->parameters->ultima_tara1;
  ultima_tara2 = settings->parameters->ultima_tara2;

  // INICIAR EL SISTEMA
  // ------------------------------------------------
  // iniciar en modo seguro (ventilador off, resitencia off)
  digitalWrite(RELE_RESISTENCIA_PIN, LOW);
  digitalWrite(RELE_VENTILADOR_PIN, LOW);

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

  // iniciar sensores de temperatura
  dht_ext.begin();
  dht_int.begin();
  leer_temp_int(); // Primera lectura
  leer_temp_ext(); // Primera lectura

  // iniciar tiempos
  windowStartTime = millis(); // para el PID
  startTime = millis();       // para el temporizador

  // ajustar y encender el PID:
  myPID.SetOutputLimits(0, WindowSize); // Ajustamos para que el PID vaya entre 0 y la WindowSize.
  myPID.Start(input, output, setpoint); // Encender el PID

  // bienvenida al iniciar correctamente
  alarma(0); // señal acústica de bienvenida

// RTC CHECK - RELOJ
// ------------------------------------------------
#ifdef ARDUINO_RTC
  if (!rtc.begin())
  {
    if (SERIAL_DEBUG)
    {
      Serial.print(F("RTC no encontrado o sin batería\n"));
      Serial.flush();
    }
    alarma(3);
    delay(1500);
  }
  // When time needs to be set on a new device, or after a power loss, the
  // following line sets the RTC to the date & time this sketch was compiled
  if (!rtc.isrunning())
  {
    if (SERIAL_DEBUG)
      Serial.println(F("¡RTC no se está ejecutando!"));
    // following line sets the RTC to the date & time this sketch was compiled from computer
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
#endif
}

/************************************************************************************/
/************************************** LOOP ****************************************/
/************************************************************************************/

void loop()
{

// LEER ENTRADAS DE LA PANTALLA NEXTION
// ------------------------------------------------
#ifdef NEXTION
  myNex.NextionListen(); // Esta funcion hay que llamarla en el bucle esperando respuesta de Nextion

  if ((millis() - pageRefreshTimer) > DATA_REFRESH_RATE)
  {
    myNex.RefreshCurrentPage();
    pageRefreshTimer = millis();
  }

  //Cargar por primera vez la pagina al cambiar a ella
  if (myNex.currentPageId != myNex.lastCurrentPageId)
  {                       // Si las dos variables son distintas, quiere decir que se ha cargado una página nueva
    newPageLoaded = true; // Pagina nueva cargada
                          // Esta variable se usa como argumento en el if() de los refreshPageXX(),
                          // si es verdadera se cargan todos los valores de la página con su valor actual
                          // sin tener que comparar si los valores han cambiado o no
    myNex.RefreshCurrentPage();
    newPageLoaded = false;                         // Despues de actualizar la página por primera vez, cambiamos la variable a falso
                                                   // Ahora los valores so actualizan SOLO si el valor nuevo es diferente del ultimo enviado
    myNex.lastCurrentPageId = myNex.currentPageId; // e igualamos las variables para identificar el siguiente cambio de página
  }
#endif

  // RTC - RELOJ
  // ------------------------------------------------

  //? comprobar si se puede optimizar.. usa demasiada RAM 12%!!!!!!!!

#ifdef ARDUINO_RTC
  DateTime ahora = rtc.now();
  hora_actual = ahora.hour();
  minuto_actual = ahora.minute();
  if (SERIAL_DEBUG)
  {
    Serial.print(F("Reloj RTC: "));
    Serial.print(hora_actual, DEC);
    Serial.print(F(":"));
    Serial.println(minuto_actual, DEC);
  }
#endif

  // LEER TIEMPO Y COMPROBAR TEMPORIZADOR
  // ------------------------------------------------
  now = millis(); //Iniciar variable now en cada bucle
  bool temporizador_end = false;
  // si está activado el temporizador, comprobar si ha llegado al final del programa
  if (timer)
    temporizador_end = end_time();

  // LEER TEMPERATURA
  // ------------------------------------------------
  leer_temp_int(); // se lee la temperatura interior. El propio método regula la frecuencia de lectura
  input = T_int;   //TEMPERATURA REAL DEL SENSOR
  current_temp = T_int;

  // LECTURAS INICIALES AL EMPEZAR UN PROGRAMA / TEMPORIZADOR
  // ------------------------------------------------

  if (empezar_programa) // variable que solo es verdadera justo al empezar un nuevo programa
  {
    target_temp = setpoint;
    start_temp = T_int;       // variable de temperatura inicial para calculos de % de temperatura subida
    empezar_programa = false; // y cambiamos a falso para seguir con el programa
  }

  // COMPROBAR TEMPERATURAS (ACTUAL - FINAL)
  // ------------------------------------------------
  // si hemos llegado a la temp. objetivo por primera vez en este programa
  if (T_int >= setpoint && !HeatingDONE)
  {
    HeatingDONE = true; // hemos llegado por 1ª vez a la temperatura final, proceso de calentamiento terminado
                        // ahora sólo tenemos que mantener la temperatura con una oscilación de - margen_temp
    keepWARM = false;   // inicialmente tenemos que apagar la resistencia ya que acabamos de llegar a la temp. final
  }
  // en este programa ya hemos llegado a la temperatura final, comprobamos si hace falta encender la resistencia
  // para mantener la temperatura, tenemos que comprobar que no baje por debajo del margen indicado (margen_temp).
  if (HeatingDONE)
  {
    if (T_int + margen_temp >= target_temp) // todavia no estamos por debajo del margen de temperatura establecido
      keepWARM = false;
    else
    { // estamos por debajo del margen de temperatura establecido, encedemos la resistencia
      keepWARM = true;
    }
  }

  // PID
  // ------------------------------------------------
  input = T_int;             // input se utiliza para el cálculo del PID = LECTURA REAL DEL SENSOR
  output = myPID.Run(input); // output es la salida de la funcion PID la que regula el "PWM" para la resistencia

  // Ejecuta la salida del PID solo si el temporizador no ha terminado y si esta activado el temporizador
  // por seguridad solo permitiremos que se encienda la resistencia si el temporizador está en marcha
  // asi no permitiremos que esté encendida por tiempo indefinido
  // Ejecuta solo si no hemos llegado a la temperatura final, o si hemos llegado, pero hay que mantener caliente.
  if ((!temporizador_end && timer) && ((HeatingDONE && keepWARM) || (!HeatingDONE)))
  {
    /************************************************
     "PWM A TIEMPO ON/OFF DEL RELE"
     enciende y apaga la salida del relé dependiendo en la salida del PID y lo traduce a "tiempo encendido" y "tiempo apagado"
     en cada ciclo del PID (tiempo del ciclo = WindowSize). Convierte el ciclo PWM a tiempo de 0 a WindowSize (100%) Enciende y apaga en consecuencia
    *************************************************/
    uint32_t CurrentWindow = now - windowStartTime; // tiempo actual ya pasado dentro del ciclo PID
    if (CurrentWindow > WindowSize)
      windowStartTime += WindowSize;            // si hemos cumplido un ciclo PID la variable de inicio de ciclo se modifica
    if (output > CurrentWindow)                 // i el tiempo determinado por "output" todavia no se ha cumplido en este ciclo
      digitalWrite(RELE_RESISTENCIA_PIN, HIGH); // la resistencia se enciende (o mantiene encendida)
    else
      digitalWrite(RELE_RESISTENCIA_PIN, LOW); // si ya se ha cumplido se apaga

    // Cambia la agresividad del PID dependiendo de lo lejos que estemos del objetivo:
    // La razón es que se ponga al máximo enseguida porque la resistencia es muy lenta
    factores_PID();
  }

  // CUMPLIDO EL TIEMPO DEL TEMPORIZADOR, APAGA Y RESETEA VARIABLES
  // ------------------------------------------------
  if (temporizador_end && timer) //TEMPORIZADOR, si es verdadero, ya ha llegado o pasado el tiempo marcado APAGAMOS vent/resist
  {
    if (timer_beep)
      alarma(2); // alarma sonora de programa finalizado

    reset_var(); // al terminar el prorama se resetean todas las variables relevantes //TODO comprobar que esten todas las variables necesarias
    //apagar todo
    digitalWrite(RELE_RESISTENCIA_PIN, LOW);
    digitalWrite(RELE_VENTILADOR_PIN, LOW);

    // secuencia visual del led cuando se termina el programa
    #ifdef NEOPIXEL_LED
    //TODO neo.onPrintCompleted();
    #endif
  }

  // PESOS
  // ------------------------------------------------
  //tara1_cambio = true; // lo manda el nextion
  //tara2_cambio = true; // lo manda el nextion
  //tara1_actual_g = //leer del nextion
  //tara1_actual_g = //leer del nextion

  //if (tara1_cambio){ ultima_tara1=xx; //leer del nextion tara 1..5
  //ultima_tara1 al EEPROM
  //if (tara2_cambio) ultima_tara2=xx; //leer del nextion tara 1..5
  //ultima_tara1 al EEPROM

  //calibrar_factor1();
  //calibrar_factor2();
  //calibrar_peso1();
  //calibrar_peso2();

  if (tara1_cambio || tara2_cambio)
  {
    cambioTara();     //si se cambia la tara del rollo de filamento
    settings->save(); //GUARDAR CAMBIOS EN LA CONFIGURACIÓN
  }

  // leer pesos y temperatura exterior al ritmo marcado por la variable WeightReadingRefreshTime (en ms)
  //calibrar_peso1();
  //calibrar_factor1();
  leerBascula1();
  //calibrar_peso2();
  //calibrar_factor2();
  leerBascula2();
  leer_temp_ext();



  // AUTO APAGADO DEL VENTILADOR
  // ------------------------------------------------
  //Apaga el ventilador en el caso en el que la resistencia apenas se encienda
  //No necesaria la recirculación de aire
  if (vent_auto_off && !temporizador_end)
  {
    unsigned int dutyCycle = (output / WindowSize) * 100;
    if (dutyCycle < VentMinDutyCycle)
      digitalWrite(RELE_VENTILADOR_PIN, LOW); //menos del VentMinDutyCycle
    else
      digitalWrite(RELE_VENTILADOR_PIN, HIGH); //más del VentMinDutyCycle
  }

  // DEBUG
  // ------------------------------------------------
  if (now > serialTime && SERIAL_DEBUG) //send-receive with processing if it's time
  {
    SerialSend();
    serialTime += 10000;
  }

// NEOPIXEL LED
// ------------------------------------------------
// se ejecuta la secuencia de calentamiento con los NeoPixel LED, si no hemos llegado todavia a la temperatura final
#ifdef NEOPIXEL_LED
  if (!HeatingDONE)
    LEDonHeating(start_temp, T_int, target_temp);
#endif

  //FUNCIONES QUE HAY QUE LLAMAR DESDE EL NEXTION
  // CAMBIAR CONFIGURACION DE VARIABLE

  // SERIAL_DEBUG = false / true;  //activar/desactivar serial debug
  //  temp_prog1 = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  temp_prog2 = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  temp_prog3 = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  temp_prog4 = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  temp_prog5 = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  timer = true / false; // CAMBIAR CONFIGURACION DE VARIABLE
  //  set_time_min = xx ; // CAMBIAR CONFIGURACION DE VARIABLE
  //  timer_beep = true / false; // CAMBIAR CONFIGURACION DE VARIABLE
  //  vent_auto_off = true / false; // CAMBIAR CONFIGURACION DE VARIABLE
  //  VentMinDutyCycle = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  calibration_factor1 = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  zero_offset1 = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  calibration_factor2 = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  zero_offset2 = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  vaggKp = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  vaggKi = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  vaggKd = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  aggKp = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  aggKi = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  aggKd = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  consKp = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  consKi = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  consKd = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  WindowSize = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  dht_T_ext_correccion = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  dht_T_int_correccion = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  dht_H_ext_correccion = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  dht_H_int_correccion = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  temp_prog1 = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  temp_prog2 = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  temp_prog3 = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  temp_prog4 = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  temp_prog5 = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  WeightReadingRefreshTime = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  setpoint = xx; // CAMBIAR CONFIGURACION DE VARIABLE

  //  NEOPIXEL_TYPE = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  NEOPIXEL_PIXELS = xx; // CAMBIAR CONFIGURACION DE VARIABLE
  //  if... ledStatus = true/false; // CAMBIAR CONFIGURACION DE VARIABLE
  //"TIPO1",//char name_tara1[10];
  //"TIPO2",//char name_tara2[10];
  //"TIPO3",//char name_tara3[10];
  //"TIPO4",//char name_tara4[10];
  //"TIPO5",//char name_tara5[10];
  //0,      //int peso_tara1;
  //0,      //int peso_tara2;
  //0,      //int peso_tara3;
  //0,      //int peso_tara4;
  //0,      //int peso_tara5;
  //0,      //int ultima_tara1;
  //0,      //int ultima_tara2;

  // settings->save();  //GUARDAR CAMBIOS EN LA CONFIGURACIÓN
  //  writeStringToEEPROM(0, "Hello Arduino");

} //LOOP

//***********************************************************
