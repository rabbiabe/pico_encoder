/**************************************************
 * Rotary Encoder Object for Raspberry Pi Pico
 * by Abe Friedman | https://github.com/rabbiabe
 * v1.09 2024-05-19
 * 
 * Incorporates code by Ralph S. Bacon 
 * https://github.com/RalphBacon/226-Better-Rotary-Encoder---no-switch-bounce 
 * 
 * Example Program
 **************************************************/

#pragma once

#include "pico_encoder.h"

void encoderCallback(uint gpio, uint32_t event);

const uint8_t   pinEncCW = 20,  // pin 26 
                pinEncCCW = 21, // pin 27
                pinEncSW = 22;  // pin 29

int32_t counter;

Rotary encoder(&encoderCallback, pinEncCW, pinEncCCW, pinEncSW);

