# This is the code that is used to augment the MNIST dataset by performing rotations, zooms, and shifts on the images

import numpy as np
import struct
import cv2

# Specify the file paths for the MNIST ubyte files
images_file = "data/train-images-idx3-ubyte"
labels_file = "data/train-labels-idx1-ubyte"

# Read the images file
with open(images_file, 'rb') as f_images:
    # Read the file header
    magic_number, num_images, image_height, image_width = struct.unpack('>4I', f_images.read(16))

    # Read the image data
    images_data = np.frombuffer(f_images.read(), dtype=np.uint8)
    images_data = images_data.reshape(num_images, image_height, image_width)

# Read the labels file
with open(labels_file, 'rb') as f_labels:
    # Read the file header
    magic_number, num_labels = struct.unpack('>2I', f_labels.read(8))

    # Read the label data
    labels_data = np.frombuffer(f_labels.read(), dtype=np.uint8)

# Now you have the MNIST data loaded into NumPy arrays
print("Images shape:", images_data.shape)  # (num_images, image_height, image_width)
print("Labels shape:", labels_data.shape)  # (num_labels,)

# Perform augmentation or any other operations on the loaded data as needed
# Binarization
images_data = np.where(images_data > 0, np.uint8(255), np.uint8(0))
# Define the rotation and zoom parameters
rotation_angles = [-15, 15]  # Rotation angles in degrees
zoom_factors = [0.8, 1.2]  # Zoom factors
shift_factors = [-5, 6]  # Shift factors

# Create empty lists to store the augmented data
augmented_images = []
augmented_labels = []

def beta_2_2(lboundery, rboundery, num):
    return np.random.beta(2, 2, num) * (rboundery - lboundery) + lboundery

def img_shift(img, shift_factor_lr, shift_factor_ud):
    # Shift the image
    shifted_image = np.zeros_like(img)
    if shift_factor_lr > 0:
        shifted_image[:, shift_factor_lr:] = image[:, :-shift_factor_lr]  # Shift right
        if shift_factor_ud > 0:
            shifted_image[shift_factor_ud:, :] = shifted_image[:-shift_factor_ud, :]  # Shift down
        elif shift_factor_ud < 0:    
            shifted_image[:shift_factor_ud, :] = shifted_image[-shift_factor_ud:, :]  # Shift up

    elif shift_factor_lr < 0:
        shifted_image[:, :shift_factor_lr] = image[:, -shift_factor_lr:]  # Shift left
        if shift_factor_ud > 0:
            shifted_image[shift_factor_ud:, :] = shifted_image[:-shift_factor_ud, :]
        elif shift_factor_ud < 0:
            shifted_image[:shift_factor_ud, :] = shifted_image[-shift_factor_ud:, :]
    else:
        if shift_factor_ud > 0:
            shifted_image[shift_factor_ud:, :] = shifted_image[:-shift_factor_ud, :]
        elif shift_factor_ud < 0:
            shifted_image[:shift_factor_ud, :] = shifted_image[-shift_factor_ud:, :]
        else:
            shifted_image = image  # No shift

    return shifted_image

# Iterate over each image in the dataset
for image, label in zip(images_data, labels_data):
    # Apply rotations
    for angle in np.random.uniform(rotation_angles[0], rotation_angles[1], 2):  # beta_2_2(rotation_angles[0], rotation_angles[1], 2):
        # Rotate the image
        rotated_image = cv2.warpAffine(image, cv2.getRotationMatrix2D((14, 14), angle, 1.0), (28, 28))
        rotated_image = cv2.resize(rotated_image, (28, 28), interpolation=cv2.INTER_LINEAR)

        augmented_images.append(rotated_image)
        augmented_labels.append(label)

    # Apply zooms
    for zoom_factor in np.random.uniform(zoom_factors[0], zoom_factors[1], 2): # beta_2_2(zoom_factors[0], zoom_factors[1], 2):
        # Zoom the image
        zoomed_image = cv2.resize(image, None, fx=zoom_factor, fy=zoom_factor, interpolation=cv2.INTER_LINEAR)
        zoomed_image = cv2.resize(zoomed_image, (28, 28), interpolation=cv2.INTER_LINEAR)

        augmented_images.append(zoomed_image)
        augmented_labels.append(label)

    # Apply shifts
    for shift_factor in np.random.randint(shift_factors[0], shift_factors[1], 1):
        shift_factor_ud = np.random.randint(shift_factors[0], shift_factors[1])

        shifted_image = img_shift(image, shift_factor, shift_factor_ud)

        augmented_images.append(shifted_image)
        augmented_labels.append(label)

    rotated_image = cv2.warpAffine(image, cv2.getRotationMatrix2D((14, 14), angle, 1.0), (28, 28))
    rotated_image = cv2.resize(rotated_image, (28, 28), interpolation=cv2.INTER_LINEAR)
    zoomed_image = cv2.resize(rotated_image, None, fx=zoom_factor, fy=zoom_factor, interpolation=cv2.INTER_LINEAR)
    zoomed_image = cv2.resize(zoomed_image, (28, 28), interpolation=cv2.INTER_LINEAR)
    do_everything_image = img_shift(zoomed_image, shift_factor, shift_factor_ud)
    # do_everything_image = zoomed_image
    augmented_images.append(do_everything_image)
    augmented_labels.append(label)


# Convert the augmented data to numpy arrays
augmented_images = np.array(augmented_images)
augmented_labels = np.array(augmented_labels)

# Combine the augmented data with the original dataset
x_train_augmented = np.concatenate((images_data, augmented_images), axis=0)
y_train_augmented = np.concatenate((labels_data, augmented_labels), axis=0)

# Shuffle the augmented dataset
indices = np.random.permutation(len(x_train_augmented))
x_train_augmented = x_train_augmented[indices]
y_train_augmented = y_train_augmented[indices]

# Export the augmented data back to ubyte files if desired
# Specify the file paths for the exported dataset
output_dir = "data/"
output_images_file = output_dir + "train-images-idx3-augmented-binarized-ubyte"
output_labels_file = output_dir + "train-labels-idx1-augmented-binarized-ubyte"

# Convert the augmented data to the ubyte format
num_images = len(x_train_augmented)
print("Number of images:", num_images)
image_height = x_train_augmented.shape[1]
image_width = x_train_augmented.shape[2]

# Write the images file
with open(output_images_file, 'wb') as f_images:
    # Write the file header
    f_images.write(struct.pack('>4B', 0x00, 0x00, 0x08, 0x03))
    f_images.write(struct.pack('>I', num_images))
    f_images.write(struct.pack('>I', image_height))
    f_images.write(struct.pack('>I', image_width))

    # Write the image data
    for image in x_train_augmented:
        f_images.write(image.tobytes())

# Write the labels file
with open(output_labels_file, 'wb') as f_labels:
    # Write the file header
    f_labels.write(struct.pack('>4B', 0x00, 0x00, 0x08, 0x01))
    f_labels.write(struct.pack('>I', num_images))

    # Write the label data
    for label in y_train_augmented:
        f_labels.write(struct.pack('B', label))

# ----------------------------------------------
# Binarize the TEST dataset
test_images_file = "data/t10k-images-idx3-ubyte"
test_labels_file = "data/t10k-labels-idx1-ubyte"
# Read the images file
with open(test_images_file, 'rb') as f_images:
    # Read the file header
    magic_number, num_images, image_height, image_width = struct.unpack('>4I', f_images.read(16))

    # Read the image data
    images_data = np.frombuffer(f_images.read(), dtype=np.uint8)
    images_data = images_data.reshape(num_images, image_height, image_width)

images_data = np.where(images_data > 0, np.uint8(255), np.uint8(0))
print("Test images shape:", images_data.shape)

output_test_images_file = "data/t10k-images-idx3-binarized-ubyte"

# Convert the augmented data to the ubyte format
num_images = images_data.shape[0]

# Write the images file
with open(output_test_images_file, 'wb') as f_images:
    # Write the file header
    f_images.write(struct.pack('>4B', 0x00, 0x00, 0x08, 0x03))
    f_images.write(struct.pack('>I', num_images))
    f_images.write(struct.pack('>I', image_height))
    f_images.write(struct.pack('>I', image_width))

    # Write the image data
    for image in images_data:
        f_images.write(image.tobytes())

print("Dataset exported successfully!")