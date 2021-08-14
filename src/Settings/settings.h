//
// Settings Class
//
// Agradecimientos por la inspiración a http://www.grumpyoldtech.technology/Blog/Arduino_Tutorial___saving_settings_to_the_EEPROM
/*
* This software is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

/*
* METODOS PARA MAIN.CPP 
* settings->save(); //guardar configuracion actual en la EEPROM
* settings->setEEPROM(); //cargar configuracion de la EEPROM, descartando cambios
* settings->clearEEPROM(); //borrar EEPROM - todas las posiciones 255 bytes
* settings->printSerial(); //Mostrar valores en serial si SERIAL_DEBUG es true 
* settings->setDEFAULT(actSettings); // cargar configuracion por defecto de la PROGMEM, hace falta el siguiente código en main.cpp:
*/
//   EEPROM.put(EEPROM.length() - 1, false);
//   SettingsType actSettings = defaults;
//   PROGMEM_readAnything(&defaults, actSettings); //leer la estructura en PROGMEM y copiarla a RAM actSettings
//   settings->setDEFAULT(actSettings);

/*
* COMO ASIGNAR VALORES NUEVOS A LA CONFIGURACIÓN ACTUAL
*   CHAR:
*   strcpy(settings->parameters->name_prog1, "ABS"); //los char array se modifican con strcpy
*   OTROS TIPOS DE DATOS:
*   settings->parameters->temp_prog1 = 150;
*/

#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include <EEPROM.h>

// Settings Version
#define SETTING_VER "DHF1.0"

// Parameters Structure
// typedef (existing name:struct SettingsType) (alternate name:SettingsType)
typedef struct //no tag
{
  bool SERIAL_DEBUG;
  char name_prog1[6]; //solo se pueden usar 5 caracteresp
  char name_prog2[6];
  char name_prog3[6];
  char name_prog4[6];
  char name_prog5[6];
  uint8_t temp_prog1;
  uint8_t temp_prog2;
  uint8_t temp_prog3;
  uint8_t temp_prog4;
  uint8_t temp_prog5;
  bool timer;
  uint16_t set_time_min;
  bool timer_beep;
  bool vent_auto_off;
  uint8_t VentMinDutyCycle;
  float calibration_factor1;
  int32_t zero_offset1;
  bool bascula1_calibrada;
  float calibration_factor2;
  int32_t zero_offset2;
  bool bascula2_calibrada;
  float vaggKp;
  float vaggKi;
  float vaggKd;
  float aggKp;
  float aggKi;
  float aggKd;
  float consKp;
  float consKi;
  float consKd;
  uint16_t WindowSize;
  float dht_T_ext_correccion;
  float dht_T_int_correccion;
  float dht_H_ext_correccion;
  float dht_H_int_correccion;
  uint16_t time_prog1;
  uint16_t time_prog2;
  uint16_t time_prog3;
  uint16_t time_prog4;
  uint16_t time_prog5;
  uint32_t WeightReadingRefreshTime;
  uint8_t margen_temp;
  uint8_t setpoint;
  uint16_t neopixel_type;
  uint8_t neopixel_pixels;
  bool ledStatus;
  char name_tara1[10];
  char name_tara2[10];
  char name_tara3[10];
  char name_tara4[10];
  char name_tara5[10];
  uint16_t peso_tara1;
  uint16_t peso_tara2;
  uint16_t peso_tara3;
  uint16_t peso_tara4;
  uint16_t peso_tara5;
  uint8_t ultima_tara1;
  uint8_t ultima_tara2;
  char ver[sizeof(SETTING_VER)];
} SettingsType; // typedef

//PROGMEM READ-ANYTHING LIBRARY:
template <typename T>
void PROGMEM_readAnything(const T *sce, T &dest)
{
  memcpy_P(&dest, sce, sizeof(T));
}

class Settings
{
public:
  Settings(SettingsType defaults, bool SERIAL_DEBUG); //constructor - se crea una instancia de settings, y se pasan los valores defaults
  void save();                                        //guarda la configuracion en RAM en la EEPROM
  void setEEPROM();                                   //para cambiar en tiempo real entre configuración default y de usuario
  void setDEFAULT(SettingsType actSettings);          //para cambiar en tiempo real entre configuración default y de usuario pasamos defaults desde main
  void clearEEPROM();                                 //guarda 255 en cada posción de la EEPROM
  void printSerial();                                 //debug - serial
  SettingsType *parameters;                           //pointer de tipo SettingsType

private:
  bool _useUserSettings;
  bool _SERIAL_DEBUG;
  byte buffer[sizeof(SettingsType)];
};

#endif
