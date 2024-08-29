/**************************************************
 * Rotary Encoder Object for Raspberry Pi Pico
 * by Abe Friedman | https://github.com/rabbiabe
 * v1.09 2024-05-19
 * 
 * Incorporates code by Ralph S. Bacon 
 * https://github.com/RalphBacon/226-Better-Rotary-Encoder---no-switch-bounce 
 **************************************************/

#include "pico_encoder.h"

/*!
    @brief Constructor

    @param callback the interrupt callback function
    @param pin_clockwise the pin connected to the encoder pin that you "hit" first when turning clockwise
    @param pin_counterclockwise is the pin connected to the encoder pin that you "hit" first when turning counter-clockwise
    @param pin_switch the pin connected to the encoder's switch pin. The software does not debounce the switch, so you need 
        to connect a 1uF capacitor from `pin_switch` to ground. Pass a negative value to skip initializing this pin. Defaults 
        to -1, so if omitted the switch pin will not be initialized.
    @param long_press_ms the time to hold the button. Defaults to 3000ms if omitted.
*/
Rotary::Rotary(gpio_irq_callback_t callback, uint8_t pin_clockwise, uint8_t pin_counterclockwise, int8_t pin_switch, uint32_t long_press_ms)
{
    pin_sw = pin_switch;
    pin_cw = pin_clockwise;
    pin_ccw = pin_counterclockwise;
    long_press_threshold = long_press_ms * 1000;

    gpio_init(pin_cw);
    gpio_init(pin_ccw);
    gpio_set_dir(pin_cw, false);
    gpio_set_dir(pin_ccw, false);
    gpio_pull_up(pin_cw);
    gpio_pull_up(pin_ccw);

    gpio_set_irq_enabled_with_callback(pin_cw, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, callback);    
    gpio_set_irq_enabled(pin_ccw, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true);

    if (pin_sw >= 0) {
        gpio_init(pin_sw);
        gpio_set_dir(pin_sw, false);
        gpio_pull_up(pin_sw); 
        gpio_set_irq_enabled(pin_sw, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true); 
    }
}

/*!
    @brief This function takes the unaltered event mask from the GPIO interrupt callback and either marks the time the button is pushed down or 
        evaluates if the release time is short or long. 

    @param event should be the same `event` passed to the callback, just pass it through to the encoder.

    @return BTN_DOWN if the button is pressed, BTN_UP_SHORT if released as a "short" press, BTN_UP_LONG if released as a "long" press
*/
button_state_t Rotary::buttonPress(uint32_t event)
{
    button_state_t returnValue = BTN_UP_SHORT;

    if (event & GPIO_IRQ_EDGE_FALL) {
        press_time = time_us_32();
        long_press = false;
        returnValue = BTN_DOWN;
    } else if (event & GPIO_IRQ_EDGE_RISE) {
        long_press = ((press_time > 0) && (time_us_32() - press_time > long_press_threshold)) ? true : false;
        returnValue = long_press ? BTN_UP_LONG : BTN_UP_SHORT;
        press_time = 0;
    } 
    return returnValue;
}

/*!
    @brief Reads the encoder pin states to determine if the state is "valid," i.e., not switch bounce. 

    @return +1 for clockwise movement, -1 for counterclockwise
*/
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

/*!
    @brief updates the interrupt callback function

    @param callback the interrupt callback function
*/
void inline Rotary::updateCallback(gpio_irq_callback_t callback)
{
    gpio_set_irq_callback(callback);
}

/*!
    @brief Find out how long the button has been held

    @return the time in ms since the button was pressed
*/
uint32_t Rotary::getPressTime_ms()
{
    if (press_time == 0) {
       return 0;
    } else {
       return ((time_us_32() - press_time) / 1000);
    }
}