# pico_encoder
I wanted an easy drop-in object to implement rotary encoders in my Pico projects. The `Rotary` object provides full functionality for a standard rotary encoder with push button switch. (Although I am open to hearing if there is anything I missed.)
## Implementation
### Hardware setup
The `Rotary` object can use any GPIO pins you want - the GPIO numbers are identified in the constructor, and you can set up multiple `Rotary` objects with different pins. the switch pin is optional in case you do not use it or are working with an encoder that doesn't have a switch. 

The `read()` method (see below) filters out any switch bounce. The `buttonPress()` method does not include any software debouncing; a 1uF capacitor from the GPIO pin to ground should effectively eliminate any switch bounce. 
### Methods 
You will need to write a standard interrupt callback function (as described in the C SDK documentation), but you **do not** need to call any of the interrupt setup functions; all of that is handled in the constructor. 

In your callback function, you should call the `buttonPress()` method when the switch pin triggers the interrupt and `read()` for either of the encoder pins. 
#### Rotary(gpio_irq_callback_t callback, uint8_t pin_clockwise, uint8_t pin_counterclockwise, int8_t pin_switch, uint32_t long_press_ms = 3000);
The constructor takes four or five arguments: 
- `callback`: Pointer to the interrupt callback function 
- `pin_clockwise`: GPIO number of the pin connected to the encoder's clockwise terminal. 
- `pin_counterclockwise`: GPIO number of the pin connected to the encoder's counter-clockwise terminal. 
- `pin_sw`: GPIO number of the pin connected to the encoder's switch terminal. If you have not connected a switch, passing a negative value will tell the constructor to skip initializing the switch. 
- `long_press_ms`: Optional argument specifying the duration (expressed in ms) that should be considered a "long" hold of the button. defaults to 3,000 ms if not specified. 

When called, the constructor initializes the pins, sets them as inputs with pull up, and attaches the GPIO interrupts to the specified callback function. 
#### int8_t read(); 
Reads the state of the rotary 
#### ButtonState buttonPress(uint32_t event);
#### void updateCallback(gpio_irq_callback_t callback);
#### uint32_t getPressTime_ms();
## Example program