#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#include "mnist_file.h"
#include "neural_network.h"

#define STEPS 1000
#define BATCH_SIZE 100

/**
 * Downloaded from: http://yann.lecun.com/exdb/mnist/
 */
// const char * train_images_file = "data/train-images-idx3-augmented-ubyte";
// const char * train_labels_file = "data/train-labels-idx1-augmented-ubyte";
// const char * test_images_file = "data/t10k-images-idx3-ubyte";
// const char * test_labels_file = "data/t10k-labels-idx1-ubyte";
const char * train_images_file = "data/train-images-idx3-augmented-binarized-ubyte";
const char * train_labels_file = "data/train-labels-idx1-augmented-binarized-ubyte";
const char * test_images_file = "data/t10k-images-idx3-binarized-ubyte";
const char * test_labels_file = "data/t10k-labels-idx1-ubyte";


/**
 * Calculate the accuracy of the predictions of a neural network on a dataset.
 */
float calculate_accuracy(mnist_dataset_t * dataset, neural_network_t * network)
{
    float activations[MNIST_LABELS], max_activation;
    int i, j, correct, predict;

    // Loop through the dataset
    for (i = 0, correct = 0; i < dataset->size; i++) {
        // Calculate the activations for each image using the neural network
        neural_network_hypothesis(&dataset->images[i], network, activations);

        // Set predict to the index of the greatest activation
        for (j = 0, predict = 0, max_activation = activations[0]; j < MNIST_LABELS; j++) {
            if (max_activation < activations[j]) {
                max_activation = activations[j];
                predict = j;
            }
        }

        // Increment the correct count if we predicted the right label
        if (predict == dataset->labels[i]) {
            correct++;
        }
    }

    // Return the percentage we predicted correctly as the accuracy
    return ((float) correct) / ((float) dataset->size);
}



/**
 * Export the network values to a file.
 */
void exportNetwork(const neural_network_t* net, const char* filename) {
    // Open the file for writing
    FILE* file = fopen(filename, "w");

    if (file != NULL) {
        // Write the 'b' array to the file
        fprintf(file, "{");
        for (int i = 0; i < MNIST_LABELS; i++) {
            fprintf(file, "%.6f", net->b[i]);
            if (i != MNIST_LABELS - 1) {
                fprintf(file, ", ");
            }
        }
        fprintf(file, "},\n");

        // Write the 'W' array to the file
        fprintf(file, "{");
        for (int i = 0; i < MNIST_LABELS; i++) {
            fprintf(file, "{");
            for (int j = 0; j < MNIST_IMAGE_SIZE; j++) {
                fprintf(file, "%.6f", net->W[i][j]);
                if (j != MNIST_IMAGE_SIZE - 1) {
                    fprintf(file, ", ");
                }
            }
            fprintf(file, "}");
            if (i != MNIST_LABELS - 1) {
                fprintf(file, ",\n");
            }
        }
        fprintf(file, "}\n");

        // Close the file
        fclose(file);
        printf("Network exported successfully.\n");
    } else {
        printf("Failed to open the file for writing.\n");
    }
}

/**
 * Import the network values from a file.
 */
void importNetwork(neural_network_t* net, const char* filename) {
    // Open the file for reading
    FILE* file = fopen(filename, "r");

    if (file != NULL) {
        // Read the 'b' array from the file
        fscanf(file, "{");
        for (int i = 0; i < MNIST_LABELS; i++) {
            fscanf(file, "%f", &net->b[i]);
            if (i != MNIST_LABELS - 1) {
                fscanf(file, ", ");
            }
        }
        fscanf(file, "},\n");

        // Read the 'W' array from the file
        fscanf(file, "{");
        for (int i = 0; i < MNIST_LABELS; i++) {
            fscanf(file, "{");
            for (int j = 0; j < MNIST_IMAGE_SIZE; j++) {
                fscanf(file, "%f", &net->W[i][j]);
                if (j != MNIST_IMAGE_SIZE - 1) {
                    fscanf(file, ", ");
                }
            }
            fscanf(file, "}");
            if (i != MNIST_LABELS - 1) {
                fscanf(file, ",\n");
            }
        }
        fscanf(file, "}\n");

        // Close the file
        fclose(file);
        printf("Network imported successfully.\n");
    } else {
        printf("Failed to open the file for reading.\n");
    }
}


int main(int argc, char *argv[])
{
    mnist_dataset_t * train_dataset, * test_dataset;
    mnist_dataset_t batch;
    neural_network_t network;
    float loss, accuracy;
    int i, batches;

    // Read the datasets from the files
    train_dataset = mnist_get_dataset(train_images_file, train_labels_file);
    test_dataset = mnist_get_dataset(test_images_file, test_labels_file);

    // Initialise weights and biases with random values
    neural_network_random_weights(&network);

    // Calculate how many batches (so we know when to wrap around)
    batches = train_dataset->size / BATCH_SIZE;

    for (i = 0; i < STEPS; i++) {
        // Initialise a new batch
        mnist_batch(train_dataset, &batch, 100, i % batches);

        // Run one step of gradient descent and calculate the loss
        loss = neural_network_training_step(&batch, &network, 0.5);

        // Calculate the accuracy using the whole test dataset
        accuracy = calculate_accuracy(test_dataset, &network);

        printf("Step %04d\tAverage Loss: %.2f\tAccuracy: %.3f\n", i, loss / batch.size, accuracy);
    }

    // Export the network to a file
    exportNetwork(&network, "network_binarized.txt");


    // Cleanup
    mnist_free_dataset(train_dataset);
    mnist_free_dataset(test_dataset);

    return 0;
}
