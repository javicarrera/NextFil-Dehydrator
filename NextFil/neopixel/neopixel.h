/**
 * NeoPixel support
 */

// ------------------------
// Includes
// ------------------------

#include "arduino.h"

#include <Adafruit_NeoPixel.h>
#include <stdint.h>

// ------------------------
// Defines
// ------------------------

#if NEOPIXEL_TYPE == NEO_RGB || NEOPIXEL_TYPE == NEO_RBG || NEOPIXEL_TYPE == NEO_GRB || NEOPIXEL_TYPE == NEO_GBR || NEOPIXEL_TYPE == NEO_BRG || NEOPIXEL_TYPE == NEO_BGR
  #define NEOPIXEL_IS_RGB 1
#else
  #define NEOPIXEL_IS_RGBW 1
#endif

#if NEOPIXEL_IS_RGB
  #define NEO_WHITE 255, 255, 255, 0
#else
  #define NEO_WHITE 0, 0, 0, 255
#endif

// ------------------------
// Function prototypes
// ------------------------

class NeoPixel {
private:
  static Adafruit_NeoPixel adaneo1 ;

public:
  static int8_t neoindex;

  static void init();
  static void set_color_startup(const uint32_t c);

  static void set_color(const uint32_t c);

  #ifdef NEOPIXEL_BKGD_LED_INDEX
    static void set_color_background();
  #endif

  static inline void begin() { adaneo1.begin(); }
  static inline void set_pixel_color(const uint16_t n, const uint32_t c) {adaneo1.setPixelColor(n, c); }
  static inline void set_brightness(const uint8_t b) {adaneo1.setBrightness(b);}
  static inline void show() { adaneo1.show(); }
  bool set_led_color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t w, const uint8_t p);

  // Accessors
  static inline uint16_t pixels() { return adaneo1.numPixels(); }
  static inline uint8_t brightness() { return adaneo1.getBrightness(); }
  static inline uint32_t Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    return adaneo1.Color(r, g, b, w);
  }
};

extern NeoPixel neo;
