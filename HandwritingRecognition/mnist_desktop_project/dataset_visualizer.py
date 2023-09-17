# This is the code that is used to export the MNIST dataset to images
# Currently, it only export the first 500 images from the test set

import numpy as np
import cv2
from tqdm import tqdm

def export_mnist_images(images_file, labels_file, num_images):
    # Read the images file
    with open(images_file, 'rb') as f:
        f.read(16)  # Skip the file header
        buf = f.read(28 * 28 * num_images)  # Each image is 28x28 pixels
        images = np.frombuffer(buf, dtype=np.uint8).reshape(num_images, 28, 28)

    # Read the labels file
    with open(labels_file, 'rb') as f:
        f.read(8)  # Skip the file header
        buf = f.read(num_images)  # Each label is a single byte
        labels = np.frombuffer(buf, dtype=np.uint8)

    # Export the images
    for i in tqdm(range(num_images)):
        image = images[i]
        label = labels[i]
        filename = f"mnist_{i}_{label}.png"  # Provide a suitable filename
        cv2.imwrite(filename, image)

# Specify the paths to the MNIST dataset files
train_images_file = 'train-images-idx3-ubyte'
train_labels_file = 'train-labels-idx1-ubyte'
test_images_file = 't10k-images-idx3-ubyte'
test_labels_file = 't10k-labels-idx1-ubyte'

# Export a specified number of training images
# num_train_images = 1000  # Choose the desired number of images to export
# export_mnist_images(train_images_file, train_labels_file, num_train_images)

# Export a specified number of test images
num_test_images = 500  # Choose the desired number of images to export
export_mnist_images(test_images_file, test_labels_file, num_test_images)