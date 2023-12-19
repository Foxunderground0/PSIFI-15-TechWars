import os


def generate_cpp_array(files_dir, array_name, output_dir):
    # Get a list of all files in the data directory
    files = [f for f in os.listdir(files_dir) if os.path.isfile(
        os.path.join(files_dir, f))]

    # Sort the files to ensure consistent order
    files.sort()

    # Start generating the C++ code
    cpp_code = f"#include <Arduino.h>\nconst String {array_name}[] = {{\n"

    # Add each filename to the array
    for file_name in files:
        cpp_code += f'  "{file_name}",\n'

    # Complete the C++ code
    cpp_code += "};"

    # Save the generated code to a .cpp file in the specified output directory
    output_file_path = os.path.join(output_dir, 'file_names.cpp')
    with open(output_file_path, 'w') as output_file:
        output_file.write(cpp_code)

    print(f"C++ array code saved to {output_file_path}")


# Specify the directory containing data files, the desired array name, and the output directory
data_directory = ".\Main\data"
array_name = "verificationFilenames"
output_directory = ".\Main"

# Generate the C++ array code and save it to the specified output directory
generate_cpp_array(data_directory, array_name, output_directory)
