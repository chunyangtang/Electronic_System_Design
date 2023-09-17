#ifndef __NETWORK_TOOLS_H__
#define __NETWORK_TOOLS_H__

#include <math.h>
#include <stdint.h>
// #include "nice_sum_accelerator.h"


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

	// unsigned int begin_cycle,   end_cycle,   cycle_normal,   cycle_nice;
	// begin_cycle   =  __get_rv_cycle();
	// for (i = 0; i < length; i++) {
	// 	sum += activations[i];
	// }
	// end_cycle   =  __get_rv_cycle();
	// cycle_normal = end_cycle - begin_cycle;

	// sum = 0;
	// begin_cycle   =  __get_rv_cycle();
	// custom_lbuf(activations);
	// sum = custom_rowsum(activations);
	// end_cycle   =  __get_rv_cycle();
	// cycle_nice = end_cycle - begin_cycle;
	// printf("cycle_normal: %d, cycle_nice: %d\n", cycle_normal, cycle_nice);

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





#endif // __NETWORK_TOOLS_H__