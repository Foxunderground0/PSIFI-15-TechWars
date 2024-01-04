import shutil
import sys
import os
import gzip


def copy_files(challenge_number):
    source_paths = [
        f'Challenges\\QR Code Pattern\\HiddenQRcodes\\hidden_qrcode_{challenge_number}.png',
        f'Challenges\\Audio Steganography\\Merged Audio (Music+Key)\\{challenge_number}.opus',
        f'Challenges\\Image Hex Dump\\Immages\\{challenge_number}.ppm'
    ]

    destination_paths = [
        'Main\\data\\qrcode.png',
        'Main\\data\\audio.opus',
        'Main\\data\\image.ppm'
    ]

    for source, destination in zip(source_paths, destination_paths):
        try:
            # Copy the file
            shutil.copyfile(source, destination)

            # Compress the destination file to .gz format
            with open(destination, 'rb') as f_in:
                with gzip.open(destination + '.gz', 'wb', compresslevel=9) as f_out:
                    shutil.copyfileobj(f_in, f_out)

            # Remove the original uncompressed file
            os.remove(destination)

            print(
                f"File '{source}' copied and compressed to '{destination}.gz' successfully.")
        except FileNotFoundError:
            print(f"Error: File '{source}' not found.")
        except Exception as e:
            print(f"Error: {e}")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python script.py <challenge_number>")
    else:
        try:
            challenge_number = int(sys.argv[1])
            copy_files(challenge_number)
        except ValueError:
            print("Error: Please provide a valid number as an argument.")
