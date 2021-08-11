/*!
 * Bascula.h - Funciones para el uso con el HX711
 * Clase EMA -  Exponential Moving Average filter 
 * 2021 Javier Carrera Arias   
 * All rights reserved under the library's licence
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef PESOS_H
#define PESOS_H

#if ARDUINO >= 100    
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#ifndef HX711_H
 #include "HX711.h" //Para las básculas
#endif

#ifndef SERIALDEBUG_H
 #include "SerialDebug.h" //Para las básculas
#endif

//***************************************************
//        Exponential moving average filter EMA
//***************************************************

/**
 * @brief   Exponential moving average filter.
 * 
 * Fast integer EMA implementation where the weight factor is a power of two.
 * 
 * The factor alpha in the difference equation of the Exponential Moving Average filter is a number between zero and one.
 * If alpha=1 the output is just equal to the input, and no filtering takes place. 
 * 
 * There are two main ways to implement this multiplication by alpha: Either we use floating point numbers and calculate
 * the multiplication directly, or we use integers, and express the multiplication as a division by 1/alpha > 1.
 * We can choose the value for alpha in such a way that 1/alpha = 2^k, k=numero natural
 * This is useful, because a division by a power of two can be replaced by a very fast right bitshift: 
 * alpha·x = x / 2^k =     x >> k
 * 
 * The difference equation of an exponential moving average filter: 
 * y[n] = alpha·x[n]+(1-alpha)·y[n-1] 
 * 
 * We can now rewrite the difference equation of the EMA with this optimization in mind:  
 * y[n] = y[n-1] + (x[n]- y[n-1]) >> k
 *  
 * Where x is the input sequence, and y is the output sequence.
 * 
 * Rounding. A final improvement we can make to our division algorithm is to round the result to the nearest integer, instead of truncating it towards zero
 * Consider the rounded result of the division a/b We can then express it as a flooring of the result plus one half: 
 * a/b = a/b + 1/2 = (a + b/2) / b
 * When b is a power of two, is equivalent to:
 * [a/2^k] = (a+1 << (k-1) >> k)
 *
 * [An in-depth explanation of the EMA filter](https://tttapa.github.io/Pages/Mathematics/Systems-and-Control-Theory/Digital-filters/Exponential%20Moving%20Average/)
 *
 * @tparam  K
 *          The amount of bits to shift by. This determines the location
 *          of the pole in the EMA transfer function, and therefore the
 *          cut-off frequency.  
 *          The higher this number, the more filtering takes place.  
 *          The pole location is @f$ 1 - 2^{-K} @f$.
 * @tparam  input_t
 *          The integer type to use for the input and output of the filter. 
 *          Can be signed or unsigned.
 * @tparam  state_t
 *          The unsigned integer type to use for the internal state of the
 *          filter. A fixed-point representation with @f$ K @f$ fractional
 *          bits is used, so this type should be at least @f$ M + K @f$ bits
 *          wide, where @f$ M @f$ is the maximum number of bits of the input.
 *
 * Some examples of different combinations of template parameters:
 * 
 * 1. Filtering the result of `analogRead`: analogRead returns an integer
 *    between 0 and 1023, which can be represented using 10 bits, so 
 *    @f$ M = 10 @f$. If `input_t` and `output_t` are both `uint16_t`,
 *    the maximum shift factor `K` is @f$ 16 - M = 6 @f$. If `state_t`
 *    is increased to `uint32_t`, the maximum shift factor `K` is 
 *    @f$ 32 - M = 22 @f$.
 * 2. Filtering a signed integer between -32768 and 32767: this can be 
 *    represented using a 16-bit signed integer, so `input_t` is `int16_t`,
 *    and @f$ M = 16 @f$. (2¹⁵ = 32768)
 *    Let's say the shift factor `K` is 1, then the minimum width of 
 *    `state_t` should be @f$ M + K = 17 @f$ bits, so `uint32_t` would be 
 *    a sensible choice.
 */

// Variables comunes modificables
const int EMA_K = 2;            // variable K de la media dinamica exponencial regula la agresividad del filtro.K=2 se ajusta mejor al cambio
// TODO: NO SE COMO HACER QUE ESTA VARIABLE ESTE DISPONIBLE EN MAIN

// Variables importadas de main
extern float WeightReadingRefreshTime; // (s)entre lecturas de peso y temp. exterior (min. aprox 1s) si da error de sensor no conectado aumentar!
extern const uint8_t umbral_display;   // en decigramos el umbral para la salida por pantalla 5=0.5g,.
extern const uint8_t umbral_EMA;       // en decigramos el umbral para la media dinamica 1=0.1g,.
extern const uint8_t AVG_FACT;        // cuantas lecturas se hacen y mandan al EMA en un ciclo 

// Bascula 1
// ------------------------------------------------
// Variables modificables:
extern int32_t zero_offset1;
extern float calibration_factor1;
extern int32_t new_dif_EMA1;    // Diferencia entre la nueva lectura y la media del EMA
extern int32_t last_dif_EMA1;   // Diferencia entre la ultima lectura y la media del
extern int32_t new_dif_last1;   // Diferencia entre la anterior lectura y la actual
extern int32_t tara1_actual_g;
extern int32_t tara1_actual_offset;
extern uint32_t prevMicros1;      // tiempo de la ultima lectura - en us
extern volatile float PESO1_f;
extern volatile int32_t PESO1;
extern volatile int32_t ult_PESO1;
extern volatile int32_t ult_PESO1_DISPLAY;
extern uint32_t filteredValue1;
extern bool update_output1;       // si se actualiza o no el peso en pantalla.
extern bool bascula1_conectada;   //por defecto conectada
extern bool tara1_cambio;        //cuando cuambias de tara (rollo de filamento distinto)
extern bool bascula1_calibrada;
// Iniciar el sensor HX711 de la bascula 1
extern HX711 bascula1;   

// Bascula 2
// ------------------------------------------------
// Variables modificables:
extern int32_t zero_offset2;
extern float calibration_factor2;
extern int32_t new_dif_EMA2;    // Diferencia entre la nueva lectura y la media del EMA
extern int32_t last_dif_EMA2;   // Diferencia entre la ultima lectura y la media del
extern int32_t new_dif_last2;   // Diferencia entre la anterior lectura y la actual
extern int32_t tara2_actual_g;
extern int32_t tara2_actual_offset;
extern uint32_t prevMicros2;      // tiempo de la ultima lectura - en us
extern volatile float PESO2_f;
extern volatile int32_t PESO2;
extern volatile int32_t ult_PESO2;
extern volatile int32_t ult_PESO2_DISPLAY;
extern uint32_t filteredValue2;
extern bool update_output2;       // si se actualiza o no el peso en pantalla.
extern bool bascula2_conectada;   //por defecto conectada
extern bool tara2_cambio;        //cuando cuambias de tara (rollo de filamento distinto)
extern bool bascula2_calibrada;
// Iniciar el sensor HX711 de la bascula 2
extern HX711 bascula2;   

// SERIAL DEBUG
// ------------------------------------------------
extern bool SERIAL_DEBUG; // variable que determina si tenemos serial o no

template <int K, class uint_t = uint32_t>
class EMA
{
public:
  /// Update the filter      with the given input and return the filtered output.
  uint_t operator()(bool restart, uint_t input)
  {
    //if (restart)
    //  state = 0; // reinicia la EMA
    state += input;
    uint_t output = (state + half) >> K;
    state -= output;
    return output;
  }

  static_assert(
      uint_t(0) < uint_t(-1), // Check that `uint_t` is an unsigned type
      "The `uint_t` type should be an unsigned integer, otherwise, "
      "the division using bit shifts is invalid.");

  /// Fixed point representation of one half, used for rounding.
  constexpr static uint_t half = 1 << (K - 1);

private:
  uint_t state = 0;
};

void leerBascula1();
void leerBascula2();
void calibrar_peso1();
void calibrar_peso2();
void calibrar_factor1();
void calibrar_factor2();
void cambioTara();

#endif