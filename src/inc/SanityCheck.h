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
 * SanityCheck.h
 *
 * Test configuration values for errors at compile-time.
 */

/**
 * Require gcc 4.7 or newer (first included with Arduino 1.6.8) for C++11 features.
 */
#if __cplusplus < 201103L
  #error "Marlin requires C++11 support (gcc >= 4.7, Arduino IDE >= 1.6.8). Please upgrade your toolchain."
#endif

// Make sure macros aren't borked
#define TEST1
#define TEST2 1
#define TEST3 0
#define TEST4 true
#if ENABLED(TEST0) || !ENABLED(TEST2) || ENABLED(TEST3)
  #error "ENABLED is borked!"
#endif
#if BOTH(TEST0, TEST1)
  #error "BOTH is borked!"
#endif
#if DISABLED(TEST1) || !DISABLED(TEST3) || DISABLED(TEST4) || DISABLED(TEST0, TEST1, TEST2, TEST4) || !DISABLED(TEST0, TEST3)
  #error "DISABLED is borked!"
#endif
#if !ANY(TEST1, TEST2, TEST3, TEST4) || ANY(TEST0, TEST3)
  #error "ANY is borked!"
#endif
#if NONE(TEST0, TEST1, TEST2, TEST4) || !NONE(TEST0, TEST3)
  #error "NONE is borked!"
#endif
#undef TEST1
#undef TEST2
#undef TEST3
#undef TEST4

/**
 * This is to alert you about non-matching versions of config files.
 *
 * You can edit the version tag in your old config files and try the build again.
 * The checks below will alert you about options that need to be changed, but they won't
 * tell you about new options that you might find useful. So it's recommended to transfer
 * your settings to new Configuration files matching your Marlin version as soon as possible.
 */
#define HEXIFY(H) _CAT(0x,H)
#if !defined(CONFIGURATION_H_VERSION) || HEXIFY(CONFIGURATION_H_VERSION) < HEXIFY(REQUIRED_CONFIGURATION_H_VERSION)
  #error "Your Configuration.h file is for an old version of Marlin. Downgrade Marlin or upgrade your Configuration.h."
#elif HEXIFY(CONFIGURATION_H_VERSION) > HEXIFY(REQUIRED_CONFIGURATION_H_VERSION)
  #error "Your Configuration.h file is for a newer version of Marlin. Upgrade Marlin or downgrade your Configuration.h."
#endif
#if !defined(CONFIGURATION_ADV_H_VERSION) || HEXIFY(CONFIGURATION_ADV_H_VERSION) < HEXIFY(REQUIRED_CONFIGURATION_ADV_H_VERSION)
  #error "Your Configuration_adv.h file is for an old version of Marlin. Downgrade Marlin or upgrade your Configuration_adv.h."
#elif HEXIFY(CONFIGURATION_ADV_H_VERSION) > HEXIFY(REQUIRED_CONFIGURATION_ADV_H_VERSION)
  #error "Your Configuration_adv.h file is for a newer version of Marlin. Upgrade Marlin or downgrade your Configuration_adv.h."
#endif
#undef HEXIFY

/**
 * Warnings for old configurations
 */
#ifndef MOTHERBOARD
  #error "MOTHERBOARD is required."
#elif !defined(X_BED_SIZE) || !defined(Y_BED_SIZE)
  #error "X_BED_SIZE and Y_BED_SIZE are now required!"
#elif WATCH_TEMP_PERIOD > 500
  #error "WATCH_TEMP_PERIOD now uses seconds instead of milliseconds."
#elif DISABLED(THERMAL_PROTECTION_HOTENDS) && (defined(WATCH_TEMP_PERIOD) || defined(THERMAL_PROTECTION_PERIOD))
  #error "Thermal Runaway Protection for hotends is now enabled with THERMAL_PROTECTION_HOTENDS."
#elif DISABLED(THERMAL_PROTECTION_BED) && defined(THERMAL_PROTECTION_BED_PERIOD)
  #error "Thermal Runaway Protection for the bed is now enabled with THERMAL_PROTECTION_BED."
#elif (CORE_IS_XZ || CORE_IS_YZ) && ENABLED(Z_LATE_ENABLE)
  #error "Z_LATE_ENABLE can't be used with COREXZ, COREZX, COREYZ, or COREZY."
#elif defined(X_HOME_RETRACT_MM)
  #error "[XYZ]_HOME_RETRACT_MM settings have been renamed [XYZ]_HOME_BUMP_MM."
#elif defined(SDCARDDETECTINVERTED)
  #error "SDCARDDETECTINVERTED is now SD_DETECT_STATE (HIGH)."
#elif defined(SD_DETECT_INVERTED)
  #error "SD_DETECT_INVERTED is now SD_DETECT_STATE (HIGH)."
#elif defined(BTENABLED)
  #error "BTENABLED is now BLUETOOTH."
#elif defined(CUSTOM_MENDEL_NAME)
  #error "CUSTOM_MENDEL_NAME is now CUSTOM_MACHINE_NAME."
#elif defined(HAS_AUTOMATIC_VERSIONING)
  #error "HAS_AUTOMATIC_VERSIONING is now CUSTOM_VERSION_FILE."
#elif defined(USE_AUTOMATIC_VERSIONING)
  #error "USE_AUTOMATIC_VERSIONING is now CUSTOM_VERSION_FILE."
#elif defined(SDSLOW)
  #error "SDSLOW deprecated. Set SD_SPI_SPEED to SPI_HALF_SPEED instead."
#elif defined(SDEXTRASLOW)
  #error "SDEXTRASLOW deprecated. Set SD_SPI_SPEED to SPI_QUARTER_SPEED instead."
#elif defined(FILAMENT_SENSOR)
  #error "FILAMENT_SENSOR is now FILAMENT_WIDTH_SENSOR."
#elif defined(ENDSTOPPULLUP_FIL_RUNOUT)
  #error "ENDSTOPPULLUP_FIL_RUNOUT is now FIL_RUNOUT_PULLUP."
#elif defined(DISABLE_MAX_ENDSTOPS) || defined(DISABLE_MIN_ENDSTOPS)
  #error "DISABLE_MAX_ENDSTOPS and DISABLE_MIN_ENDSTOPS deprecated. Use individual USE_*_PLUG options instead."
#elif defined(LANGUAGE_INCLUDE)
  #error "LANGUAGE_INCLUDE has been replaced by LCD_LANGUAGE."
#elif defined(EXTRUDER_OFFSET_X) || defined(EXTRUDER_OFFSET_Y)
  #error "EXTRUDER_OFFSET_[XY] is deprecated. Use HOTEND_OFFSET_[XY] instead."
#elif defined(PID_PARAMS_PER_EXTRUDER)
  #error "PID_PARAMS_PER_EXTRUDER is deprecated. Use PID_PARAMS_PER_HOTEND instead."
#elif defined(EXTRUDER_WATTS) || defined(BED_WATTS)
  #error "EXTRUDER_WATTS and BED_WATTS are deprecated and should be removed."
#elif defined(SERVO_ENDSTOP_ANGLES)
  #error "SERVO_ENDSTOP_ANGLES is deprecated. Use Z_SERVO_ANGLES instead."
#elif defined(X_ENDSTOP_SERVO_NR) || defined(Y_ENDSTOP_SERVO_NR)
  #error "X_ENDSTOP_SERVO_NR and Y_ENDSTOP_SERVO_NR are deprecated and should be removed."
#elif defined(Z_ENDSTOP_SERVO_NR)
  #error "Z_ENDSTOP_SERVO_NR is now Z_PROBE_SERVO_NR."
#elif defined(DEFAULT_XYJERK)
  #error "DEFAULT_XYJERK is deprecated. Use DEFAULT_XJERK and DEFAULT_YJERK instead."
#elif defined(XY_TRAVEL_SPEED)
  #error "XY_TRAVEL_SPEED is now XY_PROBE_FEEDRATE."
#elif defined(XY_PROBE_SPEED)
  #error "XY_PROBE_SPEED is now XY_PROBE_FEEDRATE."
#elif defined(Z_PROBE_SPEED_FAST)
  #error "Z_PROBE_SPEED_FAST is now Z_PROBE_FEEDRATE_FAST."
#elif defined(Z_PROBE_SPEED_SLOW)
  #error "Z_PROBE_SPEED_SLOW is now Z_PROBE_FEEDRATE_SLOW."
#elif defined(PROBE_SERVO_DEACTIVATION_DELAY)
  #error "PROBE_SERVO_DEACTIVATION_DELAY is deprecated. Use DEACTIVATE_SERVOS_AFTER_MOVE instead."
#elif defined(SERVO_DEACTIVATION_DELAY)
  #error "SERVO_DEACTIVATION_DELAY is now SERVO_DELAY."
#elif ENABLED(FILAMENTCHANGEENABLE)
  #error "FILAMENTCHANGEENABLE is now ADVANCED_PAUSE_FEATURE."
#elif ENABLED(FILAMENT_CHANGE_FEATURE)
  #error "FILAMENT_CHANGE_FEATURE is now ADVANCED_PAUSE_FEATURE."
#elif defined(FILAMENT_CHANGE_X_POS) || defined(FILAMENT_CHANGE_Y_POS)
  #error "FILAMENT_CHANGE_[XY]_POS is now set with NOZZLE_PARK_POINT."
#elif defined(FILAMENT_CHANGE_Z_ADD)
  #error "FILAMENT_CHANGE_Z_ADD is now set with NOZZLE_PARK_POINT."
#elif defined(FILAMENT_CHANGE_XY_FEEDRATE)
  #error "FILAMENT_CHANGE_XY_FEEDRATE is now NOZZLE_PARK_XY_FEEDRATE."
#elif defined(FILAMENT_CHANGE_Z_FEEDRATE)
  #error "FILAMENT_CHANGE_Z_FEEDRATE is now NOZZLE_PARK_Z_FEEDRATE."
#elif defined(PAUSE_PARK_X_POS) || defined(PAUSE_PARK_Y_POS)
  #error "PAUSE_PARK_[XY]_POS is now set with NOZZLE_PARK_POINT."
#elif defined(PAUSE_PARK_Z_ADD)
  #error "PAUSE_PARK_Z_ADD is now set with NOZZLE_PARK_POINT."
#elif defined(PAUSE_PARK_XY_FEEDRATE)
  #error "PAUSE_PARK_XY_FEEDRATE is now NOZZLE_PARK_XY_FEEDRATE."
#elif defined(PAUSE_PARK_Z_FEEDRATE)
  #error "PAUSE_PARK_Z_FEEDRATE is now NOZZLE_PARK_Z_FEEDRATE."
#elif defined(FILAMENT_CHANGE_RETRACT_FEEDRATE)
  #error "FILAMENT_CHANGE_RETRACT_FEEDRATE is now PAUSE_PARK_RETRACT_FEEDRATE."
#elif defined(FILAMENT_CHANGE_RETRACT_LENGTH)
  #error "FILAMENT_CHANGE_RETRACT_LENGTH is now PAUSE_PARK_RETRACT_LENGTH."
#elif defined(FILAMENT_CHANGE_EXTRUDE_FEEDRATE)
  #error "FILAMENT_CHANGE_EXTRUDE_FEEDRATE is now ADVANCED_PAUSE_PURGE_FEEDRATE."
#elif defined(ADVANCED_PAUSE_EXTRUDE_FEEDRATE)
  #error "ADVANCED_PAUSE_EXTRUDE_FEEDRATE is now ADVANCED_PAUSE_PURGE_FEEDRATE."
#elif defined(FILAMENT_CHANGE_EXTRUDE_LENGTH)
  #error "FILAMENT_CHANGE_EXTRUDE_LENGTH is now ADVANCED_PAUSE_PURGE_LENGTH."
#elif defined(ADVANCED_PAUSE_EXTRUDE_LENGTH)
  #error "ADVANCED_PAUSE_EXTRUDE_LENGTH is now ADVANCED_PAUSE_PURGE_LENGTH."
#elif defined(FILAMENT_CHANGE_NOZZLE_TIMEOUT)
  #error "FILAMENT_CHANGE_NOZZLE_TIMEOUT is now PAUSE_PARK_NOZZLE_TIMEOUT."
#elif defined(FILAMENT_CHANGE_NUMBER_OF_ALERT_BEEPS)
  #error "FILAMENT_CHANGE_NUMBER_OF_ALERT_BEEPS is now FILAMENT_CHANGE_ALERT_BEEPS."
#elif defined(FILAMENT_CHANGE_NO_STEPPER_TIMEOUT)
  #error "FILAMENT_CHANGE_NO_STEPPER_TIMEOUT is now PAUSE_PARK_NO_STEPPER_TIMEOUT."
#elif defined(PLA_PREHEAT_HOTEND_TEMP)
  #error "PLA_PREHEAT_HOTEND_TEMP is now PREHEAT_1_TEMP_HOTEND."
#elif defined(PLA_PREHEAT_HPB_TEMP)
  #error "PLA_PREHEAT_HPB_TEMP is now PREHEAT_1_TEMP_BED."
#elif defined(PLA_PREHEAT_FAN_SPEED)
  #error "PLA_PREHEAT_FAN_SPEED is now PREHEAT_1_FAN_SPEED."
#elif defined(ABS_PREHEAT_HOTEND_TEMP)
  #error "ABS_PREHEAT_HOTEND_TEMP is now PREHEAT_2_TEMP_HOTEND."
#elif defined(ABS_PREHEAT_HPB_TEMP)
  #error "ABS_PREHEAT_HPB_TEMP is now PREHEAT_2_TEMP_BED."
#elif defined(ABS_PREHEAT_FAN_SPEED)
  #error "ABS_PREHEAT_FAN_SPEED is now PREHEAT_2_FAN_SPEED."
#elif defined(ENDSTOPS_ONLY_FOR_HOMING)
  #error "ENDSTOPS_ONLY_FOR_HOMING is deprecated. Use (disable) ENDSTOPS_ALWAYS_ON_DEFAULT instead."
#elif defined(HOMING_FEEDRATE)
  #error "HOMING_FEEDRATE is now set using the HOMING_FEEDRATE_MM_M array instead."
#elif (defined(HOMING_FEEDRATE_XY) || defined(HOMING_FEEDRATE_Z)) && !defined(HOMING_FEEDRATE_MM_M)
  #error "HOMING_FEEDRATE_XY and HOMING_FEEDRATE_Z are now set using the HOMING_FEEDRATE_MM_M array instead."
#elif defined(MANUAL_HOME_POSITIONS)
  #error "MANUAL_HOME_POSITIONS is deprecated. Set MANUAL_[XYZ]_HOME_POS as-needed instead."
#elif defined(PID_ADD_EXTRUSION_RATE)
  #error "PID_ADD_EXTRUSION_RATE is now PID_EXTRUSION_SCALING and is DISABLED by default."
#elif defined(Z_RAISE_BEFORE_HOMING)
  #error "Z_RAISE_BEFORE_HOMING is now Z_HOMING_HEIGHT."
#elif defined(MIN_Z_HEIGHT_FOR_HOMING)
  #error "MIN_Z_HEIGHT_FOR_HOMING is now Z_HOMING_HEIGHT."
#elif defined(Z_RAISE_BEFORE_PROBING) || defined(Z_RAISE_AFTER_PROBING)
  #error "Z_RAISE_(BEFORE|AFTER)_PROBING are deprecated. Use Z_CLEARANCE_DEPLOY_PROBE and Z_AFTER_PROBING instead."
#elif defined(Z_RAISE_PROBE_DEPLOY_STOW) || defined(Z_RAISE_BETWEEN_PROBINGS)
  #error "Z_RAISE_PROBE_DEPLOY_STOW and Z_RAISE_BETWEEN_PROBINGS are now Z_CLEARANCE_DEPLOY_PROBE and Z_CLEARANCE_BETWEEN_PROBES."
#elif defined(Z_PROBE_DEPLOY_HEIGHT) || defined(Z_PROBE_TRAVEL_HEIGHT)
  #error "Z_PROBE_DEPLOY_HEIGHT and Z_PROBE_TRAVEL_HEIGHT are now Z_CLEARANCE_DEPLOY_PROBE and Z_CLEARANCE_BETWEEN_PROBES."
#elif defined(MANUAL_BED_LEVELING)
  #error "MANUAL_BED_LEVELING is now LCD_BED_LEVELING."
#elif defined(MESH_HOME_SEARCH_Z)
  #error "MESH_HOME_SEARCH_Z is now LCD_PROBE_Z_RANGE."
#elif defined(MANUAL_PROBE_Z_RANGE)
  #error "MANUAL_PROBE_Z_RANGE is now LCD_PROBE_Z_RANGE."
#elif !defined(MIN_STEPS_PER_SEGMENT)
  #error "Please replace 'const int dropsegments' with '#define MIN_STEPS_PER_SEGMENT' (and increase by 1)."
#elif MIN_STEPS_PER_SEGMENT <= 0
  #error "MIN_STEPS_PER_SEGMENT must be at least 1."
#elif defined(PREVENT_DANGEROUS_EXTRUDE)
  #error "PREVENT_DANGEROUS_EXTRUDE is now PREVENT_COLD_EXTRUSION."
#elif defined(SCARA)
  #error "SCARA is now MORGAN_SCARA."
#elif defined(ENABLE_AUTO_BED_LEVELING)
  #error "ENABLE_AUTO_BED_LEVELING is deprecated. Specify AUTO_BED_LEVELING_LINEAR, AUTO_BED_LEVELING_BILINEAR, or AUTO_BED_LEVELING_3POINT."
#elif defined(AUTO_BED_LEVELING_FEATURE)
  #error "AUTO_BED_LEVELING_FEATURE is deprecated. Specify AUTO_BED_LEVELING_LINEAR, AUTO_BED_LEVELING_BILINEAR, or AUTO_BED_LEVELING_3POINT."
#elif defined(ABL_GRID_POINTS)
  #error "ABL_GRID_POINTS is now GRID_MAX_POINTS_X and GRID_MAX_POINTS_Y."
#elif defined(ABL_GRID_POINTS_X) || defined(ABL_GRID_POINTS_Y)
  #error "ABL_GRID_POINTS_[XY] is now GRID_MAX_POINTS_[XY]."
#elif defined(ABL_GRID_MAX_POINTS_X) || defined(ABL_GRID_MAX_POINTS_Y)
  #error "ABL_GRID_MAX_POINTS_[XY] is now GRID_MAX_POINTS_[XY]."
#elif defined(MESH_NUM_X_POINTS) || defined(MESH_NUM_Y_POINTS)
  #error "MESH_NUM_[XY]_POINTS is now GRID_MAX_POINTS_[XY]."
#elif defined(UBL_MESH_NUM_X_POINTS) || defined(UBL_MESH_NUM_Y_POINTS)
  #error "UBL_MESH_NUM_[XY]_POINTS is now GRID_MAX_POINTS_[XY]."
#elif defined(UBL_G26_MESH_VALIDATION)
  #error "UBL_G26_MESH_VALIDATION is now G26_MESH_VALIDATION."
#elif defined(UBL_MESH_EDIT_ENABLED)
  #error "UBL_MESH_EDIT_ENABLED is now G26_MESH_VALIDATION."
#elif defined(UBL_MESH_EDITING)
  #error "UBL_MESH_EDITING is now G26_MESH_VALIDATION."
#elif defined(BLTOUCH_HEATERS_OFF)
  #error "BLTOUCH_HEATERS_OFF is now PROBING_HEATERS_OFF."
#elif defined(BLTOUCH_V3)
  #error "BLTOUCH_V3 is obsolete."
#elif defined(BLTOUCH_FORCE_OPEN_DRAIN_MODE)
  #error "BLTOUCH_FORCE_OPEN_DRAIN_MODE is obsolete."
#elif defined(BEEPER)
  #error "BEEPER is now BEEPER_PIN."
#elif defined(SDCARDDETECT)
  #error "SDCARDDETECT is now SD_DETECT_PIN."
#elif defined(STAT_LED_RED) || defined(STAT_LED_BLUE)
  #error "STAT_LED_RED/STAT_LED_BLUE are now STAT_LED_RED_PIN/STAT_LED_BLUE_PIN."
#elif defined(LCD_PIN_BL)
  #error "LCD_PIN_BL is now LCD_BACKLIGHT_PIN."
#elif defined(LCD_PIN_RESET)
  #error "LCD_PIN_RESET is now LCD_RESET_PIN."
#elif defined(EXTRUDER_0_AUTO_FAN_PIN) || defined(EXTRUDER_1_AUTO_FAN_PIN) || defined(EXTRUDER_2_AUTO_FAN_PIN) || defined(EXTRUDER_3_AUTO_FAN_PIN)
  #error "EXTRUDER_[0123]_AUTO_FAN_PIN is now E[0123]_AUTO_FAN_PIN."
#elif defined(PID_FAN_SCALING) && !HAS_FAN
  #error "PID_FAN_SCALING needs at least one fan enabled."
#elif defined(min_software_endstops) || defined(max_software_endstops)
  #error "(min|max)_software_endstops are now (MIN|MAX)_SOFTWARE_ENDSTOPS."
#elif ENABLED(Z_PROBE_SLED) && defined(SLED_PIN)
  #error "Replace SLED_PIN with SOL1_PIN (applies to both Z_PROBE_SLED and SOLENOID_PROBE)."
#elif defined(CONTROLLERFAN_PIN)
  #error "CONTROLLERFAN_PIN is now CONTROLLER_FAN_PIN, enabled with USE_CONTROLLER_FAN."
#elif defined(CONTROLLERFAN_SPEED)
  #error "CONTROLLERFAN_SPEED is now CONTROLLERFAN_SPEED_ACTIVE."
#elif defined(CONTROLLERFAN_SECS)
  #error "CONTROLLERFAN_SECS is now CONTROLLERFAN_IDLE_TIME."
#elif defined(MIN_RETRACT)
  #error "MIN_RETRACT is now MIN_AUTORETRACT and MAX_AUTORETRACT."
#elif defined(ADVANCE)
  #error "ADVANCE is now LIN_ADVANCE."
#elif defined(LIN_ADVANCE_E_D_RATIO)
  #error "LIN_ADVANCE (1.5) no longer uses LIN_ADVANCE_E_D_RATIO."
#elif defined(NEOPIXEL_RGBW_LED)
  #error "NEOPIXEL_RGBW_LED is now NEOPIXEL_LED."
#elif ENABLED(DELTA) && defined(DELTA_PROBEABLE_RADIUS)
  #error "Remove DELTA_PROBEABLE_RADIUS and use PROBING_MARGIN to inset the probe area instead."
#elif ENABLED(DELTA) && defined(DELTA_CALIBRATION_RADIUS)
  #error "Remove DELTA_CALIBRATION_RADIUS and use PROBING_MARGIN to inset the probe area instead."
#elif defined(UBL_MESH_INSET)
  #error "UBL_MESH_INSET is now just MESH_INSET."
#elif defined(UBL_MESH_MIN_X) || defined(UBL_MESH_MIN_Y) || defined(UBL_MESH_MAX_X) || defined(UBL_MESH_MAX_Y)
  #error "UBL_MESH_(MIN|MAX)_[XY] is now just MESH_(MIN|MAX)_[XY]."
#elif defined(ABL_PROBE_PT_1_X) || defined(ABL_PROBE_PT_1_Y) || defined(ABL_PROBE_PT_2_X) || defined(ABL_PROBE_PT_2_Y) || defined(ABL_PROBE_PT_3_X) || defined(ABL_PROBE_PT_3_Y)
  #error "ABL_PROBE_PT_[123]_[XY] is no longer required. Please remove it."
#elif defined(UBL_PROBE_PT_1_X) || defined(UBL_PROBE_PT_1_Y) || defined(UBL_PROBE_PT_2_X) || defined(UBL_PROBE_PT_2_Y) || defined(UBL_PROBE_PT_3_X) || defined(UBL_PROBE_PT_3_Y)
  #error "UBL_PROBE_PT_[123]_[XY] is no longer required. Please remove it."
#elif defined(MIN_PROBE_EDGE)
  #error "MIN_PROBE_EDGE is now called PROBING_MARGIN."
#elif defined(MIN_PROBE_EDGE_LEFT)
  #error "MIN_PROBE_EDGE_LEFT is now called PROBING_MARGIN_LEFT."
#elif defined(MIN_PROBE_EDGE_RIGHT)
  #error "MIN_PROBE_EDGE_RIGHT is now called PROBING_MARGIN_RIGHT."
#elif defined(MIN_PROBE_EDGE_FRONT)
  #error "MIN_PROBE_EDGE_FRONT is now called PROBING_MARGIN_FRONT."
#elif defined(MIN_PROBE_EDGE_BACK)
  #error "MIN_PROBE_EDGE_BACK is now called PROBING_MARGIN_BACK."
#elif defined(LEFT_PROBE_BED_POSITION)
  #error "LEFT_PROBE_BED_POSITION is obsolete. Set a margin with PROBING_MARGIN or PROBING_MARGIN_LEFT instead."
#elif defined(RIGHT_PROBE_BED_POSITION)
  #error "RIGHT_PROBE_BED_POSITION is obsolete. Set a margin with PROBING_MARGIN or PROBING_MARGIN_RIGHT instead."
#elif defined(FRONT_PROBE_BED_POSITION)
  #error "FRONT_PROBE_BED_POSITION is obsolete. Set a margin with PROBING_MARGIN or PROBING_MARGIN_FRONT instead."
#elif defined(BACK_PROBE_BED_POSITION)
  #error "BACK_PROBE_BED_POSITION is obsolete. Set a margin with PROBING_MARGIN or PROBING_MARGIN_BACK instead."
#elif defined(ENABLE_MESH_EDIT_GFX_OVERLAY)
  #error "ENABLE_MESH_EDIT_GFX_OVERLAY is now MESH_EDIT_GFX_OVERLAY."
#elif defined(BABYSTEP_ZPROBE_GFX_REVERSE)
  #error "BABYSTEP_ZPROBE_GFX_REVERSE is now set by OVERLAY_GFX_REVERSE."
#elif defined(UBL_GRANULAR_SEGMENTATION_FOR_CARTESIAN)
  #error "UBL_GRANULAR_SEGMENTATION_FOR_CARTESIAN is now SEGMENT_LEVELED_MOVES."
#elif HAS_PID_HEATING && (defined(K1) || !defined(PID_K1))
  #error "K1 is now PID_K1."
#elif defined(PROBE_DOUBLE_TOUCH)
  #error "PROBE_DOUBLE_TOUCH is now MULTIPLE_PROBING."
#elif defined(ANET_KEYPAD_LCD)
  #error "ANET_KEYPAD_LCD is now ZONESTAR_LCD."
#elif defined(LCD_I2C_SAINSMART_YWROBOT)
  #error "LCD_I2C_SAINSMART_YWROBOT is now LCD_SAINSMART_I2C_(1602|2004)."
#elif defined(MEASURED_LOWER_LIMIT) || defined(MEASURED_UPPER_LIMIT)
  #error "MEASURED_(UPPER|LOWER)_LIMIT is now FILWIDTH_ERROR_MARGIN."
#elif defined(HAVE_TMCDRIVER)
  #error "HAVE_TMCDRIVER is now [AXIS]_DRIVER_TYPE TMC26X."
#elif defined(STEALTHCHOP)
  #error "STEALTHCHOP is now STEALTHCHOP_(XY|Z|E)."
#elif defined(HAVE_TMC26X)
  #error "HAVE_TMC26X is now [AXIS]_DRIVER_TYPE TMC26X."
#elif defined(HAVE_TMC2130)
  #error "HAVE_TMC2130 is now [AXIS]_DRIVER_TYPE TMC2130."
#elif defined(HAVE_TMC2208)
  #error "HAVE_TMC2208 is now [AXIS]_DRIVER_TYPE TMC2208."
#elif defined(HAVE_L6470DRIVER)
  #error "HAVE_L6470DRIVER is now [AXIS]_DRIVER_TYPE L6470."
#elif defined(X_IS_TMC) || defined(X2_IS_TMC) || defined(Y_IS_TMC) || defined(Y2_IS_TMC) || defined(Z_IS_TMC) || defined(Z2_IS_TMC) || defined(Z3_IS_TMC) \
   || defined(E0_IS_TMC) || defined(E1_IS_TMC) || defined(E2_IS_TMC) || defined(E3_IS_TMC) || defined(E4_IS_TMC) || defined(E5_IS_TMC) || defined(E6_IS_TMC) || defined(E7_IS_TMC)
  #error "[AXIS]_IS_TMC is now [AXIS]_DRIVER_TYPE TMC26X."
#elif defined(X_IS_TMC26X) || defined(X2_IS_TMC26X) || defined(Y_IS_TMC26X) || defined(Y2_IS_TMC26X) || defined(Z_IS_TMC26X) || defined(Z2_IS_TMC26X) || defined(Z3_IS_TMC26X) \
   || defined(E0_IS_TMC26X) || defined(E1_IS_TMC26X) || defined(E2_IS_TMC26X) || defined(E3_IS_TMC26X) || defined(E4_IS_TMC26X) || defined(E5_IS_TMC26X) || defined(E6_IS_TMC26X) || defined(E7_IS_TMC26X)
  #error "[AXIS]_IS_TMC26X is now [AXIS]_DRIVER_TYPE TMC26X."
#elif defined(X_IS_TMC2130) || defined(X2_IS_TMC2130) || defined(Y_IS_TMC2130) || defined(Y2_IS_TMC2130) || defined(Z_IS_TMC2130) || defined(Z2_IS_TMC2130) || defined(Z3_IS_TMC2130) \
   || defined(E0_IS_TMC2130) || defined(E1_IS_TMC2130) || defined(E2_IS_TMC2130) || defined(E3_IS_TMC2130) || defined(E4_IS_TMC2130) || defined(E5_IS_TMC2130) || defined(E6_IS_TMC2130) || defined(E7_IS_TMC2130)
  #error "[AXIS]_IS_TMC2130 is now [AXIS]_DRIVER_TYPE TMC2130."
#elif defined(X_IS_TMC2208) || defined(X2_IS_TMC2208) || defined(Y_IS_TMC2208) || defined(Y2_IS_TMC2208) || defined(Z_IS_TMC2208) || defined(Z2_IS_TMC2208) || defined(Z3_IS_TMC2208) \
   || defined(E0_IS_TMC2208) || defined(E1_IS_TMC2208) || defined(E2_IS_TMC2208) || defined(E3_IS_TMC2208) || defined(E4_IS_TMC2208) || defined(E5_IS_TMC2208) || defined(E6_IS_TMC2208) || defined(E7_IS_TMC2208)
  #error "[AXIS]_IS_TMC2208 is now [AXIS]_DRIVER_TYPE TMC2208."
#elif defined(X_IS_L6470) || defined(X2_IS_L6470) || defined(Y_IS_L6470) || defined(Y2_IS_L6470) || defined(Z_IS_L6470) || defined(Z2_IS_L6470) || defined(Z3_IS_L6470) \
   || defined(E0_IS_L6470) || defined(E1_IS_L6470) || defined(E2_IS_L6470) || defined(E3_IS_L6470) || defined(E4_IS_L6470) || defined(E5_IS_L6470) || defined(E6_IS_L6470) || defined(E7_IS_L6470)
  #error "[AXIS]_IS_L6470 is now [AXIS]_DRIVER_TYPE L6470."
#elif defined(AUTOMATIC_CURRENT_CONTROL)
  #error "AUTOMATIC_CURRENT_CONTROL is now MONITOR_DRIVER_STATUS."
#elif defined(FILAMENT_CHANGE_LOAD_LENGTH)
  #error "FILAMENT_CHANGE_LOAD_LENGTH is now FILAMENT_CHANGE_FAST_LOAD_LENGTH."
#elif defined(LEVEL_CORNERS_INSET)
  #error "LEVEL_CORNERS_INSET is now LEVEL_CORNERS_INSET_LFRB."
#elif defined(BEZIER_JERK_CONTROL)
  #error "BEZIER_JERK_CONTROL is now S_CURVE_ACCELERATION."
#elif HAS_JUNCTION_DEVIATION && defined(JUNCTION_DEVIATION_FACTOR)
  #error "JUNCTION_DEVIATION_FACTOR is now JUNCTION_DEVIATION_MM."
#elif defined(JUNCTION_ACCELERATION_FACTOR)
  #error "JUNCTION_ACCELERATION_FACTOR is obsolete. Delete it from Configuration_adv.h."
#elif defined(JUNCTION_ACCELERATION)
  #error "JUNCTION_ACCELERATION is obsolete. Delete it from Configuration_adv.h."
#elif defined(MAX7219_DEBUG_STEPPER_HEAD)
  #error "MAX7219_DEBUG_STEPPER_HEAD is now MAX7219_DEBUG_PLANNER_HEAD."
#elif defined(MAX7219_DEBUG_STEPPER_TAIL)
  #error "MAX7219_DEBUG_STEPPER_TAIL is now MAX7219_DEBUG_PLANNER_TAIL."
#elif defined(MAX7219_DEBUG_STEPPER_QUEUE)
  #error "MAX7219_DEBUG_STEPPER_QUEUE is now MAX7219_DEBUG_PLANNER_QUEUE."
#elif defined(ENDSTOP_NOISE_FILTER)
  #error "ENDSTOP_NOISE_FILTER is now ENDSTOP_NOISE_THRESHOLD [2-7]."
#elif defined(RETRACT_ZLIFT)
  #error "RETRACT_ZLIFT is now RETRACT_ZRAISE."
#elif defined(TOOLCHANGE_PARK_ZLIFT) || defined(TOOLCHANGE_UNPARK_ZLIFT)
  #error "TOOLCHANGE_PARK_ZLIFT and TOOLCHANGE_UNPARK_ZLIFT are now TOOLCHANGE_ZRAISE."
#elif defined(SINGLENOZZLE_TOOLCHANGE_ZRAISE)
  #error "SINGLENOZZLE_TOOLCHANGE_ZRAISE is now TOOLCHANGE_ZRAISE."
#elif defined(SINGLENOZZLE_SWAP_LENGTH)
  #error "SINGLENOZZLE_SWAP_LENGTH is now TOOLCHANGE_FIL_SWAP_LENGTH."
#elif defined(SINGLENOZZLE_SWAP_RETRACT_SPEED)
  #error "SINGLENOZZLE_SWAP_RETRACT_SPEED is now TOOLCHANGE_FIL_SWAP_RETRACT_SPEED."
#elif defined(SINGLENOZZLE_SWAP_PRIME_SPEED)
  #error "SINGLENOZZLE_SWAP_PRIME_SPEED is now TOOLCHANGE_FIL_SWAP_PRIME_SPEED."
#elif defined(SINGLENOZZLE_SWAP_PARK)
  #error "SINGLENOZZLE_SWAP_PARK is now TOOLCHANGE_PARK."
#elif defined(SINGLENOZZLE_TOOLCHANGE_XY)
  #error "SINGLENOZZLE_TOOLCHANGE_XY is now TOOLCHANGE_PARK_XY."
#elif defined(SINGLENOZZLE_PARK_XY_FEEDRATE)
  #error "SINGLENOZZLE_PARK_XY_FEEDRATE is now TOOLCHANGE_PARK_XY_FEEDRATE."
#elif defined(PARKING_EXTRUDER_SECURITY_RAISE)
  #error "PARKING_EXTRUDER_SECURITY_RAISE is now TOOLCHANGE_ZRAISE."
#elif defined(SWITCHING_TOOLHEAD_SECURITY_RAISE)
  #error "SWITCHING_TOOLHEAD_SECURITY_RAISE is now TOOLCHANGE_ZRAISE."
#elif defined(G0_FEEDRATE) && G0_FEEDRATE == 0
  #error "G0_FEEDRATE is now used to set the G0 feedrate."
#elif defined(MBL_Z_STEP)
  #error "MBL_Z_STEP is now MESH_EDIT_Z_STEP."
#elif defined(CHDK)
  #error "CHDK is now CHDK_PIN."
#elif ANY_PIN( \
        MAX6675_SS, MAX6675_SS2, MAX6675_CS, MAX6675_CS2, \
        MAX31855_SS, MAX31855_SS2, MAX31855_CS, MAX31855_CS2, \
        MAX31865_SS, MAX31865_SS2, MAX31865_CS, MAX31865_CS2)
  #warning "MAX*_SS_PIN, MAX*_SS2_PIN, MAX*_CS_PIN, and MAX*_CS2_PIN are deprecated and will be removed in a future version. Please use TEMP_0_CS_PIN/TEMP_1_CS_PIN instead."
#elif ANY_PIN(MAX6675_SCK, MAX31855_SCK, MAX31865_SCK)
  #warning "MAX*_SCK_PIN is deprecated and will be removed in a future version. Please use TEMP_0_SCK_PIN/TEMP_1_SCK_PIN instead."
#elif ANY_PIN(MAX6675_MISO, MAX6675_DO, MAX31855_MISO, MAX31855_DO, MAX31865_MISO, MAX31865_DO)
  #warning "MAX*_MISO_PIN and MAX*_DO_PIN are deprecated and will be removed in a future version. Please use TEMP_0_MISO_PIN/TEMP_1_MISO_PIN instead."
#elif PIN_EXISTS(MAX31865_MOSI)
  #warning "MAX31865_MOSI_PIN is deprecated and will be removed in a future version. Please use TEMP_0_MOSI_PIN/TEMP_1_MOSI_PIN instead."
#elif ANY_PIN(THERMO_CS1_PIN, THERMO_CS2_PIN, THERMO_DO_PIN, THERMO_SCK_PIN)
  #error "THERMO_*_PIN is now TEMP_n_CS_PIN, TEMP_n_SCK_PIN, TEMP_n_MOSI_PIN, TEMP_n_MISO_PIN."
#elif defined(MAX31865_SENSOR_OHMS)
  #error "MAX31865_SENSOR_OHMS is now MAX31865_SENSOR_OHMS_0."
#elif defined(MAX31865_CALIBRATION_OHMS)
  #error "MAX31865_CALIBRATION_OHMS is now MAX31865_CALIBRATION_OHMS_0."
#elif defined(SPINDLE_LASER_ENABLE)
  #error "SPINDLE_LASER_ENABLE is now SPINDLE_FEATURE or LASER_FEATURE."
#elif defined(SPINDLE_LASER_ENABLE_PIN)
  #error "SPINDLE_LASER_ENABLE_PIN is now SPINDLE_LASER_ENA_PIN."
#elif defined(SPINDLE_DIR_CHANGE)
  #error "SPINDLE_DIR_CHANGE is now SPINDLE_CHANGE_DIR."
#elif defined(SPINDLE_STOP_ON_DIR_CHANGE)
  #error "SPINDLE_STOP_ON_DIR_CHANGE is now SPINDLE_CHANGE_DIR_STOP."
#elif defined(SPINDLE_LASER_ACTIVE_HIGH)
  #error "SPINDLE_LASER_ACTIVE_HIGH is now SPINDLE_LASER_ACTIVE_STATE."
#elif defined(SPINDLE_LASER_ENABLE_INVERT)
  #error "SPINDLE_LASER_ENABLE_INVERT is now SPINDLE_LASER_ACTIVE_STATE."
#elif defined(CUTTER_POWER_DISPLAY)
  #error "CUTTER_POWER_DISPLAY is now CUTTER_POWER_UNIT."
#elif defined(CHAMBER_HEATER_PIN)
  #error "CHAMBER_HEATER_PIN is now HEATER_CHAMBER_PIN."
#elif defined(TMC_Z_CALIBRATION)
  #error "TMC_Z_CALIBRATION has been deprecated in favor of MECHANICAL_GANTRY_CALIBRATION."
#elif defined(Z_MIN_PROBE_ENDSTOP)
  #error "Z_MIN_PROBE_ENDSTOP is no longer required. Please remove it."
#elif defined(DUAL_NOZZLE_DUPLICATION_MODE)
  #error "DUAL_NOZZLE_DUPLICATION_MODE is now MULTI_NOZZLE_DUPLICATION."
#elif defined(MENU_ITEM_CASE_LIGHT)
  #error "MENU_ITEM_CASE_LIGHT is now CASE_LIGHT_MENU."
#elif defined(CASE_LIGHT_NEOPIXEL_COLOR)
  #error "CASE_LIGHT_NEOPIXEL_COLOR is now CASE_LIGHT_DEFAULT_COLOR."
#elif defined(ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED)
  #error "ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED is now SD_ABORT_ON_ENDSTOP_HIT."
#elif defined(LPC_SD_LCD) || defined(LPC_SD_ONBOARD) || defined(LPC_SD_CUSTOM_CABLE)
  #error "LPC_SD_(LCD|ONBOARD|CUSTOM_CABLE) are now SDCARD_CONNECTION."
#elif defined(USB_SD_DISABLED)
  #error "USB_SD_DISABLED is now NO_SD_HOST_DRIVE."
#elif defined(USB_SD_ONBOARD)
  #error "USB_SD_ONBOARD is obsolete. Disable NO_SD_HOST_DRIVE instead."
#elif defined(PSU_ACTIVE_HIGH)
  #error "PSU_ACTIVE_HIGH is now PSU_ACTIVE_STATE."
#elif POWER_SUPPLY == 1
  #error "Replace POWER_SUPPLY 1 by enabling PSU_CONTROL and setting PSU_ACTIVE_STATE to 'LOW'."
#elif POWER_SUPPLY == 2
  #error "Replace POWER_SUPPLY 2 by enabling PSU_CONTROL and setting PSU_ACTIVE_STATE to 'HIGH'."
#elif defined(POWER_SUPPLY)
  #error "POWER_SUPPLY is now obsolete. Please remove it."
#elif defined(MKS_ROBIN_TFT)
  #error "MKS_ROBIN_TFT is now FSMC_GRAPHICAL_TFT."
#elif defined(SDPOWER)
  #error "SDPOWER is now SDPOWER_PIN."
#elif defined(STRING_SPLASH_LINE1) || defined(STRING_SPLASH_LINE2)
  #error "STRING_SPLASH_LINE[12] are now obsolete. Please remove them."
#elif defined(Z_PROBE_ALLEN_KEY_DEPLOY_1_X) || defined(Z_PROBE_ALLEN_KEY_STOW_1_X)
  #error "Z_PROBE_ALLEN_KEY_(DEPLOY|STOW) coordinates are now a single setting."
#elif defined(X_PROBE_OFFSET_FROM_EXTRUDER) || defined(Y_PROBE_OFFSET_FROM_EXTRUDER) || defined(Z_PROBE_OFFSET_FROM_EXTRUDER)
  #error "[XYZ]_PROBE_OFFSET_FROM_EXTRUDER is now NOZZLE_TO_PROBE_OFFSET."
#elif defined(MIN_PROBE_X) || defined(MIN_PROBE_Y) || defined(MAX_PROBE_X) || defined(MAX_PROBE_Y)
  #error "(MIN|MAX)_PROBE_[XY] are now calculated at runtime. Please remove them."
#elif defined(Z_STEPPER_ALIGN_X) || defined(Z_STEPPER_ALIGN_X)
  #error "Z_STEPPER_ALIGN_X and Z_STEPPER_ALIGN_Y are now combined as Z_STEPPER_ALIGN_XY."
#elif defined(JUNCTION_DEVIATION)
  #error "JUNCTION_DEVIATION is no longer required. (See CLASSIC_JERK). Please remove it."
#elif defined(BABYSTEP_MULTIPLICATOR)
  #error "BABYSTEP_MULTIPLICATOR is now BABYSTEP_MULTIPLICATOR_[XY|Z]."
#elif defined(LULZBOT_TOUCH_UI)
  #error "LULZBOT_TOUCH_UI is now TOUCH_UI_FTDI_EVE."
#elif defined(PS_DEFAULT_OFF)
  #error "PS_DEFAULT_OFF is now PSU_DEFAULT_OFF."
#elif defined(FILAMENT_UNLOAD_RETRACT_LENGTH)
  #error "FILAMENT_UNLOAD_RETRACT_LENGTH is now FILAMENT_UNLOAD_PURGE_RETRACT."
#elif defined(FILAMENT_UNLOAD_DELAY)
  #error "FILAMENT_UNLOAD_DELAY is now FILAMENT_UNLOAD_PURGE_DELAY."
#elif defined(HOME_USING_SPREADCYCLE)
  #error "HOME_USING_SPREADCYCLE is now obsolete. Please remove it."
#elif defined(DGUS_LCD)
  #error "DGUS_LCD is now DGUS_LCD_UI_(ORIGIN|FYSETC|HIPRECY)."
#elif defined(DGUS_SERIAL_PORT)
  #error "DGUS_SERIAL_PORT is now LCD_SERIAL_PORT."
#elif defined(DGUS_BAUDRATE)
  #error "DGUS_BAUDRATE is now LCD_BAUDRATE."
#elif defined(DGUS_STATS_RX_BUFFER_OVERRUNS)
  #error "DGUS_STATS_RX_BUFFER_OVERRUNS is now STATS_RX_BUFFER_OVERRUNS."
#elif defined(ANYCUBIC_LCD_SERIAL_PORT)
  #error "ANYCUBIC_LCD_SERIAL_PORT is now LCD_SERIAL_PORT."
#elif defined(INTERNAL_SERIAL_PORT)
  #error "INTERNAL_SERIAL_PORT is now MMU2_SERIAL_PORT."
#elif defined(X_DUAL_ENDSTOPS_ADJUSTMENT) || defined(Y_DUAL_ENDSTOPS_ADJUSTMENT) || defined(Z_DUAL_ENDSTOPS_ADJUSTMENT)
  #error "[XYZ]_DUAL_ENDSTOPS_ADJUSTMENT is now [XYZ]2_ENDSTOP_ADJUSTMENT."
#elif defined(Z_TRIPLE_ENDSTOPS_ADJUSTMENT2) || defined(Z_TRIPLE_ENDSTOPS_ADJUSTMENT3)
  #error "Z_TRIPLE_ENDSTOPS_ADJUSTMENT[23] is now Z[23]_ENDSTOP_ADJUSTMENT."
#elif defined(Z_QUAD_ENDSTOPS_ADJUSTMENT2) || defined(Z_QUAD_ENDSTOPS_ADJUSTMENT3) || defined(Z_QUAD_ENDSTOPS_ADJUSTMENT4)
  #error "Z_QUAD_ENDSTOPS_ADJUSTMENT[234] is now Z[234]_ENDSTOP_ADJUSTMENT."
#elif defined(Z_DUAL_STEPPER_DRIVERS)
  #error "Z_DUAL_STEPPER_DRIVERS is now NUM_Z_STEPPER_DRIVERS with a value of 2."
#elif defined(Z_TRIPLE_STEPPER_DRIVERS)
  #error "Z_TRIPLE_STEPPER_DRIVERS is now NUM_Z_STEPPER_DRIVERS with a value of 3."
#elif defined(Z_QUAD_STEPPER_DRIVERS)
  #error "Z_QUAD_STEPPER_DRIVERS is now NUM_Z_STEPPER_DRIVERS with a value of 4."
#elif defined(Z_DUAL_ENDSTOPS) || defined(Z_TRIPLE_ENDSTOPS) || defined(Z_QUAD_ENDSTOPS)
  #error "Z_(DUAL|TRIPLE|QUAD)_ENDSTOPS is now Z_MULTI_ENDSTOPS."
#elif defined(DUGS_UI_MOVE_DIS_OPTION)
  #error "DUGS_UI_MOVE_DIS_OPTION is spelled DGUS_UI_MOVE_DIS_OPTION."
#elif defined(ORIG_E0_AUTO_FAN_PIN) || defined(ORIG_E1_AUTO_FAN_PIN) || defined(ORIG_E2_AUTO_FAN_PIN) || defined(ORIG_E3_AUTO_FAN_PIN) || defined(ORIG_E4_AUTO_FAN_PIN) || defined(ORIG_E5_AUTO_FAN_PIN) || defined(ORIG_E6_AUTO_FAN_PIN) || defined(ORIG_E7_AUTO_FAN_PIN)
  #error "ORIG_Ex_AUTO_FAN_PIN is now just Ex_AUTO_FAN_PIN."
#elif defined(ORIG_CHAMBER_AUTO_FAN_PIN)
  #error "ORIG_CHAMBER_AUTO_FAN_PIN is now just CHAMBER_AUTO_FAN_PIN."
#elif defined(HOMING_BACKOFF_MM)
  #error "HOMING_BACKOFF_MM is now HOMING_BACKOFF_POST_MM."
#elif defined(X_HOME_BUMP_MM) || defined(Y_HOME_BUMP_MM) || defined(Z_HOME_BUMP_MM)
  #error "[XYZ]_HOME_BUMP_MM is now HOMING_BUMP_MM."
#elif defined(DIGIPOT_I2C)
  #error "DIGIPOT_I2C is now DIGIPOT_MCP4451 (or DIGIPOT_MCP4018)."
#elif defined(TOUCH_BUTTONS)
  #error "TOUCH_BUTTONS is now TOUCH_SCREEN."
#elif defined(LCD_FULL_PIXEL_HEIGHT) || defined(LCD_FULL_PIXEL_WIDTH)
  #error "LCD_FULL_PIXEL_(WIDTH|HEIGHT) is deprecated and should be removed."
#elif defined(FSMC_UPSCALE)
  #error "FSMC_UPSCALE is now GRAPHICAL_TFT_UPSCALE."
#elif defined(ANYCUBIC_TFT_MODEL)
  #error "ANYCUBIC_TFT_MODEL is now ANYCUBIC_LCD_I3MEGA."
#elif defined(EVENT_GCODE_SD_STOP)
  #error "EVENT_GCODE_SD_STOP is now EVENT_GCODE_SD_ABORT."
#elif defined(GRAPHICAL_TFT_ROTATE_180)
  #error "GRAPHICAL_TFT_ROTATE_180 is now TFT_ROTATION set to TFT_ROTATE_180."
#elif defined(PROBE_OFFSET_START)
  #error "PROBE_OFFSET_START is now PROBE_OFFSET_WIZARD_START_Z."
#elif defined(POWER_LOSS_PULL)
  #error "POWER_LOSS_PULL is now specifically POWER_LOSS_PULL(UP|DOWN)."
#elif defined(SHORT_MANUAL_Z_MOVE)
  #error "SHORT_MANUAL_Z_MOVE is now FINE_MANUAL_MOVE, applying to Z on most printers."
#elif defined(FIL_RUNOUT_INVERTING)
  #if FIL_RUNOUT_INVERTING
    #error "FIL_RUNOUT_INVERTING true is now FIL_RUNOUT_STATE HIGH."
  #else
    #error "FIL_RUNOUT_INVERTING false is now FIL_RUNOUT_STATE LOW."
  #endif
#elif defined(ASSISTED_TRAMMING_MENU_ITEM)
  #error "ASSISTED_TRAMMING_MENU_ITEM is deprecated and should be removed."
#elif defined(UNKNOWN_Z_NO_RAISE)
  #error "UNKNOWN_Z_NO_RAISE is replaced by setting Z_IDLE_HEIGHT to Z_MAX_POS."
#elif defined(Z_AFTER_DEACTIVATE)
  #error "Z_AFTER_DEACTIVATE is replaced by Z_IDLE_HEIGHT."
#elif defined(MEATPACK)
  #error "MEATPACK is now enabled with MEATPACK_ON_SERIAL_PORT_1, MEATPACK_ON_SERIAL_PORT_2, etc."
#elif defined(CUSTOM_USER_MENUS)
  #error "CUSTOM_USER_MENUS has been replaced by CUSTOM_MENU_MAIN and CUSTOM_MENU_CONFIG."
#elif defined(MKS_LCD12864)
  #error "MKS_LCD12864 is now MKS_LCD12864A or MKS_LCD12864B."
#elif defined(NEOPIXEL_BKGD_LED_INDEX)
  #error "NEOPIXEL_BKGD_LED_INDEX is now NEOPIXEL_BKGD_INDEX_FIRST."
#elif defined(TEMP_SENSOR_1_AS_REDUNDANT)
  #error "TEMP_SENSOR_1_AS_REDUNDANT is now TEMP_SENSOR_REDUNDANT, with associated TEMP_SENSOR_REDUNDANT_* config."
#elif defined(MAX_REDUNDANT_TEMP_SENSOR_DIFF)
  #error "MAX_REDUNDANT_TEMP_SENSOR_DIFF is now TEMP_SENSOR_REDUNDANT_MAX_DIFF"
#elif MOTHERBOARD == BOARD_DUE3DOM_MINI && PIN_EXISTS(TEMP_2) && DISABLED(TEMP_SENSOR_BOARD)
  #warning "Onboard temperature sensor for BOARD_DUE3DOM_MINI has moved from TEMP_SENSOR_2 (TEMP_2_PIN) to TEMP_SENSOR_BOARD (TEMP_BOARD_PIN)."
#elif MOTHERBOARD == BOARD_BTT_SKR_E3_TURBO && PIN_EXISTS(TEMP_2) && DISABLED(TEMP_SENSOR_BOARD)
  #warning "Onboard temperature sensor for BOARD_BTT_SKR_E3_TURBO has moved from TEMP_SENSOR_2 (TEMP_2_PIN) to TEMP_SENSOR_BOARD (TEMP_BOARD_PIN)."
#endif

/**
 * Probe temp compensation requirements
 */

#if ENABLED(PROBE_TEMP_COMPENSATION)
  #if defined(PTC_PARK_POS_X) || defined(PTC_PARK_POS_Y) || defined(PTC_PARK_POS_Z)
    #error "PTC_PARK_POS_[XYZ] is now PTC_PARK_POS (array)."
  #elif !defined(PTC_PARK_POS)
    #error "PROBE_TEMP_COMPENSATION requires PTC_PARK_POS."
  #elif defined(PTC_PROBE_POS_X) || defined(PTC_PROBE_POS_Y)
    #error "PTC_PROBE_POS_[XY] is now PTC_PROBE_POS (array)."
  #elif !defined(PTC_PROBE_POS)
    #error "PROBE_TEMP_COMPENSATION requires PTC_PROBE_POS."
  #endif

  #ifdef PTC_SAMPLE_START
    constexpr auto _ptc_sample_start = PTC_SAMPLE_START;
    constexpr decltype(_ptc_sample_start) _test_ptc_sample_start = 12.3f;
    static_assert(_test_ptc_sample_start != 12.3f, "PTC_SAMPLE_START must be a whole number.");
  #endif
  #ifdef PTC_SAMPLE_RES
    constexpr auto _ptc_sample_res = PTC_SAMPLE_RES;
    constexpr decltype(_ptc_sample_res) _test_ptc_sample_res = 12.3f;
    static_assert(_test_ptc_sample_res != 12.3f, "PTC_SAMPLE_RES must be a whole number.");
  #endif
  #ifdef BTC_SAMPLE_START
    constexpr auto _btc_sample_start = BTC_SAMPLE_START;
    constexpr decltype(_btc_sample_start) _test_btc_sample_start = 12.3f;
    static_assert(_test_btc_sample_start != 12.3f, "BTC_SAMPLE_START must be a whole number.");
  #endif
  #ifdef BTC_SAMPLE_RES
    constexpr auto _btc_sample_res = BTC_SAMPLE_RES;
    constexpr decltype(_btc_sample_res) _test_btc_sample_res = 12.3f;
    static_assert(_test_btc_sample_res != 12.3f, "BTC_SAMPLE_RES must be a whole number.");
  #endif
  #ifdef BTC_PROBE_TEMP
    constexpr auto _btc_probe_temp = BTC_PROBE_TEMP;
    constexpr decltype(_btc_probe_temp) _test_btc_probe_temp = 12.3f;
    static_assert(_test_btc_probe_temp != 12.3f, "BTC_PROBE_TEMP must be a whole number.");
  #endif
#endif

/**
 * Marlin release, version and default string
 */
#ifndef SHORT_BUILD_VERSION
  #error "SHORT_BUILD_VERSION must be specified."
#elif !defined(DETAILED_BUILD_VERSION)
  #error "BUILD_VERSION must be specified."
#elif !defined(STRING_DISTRIBUTION_DATE)
  #error "STRING_DISTRIBUTION_DATE must be specified."
#elif !defined(PROTOCOL_VERSION)
  #error "PROTOCOL_VERSION must be specified."
#elif !defined(MACHINE_NAME)
  #error "MACHINE_NAME must be specified."
#elif !defined(SOURCE_CODE_URL)
  #error "SOURCE_CODE_URL must be specified."
#elif !defined(DEFAULT_MACHINE_UUID)
  #error "DEFAULT_MACHINE_UUID must be specified."
#elif !defined(WEBSITE_URL)
  #error "WEBSITE_URL must be specified."
#endif

/**
 * Serial
 */
#ifndef SERIAL_PORT
  #error "SERIAL_PORT must be defined."
#elif defined(SERIAL_PORT_2) && SERIAL_PORT_2 == SERIAL_PORT
  #error "SERIAL_PORT_2 cannot be the same as SERIAL_PORT."
#elif defined(SERIAL_PORT_3)
  #ifndef SERIAL_PORT_2
    #error "Use SERIAL_PORT_2 before using SERIAL_PORT_3"
  #elif SERIAL_PORT_3 == SERIAL_PORT
    #error "SERIAL_PORT_3 cannot be the same as SERIAL_PORT."
  #elif SERIAL_PORT_3 == SERIAL_PORT_2
    #error "SERIAL_PORT_3 cannot be the same as SERIAL_PORT_2."
  #endif
#endif
#if !(defined(__AVR__) && defined(USBCON))
  #if ENABLED(SERIAL_XON_XOFF) && RX_BUFFER_SIZE < 1024
    #error "SERIAL_XON_XOFF requires RX_BUFFER_SIZE >= 1024 for reliable transfers without drops."
  #elif RX_BUFFER_SIZE && (RX_BUFFER_SIZE < 2 || !IS_POWER_OF_2(RX_BUFFER_SIZE))
    #error "RX_BUFFER_SIZE must be a power of 2 greater than 1."
  #elif TX_BUFFER_SIZE && (TX_BUFFER_SIZE < 2 || TX_BUFFER_SIZE > 256 || !IS_POWER_OF_2(TX_BUFFER_SIZE))
    #error "TX_BUFFER_SIZE must be 0 or a power of 2 between 1 and 256."
  #endif
#elif ANY(SERIAL_XON_XOFF, SERIAL_STATS_MAX_RX_QUEUED, SERIAL_STATS_DROPPED_RX)
  #error "SERIAL_XON_XOFF and SERIAL_STATS_* features not supported on USB-native AVR devices."
#endif


/**
 * Granular software endstops (Marlin >= 1.1.7)
 */

#if BOTH(ENDSTOPPULLUPS, ENDSTOPPULLDOWNS)
  #error "Enable only one of ENDSTOPPULLUPS or ENDSTOPPULLDOWNS."
#elif BOTH(FIL_RUNOUT_PULLUP, FIL_RUNOUT_PULLDOWN)
  #error "Enable only one of FIL_RUNOUT_PULLUP or FIL_RUNOUT_PULLDOWN."
#elif BOTH(ENDSTOPPULLUP_XMAX, ENDSTOPPULLDOWN_XMAX)
  #error "Enable only one of ENDSTOPPULLUP_X_MAX or ENDSTOPPULLDOWN_X_MAX."
#elif BOTH(ENDSTOPPULLUP_YMAX, ENDSTOPPULLDOWN_YMAX)
  #error "Enable only one of ENDSTOPPULLUP_Y_MAX or ENDSTOPPULLDOWN_Y_MAX."
#elif BOTH(ENDSTOPPULLUP_ZMAX, ENDSTOPPULLDOWN_ZMAX)
  #error "Enable only one of ENDSTOPPULLUP_Z_MAX or ENDSTOPPULLDOWN_Z_MAX."
#elif BOTH(ENDSTOPPULLUP_XMIN, ENDSTOPPULLDOWN_XMIN)
  #error "Enable only one of ENDSTOPPULLUP_X_MIN or ENDSTOPPULLDOWN_X_MIN."
#elif BOTH(ENDSTOPPULLUP_YMIN, ENDSTOPPULLDOWN_YMIN)
  #error "Enable only one of ENDSTOPPULLUP_Y_MIN or ENDSTOPPULLDOWN_Y_MIN."
#elif BOTH(ENDSTOPPULLUP_ZMIN, ENDSTOPPULLDOWN_ZMIN)
  #error "Enable only one of ENDSTOPPULLUP_Z_MIN or ENDSTOPPULLDOWN_Z_MIN."
#elif BOTH(ENDSTOPPULLUP_IMIN, ENDSTOPPULLDOWN_IMIN)
  #error "Enable only one of ENDSTOPPULLUP_I_MIN or ENDSTOPPULLDOWN_I_MIN."
#elif BOTH(ENDSTOPPULLUP_JMIN, ENDSTOPPULLDOWN_JMIN)
  #error "Enable only one of ENDSTOPPULLUP_J_MIN or ENDSTOPPULLDOWN_J_MIN."
#elif BOTH(ENDSTOPPULLUP_KMIN, ENDSTOPPULLDOWN_KMIN)
  #error "Enable only one of ENDSTOPPULLUP_K_MIN or ENDSTOPPULLDOWN_K_MIN."
#endif

/**
 * LCD Info Screen Style
 */
#if LCD_INFO_SCREEN_STYLE > 0
  #if HAS_MARLINUI_U8GLIB || LCD_WIDTH < 20 || LCD_HEIGHT < 4
    #error "Alternative LCD_INFO_SCREEN_STYLE requires 20x4 Character LCD."
  #elif LCD_INFO_SCREEN_STYLE > 1
    #error "LCD_INFO_SCREEN_STYLE only has options 0 and 1 at this time."
  #endif
#endif

/**
 * Progress Bar
 */
#if ENABLED(LCD_PROGRESS_BAR)
  #if NONE(SDSUPPORT, LCD_SET_PROGRESS_MANUALLY)
    #error "LCD_PROGRESS_BAR requires SDSUPPORT or LCD_SET_PROGRESS_MANUALLY."
  #elif NONE(HAS_MARLINUI_HD44780, IS_TFTGLCD_PANEL)
    #error "LCD_PROGRESS_BAR only applies to HD44780 character LCD and TFTGLCD_PANEL_(SPI|I2C)."
  #elif HAS_MARLINUI_U8GLIB
    #error "LCD_PROGRESS_BAR does not apply to graphical displays."
  #elif ENABLED(FILAMENT_LCD_DISPLAY)
    #error "LCD_PROGRESS_BAR and FILAMENT_LCD_DISPLAY are not fully compatible. Comment out this line to use both."
  #elif PROGRESS_MSG_EXPIRE < 0
    #error "PROGRESS_MSG_EXPIRE must be greater than or equal to 0."
  #endif
#elif ENABLED(LCD_SET_PROGRESS_MANUALLY)
  #if NONE(HAS_MARLINUI_U8GLIB, HAS_GRAPHICAL_TFT, HAS_MARLINUI_HD44780, EXTENSIBLE_UI)
    #error "LCD_SET_PROGRESS_MANUALLY requires LCD_PROGRESS_BAR, Character LCD, Graphical LCD, TFT, or EXTENSIBLE_UI."
  #endif
#endif

#if ENABLED(USE_M73_REMAINING_TIME) && DISABLED(LCD_SET_PROGRESS_MANUALLY)
  #error "USE_M73_REMAINING_TIME requires LCD_SET_PROGRESS_MANUALLY"
#endif

#if !HAS_LCD_MENU && ENABLED(SD_REPRINT_LAST_SELECTED_FILE)
  #error "SD_REPRINT_LAST_SELECTED_FILE currently requires a Marlin-native LCD menu."
#endif

/**
 * Custom Boot and Status screens
 */
#if ENABLED(SHOW_CUSTOM_BOOTSCREEN) && NONE(HAS_MARLINUI_U8GLIB, TOUCH_UI_FTDI_EVE)
  #error "SHOW_CUSTOM_BOOTSCREEN requires Graphical LCD or TOUCH_UI_FTDI_EVE."
#elif ENABLED(CUSTOM_STATUS_SCREEN_IMAGE) && !HAS_MARLINUI_U8GLIB
  #error "CUSTOM_STATUS_SCREEN_IMAGE requires a 128x64 DOGM B/W Graphical LCD."
#endif

/**
 * LCD Lightweight Screen Style
 */
#if ENABLED(LIGHTWEIGHT_UI) && DISABLED(U8GLIB_ST7920)
  #error "LIGHTWEIGHT_UI requires a U8GLIB_ST7920-based display."
#endif

/**
 * I2C Position Encoders
 */
#if ENABLED(I2C_POSITION_ENCODERS)
  #if !BOTH(BABYSTEPPING, BABYSTEP_XY)
    #error "I2C_POSITION_ENCODERS requires BABYSTEPPING and BABYSTEP_XY."
  #elif !WITHIN(I2CPE_ENCODER_CNT, 1, 5)
    #error "I2CPE_ENCODER_CNT must be between 1 and 5."
  #endif
#endif

/**
 * Required LCD language
 */
#if HAS_MARLINUI_HD44780 && !defined(DISPLAY_CHARSET_HD44780)
  #error "You must set DISPLAY_CHARSET_HD44780 to JAPANESE, WESTERN or CYRILLIC for your LCD controller."
#endif

/**
 * Chamber Heating Options - PID vs Limit Switching
 */
#if BOTH(PIDTEMPCHAMBER, CHAMBER_LIMIT_SWITCHING)
  #error "To use CHAMBER_LIMIT_SWITCHING you must disable PIDTEMPCHAMBER."
#endif

/**
 * Case Light requirements
 */
#if NEED_CASE_LIGHT_PIN
  #if !PIN_EXISTS(CASE_LIGHT)
    #error "CASE_LIGHT_ENABLE requires CASE_LIGHT_PIN, CASE_LIGHT_USE_NEOPIXEL, or CASE_LIGHT_USE_RGB_LED."
  #elif CASE_LIGHT_PIN == FAN_PIN
    #error "CASE_LIGHT_PIN conflicts with FAN_PIN. Resolve before continuing."
  #endif
#endif

/**
 * Required custom thermistor settings
 */
#if   TEMP_SENSOR_0_IS_CUSTOM && !(defined(HOTEND0_PULLUP_RESISTOR_OHMS) && defined(HOTEND0_RESISTANCE_25C_OHMS) && defined(HOTEND0_BETA))
  #error "TEMP_SENSOR_0 1000 requires HOTEND0_PULLUP_RESISTOR_OHMS, HOTEND0_RESISTANCE_25C_OHMS and HOTEND0_BETA in Configuration_adv.h."
#elif TEMP_SENSOR_1_IS_CUSTOM && !(defined(HOTEND1_PULLUP_RESISTOR_OHMS) && defined(HOTEND1_RESISTANCE_25C_OHMS) && defined(HOTEND1_BETA))
  #error "TEMP_SENSOR_1 1000 requires HOTEND1_PULLUP_RESISTOR_OHMS, HOTEND1_RESISTANCE_25C_OHMS and HOTEND1_BETA in Configuration_adv.h."
#elif TEMP_SENSOR_2_IS_CUSTOM && !(defined(HOTEND2_PULLUP_RESISTOR_OHMS) && defined(HOTEND2_RESISTANCE_25C_OHMS) && defined(HOTEND2_BETA))
  #error "TEMP_SENSOR_2 1000 requires HOTEND2_PULLUP_RESISTOR_OHMS, HOTEND2_RESISTANCE_25C_OHMS and HOTEND2_BETA in Configuration_adv.h."
#elif TEMP_SENSOR_3_IS_CUSTOM && !(defined(HOTEND3_PULLUP_RESISTOR_OHMS) && defined(HOTEND3_RESISTANCE_25C_OHMS) && defined(HOTEND3_BETA))
  #error "TEMP_SENSOR_3 1000 requires HOTEND3_PULLUP_RESISTOR_OHMS, HOTEND3_RESISTANCE_25C_OHMS and HOTEND3_BETA in Configuration_adv.h."
#elif TEMP_SENSOR_4_IS_CUSTOM && !(defined(HOTEND4_PULLUP_RESISTOR_OHMS) && defined(HOTEND4_RESISTANCE_25C_OHMS) && defined(HOTEND4_BETA))
  #error "TEMP_SENSOR_4 1000 requires HOTEND4_PULLUP_RESISTOR_OHMS, HOTEND4_RESISTANCE_25C_OHMS and HOTEND4_BETA in Configuration_adv.h."
#elif TEMP_SENSOR_5_IS_CUSTOM && !(defined(HOTEND5_PULLUP_RESISTOR_OHMS) && defined(HOTEND5_RESISTANCE_25C_OHMS) && defined(HOTEND5_BETA))
  #error "TEMP_SENSOR_5 1000 requires HOTEND5_PULLUP_RESISTOR_OHMS, HOTEND5_RESISTANCE_25C_OHMS and HOTEND5_BETA in Configuration_adv.h."
#elif TEMP_SENSOR_6_IS_CUSTOM && !(defined(HOTEND6_PULLUP_RESISTOR_OHMS) && defined(HOTEND6_RESISTANCE_25C_OHMS) && defined(HOTEND6_BETA))
  #error "TEMP_SENSOR_6 1000 requires HOTEND6_PULLUP_RESISTOR_OHMS, HOTEND6_RESISTANCE_25C_OHMS and HOTEND6_BETA in Configuration_adv.h."
#elif TEMP_SENSOR_7_IS_CUSTOM && !(defined(HOTEND7_PULLUP_RESISTOR_OHMS) && defined(HOTEND7_RESISTANCE_25C_OHMS) && defined(HOTEND7_BETA))
  #error "TEMP_SENSOR_7 1000 requires HOTEND7_PULLUP_RESISTOR_OHMS, HOTEND7_RESISTANCE_25C_OHMS and HOTEND7_BETA in Configuration_adv.h."
#elif TEMP_SENSOR_BED_IS_CUSTOM && !(defined(BED_PULLUP_RESISTOR_OHMS) && defined(BED_RESISTANCE_25C_OHMS) && defined(BED_BETA))
  #error "TEMP_SENSOR_BED 1000 requires BED_PULLUP_RESISTOR_OHMS, BED_RESISTANCE_25C_OHMS and BED_BETA in Configuration_adv.h."
#elif TEMP_SENSOR_CHAMBER_IS_CUSTOM && !(defined(CHAMBER_PULLUP_RESISTOR_OHMS) && defined(CHAMBER_RESISTANCE_25C_OHMS) && defined(CHAMBER_BETA))
  #error "TEMP_SENSOR_CHAMBER 1000 requires CHAMBER_PULLUP_RESISTOR_OHMS, CHAMBER_RESISTANCE_25C_OHMS and CHAMBER_BETA in Configuration_adv.h."
#elif TEMP_SENSOR_PROBE_IS_CUSTOM && !(defined(PROBE_PULLUP_RESISTOR_OHMS) && defined(PROBE_RESISTANCE_25C_OHMS) && defined(PROBE_BETA))
  #error "TEMP_SENSOR_PROBE 1000 requires PROBE_PULLUP_RESISTOR_OHMS, PROBE_RESISTANCE_25C_OHMS and PROBE_BETA in Configuration_adv.h."
#elif TEMP_SENSOR_BOARD_IS_CUSTOM && !(defined(BOARD_PULLUP_RESISTOR_OHMS) && defined(BOARD_RESISTANCE_25C_OHMS) && defined(BOARD_BETA))
  #error "TEMP_SENSOR_BOARD 1000 requires BOARD_PULLUP_RESISTOR_OHMS, BOARD_RESISTANCE_25C_OHMS and BOARD_BETA in Configuration_adv.h."
#elif TEMP_SENSOR_REDUNDANT_IS_CUSTOM && !(defined(REDUNDANT_PULLUP_RESISTOR_OHMS) && defined(REDUNDANT_RESISTANCE_25C_OHMS) && defined(REDUNDANT_BETA))
  #error "TEMP_SENSOR_REDUNDANT 1000 requires REDUNDANT_PULLUP_RESISTOR_OHMS, REDUNDANT_RESISTANCE_25C_OHMS and REDUNDANT_BETA in Configuration_adv.h."
#endif

/**
 * Test Sensor & Heater pin combos.
 * Pins and Sensor IDs must be set for each heater
 */
#if !ANY_PIN(TEMP_0, TEMP_0_CS)
  #error "TEMP_0_PIN or TEMP_0_CS_PIN not defined for this board."
#elif !HAS_HEATER_0 && EXTRUDERS
  #error "HEATER_0_PIN not defined for this board."
#elif TEMP_SENSOR_0_IS_MAX_TC && !PIN_EXISTS(TEMP_0_CS)
  #error "TEMP_SENSOR_0 MAX thermocouple requires TEMP_0_CS_PIN."
#elif HAS_HOTEND && !HAS_TEMP_HOTEND && !TEMP_SENSOR_0_IS_DUMMY
  #error "TEMP_0_PIN (required for TEMP_SENSOR_0) not defined for this board."
#elif EITHER(HAS_MULTI_HOTEND, HEATERS_PARALLEL) && !HAS_HEATER_1
  #error "HEATER_1_PIN is not defined. TEMP_SENSOR_1 might not be set, or the board (not EEB / EEF?) doesn't define a pin."
#endif


/**
 * Pins must be set for temp sensors, with some other feature requirements.
 */
#if TEMP_SENSOR_CHAMBER && !PIN_EXISTS(TEMP_CHAMBER)
  #error "TEMP_SENSOR_CHAMBER requires TEMP_CHAMBER_PIN."
#endif

#if TEMP_SENSOR_BOARD
  #if !PIN_EXISTS(TEMP_BOARD)
    #error "TEMP_SENSOR_BOARD requires TEMP_BOARD_PIN."
  #elif !HAS_TEMP_ADC_BOARD
    #error "TEMP_BOARD_PIN must be an ADC pin."
  #elif ENABLED(THERMAL_PROTECTION_BOARD) && (!defined(BOARD_MINTEMP) || !defined(BOARD_MAXTEMP))
    #error "THERMAL_PROTECTION_BOARD requires BOARD_MINTEMP and BOARD_MAXTEMP."
  #endif
#elif CONTROLLER_FAN_MIN_BOARD_TEMP
  #error "CONTROLLER_FAN_MIN_BOARD_TEMP requires TEMP_SENSOR_BOARD."
#endif

#if ENABLED(CHAMBER_FAN) && !(defined(CHAMBER_FAN_MODE) && WITHIN(CHAMBER_FAN_MODE, 0, 2))
  #error "CHAMBER_FAN_MODE must be between 0 and 2."
#endif



/**
 * Temperature status LEDs
 */
#if ENABLED(TEMP_STAT_LEDS) && !ANY_PIN(STAT_LED_RED, STAT_LED_BLUE)
  #error "TEMP_STAT_LEDS requires STAT_LED_RED_PIN or STAT_LED_BLUE_PIN, preferably both."
#endif

/**
 * LED Control Menu
 */
#if ENABLED(LED_CONTROL_MENU) && !HAS_COLOR_LEDS
  #error "LED_CONTROL_MENU requires BLINKM, RGB_LED, RGBW_LED, PCA9533, PCA9632, or NEOPIXEL_LED."
#endif

/**
 * LED Backlight Timeout
 */
#if defined(LED_BACKLIGHT_TIMEOUT) && !(ENABLED(PSU_CONTROL) && ANY(FYSETC_MINI_12864_2_0, FYSETC_MINI_12864_2_1, FYSETC_242_OLED_12864))
  #error "LED_BACKLIGHT_TIMEOUT requires a FYSETC Mini Panel and a Power Switch."
#endif

/**
 * I2C bus
 */
#if ENABLED(EXPERIMENTAL_I2CBUS) && I2C_SLAVE_ADDRESS > 0
  #if I2C_SLAVE_ADDRESS < 8
    #error "I2C_SLAVE_ADDRESS can't be less than 8. (Addresses 0 - 7 are reserved.)"
  #elif I2C_SLAVE_ADDRESS > 127
    #error "I2C_SLAVE_ADDRESS can't be over 127. (Only 7 bits allowed.)"
  #endif
#endif

/**
 * RGB_LED Requirements
 */
#define _RGB_TEST (PINS_EXIST(RGB_LED_R, RGB_LED_G, RGB_LED_B))
#if ENABLED(PRINTER_EVENT_LEDS) && !HAS_COLOR_LEDS
  #error "PRINTER_EVENT_LEDS requires BLINKM, PCA9533, PCA9632, RGB_LED, RGBW_LED or NEOPIXEL_LED."
#elif ENABLED(RGB_LED)
  #if !_RGB_TEST
    #error "RGB_LED requires RGB_LED_R_PIN, RGB_LED_G_PIN, and RGB_LED_B_PIN."
  #elif ENABLED(RGBW_LED)
    #error "Please enable only one of RGB_LED and RGBW_LED."
  #endif
#elif ENABLED(RGBW_LED)
  #if !(_RGB_TEST && PIN_EXISTS(RGB_LED_W))
    #error "RGBW_LED requires RGB_LED_R_PIN, RGB_LED_G_PIN, RGB_LED_B_PIN, and RGB_LED_W_PIN."
  #endif
#endif
#undef _RGB_TEST

// NeoPixel requirements
#if ENABLED(NEOPIXEL_LED)
  #if !PIN_EXISTS(NEOPIXEL) || NEOPIXEL_PIXELS == 0
    #error "NEOPIXEL_LED requires NEOPIXEL_PIN and NEOPIXEL_PIXELS."
  #elif ENABLED(NEOPIXEL2_SEPARATE) && !(defined(NEOPIXEL2_TYPE) && PIN_EXISTS(NEOPIXEL2) && NEOPIXEL2_PIXELS > 0)
    #error "NEOPIXEL2_SEPARATE requires NEOPIXEL2_TYPE, NEOPIXEL2_PIN and NEOPIXEL2_PIXELS."
  #elif ENABLED(NEO2_COLOR_PRESETS) && DISABLED(NEOPIXEL2_SEPARATE)
    #error "NEO2_COLOR_PRESETS requires NEOPIXEL2_SEPARATE to be enabled."
  #endif
#endif

#if DISABLED(NO_COMPILE_TIME_PWM)
  #define _TEST_PWM(P) PWM_PIN(P)
#else
  #define _TEST_PWM(P) 1 // pass
#endif

/**
 * Make sure only one EEPROM type is enabled
 */
#if ENABLED(EEPROM_SETTINGS)
  #if 1 < 0 \
    + ENABLED(I2C_EEPROM) \
    + ENABLED(SPI_EEPROM) \
    + ENABLED(QSPI_EEPROM) \
    + ENABLED(SDCARD_EEPROM_EMULATION) \
    + ENABLED(FLASH_EEPROM_EMULATION) \
    + ENABLED(SRAM_EEPROM_EMULATION) \
    + ENABLED(IIC_BL24CXX_EEPROM)
    #error "Please select only one method of EEPROM Persistent Storage."
  #endif
#endif

/**
 * Make sure only one display is enabled
 */
#if 1 < 0 \
  + ENABLED(REPRAP_DISCOUNT_SMART_CONTROLLER) \
  + ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER) \
  + (ENABLED(U8GLIB_SSD1306) && DISABLED(IS_U8GLIB_SSD1306)) \
  + (ENABLED(MINIPANEL) && NONE(MKS_MINI_12864, ENDER2_STOCKDISPLAY)) \
  + (ENABLED(MKS_MINI_12864) && NONE(MKS_LCD12864A, MKS_LCD12864B)) \
  + (ENABLED(EXTENSIBLE_UI) && DISABLED(IS_EXTUI)) \
  + (DISABLED(IS_LEGACY_TFT) && ENABLED(TFT_GENERIC)) \
  + (ENABLED(IS_LEGACY_TFT) && COUNT_ENABLED(TFT_320x240, TFT_320x240_SPI, TFT_480x320, TFT_480x320_SPI)) \
  + COUNT_ENABLED(ANYCUBIC_LCD_I3MEGA, ANYCUBIC_LCD_CHIRON, ANYCUBIC_TFT35) \
  + COUNT_ENABLED(DGUS_LCD_UI_ORIGIN, DGUS_LCD_UI_FYSETC, DGUS_LCD_UI_HIPRECY, DGUS_LCD_UI_MKS) \
  + COUNT_ENABLED(ENDER2_STOCKDISPLAY, CR10_STOCKDISPLAY, DWIN_CREALITY_LCD) \
  + COUNT_ENABLED(FYSETC_MINI_12864_X_X, FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0, FYSETC_MINI_12864_2_1, FYSETC_GENERIC_12864_1_1) \
  + COUNT_ENABLED(LCD_SAINSMART_I2C_1602, LCD_SAINSMART_I2C_2004) \
  + COUNT_ENABLED(MKS_12864OLED, MKS_12864OLED_SSD1306) \
  + COUNT_ENABLED(MKS_TS35_V2_0, MKS_ROBIN_TFT24, MKS_ROBIN_TFT28, MKS_ROBIN_TFT32, MKS_ROBIN_TFT35, MKS_ROBIN_TFT43, MKS_ROBIN_TFT_V1_1R, ANET_ET4_TFT28, ANET_ET5_TFT35) \
  + COUNT_ENABLED(TFTGLCD_PANEL_SPI, TFTGLCD_PANEL_I2C) \
  + COUNT_ENABLED(VIKI2, miniVIKI) \
  + COUNT_ENABLED(ZONESTAR_12864LCD, ZONESTAR_12864OLED, ZONESTAR_12864OLED_SSD1306) \
  + COUNT_ENABLED(ANET_FULL_GRAPHICS_LCD, ANET_FULL_GRAPHICS_LCD_ALT_WIRING) \
  + ENABLED(AZSMZ_12864) \
  + ENABLED(BQ_LCD_SMART_CONTROLLER) \
  + ENABLED(CARTESIO_UI) \
  + ENABLED(ELB_FULL_GRAPHIC_CONTROLLER) \
  + ENABLED(FF_INTERFACEBOARD) \
  + ENABLED(FYSETC_242_OLED_12864) \
  + ENABLED(G3D_PANEL) \
  + ENABLED(LCD_FOR_MELZI) \
  + ENABLED(LCD_I2C_PANELOLU2) \
  + ENABLED(LCD_I2C_VIKI) \
  + ENABLED(LCM1602) \
  + ENABLED(LONGER_LK_TFT28) \
  + ENABLED(MAKEBOARD_MINI_2_LINE_DISPLAY_1602) \
  + ENABLED(MAKRPANEL) \
  + ENABLED(MALYAN_LCD) \
  + ENABLED(NEXTION_TFT) \
  + ENABLED(MKS_LCD12864A) \
  + ENABLED(MKS_LCD12864B) \
  + ENABLED(OLED_PANEL_TINYBOY2) \
  + ENABLED(OVERLORD_OLED) \
  + ENABLED(PANEL_ONE) \
  + ENABLED(RA_CONTROL_PANEL) \
  + ENABLED(RADDS_DISPLAY) \
  + ENABLED(REPRAPWORLD_GRAPHICAL_LCD) \
  + ENABLED(RIGIDBOT_PANEL) \
  + ENABLED(SAV_3DGLCD) \
  + ENABLED(SAV_3DLCD) \
  + ENABLED(SILVER_GATE_GLCD_CONTROLLER) \
  + ENABLED(TFT_TRONXY_X5SA) \
  + ENABLED(TOUCH_UI_FTDI_EVE) \
  + ENABLED(U8GLIB_SH1106_EINSTART) \
  + ENABLED(ULTI_CONTROLLER) \
  + ENABLED(ULTIMAKERCONTROLLER) \
  + ENABLED(ULTIPANEL) \
  + ENABLED(ULTRA_LCD) \
  + ENABLED(YHCB2004) \
  + ENABLED(ZONESTAR_LCD) \
  + ENABLED(K3D_FULL_GRAPHIC_SMART_CONTROLLER) \
  + ENABLED(K3D_242_OLED_CONTROLLER)
  #error "Please select only one LCD controller option."
#endif

#undef IS_U8GLIB_SSD1306
#undef IS_EXTUI
#undef IS_LEGACY_TFT

#if ANY(TFT_GENERIC, MKS_TS35_V2_0, MKS_ROBIN_TFT24, MKS_ROBIN_TFT28, MKS_ROBIN_TFT32, MKS_ROBIN_TFT35, MKS_ROBIN_TFT43, MKS_ROBIN_TFT_V1_1R, TFT_TRONXY_X5SA, ANYCUBIC_TFT35, ANYCUBIC_TFT35, LONGER_LK_TFT28, ANET_ET4_TFT28, ANET_ET5_TFT35, BIQU_BX_TFT70)
  #if NONE(TFT_COLOR_UI, TFT_CLASSIC_UI, TFT_LVGL_UI)
    #error "TFT_COLOR_UI, TFT_CLASSIC_UI, TFT_LVGL_UI is required for your TFT. Please enable one."
  #elif 1 < ENABLED(TFT_COLOR_UI) + ENABLED(TFT_CLASSIC_UI) + ENABLED(TFT_LVGL_UI)
    #error "Please select only one of TFT_COLOR_UI, TFT_CLASSIC_UI, or TFT_LVGL_UI."
  #endif
#elif ANY(TFT_COLOR_UI, TFT_CLASSIC_UI, TFT_LVGL_UI)
  #error "TFT_(COLOR|CLASSIC|LVGL)_UI requires a TFT display to be enabled."
#endif

#if ENABLED(TFT_GENERIC) && NONE(TFT_INTERFACE_FSMC, TFT_INTERFACE_SPI)
  #error "TFT_GENERIC requires either TFT_INTERFACE_FSMC or TFT_INTERFACE_SPI interface."
#endif

#if BOTH(TFT_INTERFACE_FSMC, TFT_INTERFACE_SPI)
  #error "Please enable only one of TFT_INTERFACE_SPI or TFT_INTERFACE_SPI."
#endif

#if MANY(LCD_SCREEN_ROT_0, LCD_SCREEN_ROT_90, LCD_SCREEN_ROT_180, LCD_SCREEN_ROT_270)
  #error "Please enable only one LCD_SCREEN_ROT_* option: 0, 90, 180, or 270."
#endif

#if MANY(TFT_RES_320x240, TFT_RES_480x272, TFT_RES_480x320, TFT_RES_1024x600)
  #error "Please select only one of TFT_RES_320x240, TFT_RES_480x272, TFT_RES_480x320, or TFT_RES_1024x600."
#endif

#if HAS_TFT_LVGL_UI && DISABLED(TFT_RES_480x320)
  #error "(FMSC|SPI)TFT_LVGL_UI requires TFT_RES_480x320."
#endif

#if defined(GRAPHICAL_TFT_UPSCALE) && !WITHIN(GRAPHICAL_TFT_UPSCALE, 2, 4)
  #error "GRAPHICAL_TFT_UPSCALE must be 2, 3, or 4."
#endif

#if BOTH(CHIRON_TFT_STANDARD, CHIRON_TFT_NEW)
  #error "Please select only one of CHIRON_TFT_STANDARD or CHIRON_TFT_NEW."
#endif

/**
 * Serial displays require a dedicated serial port
 */
#ifdef LCD_SERIAL_PORT
  #if LCD_SERIAL_PORT == SERIAL_PORT
    #error "LCD_SERIAL_PORT cannot be the same as SERIAL_PORT."
  #elif defined(SERIAL_PORT_2) && LCD_SERIAL_PORT == SERIAL_PORT_2
    #error "LCD_SERIAL_PORT cannot be the same as SERIAL_PORT_2."
  #endif
#else
  #if HAS_DGUS_LCD
    #error "The DGUS LCD requires LCD_SERIAL_PORT to be defined."
  #elif EITHER(ANYCUBIC_LCD_I3MEGA, ANYCUBIC_LCD_CHIRON)
    #error "The ANYCUBIC LCD requires LCD_SERIAL_PORT to be defined."
  #elif ENABLED(MALYAN_LCD)
    #error "MALYAN_LCD requires LCD_SERIAL_PORT to be defined."
  #endif
#endif

/**
 * FYSETC Mini 12864 RGB backlighting required
 */
#if EITHER(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0) && DISABLED(RGB_LED)
  #error "RGB_LED is required for FYSETC_MINI_12864 1.2 and 2.0."
#elif EITHER(FYSETC_MINI_12864_2_0, FYSETC_MINI_12864_2_1) && DISABLED(LED_USER_PRESET_STARTUP)
  #error "LED_USER_PRESET_STARTUP is required for FYSETC_MINI_12864 2.x displays."
#endif


#if !BLOCK_BUFFER_SIZE || !IS_POWER_OF_2(BLOCK_BUFFER_SIZE)
  #error "BLOCK_BUFFER_SIZE must be a power of 2."
#elif BLOCK_BUFFER_SIZE > 64
  #error "A very large BLOCK_BUFFER_SIZE is not needed and takes longer to drain the buffer on pause / cancel."
#endif

#if ENABLED(LED_CONTROL_MENU) && !IS_ULTIPANEL
  #error "LED_CONTROL_MENU requires an LCD controller."
#endif

#if ENABLED(CASE_LIGHT_USE_NEOPIXEL) && DISABLED(NEOPIXEL_LED)
  #error "CASE_LIGHT_USE_NEOPIXEL requires NEOPIXEL_LED."
#endif


/**
 * Ensure this option is set intentionally
 */

#if NONE(HAS_MARLINUI_U8GLIB, EXTENSIBLE_UI) && ENABLED(PRINT_PROGRESS_SHOW_DECIMALS)
  #error "PRINT_PROGRESS_SHOW_DECIMALS currently requires a Graphical LCD."
#endif

#if HAS_ADC_BUTTONS && defined(ADC_BUTTON_DEBOUNCE_DELAY) && ADC_BUTTON_DEBOUNCE_DELAY < 16
  #error "ADC_BUTTON_DEBOUNCE_DELAY must be greater than 16."
#endif


/**
 * Touch Screen Calibration
 */
#if ENABLED(TFT_TOUCH_DEVICE_XPT2046) && DISABLED(TOUCH_SCREEN_CALIBRATION) \
    && (!defined(TOUCH_CALIBRATION_X) || !defined(TOUCH_CALIBRATION_Y) || !defined(TOUCH_OFFSET_X) || !defined(TOUCH_OFFSET_Y))
  #error "TOUCH_CALIBRATION_[XY] and TOUCH_OFFSET_[XY] are required for resistive touch screens with TOUCH_SCREEN_CALIBRATION disabled."
#endif

/**
 * Sanity check for WIFI
 */
#if EITHER(ESP3D_WIFISUPPORT, WIFISUPPORT) && DISABLED(ARDUINO_ARCH_ESP32)
  #error "ESP3D_WIFISUPPORT or WIFISUPPORT requires an ESP32 MOTHERBOARD."
#endif

/**
 * Sanity Check for Password Feature
 */
#if ENABLED(PASSWORD_FEATURE)
  #if NONE(HAS_LCD_MENU, PASSWORD_UNLOCK_GCODE, PASSWORD_CHANGE_GCODE)
    #error "Without PASSWORD_UNLOCK_GCODE, PASSWORD_CHANGE_GCODE, or a supported LCD there's no way to unlock the printer or set a password."
  #elif DISABLED(EEPROM_SETTINGS)
    #warning "PASSWORD_FEATURE settings will be lost on power-off without EEPROM_SETTINGS."
  #endif
#endif


/**
 * Sanity Check for Slim LCD Menus and Probe Offset Wizard
 */
#if BOTH(SLIM_LCD_MENUS, PROBE_OFFSET_WIZARD)
  #error "SLIM_LCD_MENUS disables \"Advanced Settings > Probe Offsets > PROBE_OFFSET_WIZARD.\""
#endif


// Misc. Cleanup
#undef _TEST_PWM
#undef _LINEAR_AXES_STR
#undef _LOGICAL_AXES_STR
