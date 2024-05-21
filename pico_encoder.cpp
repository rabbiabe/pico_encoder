#include "pico_encoder.h"

Rotary::Rotary(gpio_irq_callback_t callback, uint8_t pin_clockwise, uint8_t pin_counterclockwise, uint8_t pin_switch, uint32_t long_press_ms)
{
    pin_sw = pin_switch;
    pin_cw = pin_clockwise;
    pin_ccw = pin_counterclockwise;
    long_press_threshold = long_press_ms * 1000;

    uint32_t input_mask = (1 << pin_sw) | (1 << pin_cw) | (1 << pin_ccw);
    gpio_init_mask(input_mask);
    gpio_set_dir_in_masked(input_mask);

    gpio_pull_up(pin_sw);
    gpio_pull_up(pin_cw);
    gpio_pull_up(pin_ccw);

    gpio_set_irq_enabled_with_callback(pin_sw, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, callback);
    gpio_set_irq_enabled(pin_cw, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true);
    gpio_set_irq_enabled(pin_ccw, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true);
   
}

ButtonState Rotary::buttonPress(uint32_t event)
/**************************************************
 * This function takes the unaltered event mask 
 * from the GPIO interrupt callback and either 
 * marks the time the button is pushed down or 
 **************************************************/
{
    ButtonState returnValue;

    if (event & GPIO_IRQ_EDGE_FALL) {
        press_time = time_us_32();
        long_press = false;
        returnValue = BTN_DOWN;
    } else if (event & GPIO_IRQ_EDGE_RISE) {
        long_press = ((press_time > 0) && (time_us_32() - press_time > long_press_threshold)) ? true : false;
        returnValue = long_press ? BTN_UP_LONG : BTN_UP_SHORT;
        press_time = 0;
    } else {
        // do nothing for now
    }
    return returnValue;
}

int8_t Rotary::read()
{
    static uint8_t lrmem = 3;
    static int lrsum = 0;
    static int8_t TRANS[] = {0, -1, 1, 14, 1, 0, 14, -1, -1, 14, 0, 1, 14, 1, -1, 0};

    // Read BOTH pin states to deterimine validity of rotation (ie not just switch bounce)
    int8_t l = gpio_get(pin_cw);
    int8_t r = gpio_get(pin_ccw);

    // Move previous value 2 bits to the left and add in our new values
    lrmem = ((lrmem & 0x03) << 2) + (2 * l) + r;

    // Convert the bit pattern to a movement indicator (14 = impossible, ie switch bounce)
    lrsum += TRANS[lrmem];

    /* encoder not in the neutral (detent) state */
    if (lrsum % 4 != 0)
    {
        return 0;
    }

    /* encoder in the neutral state - clockwise rotation*/
    if (lrsum == 4)
    {
        lrsum = 0;
        return 1;
    }

    /* encoder in the neutral state - anti-clockwise rotation*/
    if (lrsum == -4)
    {
        lrsum = 0;
        return -1;
    }

    // An impossible rotation has been detected - ignore the movement
    lrsum = 0;
    return 0;

}

void Rotary::updateCallback(gpio_irq_callback_t callback)
{
    gpio_set_irq_callback(callback);
}

uint32_t getPressTime_ms()
{
    if (press_time == 0) {
       return 0;
    else {
       return ((time_us_32() - press_time) / 1000);
    }
}