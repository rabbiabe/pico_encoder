# pico_encoder
I wanted an easy drop-in object to implement rotary encoders in my Pico projects. The `Rotary` object provides full functionality for a standard rotary encoder with push button switch. (Although I am open to hearing if there is anything I missed.)
## Implementation
### Hardware setup
The `Rotary` object can use any GPIO pins you want - the GPIO numbers are identified in the constructor, and you can set up multiple `Rotary` objects with different pins. the switch pin is optional in case you do not use it or are working with an encoder that doesn't have a switch. 
### Methods 
You will need to write a standard interrupt callback function (as described in the C SDK documentation), but you **do not** need to call any of the interrupt setup functions; all of that is handled in the constructor. 
#### Rotary(gpio_irq_callback_t callback, uint8_t pin_clockwise, uint8_t pin_counterclockwise, int8_t pin_switch, uint32_t long_press_ms = 3000);
#### int8_t read(); 
#### ButtonState buttonPress(uint32_t event);
#### void updateCallback(gpio_irq_callback_t callback);
#### uint32_t getPressTime_ms();
## Example program