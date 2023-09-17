
#ifndef IMAGE_UTILS_H_
#define IMAGE_UTILS_H_
    
#include <string.h>
    
#define FIRST_HEIGHT 320
#define ORIGINAL_WIDTH 240
#define ORIGINAL_HEIGHT 240
#define DOWNSCALED_WIDTH 28
#define DOWNSCALED_HEIGHT 28
    
void cropImage (unsigned char* originalImage, unsigned char* croppedImage) {
    memcpy(croppedImage, originalImage+80*240, 240*240);
}
    

void downscaleImage(unsigned char* originalImage, unsigned char* downscaledImage) {
    int x_ratio = ORIGINAL_WIDTH / DOWNSCALED_WIDTH;
    int y_ratio = ORIGINAL_HEIGHT) / DOWNSCALED_HEIGHT;
    int x, y;

    for (int i = 0; i < DOWNSCALED_HEIGHT; i++) {
        for (int j = 0; j < DOWNSCALED_WIDTH; j++) {
            x = j * x_ratio;
            y = i * y_ratio;

            unsigned char* originalPixel = originalImage + y * ORIGINAL_WIDTH + x;
            unsigned char* downscaledPixel = downscaledImage + i * DOWNSCALED_WIDTH + j;

            // Average the pixel values of the corresponding region
            int sum = 0;
            int totalPixels = 0;
            for (int k = 0; k < y_ratio; k++) {
                for (int l = 0; l < x_ratio; l++) {
                    if (y + k >= 240 || x + l >= 240) {
                        continue;
                    }
                    unsigned char* currentPixel = originalPixel + k * ORIGINAL_WIDTH + l;
                    sum += *currentPixel;
                    totalPixels++;
                }
            }
            *downscaledPixel = sum / totalPixels;
        }
    }
}
    

// Function that binarize and INVERSE the color of the image into black and white
void binarizeImage(unsigned char* image, unsigned char threshold) {
    for (int i = 0; i < DOWNSCALED_HEIGHT * DOWNSCALED_WIDTH; i++)
    {
        image[i] = (image[i] > threshold)? 0: 255;
    }
}


#endif