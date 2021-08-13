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
 * Conditionals_post.h
 * Defines that depend on configuration but are not editable.
 */

#ifdef GITHUB_ACTIONS
  // Extras for CI testing
#endif

/**
 * LCD Contrast for Graphical Displays
 */
#if ENABLED(CARTESIO_UI)
  #define _LCD_CONTRAST_MIN   60
  #define _LCD_CONTRAST_INIT  90
  #define _LCD_CONTRAST_MAX  140
#elif ENABLED(miniVIKI)
  #define _LCD_CONTRAST_MIN   75
  #define _LCD_CONTRAST_INIT  95
  #define _LCD_CONTRAST_MAX  115
#elif ENABLED(VIKI2)
  #define _LCD_CONTRAST_INIT 140
#elif ENABLED(ELB_FULL_GRAPHIC_CONTROLLER)
  #define _LCD_CONTRAST_MIN   90
  #define _LCD_CONTRAST_INIT 110
  #define _LCD_CONTRAST_MAX  130
#elif ENABLED(AZSMZ_12864)
  #define _LCD_CONTRAST_MIN  120
  #define _LCD_CONTRAST_INIT 190
#elif EITHER(MKS_LCD12864A, MKS_LCD12864B)
  #define _LCD_CONTRAST_MIN  120
  #define _LCD_CONTRAST_INIT 205
#elif EITHER(MKS_MINI_12864, ENDER2_STOCKDISPLAY)
  #define _LCD_CONTRAST_MIN  120
  #define _LCD_CONTRAST_INIT 195
#elif ENABLED(MKS_MINI_12864_V3)
  #define _LCD_CONTRAST_MIN  255
  #define _LCD_CONTRAST_INIT 255
  #define _LCD_CONTRAST_MAX  255
#elif ENABLED(FYSETC_MINI_12864)
  #define _LCD_CONTRAST_INIT 220
#elif ENABLED(ULTI_CONTROLLER)
  #define _LCD_CONTRAST_INIT 127
  #define _LCD_CONTRAST_MAX  254
#elif ENABLED(MAKRPANEL)
  #define _LCD_CONTRAST_INIT  17
#elif ENABLED(MINIPANEL)
  #define _LCD_CONTRAST_INIT 150
#elif ENABLED(ZONESTAR_12864OLED)
  #define _LCD_CONTRAST_MIN   64
  #define _LCD_CONTRAST_INIT 128
  #define _LCD_CONTRAST_MAX  255
#elif IS_TFTGLCD_PANEL
  #define _LCD_CONTRAST_MIN    0
  #define _LCD_CONTRAST_INIT 250
  #define _LCD_CONTRAST_MAX  255
#endif

#ifdef _LCD_CONTRAST_INIT
  #define HAS_LCD_CONTRAST 1
#endif

#if HAS_LCD_CONTRAST
  #ifndef LCD_CONTRAST_MIN
    #ifdef _LCD_CONTRAST_MIN
      #define LCD_CONTRAST_MIN _LCD_CONTRAST_MIN
    #else
      #define LCD_CONTRAST_MIN 0
    #endif
  #endif
  #ifndef LCD_CONTRAST_INIT
    #define LCD_CONTRAST_INIT _LCD_CONTRAST_INIT
  #endif
  #ifndef LCD_CONTRAST_MAX
    #ifdef _LCD_CONTRAST_MAX
      #define LCD_CONTRAST_MAX _LCD_CONTRAST_MAX
    #elif _LCD_CONTRAST_INIT > 63
      #define LCD_CONTRAST_MAX 255
    #else
      #define LCD_CONTRAST_MAX 63   // ST7567 6-bits contrast
    #endif
  #endif
  #ifndef DEFAULT_LCD_CONTRAST
    #define DEFAULT_LCD_CONTRAST LCD_CONTRAST_INIT
  #endif
#endif


/**
 * Set ENDSTOPPULLUPS for active endstop switches
 */
#if ENABLED(ENDSTOPPULLUPS)
  #if ENABLED(USE_XMAX_PLUG)
    #define ENDSTOPPULLUP_XMAX
  #endif
#endif

/**
 * Set ENDSTOPPULLDOWNS for active endstop switches
 */
#if ENABLED(ENDSTOPPULLDOWNS)
  #if ENABLED(USE_XMAX_PLUG)
    #define ENDSTOPPULLDOWN_XMAX
  #endif
#endif

//
// ADC Temp Sensors (Thermistor or Thermocouple with amplifier ADC interface)
//
#define HAS_ADC_TEST(P) (PIN_EXISTS(TEMP_##P) && TEMP_SENSOR_##P != 0 && NONE(TEMP_SENSOR_##P##_IS_MAX_TC, TEMP_SENSOR_##P##_IS_DUMMY))
#if HAS_ADC_TEST(0)
  #define HAS_TEMP_ADC_0 1
#endif
#if HAS_ADC_TEST(1)
  #define HAS_TEMP_ADC_1 1
#endif
#if HAS_ADC_TEST(2)
  #define HAS_TEMP_ADC_2 1
#endif
#if HAS_ADC_TEST(3)
  #define HAS_TEMP_ADC_3 1
#endif
#if HAS_ADC_TEST(4)
  #define HAS_TEMP_ADC_4 1
#endif
#if HAS_ADC_TEST(5)
  #define HAS_TEMP_ADC_5 1
#endif
#if HAS_ADC_TEST(6)
  #define HAS_TEMP_ADC_6 1
#endif
#if HAS_ADC_TEST(7)
  #define HAS_TEMP_ADC_7 1
#endif
#if HAS_ADC_TEST(BED)
  #define HAS_TEMP_ADC_BED 1
#endif
#if HAS_ADC_TEST(PROBE)
  #define HAS_TEMP_ADC_PROBE 1
#endif
#if HAS_ADC_TEST(CHAMBER)
  #define HAS_TEMP_ADC_CHAMBER 1
#endif
#if HAS_ADC_TEST(COOLER)
  #define HAS_TEMP_ADC_COOLER 1
#endif
#if HAS_ADC_TEST(BOARD)
  #define HAS_TEMP_ADC_BOARD 1
#endif
#if HAS_ADC_TEST(REDUNDANT)
  #define HAS_TEMP_ADC_REDUNDANT 1
#endif

#define HAS_TEMP(N) ANY(HAS_TEMP_ADC_##N, TEMP_SENSOR_##N##_IS_MAX_TC, TEMP_SENSOR_##N##_IS_DUMMY)
#if HAS_HOTEND && HAS_TEMP(0)
  #define HAS_TEMP_HOTEND 1
#endif
#if HAS_TEMP(BED)
  #define HAS_TEMP_BED 1
#endif
#if HAS_TEMP(PROBE)
  #define HAS_TEMP_PROBE 1
#endif
#if HAS_TEMP(CHAMBER)
  #define HAS_TEMP_CHAMBER 1
#endif
#if HAS_TEMP(COOLER)
  #define HAS_TEMP_COOLER 1
#endif
#if HAS_TEMP(BOARD)
  #define HAS_TEMP_BOARD 1
#endif
#if HAS_TEMP(REDUNDANT)
  #define HAS_TEMP_REDUNDANT 1
#endif

// Heaters
#if PIN_EXISTS(HEATER_0)
  #define HAS_HEATER_0 1
#endif
#if PIN_EXISTS(HEATER_1)
  #define HAS_HEATER_1 1
#endif
#if PIN_EXISTS(HEATER_2)
  #define HAS_HEATER_2 1
#endif
#if PIN_EXISTS(HEATER_3)
  #define HAS_HEATER_3 1
#endif
#if PIN_EXISTS(HEATER_4)
  #define HAS_HEATER_4 1
#endif
#if PIN_EXISTS(HEATER_5)
  #define HAS_HEATER_5 1
#endif
#if PIN_EXISTS(HEATER_6)
  #define HAS_HEATER_6 1
#endif
#if PIN_EXISTS(HEATER_7)
  #define HAS_HEATER_7 1
#endif
#if PIN_EXISTS(HEATER_BED)
  #define HAS_HEATER_BED 1
#endif

// PID heating
#if ANY(PIDTEMP, PIDTEMPBED, PIDTEMPCHAMBER)
  #define HAS_PID_HEATING 1
#endif

// Thermal protection
#if BOTH(HAS_HEATED_BED, THERMAL_PROTECTION_BED)
  #define HAS_THERMALLY_PROTECTED_BED 1
#endif
#if ENABLED(THERMAL_PROTECTION_HOTENDS) && WATCH_TEMP_PERIOD > 0
  #define WATCH_HOTENDS 1
#endif
#if HAS_THERMALLY_PROTECTED_BED && WATCH_BED_TEMP_PERIOD > 0
  #define WATCH_BED 1
#endif
#if BOTH(HAS_HEATED_CHAMBER, THERMAL_PROTECTION_CHAMBER) && WATCH_CHAMBER_TEMP_PERIOD > 0
  #define WATCH_CHAMBER 1
#endif
#if BOTH(HAS_COOLER, THERMAL_PROTECTION_COOLER) && WATCH_COOLER_TEMP_PERIOD > 0
  #define WATCH_COOLER 1
#endif
#if  (ENABLED(THERMAL_PROTECTION_HOTENDS) || !EXTRUDERS) \
  && (ENABLED(THERMAL_PROTECTION_BED)     || !HAS_HEATED_BED) \
  && (ENABLED(THERMAL_PROTECTION_CHAMBER) || !HAS_HEATED_CHAMBER) \
  && (ENABLED(THERMAL_PROTECTION_COOLER) || !HAS_COOLER)
  #define THERMALLY_SAFE 1
#endif

/**
 * Up to 3 PWM fans
 */
#ifndef FAN_INVERTING
  #define FAN_INVERTING false
#endif

#if HAS_FAN7
  #define FAN_COUNT 8
#elif HAS_FAN6
  #define FAN_COUNT 7
#elif HAS_FAN5
  #define FAN_COUNT 6
#elif HAS_FAN4
  #define FAN_COUNT 5
#elif HAS_FAN3
  #define FAN_COUNT 4
#elif HAS_FAN2
  #define FAN_COUNT 3
#elif HAS_FAN1
  #define FAN_COUNT 2
#elif HAS_FAN0
  #define FAN_COUNT 1
#else
  #define FAN_COUNT 0
#endif

#if FAN_COUNT > 0
  #define HAS_FAN 1
#endif

/**
 * MIN/MAX fan PWM scaling
 */
#ifndef FAN_OFF_PWM
  #define FAN_OFF_PWM 0
#endif
#ifndef FAN_MIN_PWM
  #if FAN_OFF_PWM > 0
    #define FAN_MIN_PWM (FAN_OFF_PWM + 1)
  #else
    #define FAN_MIN_PWM 0
  #endif
#endif
#ifndef FAN_MAX_PWM
  #define FAN_MAX_PWM 255
#endif
#if FAN_MIN_PWM < 0 || FAN_MIN_PWM > 255
  #error "FAN_MIN_PWM must be a value from 0 to 255."
#elif FAN_MAX_PWM < 0 || FAN_MAX_PWM > 255
  #error "FAN_MAX_PWM must be a value from 0 to 255."
#elif FAN_MIN_PWM > FAN_MAX_PWM
  #error "FAN_MIN_PWM must be less than or equal to FAN_MAX_PWM."
#elif FAN_OFF_PWM > FAN_MIN_PWM
  #error "FAN_OFF_PWM must be less than or equal to FAN_MIN_PWM."
#endif

/**
 * FAST PWM FAN Settings
 */
#if ENABLED(FAST_PWM_FAN) && !defined(FAST_PWM_FAN_FREQUENCY)
  #define FAST_PWM_FAN_FREQUENCY ((F_CPU) / (2 * 255 * 1)) // Fan frequency default
#endif

/**
 * Controller Fan Settings
 */
#if PIN_EXISTS(CONTROLLER_FAN)
  #define HAS_CONTROLLER_FAN 1
  #if CONTROLLER_FAN_MIN_BOARD_TEMP
    #define HAS_CONTROLLER_FAN_MIN_BOARD_TEMP 1
  #endif
#endif

#if HAS_CONTROLLER_FAN
  #if ENABLED(CONTROLLER_FAN_USE_BOARD_TEMP)
    #define HAS_CONTROLLER_FAN_BOARD_TEMP_TRIGGER 1
    #ifndef CONTROLLER_FAN_TRIGGER_TEMP
      #define CONTROLLER_FAN_TRIGGER_TEMP 30
    #endif
  #else
    #undef CONTROLLER_FAN_TRIGGER_TEMP
  #endif
#endif

/**
 * Heated bed requires settings
 */
#if HAS_HEATED_BED
  #ifndef MIN_BED_POWER
    #define MIN_BED_POWER 0
  #endif
  #ifndef MAX_BED_POWER
    #define MAX_BED_POWER 255
  #endif
  #ifndef HEATER_BED_INVERTING
    #define HEATER_BED_INVERTING false
  #endif
  #define WRITE_HEATER_BED(v) WRITE(HEATER_BED_PIN, (v) ^ HEATER_BED_INVERTING)
#endif

/**
 * Heated chamber requires settings
 */
#if HAS_HEATED_CHAMBER
  #ifndef MIN_CHAMBER_POWER
    #define MIN_CHAMBER_POWER 0
  #endif
  #ifndef MAX_CHAMBER_POWER
    #define MAX_CHAMBER_POWER 255
  #endif
  #ifndef HEATER_CHAMBER_INVERTING
    #define HEATER_CHAMBER_INVERTING false
  #endif
  #define WRITE_HEATER_CHAMBER(v) WRITE(HEATER_CHAMBER_PIN, (v) ^ HEATER_CHAMBER_INVERTING)
#endif


/**
 * MIN/MAX case light PWM scaling
 */
#if ENABLED(CASE_LIGHT_ENABLE)
  #ifndef CASE_LIGHT_MAX_PWM
    #define CASE_LIGHT_MAX_PWM 255
  #elif !WITHIN(CASE_LIGHT_MAX_PWM, 1, 255)
    #error "CASE_LIGHT_MAX_PWM must be a value from 1 to 255."
  #endif
#endif

/**
 * Heater, Fan, and Probe interactions
 */
#if !HAS_FAN
  #undef ADAPTIVE_FAN_SLOWING
  #undef NO_FAN_SLOWING_IN_PID_TUNING
#endif
#if !BOTH(HAS_BED_PROBE, HAS_FAN)
  #undef PROBING_FANS_OFF
#endif
#if !BOTH(HAS_BED_PROBE, HAS_EXTRUDERS)
  #undef PROBING_ESTEPPERS_OFF
#endif
#if BOTH(PROBING_STEPPERS_OFF, PROBING_ESTEPPERS_OFF)
  #undef PROBING_ESTEPPERS_OFF
  #warning "PROBING_STEPPERS_OFF includes PROBING_ESTEPPERS_OFF. Disabling PROBING_ESTEPPERS_OFF."
#endif
#if EITHER(ADVANCED_PAUSE_FEATURE, PROBING_HEATERS_OFF)
  #define HEATER_IDLE_HANDLER 1
#endif
#if HAS_BED_PROBE && (ANY(PROBING_HEATERS_OFF, PROBING_STEPPERS_OFF, PROBING_ESTEPPERS_OFF, PROBING_FANS_OFF) || DELAY_BEFORE_PROBING > 0)
  #define HAS_QUIET_PROBING 1
#endif

/**
 * Buzzer/Speaker
 */

#if PIN_EXISTS(BEEPER)
  #define USE_BEEPER 1
#endif
#if USE_BEEPER || ANY(LCD_USE_I2C_BUZZER, PCA9632_BUZZER)
  #define HAS_BUZZER 1
#endif

#if ENABLED(LCD_USE_I2C_BUZZER)
  #ifndef LCD_FEEDBACK_FREQUENCY_HZ
    #define LCD_FEEDBACK_FREQUENCY_HZ 1000
  #endif
  #ifndef LCD_FEEDBACK_FREQUENCY_DURATION_MS
    #define LCD_FEEDBACK_FREQUENCY_DURATION_MS 100
  #endif
#elif HAS_BUZZER
  #ifndef LCD_FEEDBACK_FREQUENCY_HZ
    #define LCD_FEEDBACK_FREQUENCY_HZ 5000
  #endif
  #ifndef LCD_FEEDBACK_FREQUENCY_DURATION_MS
    #define LCD_FEEDBACK_FREQUENCY_DURATION_MS 2
  #endif
#endif

#if HAS_BUZZER
  #if LCD_FEEDBACK_FREQUENCY_DURATION_MS && LCD_FEEDBACK_FREQUENCY_HZ
    #define HAS_CHIRP 1
  #endif
#else
  #undef SOUND_MENU_ITEM   // No buzzer menu item without a buzzer
#endif

/**
 * Make sure DOGLCD_SCK and DOGLCD_MOSI are defined.
 */
#if HAS_MARLINUI_U8GLIB
  #ifndef DOGLCD_SCK
    #define DOGLCD_SCK  SD_SCK_PIN
  #endif
  #ifndef DOGLCD_MOSI
    #define DOGLCD_MOSI SD_MOSI_PIN
  #endif
#endif

#if HAS_LCD_MENU
  // LCD timeout to status screen default is 15s
  #ifndef LCD_TIMEOUT_TO_STATUS
    #define LCD_TIMEOUT_TO_STATUS 15000
  #endif
  #if LCD_TIMEOUT_TO_STATUS
    #define SCREENS_CAN_TIME_OUT 1
  #endif
#endif


#if HAS_WIRED_LCD
  // Get LCD character width/height, which may be overridden by pins, configs, etc.
  #ifndef LCD_WIDTH
    #if HAS_MARLINUI_U8GLIB
      #define LCD_WIDTH 21
    #else
      #define LCD_WIDTH TERN(IS_ULTIPANEL, 20, 16)
    #endif
  #endif
  #ifndef LCD_HEIGHT
    #if HAS_MARLINUI_U8GLIB
      #define LCD_HEIGHT 5
    #else
      #define LCD_HEIGHT TERN(IS_ULTIPANEL, 4, 2)
    #endif
  #endif
#endif