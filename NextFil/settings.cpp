//
// Settings Class
//
// Agradecimientos por la inspiración a http://www.grumpyoldtech.technology/Blog/Arduino_Tutorial___saving_settings_to_the_EEPROM

#include "settings.h"

Settings::Settings(ParametersType defaults){}     
/*
Settings::Settings(ParametersType defaults) { //Constructor

  memcpy(buffer, &defaults, sizeof(defaults));

  // Read version number from EEPROM
  char savedVersion[sizeof(SETTING_VER)];
  int eepromVersionOffset = sizeof(buffer) - sizeof(SETTING_VER);
  EEPROM.get(eepromVersionOffset,savedVersion);

  // If version in EEPROM is correct, overwrite the default coonfig with eeprom values
  if (strcmp(savedVersion,SETTING_VER) == 0) {

    // Overwrite the structure with EEPROM data
    EEPROM.get(0,buffer);
  }
 
  parameters = (ParametersType *) buffer;
}*/

  
void Settings::readSettings(ParametersType defaults, bool userSettings)
{
   _userSettings=userSettings;
  
/*  con este codigo original se crea otra copia de la estructura actSettings que ya esta en RAM (buffer).
 *   memcpy(buffer, &defaults, sizeof(defaults));  //copia en buffer el contenido de la memoria RAM! con direccion defaults, su longitud completa
  //HAY QUE LEER DE PROGMEM! esta en setup, intentar pasarlo aqui

  if (_userSettings){
    //Read version number from EEPROM
    char savedVersion[sizeof(SETTING_VER)];
    int eepromVersionOffset = sizeof(buffer) - sizeof(SETTING_VER);
    EEPROM.get(eepromVersionOffset,savedVersion);
  
    // If version in EEPROM is correct, overwrite the default config with eeprom values
    if (strcmp(savedVersion,SETTING_VER) == 0) 
      EEPROM.get(0,buffer); //Overwrite the structure with EEPROM data desde el byte 0
  } 
  parameters = (ParametersType *) buffer;  
  */
  /*if (_userSettings){
    //Read version number from EEPROM
    char savedVersion[sizeof(SETTING_VER)];
    int eepromVersionOffset = sizeof(defaults) - sizeof(SETTING_VER);
    EEPROM.get(eepromVersionOffset,savedVersion);
  
    // If version in EEPROM is correct, overwrite the default config with eeprom values
    if (strcmp(savedVersion,SETTING_VER) == 0) 
      EEPROM.get(0,defaults); //Overwrite the structure with EEPROM data desde el byte 0
  } */
  //Si no se usa la configuración de usuario, se carga en RAM por defecto que leimos en setup (actSettings)
  parameters = &defaults; //(defaults es el parametro pasado)en este caso pointer a actSettings

  Serial.print(F("inside reading class - temp_prog1------------------: "));
  Serial.println(parameters->temp_prog1);
}


void Settings::save(ParametersType defaults) { 
  //EEPROM.put(0,buffer); // actualizar la EEPROM desde el byte 0 
  parameters = &defaults;
  Serial.print(F("inside saving class - temp_prog1-------------------: "));
  Serial.println(parameters->temp_prog1);
  EEPROM.put(0,defaults); // actualizar la EEPROM desde el byte 0 
  
}

//Funcion "F()" ahorra memoria RAM cuando se muestran textos
void Settings::printSerial() {
  Serial.print(F("Configuración "));
  if(_userSettings)
    Serial.println(F("de usuario de la EEPROM:"));
  else
    Serial.println(F("por defecto de la PROGMEM:"));    
  Serial.print(F("userSetting: "));
  Serial.print(parameters->userSettings);Serial.print(F(" Bytes:"));     
  Serial.println(sizeof(parameters->userSettings));
  Serial.print(F("SERIAL_DEBUG: "));
  Serial.print(parameters->SERIAL_DEBUG);Serial.print(F(" Bytes:"));   
  Serial.println(sizeof(parameters->SERIAL_DEBUG));
  Serial.print(F("name_prog1: "));
  Serial.print(parameters->name_prog1); Serial.print(F(" Bytes:"));   
  Serial.println(sizeof(parameters->name_prog1));
  Serial.print(F("name_prog2: "));
  Serial.print(parameters->name_prog2); Serial.print(F(" Bytes:"));    
  Serial.println(sizeof(parameters->name_prog2));
  Serial.print(F("name_prog3: "));
  Serial.print(parameters->name_prog3);Serial.print(F(" Bytes:"));    
  Serial.println(sizeof(parameters->name_prog3));
  Serial.print(F("name_prog4: "));
  Serial.print(parameters->name_prog4);  Serial.print(F(" Bytes:"));       
  Serial.println(sizeof(parameters->name_prog4));
  Serial.print(F("name_prog5: "));
  Serial.print(parameters->name_prog5); Serial.print(F(" Bytes:"));    
  Serial.println(sizeof(parameters->name_prog5));
  Serial.print(F("temp_prog1: "));
  Serial.print(parameters->temp_prog1); Serial.print(F(" Bytes:"));   
  Serial.println(sizeof(parameters->temp_prog1));
  Serial.print(F("temp_prog2: "));
  Serial.print(parameters->temp_prog2);  Serial.print(F(" Bytes:"));   
  Serial.println(sizeof(parameters->temp_prog2));
  Serial.print(F("temp_prog3: "));
  Serial.print(parameters->temp_prog3);Serial.print(F(" Bytes:"));    
  Serial.println(sizeof(parameters->temp_prog3));
  Serial.print(F("temp_prog4: "));
  Serial.print(parameters->temp_prog4); Serial.print(F(" Bytes:"));        
  Serial.println(sizeof(parameters->temp_prog4));
  Serial.print(F("temp_prog5: "));
  Serial.print(parameters->temp_prog5); Serial.print(F(" Bytes:"));    
  Serial.println(sizeof(parameters->temp_prog5));
  Serial.print(F("timer: "));
  Serial.print(parameters->timer); Serial.print(F(" Bytes:"));   
  Serial.println(sizeof(parameters->timer));
  Serial.print(F("set_time_min: "));
  Serial.print(parameters->set_time_min);Serial.print(F(" Bytes:"));   
    Serial.println(sizeof(parameters->set_time_min));
  Serial.print(F("timer beep: "));
  Serial.print(parameters->timer_beep);  Serial.print(F(" Bytes:"));    
    Serial.println(sizeof(parameters->timer_beep));
  Serial.print(F("vent_auto_off: "));
  Serial.print(parameters->vent_auto_off); Serial.print(F(" Bytes:"));   
    Serial.println(sizeof(parameters->vent_auto_off));
  Serial.print(F("VentMinDutyCycle: "));
  Serial.print(parameters->VentMinDutyCycle); Serial.print(F(" Bytes:"));     
    Serial.println(sizeof(parameters->VentMinDutyCycle));
  Serial.print(F("calibration_factor1: "));
  Serial.print(parameters->calibration_factor1);Serial.print(F(" Bytes:"));   
    Serial.println(sizeof(parameters->calibration_factor1));
  Serial.print(F("zero_offset1: "));
  Serial.print(parameters->zero_offset1);  Serial.print(F(" Bytes:"));   
    Serial.println(sizeof(parameters->zero_offset1));
  Serial.print(F("bascula1_calibrada: "));
  Serial.print(parameters->bascula1_calibrada);  Serial.print(F(" Bytes:"));    
    Serial.println(sizeof(parameters->bascula1_calibrada));
  Serial.print(F("calibration_factor2: "));
  Serial.print(parameters->calibration_factor2);Serial.print(F(" Bytes:"));   
    Serial.println(sizeof(parameters->calibration_factor2));
  Serial.print(F("zero_offset2: "));
  Serial.print(parameters->zero_offset2);   Serial.print(F(" Bytes:"));   
    Serial.println(sizeof(parameters->zero_offset2));
  Serial.print(F("bascula2_calibrada: "));
  Serial.print(parameters->bascula2_calibrada); Serial.print(F(" Bytes:"));      
    Serial.println(sizeof(parameters->bascula2_calibrada));
  Serial.print(F("vaggKp: "));
  Serial.print(parameters->vaggKp); Serial.print(F(" Bytes:"));   
    Serial.println(sizeof(parameters->vaggKp));
  Serial.print(F("vaggKi: "));
  Serial.print(parameters->vaggKi); Serial.print(F(" Bytes:"));   
    Serial.println(sizeof(parameters->vaggKi));
  Serial.print(F("vaggKd: "));
  Serial.print(parameters->vaggKd); Serial.print(F(" Bytes:"));   
    Serial.println(sizeof(parameters->vaggKd));
  Serial.print(F("aggKp: "));
  Serial.print(parameters->aggKp); Serial.print(F(" Bytes:"));     
    Serial.println(sizeof(parameters->aggKp));
  Serial.print(F("aggKi: "));
  Serial.print(parameters->aggKi); Serial.print(F(" Bytes:"));     
    Serial.println(sizeof(parameters->aggKi));
  Serial.print(F("aggKd: "));
  Serial.print(parameters->aggKd); Serial.print(F(" Bytes:"));     
    Serial.println(sizeof(parameters->aggKd));
  Serial.print(F("consKp: "));
  Serial.print(parameters->consKp);  Serial.print(F(" Bytes:"));      
    Serial.println(sizeof(parameters->consKp));
  Serial.print(F("consKi: "));
  Serial.print(parameters->consKi); Serial.print(F(" Bytes:"));       
    Serial.println(sizeof(parameters->consKi));
  Serial.print(F("consKd: "));
  Serial.print(parameters->consKd);  Serial.print(F(" Bytes:"));      
    Serial.println(sizeof(parameters->consKd));
  Serial.print(F("WindowSize: "));
  Serial.print(parameters->WindowSize);   Serial.print(F(" Bytes:"));    
    Serial.println(sizeof(parameters->WindowSize));
  Serial.print(F("dht_T_ext_correccion "));
  Serial.print(parameters->dht_T_ext_correccion); Serial.print(F(" Bytes:"));    
    Serial.println(sizeof(parameters->dht_T_ext_correccion));
  Serial.print(F("dht_T_int_correccion "));
  Serial.print(parameters->dht_T_int_correccion); Serial.print(F(" Bytes:"));   
    Serial.println(sizeof(parameters->dht_T_int_correccion));
  Serial.print(F("dht_H_ext_correccion "));
  Serial.print(parameters->dht_H_ext_correccion); Serial.print(F(" Bytes:"));   
    Serial.println(sizeof(parameters->dht_H_ext_correccion));
  Serial.print(F("dht_H_int_correccion "));
  Serial.print(parameters->dht_H_int_correccion); Serial.print(F(" Bytes:"));   
    Serial.println(sizeof(parameters->dht_H_int_correccion));
  Serial.print(F("time_prog1 "));
  Serial.print(parameters->time_prog1); Serial.print(F(" Bytes:"));   
    Serial.println(sizeof(parameters->time_prog1));
  Serial.print(F("time_prog2 "));
  Serial.print(parameters->time_prog2); Serial.print(F(" Bytes:"));   
    Serial.println(sizeof(parameters->time_prog2));
  Serial.print(F("time_prog3 "));
  Serial.print(parameters->time_prog3); Serial.print(F(" Bytes:"));   
    Serial.println(sizeof(parameters->time_prog3));
  Serial.print(F("time_prog4 "));
  Serial.print(parameters->time_prog4); Serial.print(F(" Bytes:"));   
    Serial.println(sizeof(parameters->time_prog4));
  Serial.print(F("time_prog5 "));
  Serial.print(parameters->time_prog5);   Serial.print(F(" Bytes:"));   
    Serial.println(sizeof(parameters->time_prog5));
  Serial.print(F("WeightReadingRefreshTime: "));
  Serial.print(parameters->WeightReadingRefreshTime); Serial.print(F(" Bytes:"));   
    Serial.println(sizeof(parameters->WeightReadingRefreshTime));
  Serial.print(F("setpoint: "));
  Serial.print(parameters->setpoint);Serial.print(F(" Bytes:"));   
    Serial.println(sizeof(parameters->setpoint));
  Serial.print(F("neopixel_type: "));
  Serial.print(parameters->neopixel_type);   Serial.print(F(" Bytes:"));    
    Serial.println(sizeof(parameters->neopixel_type));
  Serial.print(F("neopixel_pixels: "));
  Serial.print(parameters->neopixel_pixels); Serial.print(F(" Bytes:"));   
    Serial.println(sizeof(parameters->neopixel_pixels));
  Serial.print(F("ledStatus: "));
  Serial.print(parameters->ledStatus); Serial.print(F(" Bytes:"));   
    Serial.println(sizeof(parameters->ledStatus));
  Serial.print(F("name_tara1: "));
  Serial.print(parameters->name_tara1); Serial.print(F(" Bytes:"));   
    Serial.println(sizeof(parameters->name_tara1));
  Serial.print(F("name_tara2: "));
  Serial.print(parameters->name_tara2); Serial.print(F(" Bytes:"));   
    Serial.println(sizeof(parameters->name_tara2));
  Serial.print(F("name_tara3: "));
  Serial.print(parameters->name_tara3); Serial.print(F(" Bytes:"));   
    Serial.println(sizeof(parameters->name_tara3));
  Serial.print(F("name_tara4: "));
  Serial.print(parameters->name_tara4); Serial.print(F(" Bytes:"));   
    Serial.println(sizeof(parameters->name_tara4));
  Serial.print(F("name_tara5: "));
  Serial.print(parameters->name_tara5);  Serial.print(F(" Bytes:"));   
    Serial.println(sizeof(parameters->name_tara5));
  Serial.print(F("peso_tara1: "));
  Serial.print(parameters->peso_tara1); Serial.print(F(" Bytes:"));   
      Serial.println(sizeof(parameters->peso_tara1));
  Serial.print(F("peso_tara2: "));
  Serial.print(parameters->peso_tara2); Serial.print(F(" Bytes:"));   
      Serial.println(sizeof(parameters->peso_tara2));
  Serial.print(F("peso_tara3: "));
  Serial.print(parameters->peso_tara3); Serial.print(F(" Bytes:"));   
    Serial.println(sizeof(parameters->peso_tara3));  
  Serial.print(F("peso_tara4: "));
  Serial.print(parameters->peso_tara4); Serial.print(F(" Bytes:"));   
    Serial.println(sizeof(parameters->peso_tara4));  
  Serial.print(F("peso_tara5: "));
  Serial.print(parameters->peso_tara5); Serial.print(F(" Bytes:"));   
    Serial.println(sizeof(parameters->peso_tara5));  
  Serial.print(F("ultima_tara1: "));
  Serial.print(parameters->ultima_tara1); Serial.print(F(" Bytes:"));   
    Serial.println(sizeof(parameters->ultima_tara1));  
  Serial.print(F("ultima_tara2: "));
  Serial.print(parameters->ultima_tara2);   Serial.print(F(" Bytes:"));    
    Serial.println(sizeof(parameters->ultima_tara2));  
  Serial.print(F("Versión: "));
  Serial.print(SETTING_VER); Serial.print(F(" Bytes:"));    
  Serial.println(sizeof(SETTING_VER));  
  //parametros personalizados EEPROM

}
  
