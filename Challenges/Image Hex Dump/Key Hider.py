import os
import zlib
import random


def hide_key_in_file(file_path, key):
    with open(file_path, 'rb') as file:
        # Read the first 30 characters
        header = file.read(30)

        # Read the remaining content
        content = file.read()

        # Construct the key string
        key_string = f"--Key: {key}--"

        # Choose a random position to insert the key within the content
        insert_position = random.randint(0, len(content))

        # Modify the content to hide the key string
        modified_content = content[:insert_position] + \
            key_string.encode('ascii') + content[insert_position:]

    # Write the modified content back to the file
    with open(file_path, 'wb') as file:
        file.write(header + modified_content)


def read_keys_from_folder(keys_folder):
    keys = []
    for file_name in os.listdir(keys_folder):
        if file_name.lower().startswith("key_") and file_name.lower().endswith(".txt"):
            key_file_path = os.path.join(keys_folder, file_name)
            with open(key_file_path, 'r') as key_file:
                key = key_file.read().strip()
                keys.append(key)
    return keys


# Input and output folders
images_folder = "./Immages/"
keys_folder = "./Keys/"

# Get a list of all .ppm files in the directory
ppm_files = [f for f in os.listdir(
    images_folder) if f.lower().endswith('.ppm')]

# Get the list of keys from the Keys folder
keys = read_keys_from_folder(keys_folder)

# Process each .ppm file
for i, ppm_file in enumerate(ppm_files):
    file_path = os.path.join(images_folder, ppm_file)

    # Use the corresponding key for each image (cycling through the keys)
    key = keys[i % len(keys)]

    hide_key_in_file(file_path, key)
    print(f"Key hidden in {ppm_file}")
