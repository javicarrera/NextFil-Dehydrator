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

#include "macros.h"

#define BOARD_UNKNOWN -1

//
// Arduino
//

#define BOARD_PRO_MINI_328          1000  // Arduino Pro mini 328
#define BOARD_PRO_MINI_168          1001  // Arduino Pro mini 168
#define BOARD_UNO                   1002  // Arduino Uno
#define BOARD_NANO                  1003  // Arduino Nano
#define BOARD_MICRO                 1004  // Arduino Nano

//
// Custom board
//

#define BOARD_CUSTOM                9998  // Custom pins definition for development and/or rare boards

#define _MB_1(B)  (defined(BOARD_##B) && MOTHERBOARD==BOARD_##B)
#define MB(V...)  DO(MB,||,V)
