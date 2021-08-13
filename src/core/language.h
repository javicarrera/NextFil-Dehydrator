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

#include "../inc/NextFilConfig.h"

#define _UxGT(a) a

// Fallback if no language is set. DON'T CHANGE
#ifndef LCD_LANGUAGE
  #define LCD_LANGUAGE en
#endif

// For character-based LCD controllers (DISPLAY_CHARSET_HD44780)
#define JAPANESE 1
#define WESTERN  2
#define CYRILLIC 3

// NOTE: IF YOU CHANGE LANGUAGE FILES OR MERGE A FILE WITH CHANGES
//
//   ==> ALWAYS TRY TO COMPILE MARLIN WITH/WITHOUT "ULTIPANEL" / "ULTRA_LCD" / "SDSUPPORT" #define IN "Configuration.h"
//   ==> ALSO TRY ALL AVAILABLE LANGUAGE OPTIONS
// See also https://marlinfw.org/docs/development/lcd_language.html

// Languages
// an         Aragonese
// bg         Bulgarian
// ca         Catalan
// cz         Czech
// da         Danish
// de         German
// el         Greek
// el_gr      Greek (Greece)
// en         English
// es         Spanish
// eu         Basque-Euskera
// fi         Finnish
// fr         French
// gl         Galician
// hr         Croatian
// hu         Hungarian
// it         Italian
// jp_kana    Japanese
// ko_KR      Korean (South Korea)
// nl         Dutch
// pl         Polish
// pt         Portuguese
// pt_br      Portuguese (Brazilian)
// ro         Romanian
// ru         Russian
// sk         Slovak
// sv         Swedish
// tr         Turkish
// uk         Ukrainian
// vi         Vietnamese
// zh_CN      Chinese (Simplified)
// zh_TW      Chinese (Traditional)

#ifdef DEFAULT_SOURCE_CODE_URL
  #undef  SOURCE_CODE_URL
  #define SOURCE_CODE_URL DEFAULT_SOURCE_CODE_URL
#endif

#ifdef CUSTOM_MACHINE_NAME
  #undef  MACHINE_NAME
  #define MACHINE_NAME CUSTOM_MACHINE_NAME
#elif defined(DEFAULT_MACHINE_NAME)
  #undef  MACHINE_NAME
  #define MACHINE_NAME DEFAULT_MACHINE_NAME
#endif

#ifndef MACHINE_UUID
  #define MACHINE_UUID DEFAULT_MACHINE_UUID
#endif

//
// Common Serial Console Messages
// Don't change these strings because serial hosts look for them.
//

#define STR_ENQUEUEING                      "enqueueing \""
#define STR_POWERUP                         "PowerUp"
#define STR_EXTERNAL_RESET                  " External Reset"
#define STR_BROWNOUT_RESET                  " Brown out Reset"
#define STR_WATCHDOG_RESET                  " Watchdog Reset"
#define STR_SOFTWARE_RESET                  " Software Reset"
#define STR_FREE_MEMORY                     " Free Memory: "
#define STR_PLANNER_BUFFER_BYTES            "  PlannerBufferBytes: "
#define STR_OK                              "ok"
#define STR_WAIT                            "wait"
#define STR_STATS                           "Stats: "
#define STR_FILE_SAVED                      "Done saving file."
#define STR_ERR_LINE_NO                     "Line Number is not Last Line Number+1, Last Line: "
#define STR_ERR_CHECKSUM_MISMATCH           "checksum mismatch, Last Line: "
#define STR_ERR_NO_CHECKSUM                 "No Checksum with line number, Last Line: "
#define STR_FILE_PRINTED                    "Done printing file"
#define STR_NO_MEDIA                        "No media"
#define STR_BEGIN_FILE_LIST                 "Begin file list"
#define STR_END_FILE_LIST                   "End file list"
#define STR_INVALID_EXTRUDER                "Invalid extruder"
#define STR_INVALID_E_STEPPER               "Invalid E stepper"
#define STR_E_STEPPER_NOT_SPECIFIED         "E stepper not specified"
#define STR_INVALID_SOLENOID                "Invalid solenoid"
#define STR_COUNT_X                         " Count X:"
#define STR_COUNT_A                         " Count A:"
#define STR_WATCHDOG_FIRED                  "Watchdog timeout. Reset required."
#define STR_ERR_KILLED                      "Printer halted. kill() called!"
#define STR_FLOWMETER_FAULT                 "Coolant flow fault. Flowmeter safety is active. Attention required."
#define STR_ERR_STOPPED                     "Printer stopped due to errors. Fix the error and use M999 to restart. (Temperature is reset. Set it after restarting)"
#define STR_ERR_SERIAL_MISMATCH             "Serial status mismatch"
#define STR_BUSY_PROCESSING                 "busy: processing"
#define STR_BUSY_PAUSED_FOR_USER            "busy: paused for user"
#define STR_BUSY_PAUSED_FOR_INPUT           "busy: paused for input"
#define STR_Z_MOVE_COMP                     "Z_move_comp"
#define STR_RESEND                          "Resend: "
#define STR_UNKNOWN_COMMAND                 "Unknown command: \""
#define STR_ACTIVE_EXTRUDER                 "Active Extruder: "

#define STR_PROBE_OFFSET                    "Probe Offset"
#define STR_SKEW_MIN                        "min_skew_factor: "
#define STR_SKEW_MAX                        "max_skew_factor: "
#define STR_ERR_MATERIAL_INDEX              "M145 S<index> out of range (0-1)"
#define STR_ERR_M421_PARAMETERS             "M421 incorrect parameter usage"
#define STR_ERR_BAD_PLANE_MODE              "G5 requires XY plane mode"
#define STR_ERR_MESH_XY                     "Mesh point out of range"
#define STR_ERR_ARC_ARGS                    "G2/G3 bad parameters"
#define STR_ERR_PROTECTED_PIN               "Protected Pin"
#define STR_ERR_M420_FAILED                 "Failed to enable Bed Leveling"
#define STR_ERR_M428_TOO_FAR                "Too far from reference point"
#define STR_ERR_M303_DISABLED               "PIDTEMP disabled"
#define STR_M119_REPORT                     "Reporting endstop status"
#define STR_ON                              "ON"
#define STR_OFF                             "OFF"
#define STR_ENDSTOP_HIT                     "TRIGGERED"
#define STR_ENDSTOP_OPEN                    "open"
#define STR_HOTEND_OFFSET                   "Hotend offsets:"
#define STR_DUPLICATION_MODE                "Duplication mode: "
#define STR_SOFT_ENDSTOPS                   "Soft endstops: "
#define STR_SOFT_MIN                        "  Min: "
#define STR_SOFT_MAX                        "  Max: "

#define STR_SAVED_POS                       "Position saved"
#define STR_RESTORING_POS                   "Restoring position"
#define STR_INVALID_POS_SLOT                "Invalid slot. Total: "

#define STR_SD_CANT_OPEN_SUBDIR             "Cannot open subdir "
#define STR_SD_INIT_FAIL                    "No SD card"
#define STR_SD_VOL_INIT_FAIL                "volume.init failed"
#define STR_SD_OPENROOT_FAIL                "openRoot failed"
#define STR_SD_CARD_OK                      "SD card ok"
#define STR_SD_WORKDIR_FAIL                 "workDir open failed"
#define STR_SD_OPEN_FILE_FAIL               "open failed, File: "
#define STR_SD_FILE_OPENED                  "File opened: "
#define STR_SD_SIZE                         " Size: "
#define STR_SD_FILE_SELECTED                "File selected"
#define STR_SD_WRITE_TO_FILE                "Writing to file: "
#define STR_SD_PRINTING_BYTE                "SD printing byte "
#define STR_SD_NOT_PRINTING                 "Not SD printing"
#define STR_SD_ERR_WRITE_TO_FILE            "error writing to file"
#define STR_SD_ERR_READ                     "SD read error"
#define STR_SD_CANT_ENTER_SUBDIR            "Cannot enter subdir: "

#define STR_ENDSTOPS_HIT                    "endstops hit: "
#define STR_ERR_COLD_EXTRUDE_STOP           " cold extrusion prevented"
#define STR_ERR_LONG_EXTRUDE_STOP           " too long extrusion prevented"
#define STR_ERR_HOTEND_TOO_COLD             "Hotend too cold"
#define STR_ERR_EEPROM_WRITE                "Error writing to EEPROM!"

#define STR_FILAMENT_CHANGE_HEAT_LCD        "Press button to heat nozzle"
#define STR_FILAMENT_CHANGE_INSERT_LCD      "Insert filament and press button"
#define STR_FILAMENT_CHANGE_WAIT_LCD        "Press button to resume"
#define STR_FILAMENT_CHANGE_HEAT_M108       "Send M108 to heat nozzle"
#define STR_FILAMENT_CHANGE_INSERT_M108     "Insert filament and send M108"
#define STR_FILAMENT_CHANGE_WAIT_M108       "Send M108 to resume"

#define STR_STOP_BLTOUCH                    "!! STOP called because of BLTouch error - restart with M999"
#define STR_STOP_UNHOMED                    "!! STOP called because of unhomed error - restart with M999"
#define STR_KILL_INACTIVE_TIME              "!! KILL caused by too much inactive time - current command: "
#define STR_KILL_BUTTON                     "!! KILL caused by KILL button/pin"

// temperature.cpp strings
#define STR_PID_AUTOTUNE_START              "PID Autotune start"
#define STR_PID_BAD_HEATER_ID               "PID Autotune failed! Bad heater id"
#define STR_PID_TEMP_TOO_HIGH               "PID Autotune failed! Temperature too high"
#define STR_PID_TIMEOUT                     "PID Autotune failed! timeout"
#define STR_BIAS                            " bias: "
#define STR_D_COLON                         " d: "
#define STR_T_MIN                           " min: "
#define STR_T_MAX                           " max: "
#define STR_KU                              " Ku: "
#define STR_TU                              " Tu: "
#define STR_CLASSIC_PID                     " Classic PID "
#define STR_KP                              " Kp: "
#define STR_KI                              " Ki: "
#define STR_KD                              " Kd: "
#define STR_PID_AUTOTUNE_FINISHED           "PID Autotune finished! Put the last Kp, Ki and Kd constants from below into Configuration.h"
#define STR_PID_DEBUG                       " PID_DEBUG "
#define STR_PID_DEBUG_INPUT                 ": Input "
#define STR_PID_DEBUG_OUTPUT                " Output "
#define STR_PID_DEBUG_PTERM                 " pTerm "
#define STR_PID_DEBUG_ITERM                 " iTerm "
#define STR_PID_DEBUG_DTERM                 " dTerm "
#define STR_PID_DEBUG_CTERM                 " cTerm "
#define STR_INVALID_EXTRUDER_NUM            " - Invalid extruder number !"

#define STR_HEATER_BED                      "bed"
#define STR_HEATER_CHAMBER                  "chamber"
#define STR_COOLER                          "cooler"
#define STR_MOTHERBOARD                     "motherboard"
#define STR_PROBE                           "probe"
#define STR_REDUNDANT                       "redundant "
#define STR_LASER_TEMP                      "laser temperature"

#define STR_STOPPED_HEATER                  ", system stopped! Heater_ID: "
#define STR_REDUNDANCY                      "Heater switched off. Temperature difference between temp sensors is too high !"
#define STR_T_HEATING_FAILED                "Heating failed"
#define STR_T_THERMAL_RUNAWAY               "Thermal Runaway"
#define STR_T_MAXTEMP                       "MAXTEMP triggered"
#define STR_T_MINTEMP                       "MINTEMP triggered"
#define STR_ERR_PROBING_FAILED              "Probing Failed"
#define STR_ZPROBE_OUT_SER                  "Z Probe Past Bed"

// Debug
#define STR_DEBUG_PREFIX                    "DEBUG:"
#define STR_DEBUG_OFF                       "off"
#define STR_DEBUG_ECHO                      "ECHO"
#define STR_DEBUG_INFO                      "INFO"
#define STR_DEBUG_ERRORS                    "ERRORS"
#define STR_DEBUG_DRYRUN                    "DRYRUN"
#define STR_DEBUG_COMMUNICATION             "COMMUNICATION"
#define STR_DEBUG_LEVELING                  "LEVELING"

#define STR_PRINTER_LOCKED                  "Printer locked! (Unlock with M511 or LCD)"
#define STR_WRONG_PASSWORD                  "Incorrect Password"
#define STR_PASSWORD_TOO_LONG               "Password too long"
#define STR_PASSWORD_REMOVED                "Password removed"
#define STR_REMINDER_SAVE_SETTINGS          "Remember to save!"
#define STR_PASSWORD_SET                    "Password is "
