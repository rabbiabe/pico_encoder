/**************************************************
 * Rotary Encoder Object for Raspberry Pi Pico
 * by Abe Friedman | https://github.com/rabbiabe
 * v1.2 2024-05-19
 * 
 * Incorporates code by Ralph S. Bacon 
 * https://github.com/RalphBacon/226-Better-Rotary-Encoder---no-switch-bounce 
 * 
 * Example Program
 **************************************************/

#pragma once

#include "../pico_encoder.h"

void encoderCallback(uint gpio, uint32_t event);

constexpr uint8_t   pinEncCW = 20,  // pin 26 
                    pinEncCCW = 21, // pin 27
                    pinEncSW = 22;  // pin 29 ** 1uF capacitor from this pin to ground will debounce the switch

Rotary encoder(&encoderCallback, pinEncCW, pinEncCCW, pinEncSW);

