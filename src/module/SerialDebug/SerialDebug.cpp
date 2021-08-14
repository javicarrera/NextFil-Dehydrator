/*!
 * SerialDebug.cpp - Funciones para el uso del serial para debug
 * 2021 Javier Carrera Arias  
 * All rights reserved under the library's licence
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 */

#ifndef SERIALDEBUG_H
  #include "SerialDebug.h"
#endif

static char inputchar[64] = {};           // para recibir del serial

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

//***********************************************************
//                      flushInput
//***********************************************************
void flushInput()
{
  for (int i = 0; i < 64; i++)
  {
    inputchar[i] = {};
  }
}

//***********************************************************
//                      serialEvent()
//***********************************************************
void serialEvent()
{
  int i = 0;
  bool inputReadingCompleted = false;
  flushInput(); // al principio vaciamos el array para recibir nuevos datos
  while (Serial.available() < 1) // esperar a presionar una tecla
    delay(100);
  while (Serial.available() > 0)
  {
    inputchar[i] = Serial.read();
    delay(2);
    i++;
    inputReadingCompleted = !(Serial.available());
  }

  // printing the input
  if (inputReadingCompleted)
  { // if input
    Serial.print("Serial input = ");
    Serial.println(inputchar);
    inputReadingCompleted = false;
  }
}

//***************************************************
//                   SERIAL RECEIVE
//***************************************************

char SerialReceive()
{
  if (Serial.available())
  {
    char b = Serial.read();
    Serial.flush();
    return b;
  }
  else
    return ' ';
}