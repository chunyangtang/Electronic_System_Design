import numpy as np

file_name = "mnist_network_1616.txt"

# Step 1: Read the file
with open(file_name, "r") as file:
    file_contents = file.read()

# Step 2: Extract network information
network_depth = int(file_contents.split("Network depth: ")[1].split("\n")[0])
network_shape = eval(file_contents.split("Network shape: ")[1].split("\n")[0])

# Step 3: Initialize arrays
weights = []
biases = []

# Step 4: Read weights and biases
for i in range(network_depth-1):
    layer_header = f"\n\nLayer {i}\n"
    weights_start = file_contents.index(layer_header) + len(layer_header) + len("Weights:\n")
    weights_end = file_contents.index("\n", weights_start)
    biases_start = file_contents.index("Biases:\n", weights_end) + len("Biases:\n")
    biases_end = file_contents.index("\n\n", biases_start)

    weights_data = file_contents[weights_start:weights_end]
    biases_data = file_contents[biases_start:biases_end]

    weights.append(np.array(eval(weights_data)))
    biases.append(np.array(eval(biases_data)))


# max=5.851126, min=-6.201904

pass