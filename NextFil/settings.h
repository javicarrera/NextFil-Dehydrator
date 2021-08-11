//
// Settings Class
//
// Agradecimientos por la inspiración a http://www.grumpyoldtech.technology/Blog/Arduino_Tutorial___saving_settings_to_the_EEPROM

#ifndef SETTINGS_H
#define SETTINGS_H

#include "Arduino.h"
#include <EEPROM.h>

// Settings Version 
#define SETTING_VER "DHF1.0"

// Parameters Structure
typedef struct 
{  
  bool userSettings;
  bool SERIAL_DEBUG;
  char name_prog1[6];
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
  uint8_t setpoint;
  int neopixel_type;
  byte neopixel_pixels;
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
}ParametersType;  // typedef


class Settings
{
  public:
    Settings(ParametersType defaults); //constructor
    void readSettings(ParametersType defaults, bool userSettings); //lee en memoria de la EEPROM / default
    void save(ParametersType defaults);  //guarda la memoria en la EEPROM
    void printSerial();                //debug - serial
    ParametersType *parameters;        //pointer
  private:
    //byte buffer[sizeof(ParametersType)]; //tamaño de la estructura
    bool _userSettings;
    
};

#endif
