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
 * File: pins/pins.h
 *
 * Include pins definitions
 *
 * Pins numbering schemes:
 *
 *  - Digital I/O pin number if used by READ/WRITE macros. (e.g., X_STEP_DIR)
 *    The FastIO headers map digital pins to their ports and functions.
 *
 *  - Analog Input number if used by analogRead or DAC. (e.g., TEMP_n_PIN)
 *    These numbers are the same in any pin mapping.
 */

//
// Arduino
//

#if MB(PRO_MINI_328) || MB(PRO_MINI_168)
  #include "pins_PRO_MINI.h"            
#elif MB(UNO)
  #include "pins_UNO.h"       
#elif MB(NANO)
  #include "pins_NANO.h"      
#elif MB(MICRO)
  #include "pins_MICRO.h"          
#else
  #error "Unknown MOTHERBOARD value set in Configuration.h"
#endif

//
// Post-process pins according to configured settings
//
//#include "pins_postprocess.h"
