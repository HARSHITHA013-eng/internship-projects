
#include "ECU1_sensor.h"       // Include sensor header for function declarations
#include "adc.h"               // Include ADC functions
#include "can.h"               // Include CAN functions
#include "MSG_ID.h"            // Include CAN message ID definitions
#include "uart.h"              // Include UART functions

char str[4];                   // String to store speed as ASCII characters

uint16_t get_speed()
{
    unsigned short adc_val;
    unsigned short speed;

    adc_val = read_adc(CHANNEL4);

    if (adc_val > 1000)
        speed = 100;
    else
        speed = ((unsigned long)adc_val * 100) / 1023;

    str[0] = (speed / 100) % 10 + '0';
    str[1] = (speed / 10) % 10 + '0';
    str[2] = (speed % 10) + '0';
    str[3] = '\0';

    puts("Speed: ");
    puts(str);
    puts("\r\n");

    return speed;
}

// Function to read and return current gear position
unsigned char get_gear_pos()
{
    static char str[8] = "N12345R";       // Gear characters: R, N, 1?5, Collision
    static unsigned char index = 1;        // Initial gear index (pointing to 'N')

    unsigned char key = read_digital_keypad(STATE_CHANGE);  // Read key press

    if (key == COLLISION)
    {
        index = 7;                         // Set to collision gear ('C')
    }
    else if (index == 7 && (key == GEAR_UP || key == GEAR_DOWN))
    {
        index = 1;                         // Reset to Neutral if gear key is pressed after collision
    }
    else if (key == GEAR_UP)
    {
        if (index < 6)                     // Increment gear up to '5'
        {
            index++;
        }
    }
    else if (key == GEAR_DOWN)
    {
        if (index > 0)                     // Decrement gear down to 'R'
        {
            index--;
        }
    }

    return str[index];                     // Return the current gear character
}
