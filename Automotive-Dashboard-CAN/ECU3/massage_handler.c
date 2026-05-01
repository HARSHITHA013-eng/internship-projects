
#include <xc.h>                       // Include processor-specific header
#include <string.h>                   // Include string functions (optional here)
#include "message_handler.h"         // Include function declarations for message handlers
#include "MSG_ID.h"                   // Include CAN message ID macros
#include "can.h"                      // Include CAN communication functions
#include "clcd.h"                     // Include CLCD display functions
#include "uart.h"                     // Include UART communication functions

volatile unsigned char led_state = LED_OFF, status = e_ind_off;  // Variables to track LED and indicator status

// Handle received speed data
void handle_speed_data(uint8_t *data, uint8_t len)
{
    clcd_putch(data[0] / 100%10+ '0', LINE2(0));  // Display tens digit of speed  // Display units digit of speed
    clcd_putch(data[0] /10 %10+ '0', LINE2(1));
    clcd_putch(data[0] %10+ '0', LINE2(2));
}

// Handle received gear data
void handle_gear_data(uint8_t *data, uint8_t len) 
{
    clcd_putch(*data, LINE2(4));               // Display gear character at position 4 on line 2
}

// Handle received RPM data
void handle_rpm_data(uint8_t *data, uint8_t len) 
{
    if (len < 2) return;

    // Step 1: Reconstruct RPM
    uint16_t rpm = (data[0] << 8) | data[1];

    // Step 2: Convert to string
    char rpm_str[5];

    rpm_str[0] = ((rpm / 1000) % 10) + '0';
    rpm_str[1] = ((rpm / 100) % 10) + '0';
    rpm_str[2] = ((rpm / 10) % 10) + '0';
    rpm_str[3] = (rpm % 10) + '0';
    rpm_str[4] = '\0';

    // Step 3: Clear old data
    clcd_print("    ", LINE2(7));

    // Step 4: Display
    for (int i = 0; i < 4; i++)
    {
        clcd_putch(rpm_str[i], LINE2(7 + i));
    }
}

// Handle received engine temperature data (not implemented)
/*void handle_engine_temp_data(uint8_t *data, uint8_t len) 
{
    // Reserved for future use
}*/

// Handle received indicator status data
void handle_indicator_data(uint8_t *data, uint8_t len) 
{
    static unsigned char toggle = 0;
        toggle = !toggle;     // change state every loop
    if (*data == 1) {
       if (toggle)
          LEFT_IND_ON();
       else
           LEFT_IND_OFF();
       RIGHT_IND_OFF();
        clcd_print("<  ", LINE2(12));          // Display left arrow
    }
    if (*data == 0) {
        LEFT_IND_OFF();                        // Turn off both indicators
        RIGHT_IND_OFF();
        clcd_print("OFF", LINE2(12));          // Display "OFF"
    }
    if (*data == 2) {
        if (toggle)
            RIGHT_IND_ON();
        else
            RIGHT_IND_OFF();  
        LEFT_IND_OFF();   // Turn on right indicator
        clcd_print("  >", LINE2(12));          // Display right arrow
    }
    if (*data == 3) {
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
        clcd_print("<->", LINE2(12));          // Display right arrow
    }
}

// Process all incoming CAN messages
void process_canbus_data() 
{
    uint16_t msg_id;                           // Variable to store received message ID
    uint8_t data[8];                           // Array to store received data
    uint8_t len;                               // Variable to store length of data

    can_receive(&msg_id, data, &len);          // Receive CAN message

    if(len <1)
        return;
    
    // Handle message based on message ID
    switch (msg_id)
    {
        case SPEED_MSG_ID: 
            handle_speed_data(data, len);      // Call speed handler
            break;

        case GEAR_MSG_ID:   
            handle_gear_data(data, len);       // Call gear handler
            break;

        case RPM_MSG_ID:   
            handle_rpm_data(data, len);        // Call RPM handler
            break;

        case INDICATOR_MSG_ID:    
            handle_indicator_data(data, len);  // Call indicator handler
            break;
    }
}

