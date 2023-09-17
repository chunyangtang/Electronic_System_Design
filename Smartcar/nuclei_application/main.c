// See LICENSE for license details.
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include "hbird_sdk_soc.h"
#include "oleddriver.h"
#include "oledfont.h"

#define SEND_CMD 'c'
#define SEND_STRING 's'

#define CMD_CLEAR 'r'
#define CMD_WELCOMEPAGE 'w'
#define CMD_AUDIOPIC 'a'

int main(void)
{
	// GPIO Init
	gpio_enable_output(GPIOA, OLED_DC_GPIO_MASK);
	gpio_enable_output(GPIOA, OLED_RES_GPIO_MASK);
	gpio_enable_output(GPIOA, OLED_D1_GPIO_MASK);
	gpio_enable_output(GPIOA, OLED_D0_GPIO_MASK);

	gpio_write(GPIOA, OLED_DC_GPIO_MASK, 0);
	gpio_write(GPIOA, OLED_RES_GPIO_MASK, 1);
	gpio_write(GPIOA, OLED_D1_GPIO_MASK, 0);
	gpio_write(GPIOA, OLED_D0_GPIO_MASK, 0);

	OLED_Init();
	// Initialize UART
	gpio_iof_config(GPIOA, IOF_UART2_MASK);
	uart_init(UART2, 115200);
	// char for receiving data
	unsigned char rcv;
	OLED_Clear();
	OLED_ShowString_Enhanced(0, 0, "UART\nConnecting...",16);

	while (1)
	{
		rcv = uart_read(UART2);
		if (rcv == SEND_STRING) {
			uint8_t x = uart_read(UART2);
			uint8_t y = uart_read(UART2);
			uint8_t size = uart_read(UART2);
			uint8_t str_len = uart_read(UART2);
			char *str = (char *)malloc(str_len);
			for (int i = 0; i < str_len; i++) {
				str[i] = uart_read(UART2);
			}
			// OLED_Clear();
			OLED_ShowString_Enhanced(x, y, str, size);
		}
		else if (rcv == SEND_CMD) {
			uint8_t command = uart_read(UART2);
			switch (command) {
			case CMD_CLEAR:
				OLED_Clear();
				break;
			case CMD_WELCOMEPAGE:
				OLED_DrawBMP(0, 0, 128, 64, *Welcome_Page);
				break;
			case CMD_AUDIOPIC:
				OLED_DrawBMP(0, 32, 32, 32, *Audio_Pic);
				break;
			default:
				break;
			}
		}
	}


    return 0;
}

