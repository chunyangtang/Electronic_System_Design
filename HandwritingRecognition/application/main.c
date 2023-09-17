// See LICENSE for license details.
#include <stdio.h>
#include <string.h>

#include "hbird_sdk_hal.h"

#include "oledfont.h"
#include "oleddriver.h"
#include "network_tools.h"
#include "network_values.h"


int main(void)
{

	// GPIO Init
	gpio_enable_output(GPIOA, OLED_DC_GPIO_MASK);
	gpio_enable_output(GPIOA, OLED_RES_GPIO_MASK);
	gpio_enable_output(GPIOA, OLED_D1_GPIO_MASK);
	gpio_enable_output(GPIOA, OLED_D0_GPIO_MASK);

	gpio_enable_input(GPIOA, SOC_BUTTON_U_GPIO_MASK);  // K3
	gpio_enable_input(GPIOA, SOC_BUTTON_D_GPIO_MASK);  // K2

	gpio_write(GPIOA, OLED_DC_GPIO_MASK, 0);
	gpio_write(GPIOA, OLED_RES_GPIO_MASK, 1);
	gpio_write(GPIOA, OLED_D1_GPIO_MASK, 0);
	gpio_write(GPIOA, OLED_D0_GPIO_MASK, 0);

	OLED_Init();

	char show_str_1[16] = "Handwriting\0";
	OLED_ShowString(16, 2, show_str_1, 16);
	char show_str_2[16] = "Recognition\0";
	OLED_ShowString(16, 4, show_str_2, 16);

	// Initialize UART
	uart_init(SOC_DEBUG_UART, 115200);
	// char for receiving data
	unsigned char rcv;

	// Initialize network variables
    float activations[10];  // The array for network result outputing.
    neural_network_t network;  // The network struct.
    
	// copy network_b to network.b
    memcpy(network.b, network_b, sizeof(network_b));
    // copy network_W to network.W
    memcpy(network.W, network_W, sizeof(network_W));
	// mnist image
    mnist_image_t image;

    delay_1ms(1000);

	while (1)
	{
		OLED_Clear();
		OLED_ShowString(16, 2, "Waiting for", 16);
		OLED_ShowString(16, 4, "Inputs...", 16);
		scanf("%c", &rcv);
		fflush(stdin);
		if (rcv == 'q')
			break;

		else if (rcv == 'h') {
			for (int i = 0; i < 784; i++) {
				scanf("%c", &rcv);
				image.pixels[i] = rcv;
			}
			fflush(stdin);
			// // Print image for check
			// printf("image: \n");
			// for (int i = 0; i < 784; i++) {
			// 	printf("%3d, ", image.pixels[i]);
			// 	if ((i % 28) == 27)
			// 			printf("\n");
			// }
			// printf("\n");

			neural_network_hypothesis(&image, &network, activations);
    		// int max_ind = max_index(activations, 10);
			// char output_1[16];
    		// sprintf(output_1, "Prediction: %d\0", max_ind);
			// printf("Prediction: %d\n", max_ind);

			unsigned char order[10];
			for (int u = 0; u < 10; u++) {
				order[u] = u;
			}
			// Sorting
			for (int i = 0; i < 9; i++) {
				for (int j = i + 1; j < 10; j++) {
					if (activations[i] < activations[j]) {
						float temp = activations[i];
						activations[i] = activations[j];
						activations[j] = temp;
						unsigned char temp2 = order[i];
						order[i] = order[j];
						order[j] = temp2;
					}
				}
			}
			printf("Predictions: ");
			for (int u = 0; u < 10; u++) {
				printf("%d, ", order[u]);
			}
			printf("\n");
			char output_1[16];
			for (int i = 0; i < 10; i++)
			{
				sprintf(output_1, "Prediction: %d\0", order[i]);
				OLED_Clear();
				OLED_ShowString(0, 0, output_1, 16);
				OLED_ShowString(16, 4, "Is it right?", 8);
				
				OLED_ShowString(16, 6, "Press K2 for Yes", 8);
				OLED_ShowString(16, 7, "Press K3 for No", 8);
				uint8_t signal = 0;
				while (1) {
					if (gpio_read(GPIOA, SOC_BUTTON_D_GPIO_MASK) != SOC_BUTTON_D_GPIO_MASK) {
						printf("Yes (K2) Pressed\n");
						signal = 1;
						break;
					}
					else if (gpio_read(GPIOA, SOC_BUTTON_U_GPIO_MASK) != SOC_BUTTON_U_GPIO_MASK) {
						printf("No (K3) Pressed\n");
						delay_1ms(500);
						break;
					}
					delay_1ms(100);
				}
				if (signal) {
					OLED_Clear();
					OLED_DrawBMP(0, 0, 128, 64, *OK);
					delay_1ms(1000);
					break;
				}
			}

		}
		else
			continue;
	}
	OLED_Clear();

	OLED_DrawBMP(0, 0, 128, 64, *Moon);
	printf("--- End of the program :) ---\n");
	delay_1ms(2000);
	OLED_Clear();
	return 0;
}
