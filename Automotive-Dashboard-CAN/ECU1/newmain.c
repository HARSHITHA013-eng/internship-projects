
#define _XTAL_FREQ 20000000
#include "adc.h"
#include "can.h"
#include "ECU1_sensor.h"
#include "MSG_ID.h"
#include "uart.h"

void init_config(void)
{
    init_adc();                  // Initialize Analog-to-Digital Converter
    init_uart();                 // Initialize UART for serial communication
    init_digital_keypad();       // Initialize digital keypad (gear position input)
    init_can();                  // Initialize CAN module
}

int main()
{
    init_config();               // Call the init_config function to set up all peripherals

    while(1)                     // Infinite loop
    {
        unsigned short int speed = get_speed();   // Read speed value from sensor
        can_transmit(SPEED_MSG_ID, (uint8_t *)&speed, 3);
        __delay_ms(100);              // Delay to avoid flooding the bus

        unsigned char gear = get_gear_pos();               // Get current gear position
        puts("Gear: ");                                      // Print "Gear: " to UART
        putch(gear);                                   // Print gear value to UART
        puts("\n\r");                             // Move to next line on UART
        can_transmit(GEAR_MSG_ID, &gear, 1);                 // Transmit gear over CAN bus
        __delay_ms(100);                            // Delay before next loop iteration
    }
}
