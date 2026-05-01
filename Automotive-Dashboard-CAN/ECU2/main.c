
#define _XTAL_FREQ 20000000             // Define crystal frequency for delay functions

#include "ECU2_sensor.h"                // Include sensor-related functions like get_rpm and process_indicator
#include "adc.h"                        // Include ADC functions
#include "can.h"                        // Include CAN functions
#include "MSG_ID.h"                     // Include CAN message IDs
#include "uart.h"                       // Include UART functions

// Function to initialize all peripherals
void init_config(void) 
{
    TRISB = 0x00;                       // Set RB2?RB5 as inputs, others as outputs
    PORTB = 0x00;                       // Initialize PORTB with 0x3C
    init_adc();                         // Initialize ADC
    init_uart();                        // Initialize UART for debugging
    init_can();                         // Initialize CAN communication
    init_digital_keypad();             // Initialize digital keypad for indicator inputs
}
int main() 
{
    init_config();                      // Call peripheral initialization

    while (1) 
    {                         // Infinite loop
        unsigned int rpm = get_rpm(); 
        uint8_t rpm_data[2];
        rpm_data[0] = (rpm >> 8) & 0xFF;   // MSB
        rpm_data[1] = rpm & 0xFF;          // LSB
        // Get current RPM from sensor
        can_transmit(RPM_MSG_ID, rpm_data, 2);  // Transmit RPM over CAN (4-byte string)
        
        __delay_ms(100);                // Delay for stability

        IndicatorStatus key = process_indicator();  // Read indicator switch status
        static int flag1;                        // Store current indicator status
        static unsigned char toggle = 0;
        toggle = !toggle;     // change state every loop
        if (key == e_ind_left) 
        {
            flag1 = 1;                           // Left indicator ON
        } else if (key == e_ind_right) 
        {
            flag1 = 2;                           // Right indicator ON
        } else if (key == e_ind_off) 
        {
            flag1 = 0;                           // Indicators OFF
        }
        else if (key == e_ind_hazard) 
        {
            flag1 = 3;                           // Indicators OFF
        }
        if (flag1 == 1)
        {
            if (toggle)
            LEFT_IND_ON();
            else
            LEFT_IND_OFF();

            RIGHT_IND_OFF();
        }
        else if (flag1 == 0) 
        {
            LEFT_IND_OFF();                      // Turn off both indicators
            RIGHT_IND_OFF();
        }
        else if (flag1 == 2)
        {
            if (toggle)
               RIGHT_IND_ON();
            else
               RIGHT_IND_OFF();
            LEFT_IND_OFF();
        }
        else if (flag1 == 3)
        {
            if (toggle)
            {
                LEFT_IND_ON();
                RIGHT_IND_ON();
            }
            else
            {
                LEFT_IND_OFF();
                RIGHT_IND_OFF();
            }
        }
        can_transmit(INDICATOR_MSG_ID, &flag1, 1);  // Transmit indicator status over CAN
        __delay_ms(100);                            // Delay before next iteration
    }
}
