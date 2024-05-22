# pico_encoder
I wanted an easy drop-in object to implement rotary encoders in my Pico projects. The `Rotary` object provides full functionality for a standard rotary encoder with push button switch. (Although I am open to hearing if there is anything I missed.)
## Implementation
### Hardware setup
The `Rotary` object can use any GPIO pins you want - the GPIO numbers are identified in the constructor, and you can set up multiple `Rotary` objects with different pins. the switch pin is optional in case you do not use it or are working with an encoder that doesn't have a switch. The two pins not connected to GPIO should be connected directly to Ground.

The `read()` method (see below) filters out any switch bounce. The `buttonPress()` method does not include any software debouncing; a 1µF capacitor from the GPIO pin to ground should effectively eliminate any switch bounce. 
### Methods 
You will need to write a standard interrupt callback function (as described in the C SDK documentation), but you **do not** need to call any of the interrupt setup functions; all of that is handled in the constructor. 

In your callback function, you should call the `buttonPress()` method when the switch pin triggers the interrupt and `read()` for either of the encoder pins. See below for details about those methods and what they return.
#### Rotary(gpio_irq_callback_t callback, uint8_t pin_clockwise, uint8_t pin_counterclockwise, int8_t pin_switch, uint32_t long_press_ms = 3000);
The constructor takes four or five arguments: 
- `callback`: Pointer to the interrupt callback function 
- `pin_clockwise`: GPIO number of the pin connected to the encoder's clockwise terminal. 
- `pin_counterclockwise`: GPIO number of the pin connected to the encoder's counter-clockwise terminal. 
- `pin_sw`: GPIO number of the pin connected to the encoder's switch terminal. If you have not connected a switch, passing a negative value will tell the constructor to skip initializing the switch. 
- `long_press_ms`: Optional argument specifying the duration (expressed in ms) that should be considered a "long" hold of the button. defaults to 3,000 ms if not specified. 

When called, the constructor initializes the pins, sets them as inputs with pull up, and attaches the GPIO interrupts to the specified callback function. It also converts `long_press_ms` to µs and stores the value.
#### int8_t read(); 
Reads the state of the rotary encoder and returns +1 (clockwise) or -1 (counter-clockwise) if there is a valid movement and 0 if the movement is not valid (i.e., switch bounce). The main code in this method was [shared by Ralph S. Bacon and based on a paper by Marko Pinteric.](https://github.com/RalphBacon/226-Better-Rotary-Encoder---no-switch-bounce)
#### ButtonState buttonPress(uint32_t event);
Reads the state of the button switch and returns one of three states in the `ButtonState` enumeration:
- `BTN_DOWN`: The button is currently pressed.
- `BTN_UP_SHORT`: The button was released as a "short" press, i.e., less than the time passed to the constructor as `long_press_ms`.
- `BTN_UP_LONG`: The button was released as a "long" press, i.e., it was held for more than the time passed to the constructor as `long_press_ms`.

This method requires you to pass the event mask from the interrupt callback as an argument so it can interpret what is happening with the button.
#### uint32_t getPressTime_ms();
This method will return the duration that the button has been held down, expressed in ms. If the button is not currently pressed, the function returns 0.
#### void inline updateCallback(gpio_irq_callback_t callback);
This method provides the option of changing the callback function for `Rotary` object that has already been initialized. It is provided for convenience and, depending on your application, it might make more sense just to call `gpio_set_irq_callback()` directly.
## Example program
The example program uses the serial connection to output the value of a counter that is incremented/decremented by the encoder. A "long" press on the button will reset the counter to 0; a "short" press toggles the oboard LED on/off.