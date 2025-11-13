#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
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
#define SERVO_PERIOD_MS 20
#define MOTOR_PERIOD_US 50

#define _delay(time) time_spinFor(msecs(time))

int main()
{
    SystemInit();

    CanInit _can_init = {
        .phase2 = 3, // Phase 2 segment
        .phase1 = 7, // Phase 1 segment
        .propag = 2, // Propagation time segment
        .sjw = 0,    // Synchronization jump width
        .brp = 20,   // Baud rate prescaler
        .smp = 0     // Sampling mode
    };

    can_init(_can_init, 0);

    WDT->WDT_MR = WDT_MR_WDDIS; // Disable Watchdog Timer

    uart_init(F_CPU, BAUD_RATE);
    printf("Hello World\r\n");

    adc_init();
    uint16_t adc_value = 0;

    struct sam_gpio_pin solenoid_pin = {
        .port = 'B',
        .pin = 25,
    };
    sam_gpio_init(solenoid_pin);
    printf("Solenoid initialized\r\n");
    int encoder_pos = 0;

    // motor controller init
    motor_init(MOTOR_PERIOD_US);
    encoder_init();
    printf("Encoder initialized\r\n");

    struct CanMsg msg;
    _delay(1000);
    servo_init(SERVO_PERIOD_MS);
    printf("PWM initialized\r\n");
    uint8_t ir_counter = 0;

    enum game_state current_state = GAME_WAIT_START;

    struct xy_coords js = {0};

    while (1)
    {

        switch(current_state) {

            case GAME_WAIT_START:
                printf("Waiting for game start...\r\n");
                if (can_rx(&msg))
                {
                    printf("Received CAN message with ID: %X\r\n", msg.id);
                    _delay(1000);
                    if (msg.id == CAN_ID_GAME_START)
                    {
                        calibrate_motor();
                        msg.id = CAN_ID_NODE2_RDY;
                        msg.length = 1;
                        msg.byte[0] = 1; // Node 2 ready signal
                        can_tx(msg);
                        current_state = GAME_RUNNING;
                        printf("Game started!\r\n");
                    }
                }
                break;

            case GAME_RUNNING:
                if (check_game_over(&msg))
                {
                    ir_counter++;
                    if (ir_counter > 5)
                    {
                        //printf("Game Over! IR LED triggered.\r\n");
                        send_game_over(&msg);
                        can_printmsg(msg);
                        ir_counter = 0;
                        current_state = GAME_OVER;
                    }
                }
                else
                {
                    ir_counter = 0;
                }

                if (can_rx(&msg))
                {
                    switch (msg.id)
                    {
                    case CAN_ID_JOYSTICK:
                    {
                        set_servo_from_js_can(&msg);
                        set_motor_from_js_can(& msg, &js);
                        break;
                    }
                    case CAN_ID_JOYSTICK_BTN:
                    {
                        set_solenoid_from_can(&msg);
                        break;
                    }
                    case CAN_ID_GAME_START:
                    {
                        msg.id = CAN_ID_NODE2_RDY;
                        msg.length = 1;
                        msg.byte[0] = 1; // Node 2 ready signal
                        can_tx(msg);
                        break;
                    }
                    default:
                        printf("Unknown CAN message ID: %d\r\n", msg.id);
                        break;
                    }
                }
                set_motor_pos(js.x);
                break;

            case GAME_OVER:
                printf("Game Over state entered.\r\n");
                while(1) {
                    // Wait here indefinitely
                }
                break;
                
            default:
                current_state = GAME_RUNNING;
                break;
                    
            }
        
    }
}