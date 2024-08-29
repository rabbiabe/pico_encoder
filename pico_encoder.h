/**************************************************
 * Rotary Encoder Object for Raspberry Pi Pico
 * by Abe Friedman | https://github.com/rabbiabe
 * v1.09 2024-05-19
 * 
 * Incorporates code by Ralph S. Bacon 
 * https://github.com/RalphBacon/226-Better-Rotary-Encoder---no-switch-bounce 
 **************************************************/

#pragma once

/*
 * C++ HEADERS
 */
#include <cstdlib>
#include <cstdint>

/*
 * PICO HEADERS
 */
#include "pico/stdlib.h"
#include "hardware/gpio.h"

enum button_state_t {
    BTN_DOWN,
    BTN_UP_SHORT,
    BTN_UP_LONG,
};

class Rotary {
  public: 
    Rotary(gpio_irq_callback_t callback, uint8_t pin_clockwise, uint8_t pin_counterclockwise, int8_t pin_switch = -1, uint32_t long_press_ms = 3000);
    int8_t read(); 
    button_state_t buttonPress(uint32_t event);
    void inline updateCallback(gpio_irq_callback_t callback);
    uint32_t getPressTime_ms();

  private:
    uint8_t pin_sw, pin_cw, pin_ccw;
    uint32_t press_time = 0, long_press_threshold;
    bool long_press = false;
};

