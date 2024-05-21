#pragma once

#include "pico_encoder.h"

void encoderCallback(uint gpio, uint32_t event);

const uint8_t   pinEncCW = 20,  // pin 26 
                pinEncCCW = 21, // pin 27
                pinEncSW = 22;  // pin 29

const uint32_t input_mask = (1 << pinEncCCW) | 
                            (1 << pinEncCW) | 
                            (1 << pinEncSW);

int32_t counter;

Rotary encoder(&encoderCallback, pinEncCW, pinEncCCW, pinEncSW);

