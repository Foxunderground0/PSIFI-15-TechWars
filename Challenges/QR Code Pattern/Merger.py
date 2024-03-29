import os
import qrcode
import numpy as np
from PIL import Image


def generate_qrcode(data):
    qr = qrcode.QRCode(
        version=1,
        error_correction=qrcode.constants.ERROR_CORRECT_L,
        box_size=1,
        border=1,
    )
    qr.add_data(data)
    qr.make(fit=True)

    img = qr.make_image(fill_color="black", back_color="white")
    return img


def generate_noisy_image(size):
    # Generate a larger noise array
    noise_array = np.random.choice([0, 255], size=(size, size), p=[0.5, 0.5])
    # Convert to uint8 without lossy conversion
    noisy_image = Image.fromarray(noise_array.astype(np.uint8))
    return noisy_image


def hide_qrcode_in_image(qrcode_img, base_img, red_img):
    # Randomize the position within the image
    position = (
        np.random.randint(0, base_img.width - qrcode_img.width),
        np.random.randint(0, base_img.height - qrcode_img.height),
    )

    # Paste QR code on the noisy image
    base_img.paste(qrcode_img, position)

    # Paste QR code on the red image
    red_img.paste(qrcode_img, position)

    return base_img, red_img


def process_keys_folder(input_folder, output_folder, solution_folder):
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)

    if not os.path.exists(solution_folder):
        os.makedirs(solution_folder)

    for i in range(50):
        input_file_path = os.path.join(input_folder, f'key_{i}.txt')
        output_file_path = os.path.join(
            output_folder, f'hidden_qrcode_{i}.png')
        solution_file_path = os.path.join(
            solution_folder, f'solution_qrcode_{i}.png')

        if os.path.exists(input_file_path):
            with open(input_file_path, 'r') as file:
                data = file.read().strip()

            qrcode_img = generate_qrcode(data)
            noisy_image = generate_noisy_image(size=1000)  # Increased size
            red_image = Image.new(
                'RGB', (noisy_image.width, noisy_image.height), 'red')

            # Hide QR code in both images
            final_noisy_image, final_solution_image = hide_qrcode_in_image(
                qrcode_img, noisy_image, red_image)

            # Convert to black and white
            final_noisy_image = final_noisy_image.convert('1')
            final_solution_image = final_solution_image.convert('1')

            # Save as PNG without lossy compression
            final_noisy_image.save(
                output_file_path, format='PNG', compress_level=0)
            final_solution_image.save(
                solution_file_path, format='PNG', compress_level=0)

            print(f'Hidden QR Code image saved: {output_file_path}')
            print(f'Solution QR Code image saved: {solution_file_path}')
        else:
            print(f'File not found: {input_file_path}')


if __name__ == "__main__":
    input_folder = "./Keys"
    output_folder = "./HiddenQRcodes"
    solution_folder = "./Solutions"

    process_keys_folder(input_folder, output_folder, solution_folder)
