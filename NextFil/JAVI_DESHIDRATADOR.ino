#include <HX711.h> //Para las básculas
#include <DHT.h> // Sensores de temperatura y humedad
#include <PID_v2.h> //https://github.com/gelraen/Arduino-PID-Library/
#include <EEPROM.h>

//PRO MINI///////////////////////////////////////////////////
//#define LCD_TX    0   //RX
//#define LCD_RX    1   //TX
//#define NOT_USED  2   //interrupt capable
#define NEOPIXEL_PIN      3   //interrupt, pwm capable
#define ALARMA      4
#define RELE_RESISTENCIA   5   //pwm capable
#define RELE_VENTILADOR    6   //pwm capable
//#define NOT_USED  7   //SOFTWARESERIAL - NEXTION
#define PESO_DATA2  8  
#define PESO_SCK2   9   //pwm capable
#define PESO_SCK    10  //pwm capable, SPI-SS
#define PESO_DATA   11  //pwm capable, SPI-MOSI
#define SWITCH      12  //SPI MISO
//#define NOT_USED  13  //SOFTWARESERIAL - NEXTION
//#define NOT_USED  A0
//#define NOT_USED  A1
#define SENSOR_INT  A2
#define SENSOR_EXT  A3
// A4 PRO MINI SDA I2C 
// A5 PRO MINI SCL I2C
//PRO MINI///////////////////////////////////////////////////
  
//#define ARDUINO_RTC //comentar para desactivar en caso de usar el de nextion
//#define NEXTION //comentar si se usa otra pantalla (habria que programarlo)
#define NEOPIXEL_LED //comentar si no hay tira LED

// NEXTION
// ------------------------------------------------
#ifdef NEXTION
  #include <SoftwareSerial.h> //Para no usar el mismo SERIAL que el programador del arduino
  #include "EasyNextionLibrary.h" //https://www.arduino.cc/reference/en/libraries/easy-nextion-library/
  #include "trigger.h" //EasyNextionLibrary
  
  SoftwareSerial nxSerial(7, 13); 
  /*Para no usar el mismo SERIAL que el programador del arduino usamos SoftwareSerial*/
  /*Arduino mini pro 7 -> nextion TX   13-> Nextion RX*/
  EasyNex myNex(nxSerial);  // Create an object of EasyNex class with the name < myNex > 
                          // Set as parameter the Hardware Serial you are going to use

  uint32_t DATA_REFRESH_RATE=1000; // The time between each Data refresh of the page
                                 // Depending on the needs of the project, the DATA_REFRESH_RATE can be set
                                 // to 50ms or 100ms without a problem. In this example, we use 1000ms, 
                                 // as DHT sensor is a slow sensor and gives measurements every 2 seconds
  uint32_t pageRefreshTimer = millis(); // Timer for DATA_REFRESH_RATE
  bool newPageLoaded = false; // true when the page is first loaded ( lastCurrentPageId != currentPageId )                          
#endif      


// RGB LED Support for Adafruit NeoPixel LED driver
// ------------------------------------------------
#ifdef NEOPIXEL_LED
  #include <Adafruit_NeoPixel.h>
#endif

// SETTINGS 
// Aqui se definen los parámetros por defecto
// ------------------------------------------------
#include "settings.h" //incluir settings .h y .cpp

// Definir las estructura defaults y la guarda en la memoria de programa no en la dinamica!
// Estos valores son constantes y no se pueden cambiar, solo en el sketch
// Asi nos ahorramos muchisima memoria dinámica

//DEFINICIÓN DE DEFAULTS - SE GUARDAN EN PROGMEM PARA AHORRAR RAM
const ParametersType defaults PROGMEM = { 
  false,  //bool userSettings; Si se hace cambios en default se guardan todos los datos en setUser y esta variable se convierte en true  
  true,   //bool SERIAL_DEBUG; //0 off / 1 on PONER A 0 para producto final
  "PLA",  //char name_prog1[6];
  "PETG", //char name_prog2[6];
  "ABS",  //char name_prog3[6];
  "NYL",  //char name_prog4[6];
  "FLEX", //char name_prog5[6];   
  10,     //byte temp_prog1 ºC
  40,     //byte temp_prog2 ºC
  50,     //byte temp_prog3 ºC
  60,     //byte temp_prog4 ºC
  70,     //byte temp_prog5 ºC
  true,   //bool timer - si se activa por defecto el temporizador
  180,    //unsigned int set_time_min;
  true,   //bool timer_beep;
  true,   //bool vent_auto_off;
  1,      //byte VentMinDutyCycle; % Porcentaje de duty cycle en el que se apaga el ventilador
  270.00f,    //float calibration_factor1; // con la calibracion
  -253970,//long zero_offset1; // con la calibracion de factor se obtiene, cuando se sabe el peso sin carga
  false,   //bascula1_calibrada);  
  0,      //float calibration_factor2; 
  0,      //long zero_offset1;
  false,   //bascula2_calibrada);  
  100.0,  //float vaggKp; //float very aggressive Kp
  4.0,    //float vaggKi; //float very aggressive Ki
  20.0,   //float vaggKd; //float very aggressive Kd
  20.0,   //float aggKp; //float aggressive Kp
  1.0,    //float aggKi; //float aggressive Ki  
  5.0,    //float aggKd; //float aggressive Kd
  1.0,    //float consKp; //float conservative Kp
  0.05,   //float consKi; //float conservative Ki
  0.25,   //float consKd; //float conservative Kd
  5000,   //unsigned int WindowSize intervalos en ms 100% duty cycle = WindowSize. Time proportioning control,  It's a really slow version of PWM
  3.0,    //float dht_T_ext_correccion; 
  0.0,    //float dht_T_int_correccion; 
  19.7,   //float dht_H_ext_correccion; 
  0.0,    //float dht_H_int_correccion;
  120,    //unsigned int time_prog1; 2h en min
  180,    //unsigned int time_prog2; 3h en min
  240,    //unsigned int time_prog3; 4h en min
  360,    //unsigned int time_prog4; 6h en min
  600,    //unsigned int time_prog5; 10h  en min
  5000,   //unsigned int (ms) WeightReadingRefreshTime cada cuanto se refresca el peso en ms
  60,    //byte setpoint ºC
  NEO_GRB + NEO_KHZ800,   //0x52 = NEO_GBR + KHZ800 abajo la tabla:
  7,      //byte neopixel_pixels
  true,   //bool ledStatus
  "TIPO1",//char name_tara1[10];
  "TIPO2",//char name_tara2[10];
  "TIPO3",//char name_tara3[10];
  "TIPO4",//char name_tara4[10];
  "TIPO5",//char name_tara5[10];   
  0,      //int peso_tara1;
  0,      //int peso_tara2;
  0,      //int peso_tara3;
  0,      //int peso_tara4;
  0,      //int peso_tara5;
  0,      //int ultima_tara1; la configuracion usada por ultima vez
  0,      //int ultima_tara2; la configuracion usada por ultima vez
  SETTING_VER
};

// tenemos 3 copias de la estructura de configuracion. 
// 1. MEMPROG - defaults, constante definido en sketch
// 2. EEPROM - configuración de usuario, que se puede modificar
// 3. RAM - donde se carga la configuración seleccionada 1 o 2.

// actSettings es la instancia de la estructura en RAM, los valores se leen directamente de la estructura
// sin necesidad de declarar variables. Los valores en esta estructura son los seleccionados por el usuario
// Dos opciones. Por defecto que se guardan en la PROGMEM y definidos por usuario

Settings settings(defaults); //Iniciar settings usando los valores de la estructura defaults
ParametersType actSettings = defaults; //Nueva instancia de la estructura.Copia el contenido de defaults en actSettings en RAM



//DEFINICIÓN DE DEFAULTS


/*
 * // RGB NeoPixel permutations; white and red offsets are always same
// Offset:     / HEX representation
#define NEO_RGB  0x06
#define NEO_RBG  0x09
#define NEO_GRB  0x52
#define NEO_GBR  0xA1
#define NEO_BRG  0x58
#define NEO_BGR  0xA4

// RGBW NeoPixel permutations; all 4 offsets are distinct
// Offset:      / HEX representation
#define NEO_WRGB  0x1B
#define NEO_WRBG  0x1E
#define NEO_WGRB  0x27
#define NEO_WGBR  0x36
#define NEO_WBRG  0x2D
#define NEO_WBGR  0x39

#define NEO_RWGB  0x4B
#define NEO_RWBG  0x4E
#define NEO_RGWB  0x87
#define NEO_RGBW  0xC6
#define NEO_RBWG  0x8D
#define NEO_RBGW  0xC9

#define NEO_GWRB  0x63
#define NEO_GWBR  0x72
#define NEO_GRWB  0x93
#define NEO_GRBW  0xD2
#define NEO_GBWR  0xB1
#define NEO_GBRW  0xE1

#define NEO_BWRG  0x6C
#define NEO_BWGR  0x78
#define NEO_BRWG  0x9C
#define NEO_BRGW  0xD8
#define NEO_BGWR  0xB4
#define NEO_BGRW  0xE4

#define NEO_KHZ800 0x0000 
#define NEO_KHZ400 0x0100 
 */


// RGB LED Support for Adafruit NeoPixel LED driver
// ------------------------------------------------
#ifdef NEOPIXEL_LED
  #define NEOPIXEL_IS_SEQUENTIAL   // Sequential display for temperature change - LED by LED. Disable to change all LEDs at once.
  #define NEOPIXEL_BRIGHTNESS 127  // Initial brightness (0-255)
  #define NEOPIXEL_STARTUP_TEST    // Cycle through colors at startup
  // Use a single NeoPixel LED for static (background) lighting
  #define NEOPIXEL_BKGD_LED_INDEX  0               // Index of the LED to use
  #define NEOPIXEL_BKGD_COLOR { 255, 255, 255, 0 } // R, G, B, W

  //NEO_KHZ800 NEO_KHZ400
  // Rather than declaring the whole NeoPixel object here, we just create
  // a pointer for one, which we'll then allocate later
  // It's helpful if you don't know NeoPixel settings at compile time or
  // just want to store this settings in EEPROM or a file on an SD card....
  Adafruit_NeoPixel *neo;
 
  byte NEOPIXEL_PIXELS;
  bool ledStatus;
  bool ledFlip=false;
  bool LEDHeatingDONE=false;
  uint32_t ledTime;  
  uint32_t ledTimeInterval = 300; //Tiempo de cambio de los LED ms
  
  /**
   * Color helpers and presets
   */
   
  uint32_t colorOff    =   neo->Color(  0,   0,   0);
  uint32_t colorRed    =   neo->Color(255,   0,   0);
  uint32_t colorOrange =   neo->Color(255,  80,   0);
  uint32_t colorYellow =   neo->Color(255, 255,   0);
  uint32_t colorGreen  =   neo->Color(  0, 255,   0);
  uint32_t colorBlue   =   neo->Color(  0,   0, 255);
  uint32_t colorIndigo =   neo->Color(  0, 255, 255);
  uint32_t colorViolet =   neo->Color(255,   0, 255);
  uint32_t colorWhite  =   neo->Color(255, 255, 255);

  uint16_t hue = 65536/3; //Color Verde
#endif

// SERIAL DEBUG
// ------------------------------------------------
bool SERIAL_DEBUG;
uint32_t serialTime;
char received; //para la funcion SerialReceive

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
float input, output; 
byte setpoint;
//Define the very aggressive, aggressive and conservative Tuning Parameters
float vaggKp, vaggKi, vaggKd;
float aggKp, aggKi, aggKd;
float consKp, consKi, consKd;
uint16_t  WindowSize;
uint32_t  windowStartTime;
bool HeatingDONE = false;
PID_v2 myPID(consKp, consKi, consKd, PID::Direct); //Specify the links and initial tuning parameters

// BASCULAS Y PESOS
// ------------------------------------------------
#define RESOLUTION  1     //decimal places of HX711 reading TO BE SHOWN (actual reading is float)
#define AVG_FACT 30        //how many reads for averaging factor of HX711
uint32_t WeightReadingRefreshTime; //ms entre lecturas de peso y temp. exterior (minimo 2seg)
uint32_t LastWeightTime = 0; 
uint16_t peso_tara1;
uint16_t peso_tara2;
uint16_t peso_tara3;
uint16_t peso_tara4;
uint16_t peso_tara5;
uint16_t ultima_tara1;
uint16_t ultima_tara2;

//bascula 1
float calibration_factor1;
int32_t zero_offset1; 
int32_t tara1_actual_g;
int32_t tara1_actual_offset;
volatile float PESO1 = 0.0f;
bool bascula1_conectada = true; //por defecto conectada
bool tara1_cambio = true; //cuando cuambias de tara (rollo de filamento distinto) cambiar a false para produccion
bool bascula1_calibrada = false;
HX711 bascula1; // Initialize HX711 sensor.

//bascula 2
float calibration_factor2;
int32_t zero_offset2; 
int32_t tara2_actual_g;
int32_t tara2_actual_offset;
volatile float PESO2 = 0.0f;
bool bascula2_conectada = true; //por defecto conectada
bool tara2_cambio = false; //cuando cuambias de tara (rollo de filamento distinto)
bool bascula2_calibrada = false;
HX711 bascula2; // Initialize HX711 sensor.

// SENSORES DE TEMPERATURA Y HUMEDAD
// ------------------------------------------------
//sensor exterior
float dht_T_ext_correccion;
float dht_H_ext_correccion;
float T_ext;
float H_ext;
DHT dht_ext (SENSOR_EXT, DHT11); // Initialize DHT sensor DHT 11.

//sensor interior
float dht_T_int_correccion; //el sensor AM2301 es mas preciso, el otro se corrige
float dht_H_int_correccion; //el sensor AM2301 es mas preciso, el otro se corrige
float T_int;
float H_int;
DHT dht_int (SENSOR_INT, DHT21); // Initialize DHT sensor DHT 21 (AM2301).

float start_temp;
float target_temp;
float current_temp;

uint32_t tempTime;
uint32_t tempTimeInterval = 4000; //Tiempo entre lecturas del sensor de temperatura interior

//temperaturas programadas
uint8_t temp_prog1;
uint8_t temp_prog2;
uint8_t temp_prog3;
uint8_t temp_prog4;
uint8_t temp_prog5;

// VENTILADOR
// ------------------------------------------------
bool vent_auto_off;
uint8_t VentMinDutyCycle; //cuando el dutyCycle de la resistencia es menor que este numero el ventilador se apaga

// TEMPORIZADOR Y TIEMPOS
// ------------------------------------------------
uint16_t set_time_min; //Temporizador en minutos
bool timer; //activar/desactivar temporizador
bool timer_beep; //señal acustica de fin de programa - temporizador

uint32_t now = 0; //ms
uint32_t startTime = 0; //ms
uint32_t totalTime = 0; //ms
bool temporizador_end = false; //Para saber si el programa /temporizador ha terminado
bool empezar_programa = false; //Cuando empieza el temporizador para iniciar variables iniciales

//temporizadores programados
uint16_t time_prog1;
uint16_t time_prog2;
uint16_t time_prog3;
uint16_t time_prog4;  
uint16_t time_prog5;  


/*----------------------------------------------------------------------------------*/
/*----------------------------------- SETUP ----------------------------------------*/
/*----------------------------------------------------------------------------------*/
void setup() 
{
  SERIAL_DEBUG = true;
  if (SERIAL_DEBUG){ 
    Serial.begin(9600);
    Serial.setTimeout(10); //Para la insercion de numeros en serial
    while (!Serial){}
  }
  
  //userSettings =  settings.parameters->userSettings; //Leer de la EEPROM para saber que configuración cargar
  bool userSettings;
  //EEPROM.get(0,userSettings); //userSettings es la primera variable de la estructura
  userSettings = true; // borrar para produccion leer de la EEPROM !---------------------------------------------------
  // borrarEEPROM(); 

  //copiar contenidos de default de PROGMEM A RAM (actSettings)
  memcpy_P(&actSettings,&defaults,sizeof defaults); 
  //si userSettings=false, se copian los valores defaults(PROGMEM) a actSettings(RAM), =true se copian los valores de usuario(EEPROM)
  settings.readSettings(actSettings,userSettings);

  // DEBUG
  // ------------------------------------------------  
  if (SERIAL_DEBUG){  
    //EEPROM
    //Print length of data to run CRC on EEPROM.
    Serial.print(F("Longitud EEPROM: "));
    Serial.println(EEPROM.length());
    //Print the result of calling eeprom_crc()
    Serial.print(F("CRC32 of EEPROM data: 0x"));
    Serial.println(eeprom_crc(), HEX);
    Serial.print(F("\nCRC Terminado!\n"));
    Serial.print("Configuracion actual - ");
    settings.printSerial(); //muestra toda los datos guardados de configuración en RAM - actSettings
  } 

  
  //nombre de los programas;
  char name_prog1[6];
  char name_prog2[6];
  char name_prog3[6];
  char name_prog4[6];
  char name_prog5[6];
  
  char name_tara1[10];
  char name_tara2[10];
  char name_tara3[10];
  char name_tara4[10];
  char name_tara5[10];
  
  // NEXTION
  // ------------------------------------------------
  #ifdef NEXTION
    myNex.begin(115200); // starting the serial port at 115200. NEXTION MUST HAVE THE SAME RATE. For this we write at
                           //  first page to the preinitialize event the command @  baud=115200  @ 
                           // NOTE "bauds" will change the default baud rate off 9600 until it changed again
    uint8_t nxPagina=0;           //pagina inicial
    delay(500);                   //give Nextion some time to finish initialize
    myNex.writeStr("page 0");     //ir a la pagina inicial en caso de que Arduino se resetee
    delay(50);
    myNex.lastCurrentPageId = 1;  // At the first run of the loop, the currentPageId and the lastCurrentPageId
                                    // must have different values, due to run the function firstRefresh()
  #endif 
  
  // NEOPIXEL RGB
  // ------------------------------------------------
  // Right about here is where we could read 'pin', 'numPixels' and/or
  // 'pixelFormat' from EEPROM or a file on SD or whatever. This is a simple
  // example and doesn't do that -- those variables are just set to fixed
  // values at the top of this code -- but this is where it would happen.
  #ifdef NEOPIXEL_LED
    // leer de la EEPROM
    int NEOPIXEL_TYPE;
    NEOPIXEL_TYPE = settings.parameters->neopixel_type;
    NEOPIXEL_PIXELS = settings.parameters->neopixel_pixels;     
    ledStatus = settings.parameters->ledStatus; 
    
    NEOPIXEL_TYPE = NEO_GRB + NEO_KHZ800;
    NEOPIXEL_PIXELS = 7;     
    ledStatus = true; 
    // Then create a new NeoPixel object dynamically with these values:
    neo = new Adafruit_NeoPixel(NEOPIXEL_PIXELS, NEOPIXEL_PIN, NEOPIXEL_TYPE); //crear instancia ya reservada con el pointer *neo 
    neo->begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
    neo->clear(); // Set all pixel colors to 'off''
    LEDBienvenida(); //LED de bienvenida
  #endif


  // INICIAR PINES
  // ------------------------------------------------  
  pinMode(ALARMA, OUTPUT); // Set buzzer 
  pinMode(RELE_RESISTENCIA, OUTPUT); //rele de la resistencia
  pinMode(RELE_VENTILADOR, OUTPUT); //rele del ventilador

  // LEER PARÁMETROS DE LA CONFIGURACIÓN GUARDADA
  // ------------------------------------------------    
  userSettings = settings.parameters->userSettings;  
  SERIAL_DEBUG = settings.parameters->SERIAL_DEBUG; 
  name_prog1[6] = settings.parameters->name_prog1;
  name_prog2[6] = settings.parameters->name_prog2;
  name_prog3[6] = settings.parameters->name_prog3;
  name_prog4[6] = settings.parameters->name_prog4;
  name_prog5[6] = settings.parameters->name_prog5;
  temp_prog1 = settings.parameters->temp_prog1;
  temp_prog2 = settings.parameters->temp_prog2;
  temp_prog3 = settings.parameters->temp_prog3;
  temp_prog4 = settings.parameters->temp_prog4;
  temp_prog5 = settings.parameters->temp_prog5;
  timer = settings.parameters->timer;
  set_time_min = settings.parameters->set_time_min;
  timer_beep = settings.parameters->timer_beep;
  vent_auto_off = settings.parameters->vent_auto_off;
  VentMinDutyCycle = settings.parameters->VentMinDutyCycle;
  calibration_factor1 = settings.parameters->calibration_factor1; 
  zero_offset1 = settings.parameters->zero_offset1;
  bascula1_calibrada = settings.parameters->bascula1_calibrada;
  calibration_factor2 = settings.parameters->calibration_factor2; 
  zero_offset2 = settings.parameters->zero_offset2;
  bascula2_calibrada = settings.parameters->bascula2_calibrada;
  vaggKp = settings.parameters->vaggKp; 
  vaggKi = settings.parameters->vaggKi;
  vaggKd = settings.parameters->vaggKd; 
  aggKp = settings.parameters->aggKp;
  aggKi = settings.parameters->aggKi; 
  aggKd = settings.parameters->aggKd; 
  consKp = settings.parameters->consKp; 
  consKi = settings.parameters->consKi;
  consKd = settings.parameters->consKd; 
  WindowSize = settings.parameters->WindowSize;
  dht_T_ext_correccion = settings.parameters->dht_T_ext_correccion; 
  dht_T_int_correccion = settings.parameters->dht_T_int_correccion; 
  dht_H_ext_correccion = settings.parameters->dht_H_ext_correccion; 
  dht_H_int_correccion = settings.parameters->dht_H_int_correccion; 
  time_prog1 = settings.parameters->time_prog1; 
  time_prog2 = settings.parameters->time_prog2;
  time_prog3 = settings.parameters->time_prog3;
  time_prog4 = settings.parameters->time_prog4;
  time_prog5 = settings.parameters->time_prog5;
  WeightReadingRefreshTime = settings.parameters->WeightReadingRefreshTime;
  setpoint = settings.parameters->setpoint;
  name_tara1[10] = settings.parameters->name_tara1;
  name_tara2[10] = settings.parameters->name_tara2;
  name_tara3[10] = settings.parameters->name_tara3;
  name_tara4[10] = settings.parameters->name_tara4;
  name_tara5[10] = settings.parameters->name_tara5;
  peso_tara1 = settings.parameters->peso_tara1; 
  peso_tara2 = settings.parameters->peso_tara2; 
  peso_tara3 = settings.parameters->peso_tara3; 
  peso_tara4 = settings.parameters->peso_tara4; 
  peso_tara5 = settings.parameters->peso_tara5;     
  ultima_tara1 = settings.parameters->ultima_tara1;     
  ultima_tara2 = settings.parameters->ultima_tara2;      

  // INICIAR EL SISTEMA
  // ------------------------------------------------    
  //iniciar en modo seguro (ventilador off, resitencia off)
  digitalWrite(RELE_RESISTENCIA, LOW);
  digitalWrite(RELE_VENTILADOR,LOW);

  //iniciar bascula 1
  bascula1.begin(PESO_DATA, PESO_SCK);
  bascula1.power_down();
  bascula1.power_up();
  bascula1.set_scale(calibration_factor1);
  bascula1.set_offset(zero_offset1); //Zero out the scale using a previously known zero_factor

  //iniciar bascula 2
  bascula2.begin(PESO_DATA2, PESO_SCK2);
  bascula2.power_down();  
  bascula2.power_up();
  bascula2.set_scale(calibration_factor2);
  bascula2.set_offset(zero_offset2); //Zero out the scale using a previously known zero_factor

  //iniciar sensores de temperatura
  dht_ext.begin();
  dht_int.begin();  
  leer_temp_int(); // Primera lectura
  leer_temp_ext(); // Primera lectura

  //iniciar tiempos
  windowStartTime = millis(); // para el PID
  startTime = millis();       //para el temporizador

  //Ajustar y encender el PID:
  myPID.SetOutputLimits(0, WindowSize); // Ajustamos para que el PID vaya entre 0 y la WindowSize.
  myPID.Start(input,output,setpoint);   // Encender el PID

  //Bienvenida al iniciar correctamente
  //alarma(0); //DESCOMENTAR

  

  // RTC CHECK - RELOJ
  // ------------------------------------------------
  #ifdef ARDUINO_RTC
    if (! rtc.begin()) {
      if (SERIAL_DEBUG) {Serial.print(F("RTC no encontrado o sin batería\n")); Serial.flush(); }
      alarma(3); 
      delay(1500);
    }
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    if (! rtc.isrunning()) {
      if (SERIAL_DEBUG) Serial.println(F("RTC is NOT running!"));
      // following line sets the RTC to the date & time this sketch was compiled from computer
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
  #endif

//  settings.parameters->userSettings=true; 
//  settings.save(actSettings);
}

/*----------------------------------------------------------------------------------*/
/*----------------------------------- LOOP -----------------------------------------*/
/*----------------------------------------------------------------------------------*/
void loop() 
{
  // LEER TIEMPO Y COMPROBAR TEMPORIZADOR
  // ------------------------------------------------  
  now = millis(); //Iniciar variable now en cada bucle
  bool temporizador_end = false;
  if (timer) {temporizador_end = end_time();}
  Serial.print("temp_prog1 RAM: "); 
  Serial.println(settings.parameters->temp_prog1); 

  
  // LEER ENTRADAS DE LA PANTALLA NEXTION
  // ------------------------------------------------ 
  #ifdef NEXTION
    myNex.NextionListen(); // This function must be called repeatedly to response touch events in loop
    
    if((millis() - pageRefreshTimer) > DATA_REFRESH_RATE){
      myNex.RefreshCurrentPage();
      pageRefreshTimer = millis();
    } 

    //Cargar por primera vez la pagina al cambiar a ella
    if(myNex.currentPageId != myNex.lastCurrentPageId){ // If the two variables are different, means a new page is loaded.
      newPageLoaded = true;    // A new page is loaded
                               // This variable is used as an argument at the if() statement on the NX._refreshPageXX() voids, 
                               // in order when is true to update all the values on the page with their current values
                               // with out run a comparison with the last value.
      myNex.RefreshCurrentPage();   
      newPageLoaded = false;  // After we have updated the new page for the first time, we update the variable to false.
                            // Now the values updated ONLY if the new value is different from the last Sent value.
                            // See void _refreshPage0()   
      myNex.lastCurrentPageId = myNex.currentPageId; // Afer the refresh of the new page We make them equal,
                                                   // in order to identify the next page change.
    }
  #endif
  
  // RTC - RELOJ
  // ------------------------------------------------ 
  #ifdef ARDUINO_RTC
    DateTime ahora = rtc.now();
    hora_actual = ahora.hour(), DEC;
    minuto_actual = ahora.minute(), DEC;
    if (SERIAL_DEBUG){
      Serial.print(F("Reloj RTC: "));
      Serial.print(hora_actual, DEC);
      Serial.print(F(":"));
      Serial.print(minuto_actual, DEC);
      Serial.println();
    }
  #endif

  // LEER TEMPERATURA
  // ------------------------------------------------
  leer_temp_int();
  input = T_int; //TEMPERATURA REAL DEL SENSOR
  current_temp = T_int;

  // LECTURAS INICIALES AL EMPEZAR UN PROGRAMA / TEMPORIZADOR
  // ------------------------------------------------

  if (empezar_programa){
    empezar_programa = false;    
    target_temp = setpoint;
    start_temp = T_int;    
  }
   
  // PID
  // ------------------------------------------------ 
  output = myPID.Run(input);
  if (! temporizador_end || ! timer )  // TEMPORIZADOR , ejecuta si falso, si no ha llegado el tiempo marcado
  {
    /************************************************
     "PWM TO RELAY ON/OFF TIME"
     turn the output pin on/off based on pid output logic that translates the PID output into "Relay On Time" with the remainder of the
     window being "Relay Off Time" Convierte el ciclo PWM a tiempo de 0 a WindowSize (100%) Enciende y apaga en consecuencia
    *************************************************/
    uint32_t CurrentWindow = now - windowStartTime;
    if (CurrentWindow > WindowSize) windowStartTime += WindowSize; //time to shift the Relay Window
    if (output > CurrentWindow) digitalWrite(RELE_RESISTENCIA, HIGH);
    else digitalWrite(RELE_RESISTENCIA, LOW);    
    /************************************************/
    
    //LED
    #ifdef NEOPIXEL_LED
      start_temp = T_int; //solo para debug
      target_temp = setpoint;//solo para debug
      if (SERIAL_DEBUG){
        Serial.print(F("start_temp "));
        Serial.println(start_temp);
        Serial.print(F("current_temp "));
        Serial.println(current_temp);
        Serial.print(F("target_temp "));
        Serial.println(target_temp);        
      } 
      if (current_temp+1 > target_temp){
        LEDHeatingDONE=true;  //1º de margen
        HeatingDONE=true;
      }
      if (ledStatus) LEDonHeating(start_temp, current_temp, target_temp);
    #else 
      if (T_int+1 > setpoint) HeatingDONE=true; //1º de margen
    #endif
    if (SERIAL_DEBUG){ Serial.print(F("HeatingDONE ")); Serial.println(HeatingDONE);}
    // Cambia la agresividad del PID dependiendo de lo lejos que estemos del objetivo:
    // La razón es que se ponga al máximo enseguida porque la resistencia es lenta
    // ------------------------------------------------------------------------------- 
    float gap = abs(setpoint - input); //distance away from setpoint
    if (gap < 5){                   myPID.SetTunings(consKp, consKi, consKd);}     //we're 5º close to setpoint, use conservative tuning parameters
    else if (gap >= 5 && gap <=10){ myPID.SetTunings(aggKp, aggKi, aggKd);}        //we're between 5 and 10º far from setpoint, use aggressive tuning parameters
    else {                          myPID.SetTunings(vaggKp, vaggKi, vaggKd);}     //> 10º far from setpoint, very aggressive tuning
      
  } 
  
  // CUMPLIDO EL TIEMPO DEL TEMPORIZADOR, APAGA
  // ------------------------------------------------    
  else if (temporizador_end && timer) //TEMPORIZADOR, si es verdadero, ya ha llegado o pasado el tiempo marcado APAGAMOS vent/resist
  {   
    if (timer_beep) alarma(2); //alarma de programa finalizado
    reset_var();
    #ifdef NEOPIXEL_LED
      //neo.onPrintCompleted();
    #endif
    //apagar todo
    digitalWrite(RELE_RESISTENCIA, LOW); 
    digitalWrite(RELE_VENTILADOR, LOW);
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
  
  if (tara1_cambio || tara2_cambio){
    cambioTara(); //si se cambia la tara del rollo de filamento
    settings.save(actSettings);  //GUARDAR CAMBIOS EN LA CONFIGURACIÓN
  }
  
  if (now > LastWeightTime + WeightReadingRefreshTime) //leer pesos y temperatura exterior al ritmo marcado
  {
    leerBascula1();
    leerBascula2();
    leer_temp_ext();
    LastWeightTime = now;
  }

  // AUTO APAGADO DEL VENTILADOR
  // ------------------------------------------------   
  //Apaga el ventilador en el caso en el que la resistencia apenas se encienda  
  //No necesaria la recirculación de aire
  if (vent_auto_off && ! temporizador_end) {
    unsigned int dutyCycle = (output / WindowSize)*100;
    if (dutyCycle<VentMinDutyCycle) digitalWrite(RELE_VENTILADOR, LOW); //menos del VentMinDutyCycle
    else digitalWrite(RELE_VENTILADOR, HIGH); //más del VentMinDutyCycle    
  }

  // DEBUG
  // ------------------------------------------------   
  if (now > serialTime && SERIAL_DEBUG)   //send-receive with processing if it's time
  {
    //received = SerialReceive();
    SerialSend();
    serialTime += 1000;
  }
  
  if (SERIAL_DEBUG){
    received = SerialReceive();
    if (received == 's'){
      settings.parameters->temp_prog1 = 150;
      Serial.println(F("\n------------------------------------------------------------------------------"));
      Serial.println(F("-------------------------------------Saving--------------------------------------"));
      settings.save(actSettings);
    }
  }


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


// settings.save();  //GUARDAR CAMBIOS EN LA CONFIGURACIÓN
//  writeStringToEEPROM(0, "Hello Arduino");

} //LOOP

//***********************************************************


//***********************************************************
//               
//                   FUNCIONES LED
//***********************************************************

// FUNCIONES AUXILIARES

float fmap (float sensorValue, float sensorMin, float sensorMax, float outMin, float outMax) {
  return (sensorValue - sensorMin) * (outMax - outMin) / (sensorMax - sensorMin) + outMin;
}

inline uint16_t convert_currentTemp_HUE(const float &start, const float &current, const float &target) {
  if (SERIAL_DEBUG){
    Serial.print(F("convert start"));  Serial.println(start);
    Serial.print(F("convert current"));  Serial.println(current);
    Serial.print(F("convert target"));  Serial.println(target);
  }
  //Funcion fmap (float map) para no tener saltos tan grandes y que el cambio de color sea mas gradual
  return (float)fmap(current, start, target, 65536/3, 0.0);  //HUE:Verde -> Rojo = 65536/3 -> 0
}

inline void set_hue(uint16_t hue, bool flip, bool heatingDONE) {
 //HUE - Neopixel
 // Cian:     65536/2 
 // Verde:    65536/3
 // Amarillo: 65536/6 
 // Rojo:     0
 // Magenta:  65536x5/6 
 // Azul:     65536x2/3
 
  uint32_t rgbcolor = neo->ColorHSV(hue);
  uint32_t rgbcolor_alt = neo->ColorHSV(hue-3000);

  if (SERIAL_DEBUG){
    Serial.print(F("HUE "));  Serial.println(hue);
    Serial.print(F("NEOPIXEL_PIXELS "));  Serial.println(NEOPIXEL_PIXELS);
    Serial.print(F("now "));  Serial.println(now);
    Serial.print(F("serialTime "));  Serial.println(serialTime);
    Serial.print(F("ledFlip "));  Serial.println(ledFlip);    
    Serial.print(F("rgbcolor "));  Serial.println(rgbcolor);    
    Serial.print(F("rgbcolor_alt "));  Serial.println(rgbcolor_alt);        
  }

  if (now > serialTime){  // Set tiempo
    if (!heatingDONE){ //modo de cambio de color
      for (int i=0;i<NEOPIXEL_PIXELS;i++){ 
        if (flip){ //Para que cambien de color par/impar - como calentado en proceso
          ledTime += ledTimeInterval; //Tiempo de cambio ms
          if ((i % 2) == 0) neo->setPixelColor(i,rgbcolor); //Pixel par
          else neo->setPixelColor(i,rgbcolor_alt); //Pixel impar      
        } 
        else {
          ledTime += ledTimeInterval;//Tiempo de cambio ms
          if ((i % 2) == 0) neo->setPixelColor(i,rgbcolor_alt); //Pixel impar
          else neo->setPixelColor(i,rgbcolor); //Pixel par          
        }
      } 
      ledFlip = !ledFlip;
    } else { //Calentado terminado - color fijo, dimming..
      for (int e=0;e<3;e++){ //repetir 3 veces
        uint8_t value=155;
        for (value;value>0;value--){ //fade out
          rgbcolor = neo->ColorHSV(hue,255,value);
          neo->fill(rgbcolor,0,NEOPIXEL_PIXELS);
          delay(10);
          neo->show();
        }
      }
      LEDHeatingDONE = false;
      HeatingDONE = true;
    }
  }
}
       

//PROGRAMAS LED

void LEDonHeating (const float &start,  const float &current, const float &target) {
  uint16_t HueActual = convert_currentTemp_HUE(start, current, target); 
  if (SERIAL_DEBUG){
    Serial.print(F("HueActual"));  
    Serial.println(HueActual);
  }
  set_hue(HueActual,ledFlip, LEDHeatingDONE);
  neo->show();
}


void LEDBienvenida (){
  uint32_t hue_;
  uint32_t rgbcolor;
  for (hue_=0;hue_<65500;hue_+=20){ //fade out
    rgbcolor = neo->ColorHSV(hue_);
    neo->fill(rgbcolor,0,NEOPIXEL_PIXELS);
    neo->show();
  }
  /*for (hue_=65535;hue_>51;hue_-=20){ //fade out
    rgbcolor = neo->ColorHSV(hue_);
    neo->fill(rgbcolor,0,NEOPIXEL_PIXELS);
    neo->show();
  }*/
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


//***********************************************************
//               OK PROBADO
//                     LEER TEMPERATURA INTERIOR
//***********************************************************

void leer_temp_int(){

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  
  if (now > tempTime){ 

    H_int = dht_int.readHumidity() + dht_H_int_correccion;
    T_int = dht_int.readTemperature() + dht_T_int_correccion;
    // Check if any reads failed and exit early (to try again).
    if (isnan(H_int) || isnan(T_int)) {
      if (SERIAL_DEBUG) Serial.println(F("Failed to read from DHT sensor!"));
      digitalWrite(RELE_RESISTENCIA, LOW); //apagar la resistencia
      alarma(1); //error crítico sensor de temperatura
      return;
    }
    if (SERIAL_DEBUG){
      Serial.print(F("Humedad int: "));
      Serial.print(H_int);
      Serial.print(F("%  Temp. int: "));
      Serial.print(T_int);
      Serial.println(F("°C "));    
    }
    tempTime += tempTimeInterval; //cuanto tiempo entre lecturas ms
  }
}

//***********************************************************
//               OK PROBADO
//                LEER TEMPERATURA EXTERIOR
//***********************************************************
void leer_temp_ext(){
    H_ext = dht_ext.readHumidity() + dht_H_ext_correccion;
    T_ext = dht_ext.readTemperature() + dht_T_ext_correccion;  
    // Check if any reads failed and exit early (to try again).
    if (isnan(H_ext) || isnan(T_ext) ) {
      if (SERIAL_DEBUG) Serial.println(F("Failed to read from DHT sensor!"));
      digitalWrite(RELE_RESISTENCIA, LOW); //apagar la resistencia
      alarma(1); //error crítico sensor de temperatura
      return;
    }  
    if (SERIAL_DEBUG){
      Serial.print(F("Humedad ext: "));
      Serial.print(H_ext);
      Serial.print(F("%  Temp. ext: "));
      Serial.print(T_ext);
      Serial.println(F("°C "));
    }  

}

//************************************************************
//************************* READ HX711 ***********************
//************************************************************

void leerBascula1() { 
  if (bascula1.wait_ready_retry(10)) //Comprueba que este conectado el HX711
  { 
    bascula1_conectada = true;
    PESO1= bascula1.get_units(AVG_FACT); //scale.get_units() returns a float
    if (SERIAL_DEBUG){
      Serial.print(F("Peso1: "));
      Serial.print(PESO1,RESOLUTION); 
      Serial.print(F(" gramos"));  //grams or kilograms depending on load cell 
      Serial.println();
      return;
    }
  } else {
    if (SERIAL_DEBUG) Serial.println(F("Lectura - Sensor HX711 (1) no conectado"));
    bascula1_conectada = false;
    return;
  }
}

void leerBascula2() {

  if (bascula1.wait_ready_retry(10)) { //Comprueba que este conectado el HX711
    bascula2_conectada = true; 
    PESO2= bascula2.get_units(AVG_FACT); //scale.get_units() returns a float
    if (SERIAL_DEBUG){     
      Serial.print(F("Peso2: "));    
      Serial.print(PESO2, RESOLUTION); 
      Serial.print(F(" gramos"));  //grams or kilograms depending on load cell
      Serial.println();
      return;
    }
  } else {
    if (SERIAL_DEBUG) Serial.println(F("Lectura - Sensor HX711 (2) no conectado"));
    bascula2_conectada = false;
    return;
  }
}


//***********************************************************
// CAMBIO DE TARA
//***********************************************************

void cambioTara() {
  if (tara1_cambio)
  {
    //tara1_actual_g = ; //en gramos, LEER DE  nextion
    tara1_actual_offset = tara1_actual_g * calibration_factor1;
    bascula1.set_offset(zero_offset1 + tara1_actual_offset); //Zero out the scale using a previously known zero_factor  
    tara1_cambio = false;        
  }
  if (tara2_cambio)
  {   
    //tara2_actual_g = ; //en gramosLEER DE  nextion
    tara2_actual_offset = tara2_actual_g * calibration_factor2;
    bascula2.set_offset(zero_offset2 + tara2_actual_offset); //Zero out the scale using a previously known zero_factor  
    tara2_cambio = false;    
  }

  Serial.println(F("CAMBIO DE TARA: "));   
  Serial.print(F("\n zero_offset1: "));   
  Serial.print(zero_offset1);
  Serial.print(F(" tara1_actual_g: "));    
  Serial.print(tara1_actual_g); 
  Serial.print(F(" tara1_actual_offset: "));    
  Serial.print(tara1_actual_offset);   
  Serial.print(F(" bascula get offset: "));    
  Serial.print(bascula1.get_offset());   
  Serial.print(F(" calibration_factor1: "));    
  Serial.print(bascula1.get_scale());   
  
 
  Serial.println();

}



//***********************************************************
//Calibración cambiando el factor de calibración - OK PROBADO
//                     CALIBRAR_FACTOR
//***********************************************************

//cambiar serial output a nextion!!

void calibrar_factor1()
{
  if (! bascula1_conectada) {
    if (SERIAL_DEBUG) Serial.println(F("calibrar factor _ HX711 (1) no conectado"));
    return;
  }
  if (SERIAL_DEBUG){
    Serial.println(F("\nCALIBRACIÓN 1"));
    Serial.println(F("Remove all weight from scale, press a key"));
    while(!Serial.available());
    while(Serial.available()) Serial.read();
  } 

  bascula1.tare(AVG_FACT);
  zero_offset1 = bascula1.read_average(); //Get a baseline reading
  char temp;

  if (SERIAL_DEBUG){
    Serial.print(F("Zero factor: ")); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
    Serial.println(zero_offset1);
    Serial.println(F("Press + or a to increase calibration factor"));
    Serial.println(F("Press - or z to decrease calibration factor"));
  }

  bascula1.tare(AVG_FACT);
  while (temp != '*')
  {
    bascula1.set_scale(calibration_factor1); //Adjust to this calibration factor
    if (SERIAL_DEBUG){
      Serial.print(F("Reading: "));
      Serial.print(bascula1.get_units(AVG_FACT), RESOLUTION);
      Serial.print(F(" g")); 
      Serial.print(F(" calibration_factor-scale: "));
      Serial.print(calibration_factor1);
      Serial.print(F(" raw: "));
      Serial.print(bascula1.read_average(5));      
      Serial.println();
      Serial.print(F("Zero factor: "));
      Serial.println(zero_offset1);      
    }
    if(Serial.available())
    {
      char temp = Serial.read();
      if(temp == '+' || temp == 'a') calibration_factor1 += 10000;
      else if(temp == '-' || temp == 'z') calibration_factor1 -= 10000;
    }    
 }
}
//***********************************************************

void calibrar_factor2()
{
  if (! bascula2_conectada) {
    if (SERIAL_DEBUG) Serial.println(F("calibrar factor _  Sensor HX711 (2) no conectado"));
    return;
  }
  if (SERIAL_DEBUG){  
    Serial.println(F("CALIBRACIÓN 2"));
    Serial.println(F("Remove all weight from scale, press a key"));
  }
  while(!Serial.available());
  while(Serial.available()) Serial.read();

  bascula2.tare(AVG_FACT);
  zero_offset2 = bascula2.read_average(); //Get a baseline reading
  char temp;

  bascula2.tare(AVG_FACT);
  if (SERIAL_DEBUG){
    Serial.print(F("Zero factor: ")); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
    Serial.println(zero_offset2);    
    Serial.println(F("Press + or a to increase calibration factor"));
    Serial.println(F("Press - or z to decrease calibration factor"));  
  }

  
  while (temp != '*')
  {
    bascula2.set_scale(calibration_factor2); //Adjust to this calibration factor

    if (SERIAL_DEBUG){  
      Serial.print(F("Reading: "));
      Serial.print(bascula2.get_units(AVG_FACT), RESOLUTION);
      Serial.print(F(" g")); 
      Serial.print(F(" calibration_factor: "));
      Serial.print(calibration_factor2);
      Serial.println();
    }
    if(Serial.available())
    {
      char temp = Serial.read();
      if(temp == '+' || temp == 'a') calibration_factor2 += 10;
      else if(temp == '-' || temp == 'z') calibration_factor2 -= 10;
    }    
 }
}

//***************************************************
//Calibración colocando un peso conocido - OK PROBADO
//                 CALIBRAR_PESO
//***************************************************

//cambiar serial output a nextion!!

void calibrar_peso1() {
  if (! bascula1_conectada) {
    if (SERIAL_DEBUG) Serial.println("calibrar peso _ Sensor HX711 (1) no conectado");
    return;
  }
  
  float i = 0;
  if (SERIAL_DEBUG) { //DESCOMENTAR
    Serial.println(F("\nQuita cualquier peso de la báscula, pulsa una tecla para continuar \n"));
    while(!Serial.available());
    while(Serial.available()) Serial.read();
  } 
  bascula1.tare(AVG_FACT);
  zero_offset1 = bascula1.get_offset(); //despues de tarar, tenemos un nuevo zero offset
  if (SERIAL_DEBUG){  
    Serial.println(F("TARA: "));
    Serial.print(bascula1.get_units(AVG_FACT));
    Serial.print(F(" g"));
    Serial.print(F("¿Peso 1 calibrado? (g) "));
  }
  while(Serial.available()) Serial.read();
  while(!Serial.available()) i = Serial.parseFloat();
  while(Serial.available()) Serial.read();
  
  if (SERIAL_DEBUG){    
    Serial.print(F("\nPon el peso de "));
    Serial.print(i);
    Serial.print(F(" g en la báscula, pulsa una tecla para continuar \n"));
  }
  
  while(Serial.available()) Serial.read();
  while(!Serial.available());
  while(Serial.available()) Serial.read();

  bascula1.callibrate_scale(i, AVG_FACT);
  calibration_factor1 = bascula1.get_scale();
  
  tara1_cambio = true;//Para actualizar calibration_factor  
  cambioTara(); //Para actualizar calibration_factor
  bascula1_calibrada = true;
  
  //falta escribir en EEPROM calibration_factor
  if (SERIAL_DEBUG){  
    Serial.print(F("PESO CALIBRADO: "));
    Serial.print(bascula1.get_units(AVG_FACT), RESOLUTION);
    Serial.println(F(" g"));
  } 
}


//***************************************************
//Calibración colocando un peso conocido - OK PROBADO
//                 CALIBRAR_PESO
//***************************************************

void calibrar_peso2() {
  if (! bascula2_conectada) {
    if (SERIAL_DEBUG) Serial.println(F("calibrar peso _ Sensor HX711 (2) no conectado"));
    return;
  }
  float i = 0;
  if (SERIAL_DEBUG) Serial.println(F("\nQuita cualquier peso de la báscula, pulsa una tecla para continuar \n"));
  while(!Serial.available());
  while(Serial.available()) Serial.read();
  
  bascula2.tare(AVG_FACT);
  zero_offset2 = bascula2.get_offset(); //despues de tarar, tenemos un nuevo zero offset
  if (SERIAL_DEBUG){  
    Serial.println(F("TARA: "));
    Serial.print(bascula2.get_units(AVG_FACT));
    Serial.print(F(" g \n"));
    Serial.print(F("¿Peso 2 calibrado? (g) "));
  }
  while(!Serial.available()) i = Serial.parseFloat();
  while(Serial.available()) Serial.read();
  
  if (SERIAL_DEBUG){    
    Serial.print(F("\nPon el peso de "));
    Serial.print(i);
    Serial.print(F(" g en la báscula, pulsa una tecla para continuar \n"));
  }
  while(!Serial.available());
  while(Serial.available()) Serial.read();

  bascula2.callibrate_scale(i, AVG_FACT);
  calibration_factor2 = bascula2.get_scale();
  //falta escribir en EEPROM calibration_factor
  
  tara2_cambio = true;//Para actualizar calibration_factor  
  cambioTara(); //Para actualizar calibration_factor  
  bascula2_calibrada = true;
  
  if (SERIAL_DEBUG){    
    Serial.print(F("PESO CALIBRADO: "));
    Serial.print(bascula2.get_units(AVG_FACT), RESOLUTION);
    Serial.print(F(" g \n"));
  }
}

//***************************************************
//                 BEEP - ok probado
//***************************************************

void beep(int HZ, int on, int off){
  tone(ALARMA, HZ); // Send sound signal...
  delay(on);        // ...time on
  noTone(ALARMA);   // Stop sound...
  delay(off);       // ...time off 
}


//***************************************************
//               ALARMA - ok probado
//***************************************************

void alarma(int modo){
  //MODO 0 = BIENVENIDA - ENCENDIDO
  //MODO 1 = ERROR CRITICO SENSOR DE TEMPERATURA
  //MODO 2 = PROGRAMA TERMINADO
  //MODO 3 = RTC no conectado o sin batería
  
  switch(modo){ 
    case 0:
      beep(500,100,300); beep(2000,300,1000);
      break;
    case 1:
      for (;;) //loop infinito
      {
        beep(2000,300,200);
        T_int = dht_int.readTemperature();
        if (!isnan(T_int)) {break;}
      }
    break;
    
    case 2:
      for (int i=0;i<3;i++){ //señal acústica de fin de programa 3x3pitidos
        beep(500,100,100); beep(1000,200,100); beep(2000,300,100);
      }
      break;  

    case 3:
      beep(500,300,300); beep(500,300,300);
      break;
      
  }
}

//***************************************************
//                 TEMPORIZADOR 
//***************************************************

bool end_time(){
  totalTime = now - startTime;
  if ((set_time_min*60) > (totalTime/1000)) {return false;} else {return true;} //cálculo en segundos
}

//***************************************************
//                   SERIAL
//***************************************************
void SerialSend()
{
  Serial.print(F("setpoint: "));Serial.print(setpoint); Serial.print(" ");
  Serial.print(F("input: "));Serial.print(input); Serial.print(" ");
  Serial.print(F("output: "));Serial.print(output); Serial.print(" ");

}

char SerialReceive()
{
  if (Serial.available())
  {
   char b = Serial.read(); 
   Serial.flush(); 
   return b;
  }
}

//***************************************************
//            RESET todas las variables
//***************************************************
void reset_var()
{
  LastWeightTime = 0; 
  totalTime = 0; //ms
  startTime = 0; //ms
  now = 0; //ms
  timer = 0;
  temporizador_end = true; 
  empezar_programa = false;

}
//***************************************************


//***************************************************
//                CRC CHECK EEPROM
//***************************************************
//https://www.arduino.cc/en/Tutorial/LibraryExamples/EEPROMCrc

uint32_t eeprom_crc() 
{
  const uint32_t crc_table[16] = {
    0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
    0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
    0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
    0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c };
  uint32_t crc = ~0L;
  for (int index = 0 ; index < EEPROM.length()  ; ++index) {
    crc = crc_table[(crc ^ EEPROM[index]) & 0x0f] ^ (crc >> 4);
    crc = crc_table[(crc ^ (EEPROM[index] >> 4)) & 0x0f] ^ (crc >> 4);
    crc = ~crc;
  }
  return crc;
}

void borrarEEPROM()
{ 
  // Borrar el EEPROM entero
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.update(i, 255);
  }
}


//check!

int readStringFromEEPROM(int addrOffset, String *strToRead)
{
  int16_t newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1];
  for (int i = 0; i < newStrLen; i++)
  {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  data[newStrLen] = '\ 0'; // !!! NOTE !!! Remove the space between the slash "/" and "0" (I've added a space because otherwise there is a display bug)
  *strToRead = String(data);
  return addrOffset + 1 + newStrLen;
}

int writeStringToEEPROM(int addrOffset, const String &strToWrite)
{
  byte len = strToWrite.length();
  EEPROM.write(addrOffset, len);
  for (int i = 0; i < len; i++)
  {
    EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
  }
  return addrOffset + 1 + len;
}


//******************************** EEPROM FUNCTIONS


//***********************************************************************************************************************************
//               NEXTION FUNCIONES
//***********************************************************************************************************************************

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

void trigger0(){              // printh 23 02 54 00
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

void trigger1(){              // printh 23 02 54 01

}
void trigger2(){              // printh 23 02 54 02 

}
void trigger3(){              // printh 23 02 54 03

}
void trigger4(){              // printh 23 02 54 04

}
void trigger5(){              // printh 23 02 54 05

}
void trigger6(){              // printh 23 02 54 06

}
void trigger7(){              // printh 23 02 54 07

}
void trigger8(){              // printh 23 02 54 08

}
void trigger9(){              // printh 23 02 54 09

}
void trigger10(){             // printh 23 02 54 0A

}
void trigger11(){             // printh 23 02 54 0B

}
void trigger12(){             // printh 23 02 54 0C

}
void trigger13(){             // printh 23 02 54 0D

}
void trigger14(){             // printh 23 02 54 0E

}
void trigger15(){             // printh 23 02 54 0F

}
void trigger16(){             // printh 23 02 54 10

}
void trigger17(){             // printh 23 02 54 11

}
void trigger18(){             // printh 23 02 54 12

}
void trigger19(){             // printh 23 02 54 13

}
void trigger20(){             // printh 23 02 54 14

}
void trigger21(){             // printh 23 02 54 15

}
void trigger22(){             // printh 23 02 54 16

}
void trigger23(){             // printh 23 02 54 17

}
void trigger24(){             // printh 23 02 54 18

}
void trigger25(){             // printh 23 02 54 19

}
void trigger26(){             // printh 23 02 54 1A

}
void trigger27(){             // printh 23 02 54 1B

}
void trigger28(){             // printh 23 02 54 1C

}
void trigger29(){             // printh 23 02 54 1D

}
void trigger30(){             // printh 23 02 54 1E

}
void trigger31(){             // printh 23 02 54 1F

}
void trigger32(){             // printh 23 02 54 20

}
void trigger33(){             // printh 23 02 54 21

}
void trigger34(){             // printh 23 02 54 22

}
void trigger35(){             // printh 23 02 54 23

}
void trigger36(){             // printh 23 02 54 24

}
void trigger37(){             // printh 23 02 54 25

}
void trigger38(){             // printh 23 02 54 26

}
void trigger39(){             // printh 23 02 54 27

}
void trigger40(){             // printh 23 02 54 28

}
void trigger41(){             // printh 23 02 54 29

}
void trigger42(){             // printh 23 02 54 2A

}
void trigger43(){             // printh 23 02 54 2B

}
void trigger44(){             // printh 23 02 54 2C

}
void trigger45(){             // printh 23 02 54 2D

}
void trigger46(){             // printh 23 02 54 2E

}
void trigger47(){             // printh 23 02 54 2F

}
void trigger48(){             // printh 23 02 54 30

}
void trigger49(){             // printh 23 02 54 31

}
void trigger50(){             // printh 23 02 54 32

}


/*Numbers in HEX:
 DEC  HEX   -   DEC  HEX    -   DEC  HEX    -  DEC  HEX
  0   00    -    16  10     -    32  20     -   48  30
  1   01    -    17  11     -    33  21     -   49  31
  2   02    -    18  12     -    34  22     -   50  32
  3   03    -    19  13     -    35  23     -   51  33
  4   04    -    20  14     -    36  24     -   52  34
  5   05    -    21  15     -    37  25     -   53  35
  6   06    -    22  16     -    38  26     -   54  36
  7   07    -    23  17     -    39  27     -   55  37
  8   08    -    24  18     -    40  28     -   56  38
  9   09    -    25  19     -    41  29     -   57  39
  10  0A    -    26  1A     -    42  2A     -   58  3A
  11  0B    -    27  1B     -    43  2B     -   59  3B
  12  0C    -    28  1C     -    44  2C     -   60  3C
  13  0D    -    29  1D     -    45  2D     -   61  3D
  14  0E    -    30  1E     -    46  2E     -   62  3E
  15  0F    -    31  1F     -    47  2F     -   63  3F
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

void refreshPage0() // página: main 
{
//  myNex.writeStr("t0.txt","3");
  uint16_t tempInt=3;
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
readNumberFromSerial 

 */
