from PIL import Image
import os


def process_images(input_folder, output_folder, size=512):
    # Ensure output folder exists
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)

    # List all files in the input folder
    files = os.listdir(input_folder)

    for file in files:
        # Check if the file is an image
        if file.lower().endswith(('.png', '.jpg', '.jpeg', '.webp')):
            input_path = os.path.join(input_folder, file)
            output_path = os.path.join(
                output_folder, f"{os.path.splitext(file)[0]}.ppm")

            # Open the image
            img = Image.open(input_path)

            # Resize to 512 in the smallest dimension
            aspect_ratio = img.width / img.height
            if img.width < img.height:
                new_width = size
                new_height = int(size / aspect_ratio)
            else:
                new_width = int(size * aspect_ratio)
                new_height = size

            img = img.resize((new_width, new_height), Image.BICUBIC)

            # Crop to 512x512 in the center
            left = (img.width - size) // 2
            top = (img.height - size) // 2
            right = (img.width + size) // 2
            bottom = (img.height + size) // 2

            img = img.crop((left, top, right, bottom))

            # Save the processed image in PPM format
            img.save(output_path, 'PPM')

            print(f"Processed: {file}")


if __name__ == "__main__":
    input_folder = "Raw Immages/"
    output_folder = "Immages/"
    process_images(input_folder, output_folder)
