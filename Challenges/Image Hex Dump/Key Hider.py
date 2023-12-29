import os
import zlib
import random


def hide_crc32_in_file(file_path):
    with open(file_path, 'rb') as file:
        # Read the first 30 characters
        header = file.read(30)

        # Read the remaining content
        content = file.read()

        # Compute CRC32 hash of the content
        crc32_hash = zlib.crc32(content)

        # Convert CRC32 to a decimal string
        crc32_decimal_str = str(crc32_hash)

        # Construct the key string
        key_string = f"--Key: {crc32_decimal_str}--"

        # Choose a random position to insert the key within the content
        insert_position = random.randint(0, len(content))

        # Modify the content to hide the key string
        modified_content = content[:insert_position] + \
            key_string.encode('ascii') + content[insert_position:]

    # Write the modified content back to the file
    with open(file_path, 'wb') as file:
        file.write(header + modified_content)


# Get a list of all .ppm files in the directory
input_folder = "./Immages/"
ppm_files = [f for f in os.listdir(input_folder) if f.lower().endswith('.ppm')]

# Process each .ppm file
for ppm_file in ppm_files:
    file_path = os.path.join(input_folder, ppm_file)
    hide_crc32_in_file(file_path)
    print(f"Key hidden in {ppm_file}")
