#ifndef CAMERCA_CONNECT_H_
#define CAMERCA_CONNECT_H_
    
#include <project.h>
#include <stdlib.h>
    
#define IMAGE_HEIGHT 160
#define IMAGE_WIDTH 120
    
void camera_init() {
    UART_CAMERA_Start();
}

unsigned char* camera_img_get() {
    unsigned char* image = (unsigned char *)malloc(IMAGE_HEIGHT * IMAGE_WIDTH);
    unsigned char rcv;
    char validation[4] = "GAMI";
    unsigned int vali_int;
    unsigned int rcv_int = 0;
    memcpy(&vali_int, validation, 4);
    _Bool transfer_signal = 0;
    while (!transfer_signal) {
        rcv = UART_CAMERA_GetChar();
        rcv_int <<= 8;
        memcpy(&rcv_int, &rcv, 1);
        transfer_signal = (rcv_int == vali_int);
    }
    
    for (int i = 0; i < IMAGE_HEIGHT * IMAGE_WIDTH; i++) {
        rcv = UART_CAMERA_GetChar();
        image[i] = rcv;
    }
    return image;
}


void image_del(char *image) {
    free(image);
}
    
#endif