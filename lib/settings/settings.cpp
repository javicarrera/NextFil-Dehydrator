//
// Settings Class
//
// Agradecimientos por la inspiración a http://www.grumpyoldtech.technology/Blog/Arduino_Tutorial___saving_settings_to_the_EEPROM
/*
* This software is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include "settings.h"

Settings::Settings(SettingsType actSettings, bool SERIAL_DEBUG)
{ //Constructor

  _SERIAL_DEBUG = SERIAL_DEBUG;
  EEPROM.get(EEPROM.length() - 1, _useUserSettings);
  memcpy(buffer, &actSettings, sizeof(actSettings));

  //Read version number from EEPROM
  char savedVersion[sizeof(SETTING_VER)];
  uint16_t eepromVersionOffset = sizeof(buffer) - sizeof(SETTING_VER);
    if (_SERIAL_DEBUG) { 
      Serial.print(F("eepromVersionOffset"));
      Serial.println(eepromVersionOffset);
    }
  EEPROM.get(eepromVersionOffset, savedVersion);

  // Si la version de la EEPROM es correcta, se sobrescribe la configuración por defecto por los valores de la EEPROM
  // Si el usuario quiere cargar los valores por defecto, no se sobrescribe (userSettings=false).

  if (strcmp(savedVersion, SETTING_VER) == 0 && _useUserSettings)
  {
    setEEPROM();
    if (_SERIAL_DEBUG) Serial.print(F("Inicio correcto, EEPROM cargada"));        
  }
  else
  { //Se carga defaults
    EEPROM.put(EEPROM.length() - 1, false);
    //si los datos de versión no concuerdan, ha habido algún tipo de corrupción y se sobreescribe la EEPROM con defaults
    if (strcmp(savedVersion, SETTING_VER) != 0)
    {
      if (_SERIAL_DEBUG)
      {
        Serial.print(F("Config. EEPROM corrupta, iniciando config. por defecto "));
        Serial.print(F("Guardando config. por defecto en la EEPROM"));
        Serial.println();
      }
      save();
    }
  }

  parameters = (SettingsType *)buffer; //Crear pointer (parameters) a buffer
  if (_SERIAL_DEBUG) Serial.print(F("Inicio correcto, configuracion cargada a RAM"));    
}

void Settings::save()
{
  EEPROM.put(0, buffer); // Actualizar la EEPROM desde el byte 0 con los datos de buffer (pointer parameters)
  if (_SERIAL_DEBUG) {
     Serial.println();
    Serial.println(F("Cambios guardados en la EEPROM"));
    Serial.println();
  }
}

void Settings::clearEEPROM() // Borrar el EEPROM entero
{
  for (uint16_t i = 0; i < EEPROM.length(); i++)
  {
    EEPROM.update(i, 255);
  }
}

void Settings::setEEPROM()
{
  EEPROM.put(EEPROM.length() - 1, true); //el ultimo byte (1023) tiene el bool para saber que configuración cargar, guardar en _useUserSettings
  EEPROM.get(0, buffer);                 // Leer de la EEPROM y sobrescribir la estructura buffer con los datos de la EEPROM

  if (_SERIAL_DEBUG)
    Serial.println(F("Config. EEPROM cargada y selec. por defecto"));
}

void Settings::setDEFAULT(SettingsType defaults)
{
  EEPROM.put(EEPROM.length() - 1, false); //el ultimo byte (1023) tiene el bool para saber que configuración cargar, guardar en _useUserSettings
  memcpy(buffer, &defaults, sizeof(defaults));

  if (_SERIAL_DEBUG)
    Serial.println(F("Config. DEFAULT cargada y selec. por defecto"));
}

//Funcion "F()" ahorra memoria RAM cuando se muestran textos
void Settings::printSerial()
{
  if (_SERIAL_DEBUG)
  {
    EEPROM.get(EEPROM.length() - 1, _useUserSettings);
    Serial.println(F("Config. cargada "));

    if (_useUserSettings)
      Serial.println(F("de usuario de la EEPROM:"));
    else
      Serial.println(F("por defecto de la PROGMEM:"));

    Serial.print(F("SERIAL_DEBUG: "));
    Serial.println(parameters->SERIAL_DEBUG);
    Serial.print(F("name_prog1: "));
    Serial.println(parameters->name_prog1);
    Serial.print(F("name_prog2: "));
    Serial.println(parameters->name_prog2);
    Serial.print(F("name_prog3: "));
    Serial.println(parameters->name_prog3);
    Serial.print(F("name_prog4: "));
    Serial.println(parameters->name_prog4);
    Serial.print(F("name_prog5: "));
    Serial.println(parameters->name_prog5);
    Serial.print(F("temp_prog1: "));
    Serial.println(parameters->temp_prog1);
    Serial.print(F("temp_prog2: "));
    Serial.println(parameters->temp_prog2);
    Serial.print(F("temp_prog3: "));
    Serial.println(parameters->temp_prog3);
    Serial.print(F("temp_prog4: "));
    Serial.println(parameters->temp_prog4);
    Serial.print(F("temp_prog5: "));
    Serial.println(parameters->temp_prog5);
    Serial.print(F("timer: "));
    Serial.println(parameters->timer);
    Serial.print(F("set_time_min: "));
    Serial.println(parameters->set_time_min);
    Serial.print(F("timer beep: "));
    Serial.println(parameters->timer_beep);
    Serial.print(F("vent_auto_off: "));
    Serial.println(parameters->vent_auto_off);
    Serial.print(F("VentMinDutyCycle: "));
    Serial.println(parameters->VentMinDutyCycle);
    Serial.print(F("calibration_factor1: "));
    Serial.println(parameters->calibration_factor1);
    Serial.print(F("zero_offset1: "));
    Serial.println(parameters->zero_offset1);
    Serial.print(F("bascula1_calibrada: "));
    Serial.println(parameters->bascula1_calibrada);
    Serial.print(F("calibration_factor2: "));
    Serial.println(parameters->calibration_factor2);
    Serial.print(F("zero_offset2: "));
    Serial.println(parameters->zero_offset2);
    Serial.print(F("bascula2_calibrada: "));
    Serial.println(parameters->bascula2_calibrada);
    Serial.print(F("vaggKp: "));
    Serial.println(parameters->vaggKp);
    Serial.print(F("vaggKi: "));
    Serial.println(parameters->vaggKi);
    Serial.print(F("vaggKd: "));
    Serial.println(parameters->vaggKd);
    Serial.print(F("aggKp: "));
    Serial.println(parameters->aggKp);
    Serial.print(F("aggKi: "));
    Serial.println(parameters->aggKi);
    Serial.print(F("aggKd: "));
    Serial.println(parameters->aggKd);
    Serial.print(F("consKp: "));
    Serial.println(parameters->consKp);
    Serial.print(F("consKi: "));
    Serial.println(parameters->consKi);
    Serial.print(F("consKd: "));
    Serial.println(parameters->consKd);
    Serial.print(F("WindowSize: "));
    Serial.println(parameters->WindowSize);
    Serial.print(F("dht_T_ext_correccion "));
    Serial.println(parameters->dht_T_ext_correccion);
    Serial.print(F("dht_T_int_correccion "));
    Serial.println(parameters->dht_T_int_correccion);
    Serial.print(F("dht_H_ext_correccion "));
    Serial.println(parameters->dht_H_ext_correccion);
    Serial.print(F("dht_H_int_correccion "));
    Serial.println(parameters->dht_H_int_correccion);
    Serial.print(F("time_prog1 "));
    Serial.println(parameters->time_prog1);
    Serial.print(F("time_prog2 "));
    Serial.println(parameters->time_prog2);
    Serial.print(F("time_prog3 "));
    Serial.println(parameters->time_prog3);
    Serial.print(F("time_prog4 "));
    Serial.println(parameters->time_prog4);
    Serial.print(F("time_prog5 "));
    Serial.println(parameters->time_prog5);
    Serial.print(F("WeightReadingRefreshTime: "));
    Serial.println(parameters->WeightReadingRefreshTime);
    Serial.print(F("margen_temp: "));
    Serial.println(parameters->margen_temp);    
    Serial.print(F("setpoint: "));
    Serial.println(parameters->setpoint);
    Serial.print(F("neopixel_type: "));
    Serial.println(parameters->neopixel_type);
    Serial.print(F("neopixel_pixels: "));
    Serial.println(parameters->neopixel_pixels);
    Serial.print(F("ledStatus: "));
    Serial.println(parameters->ledStatus);
    Serial.print(F("name_tara1: "));
    Serial.println(parameters->name_tara1);
    Serial.print(F("name_tara2: "));
    Serial.println(parameters->name_tara2);
    Serial.print(F("name_tara3: "));
    Serial.println(parameters->name_tara3);
    Serial.print(F("name_tara4: "));
    Serial.println(parameters->name_tara4);
    Serial.print(F("name_tara5: "));
    Serial.println(parameters->name_tara5);
    Serial.print(F("peso_tara1: "));
    Serial.println(parameters->peso_tara1);
    Serial.print(F("peso_tara2: "));
    Serial.println(parameters->peso_tara2);
    Serial.print(F("peso_tara3: "));
    Serial.println(parameters->peso_tara3);
    Serial.print(F("peso_tara4: "));
    Serial.println(parameters->peso_tara4);
    Serial.print(F("peso_tara5: "));
    Serial.println(parameters->peso_tara5);
    Serial.print(F("ultima_tara1: "));
    Serial.println(parameters->ultima_tara1);
    Serial.print(F("ultima_tara2: "));
    Serial.println(parameters->ultima_tara2);
    Serial.print(F("Versión: "));
    Serial.print(SETTING_VER);

    //parametros personalizados EEPROM
  }
}
