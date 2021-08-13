/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Arduino Pro Mini pin assignments
 */

#define BOARD_INFO_NAME "Arduino Pro Mini"

//#define LCD_TX              0 // SERIAL RX
//#define LCD_RX              1 // SERIAL TX

//#define NOT_USED            2 // Interrupt capable

#define NEOPIXEL_PIN          3 // Interrupt, PWM capable
#define ALARMA_PIN            4
#define RELE_RESISTENCIA_PIN  5 // PWM capable
#define RELE_VENTILADOR_PIN   6 // PWM capable
#define NEXTION_TX_PIN        7 // SOFTWARESERIAL - NEXTION TX
#define PESO_DATA2_PIN        8
#define PESO_SCK2_PIN         9 // PWM capable
#define PESO_SCK_PIN         10 // PWM capable, SPI-SS
#define PESO_DATA_PIN        11 // PWM capable, SPI-MOSI
#define SWITCH_PIN           12 // SPI MISO
#define NEXTION_RX_PIN       13 // SOFTWARESERIAL - NEXTION RX

//#define NOT_USED           A0
//#define NOT_USED           A1

#define SENSOR_INT_PIN       A2
#define SENSOR_EXT_PIN       A3

//#define NOT_USED           A4 // SDA I2C
//#define NOT_USED           A5 // SCL I2C