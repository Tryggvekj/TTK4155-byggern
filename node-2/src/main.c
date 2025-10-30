#include <stdio.h>
#include <stdarg.h>
#include "sam.h"

/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile. 
 * If you get somewhat cryptic errors referencing functions such as _sbrk, 
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */

#include <time.h>
#include "can.h"
#include "gpio.h"
#include "uart.h"
#include "pwm.h"

#define F_CPU 84000000
#define BAUD_RATE 115200

#define _delay(time) time_spinFor(msecs(time))

int main()
{
    SystemInit();

    CanInit _can_init = {
        .phase2 = 3,  // Phase 2 segment
        .phase1 = 7,  // Phase 1 segment
        .propag = 2,  // Propagation time segment
        .sjw = 0,     // Synchronization jump width
        .brp = 20,    // Baud rate prescaler
        .smp = 0      // Sampling mode
    };

    can_init(_can_init, 0);

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    uart_init(F_CPU, BAUD_RATE);
    printf("Hello World\r\n");
    struct sam_gpio_pin test_pin = {.port='A', .pin=24};
    sam_gpio_init(test_pin);
    struct CanMsg msg; 
    _delay(1000);
    printf("%d\r\n", pwm_init(20, 1)); //20 ms period, channel 1
    printf("%d\r\n", pwm_set_duty_cycle(7.5, 1)); //7.5% duty cycle (1.5 ms pulse width)
    printf("PWM initialized\r\n");

    while (1)
    {
        /*while(!can_rx(&msg)) {
            
        }
        can_printmsg(msg); */
        
    }
}