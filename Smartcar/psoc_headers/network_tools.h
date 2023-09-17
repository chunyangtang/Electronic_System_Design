#ifndef __NETWORK_TOOLS_H__
#define __NETWORK_TOOLS_H__

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include "network_values.h"
#include <string.h>


// Convert a pixel value from 0-255 to one from 0 to 1
#define PIXEL_SCALE(x) (((float) (x)) / 255.0f)


typedef struct neural_network_t_ {
    float b[10];
    float W[10][784];
} neural_network_t;

typedef struct mnist_image_t_ {
    uint8_t pixels[784];
} mnist_image_t;


/**
 * Calculate the index of the maximum value in the activations array.
 */
int max_index(float * activations, int length) {
    int i; 
    int max_index = 0;
    float max_activation;

    for (i = 1, max_activation = activations[0]; i < length; i++) {
        if (activations[i] > max_activation) {
            max_activation = activations[i];
            max_index = i;
        }
    }

    return max_index;
}

/**
 * Calculate the softmax vector from the activations. This uses a more
 * numerically stable algorithm that normalises the activations to prevent
 * large exponents.
 */
void neural_network_softmax(float * activations, int length)
{
    int i;
    float sum, max;

    for (i = 1, max = activations[0]; i < length; i++) {
        if (activations[i] > max) {
            max = activations[i];
        }
    }

    for (i = 0, sum = 0; i < length; i++) {
        activations[i] = exp(activations[i] - max);
        sum += activations[i];
    }

    for (i = 0; i < length; i++) {
        activations[i] /= sum;
    }
}


/**
 * Use the weights and bias vector to forward propogate through the neural
 * network and calculate the activations.
 */
void neural_network_hypothesis(mnist_image_t * image, neural_network_t * network, float activations[10])
{
    int i, j;

    for (i = 0; i < 10; i++) {
        activations[i] = network->b[i];

        for (j = 0; j < 784; j++) {
            activations[i] += network->W[i][j] * PIXEL_SCALE(image->pixels[j]);
        }
    }

    neural_network_softmax(activations, 10);
}

neural_network_t* network_init() {
    neural_network_t* network = (neural_network_t *)malloc(sizeof(neural_network_t));
    memcpy(network->b, network_b, sizeof(network_b));
    // copy network_W to network.W
    memcpy(network->W, network_W, sizeof(network_W));
    return network;
}

float* network_hypothesis(uint8_t image[784], neural_network_t* network) {
    float* activations = (float*)malloc(10*sizeof(float));
    mnist_image_t mnist_image;
    memcpy(mnist_image.pixels, image, 784*sizeof(uint8_t));
    neural_network_hypothesis(&mnist_image, network, activations);
    return activations;
}

void network_del(neural_network_t* network, float* activations) {
    free(network);
    free(activations);
}


#endif // __NETWORK_TOOLS_H__
