# pico_encoder
I wanted an easy drop-in object to implement rotary encoders in my Pico projects. The `Rotary` object provides full functionality for a standard rotary encoder with push button switch. (Although I am open to hearing if there is anything I missed.)
## Implementation
### Hardware setup
The `Rotary` object can use any GPIO pins you want - the GPIO numbers are identified in the constructor, and you can set up multiple `Rotary` objects with different pins. 