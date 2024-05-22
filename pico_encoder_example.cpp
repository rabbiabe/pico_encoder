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

#include "pico_encoder_example.h"
#include <cstdio>

int main ()
{

    stdio_init_all();
    sleep_ms(3000);

    
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, true);



    while (true) tight_loop_contents();

    return 0;
}

void encoderCallback(uint gpio, uint32_t event)
{
    int8_t newVal;
    ButtonState action;
    switch (gpio)
    {
        case pinEncCCW:
        case pinEncCW:
            //printf("{Encoder movement}\n\n");
            newVal = encoder.read();
            if (newVal != 0) {
                counter += newVal;
                printf("Valid encoder movement, Counter = %d\n", counter);
            }
            break;

        case pinEncSW:
            action = encoder.buttonPress(event);
            switch (action)
            {
                case BTN_DOWN:
                    printf("Button pressed down\n\n");
                    break;
                
                case BTN_UP_SHORT:
                    printf("Button released, short hold\n=====> TOGGLE LED\n\n");
                    gpio_put(PICO_DEFAULT_LED_PIN, !gpio_get(PICO_DEFAULT_LED_PIN));
                    break;

                case BTN_UP_LONG:
                    counter = 0;
                    printf("Button released, long hold\n=====> RESET COUNTER\n\n[Counter = %d]\n\n", counter);
                    break;

                default:
                    break;
            }
            break;
        
        default:
            break;
    }
}