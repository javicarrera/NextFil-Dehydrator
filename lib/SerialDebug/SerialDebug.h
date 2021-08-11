/*!
 * SerialDebug.cpp - Funciones para el uso del serial para debug
 * 2021 Javier Carrera Arias  
 * All rights reserved under the library's licence
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 */



#ifndef SERIALDEBUG_H
#define SERIALDEBUG_H

#if ARDUINO >= 100    
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif


void init_serial();
void flushInput();
void serialEvent();
char SerialReceive();

// declaration of the function as weak
//_weak function are methods that can be overwritten by user function with same name, used to define vector tables, and default handlers

void init_serial() __attribute__((weak));
void flushInput() __attribute__((weak));
void serialEvent() __attribute__((weak));
char SerialReceive() __attribute__((weak));

#endif