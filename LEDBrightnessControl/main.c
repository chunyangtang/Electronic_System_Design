
#include "project.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    // Initializing LCD
    LCD_Start();
    // Initializing CapSense
    CapSense_Start();
    CapSense_InitializeAllBaselines(); 
    CapSense_ScanEnabledWidgets(); 
    // Initializing PWM
    PWM_Start();
    // Initializing UART
    /* Start USBFS Operation with 3V operation */ 
    USBUART_1_Start(0u, USBUART_1_3V_OPERATION); 
    LCD_Position(0, 0); 
    LCD_PrintString("UART STARTING..."); 
    /* Wait for Device to enumerate */     
    while(!USBUART_1_GetConfiguration()); 
    /* Enumeration is done, enable OUT endpoint for receive data from Host */ 
    USBUART_1_CDC_Init(); 

    uint8 period_pwm;  // Current pwm period num (expected to be 255)
    uint8 cmpval_pwm;  // Current pwm compare value (0-255)
    uint8 sliderPosition = 0xFF;  // Newly retrieved slider position (0-255) 
    uint8 sliderPositionOld = 0xFF;  // Last slider position (0-255)
    uint8 sliderPosTemp = 0xFF;  // Raw slider position (0-100), could be user input as well.
    int16 stringNumTemp;  // Signed int to deal with invalid input fron uart
    char8 uartBuffer[64];  // Buffer for uart communication
    uint16 buffersize;  //Buffer size from uart

    LCD_Position(0, 0); 
    LCD_PrintString("CMP Val / Period"); 


    for(;;)
    {
        if (!CapSense_IsBusy()) 
        { 
            // CapSense_ProcessAllWidgets(); 
            // CapSense_RunTuner(); 
            CapSense_UpdateEnabledBaselines(); 
            CapSense_ScanEnabledWidgets(); 

            /* Test if button widget is active */             
            // statusButton0 = CapSense_CheckIsWidgetActive(CapSense_BUTTON0__BTN);             
            // statusButton1 = CapSense_CheckIsWidgetActive(CapSense_BUTTON1__BTN);  

            sliderPosTemp = (uint8)CapSense_GetCentroidPos(CapSense_LINEARSLIDER0__LS);
            if (sliderPosTemp != 0xFF) {
                sliderPosition = sliderPosTemp * 255 / 100;
                if (sliderPosition != sliderPositionOld) {
                    sliderPositionOld = sliderPosition;
                    PWM_WriteCompare(sliderPosition);
                    sprintf(uartBuffer, "Brightness: %d%%\r\n", sliderPosTemp);
                    /* Wait till component is ready to send more data to the PC */
                    while(USBUART_1_CDCIsReady() == 0u);                    
                    /* Send data back to PC */ 
                    USBUART_1_PutString(uartBuffer);
                }
            }
        }
        /* Check for input data from PC */
        if(USBUART_1_DataIsReady() != 0u)               
        {                
            /* Read received data and re-enable OUT endpoint */
            buffersize = USBUART_1_GetAll(uartBuffer);           
            if(buffersize != 0u) 
            {
                // Write input number to PWM
                stringNumTemp = atoi(uartBuffer);
                if (stringNumTemp > 100) {
                    sliderPosTemp = 100;
                }
                else if (stringNumTemp < 0) {
                    sliderPosTemp = 0;
                }
                else {
                    sliderPosTemp = stringNumTemp;
                }
                sliderPosition = sliderPosTemp * 255 / 100;
                if (sliderPosition != sliderPositionOld) {
                    sliderPositionOld = sliderPosition;
                    PWM_WriteCompare(sliderPosition);
                    sprintf(uartBuffer, "Brightness: %d%%\r\n", sliderPosTemp);
                    while(USBUART_1_CDCIsReady() == 0u);           
                    USBUART_1_PutString(uartBuffer);
                }
            }
        }



        period_pwm = PWM_ReadPeriod();
        cmpval_pwm = PWM_ReadCompare();

        LCD_Position(1, 2);
        if (cmpval_pwm < 10)
        {
            LCD_PrintString("  ");
            LCD_Position(1, 4);
        }
        else if (cmpval_pwm < 100)
        {
            LCD_PrintString(" ");
            LCD_Position(1, 3);
        }
        LCD_PrintNumber(cmpval_pwm);
        LCD_Position(1, 8);
        LCD_PutChar('/');
        LCD_Position(1, 11);
        LCD_PrintNumber(period_pwm);
        CyDelay(100);
    }
}

