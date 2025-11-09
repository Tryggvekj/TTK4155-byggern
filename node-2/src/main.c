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

#include "adc.h"
#include "can.h"
#include "game.h"
#include "gpio.h"
#include "motor_ctrl.h"
#include "pwm.h"
#include "servo.h"
#include "uart.h"

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

    adc_init();
    uint16_t adc_value = 0;

    solenoid_init();


    struct CanMsg msg; 
    _delay(1000);
    printf("%d\r\n", pwm_init(20)); //20 ms period
    printf("PWM initialized\r\n");

    while (1)
    {
        adc_read(&adc_value);
        if (adc_value < 1000) {
            printf("Sending 2 on CAN \r\n");
            can_tx((CanMsg){
                .id = CAN_ID_IR_LED,
                .length = 1,
                .byte = {0x01}
            });
        }

        if(can_rx(&msg)) {
            printf("CAN message ID: %d", msg.id);
            switch(msg.id) {
                case CAN_ID_JOYSTICK: {
                    set_servo_from_js_can(&msg);
                    break;
                }
                case CAN_ID_JOYSTICK_BTN: {
                    printf("test");
                    set_solenoid_from_can(&msg);
                    break;
                }
                default:
                    printf("Unknown CAN message ID: %d\r\n", msg.id);
                    break;
            }
        }
    }
}