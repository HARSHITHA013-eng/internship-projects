
#include "ECU2_sensor.h"
#include "adc.h"
#include "can.h"
#include "MSG_ID.h"
#include "uart.h"


uint16_t get_rpm()
{
    // Step 1: Read ADC
    unsigned int adc_val = read_adc(CHANNEL4);

    // Step 2: Convert to speed (NO FLOAT)
    unsigned int speed = adc_val / 10.23;

    // Step 3: Calculate RPM (NO OVERFLOW)
    unsigned long rpm = (unsigned long)speed * 60;

//    // Step 4: Convert to string
//    char rpm_str[5];
//
//    rpm_str[0] = ((rpm / 1000) % 10) + '0';
//    rpm_str[1] = ((rpm / 100) % 10) + '0';
//    rpm_str[2] = ((rpm / 10) % 10) + '0';
//    rpm_str[3] = (rpm % 10) + '0';
//    rpm_str[4] = '\0';
//
//    // Step 4: Send over UART 
//    puts("RPM: "); 
//    puts(rpm_str); 
//    puts("\r\n"); 
//    return rpm;
    return (uint16_t)rpm;

}

/*uint16_t get_engine_temp()
{
    //Implement the engine temperature function
}*/
unsigned char pre_key = SWITCH2;  // default OFF

IndicatorStatus process_indicator()
{
    //Implement the indicator function
    unsigned char key = read_digital_keypad(STATE_CHANGE);
    if (key != 0x0f)
    {
        pre_key = key;
    }

    if (pre_key == SWITCH1)  // Left indicator
    {
        return e_ind_left;
    }
    else if (pre_key == SWITCH3)  // Right indicator
    {
        return e_ind_right;
    }
    else if (pre_key == SWITCH2)  // Turn off all indicators
    {
        return e_ind_off;
    }
    else if (pre_key == SWITCH4)  // hazards light signals
    {
        return e_ind_hazard;
    }
    
}
