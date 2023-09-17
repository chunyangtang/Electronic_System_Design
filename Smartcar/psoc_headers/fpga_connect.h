
#ifndef FPGA_CONNECT_H_
#define FPGA_CONNECT_H_

#include <project.h>
#include <string.h>
#include <stdint.h>
    
#define SEND_CMD 'c'
#define SEND_STRING 's'
    
#define CMD_CLEAR 'r'
#define CMD_WELCOMEPAGE 'w'
#define CMD_AUDIOPIC 'a'

void fpga_init() {
    UART_FPGA_Start();
}

void fpga_SendString(const char *string, uint8_t x, uint8_t y, uint8_t size) {
    UART_FPGA_PutChar(SEND_STRING);
    UART_FPGA_PutChar(x);
    UART_FPGA_PutChar(y);
    UART_FPGA_PutChar(size);
    uint8_t str_length = strlen(string);
    UART_FPGA_PutChar(str_length);
    UART_FPGA_PutString(string);    
}

void fpga_SendString_simple(const char *string) {
    fpga_SendString(string, 0, 0, 16);
}

void fpga_ClearScreen() {
    UART_FPGA_PutChar(SEND_CMD);
    UART_FPGA_PutChar(CMD_CLEAR);
}

void fpga_ShowWelcomePage() {
    UART_FPGA_PutChar(SEND_CMD);
    UART_FPGA_PutChar(CMD_WELCOMEPAGE);
}

void fpga_ShowAudioPic() {
    UART_FPGA_PutChar(SEND_CMD);
    UART_FPGA_PutChar(CMD_AUDIOPIC);
}



#endif