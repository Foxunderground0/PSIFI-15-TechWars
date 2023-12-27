import os
import numpy as np
import matplotlib.pyplot as plt
from scipy.io import wavfile
from scipy.signal import spectrogram
from pydub import AudioSegment
import subprocess

# Function to convert Opus to WAV


def convert_opus_to_wav(opus_path, wav_path):
    # Use subprocess to call FFmpeg with different options
    command = [
        "ffmpeg",
        "-i", opus_path,
        # Set audio codec to PCM (16-bit, little-endian)
        "-acodec", "pcm_s16le",
        "-ar", "44100",          # Set audio sample rate to 44.1 kHz
        wav_path
    ]

    subprocess.run(command, check=True)

# Function to generate and save the spectrogram


def generate_spectrogram(file_path, output_path):
    # Convert Opus to WAV
    temp_wav_file = "./temp.wav"
    convert_opus_to_wav(file_path, temp_wav_file)

    # Read the audio file
    sample_rate, audio_data = wavfile.read(temp_wav_file)

    # Generate the spectrogram
    f, t, Sxx = spectrogram(audio_data, fs=sample_rate,
                            nperseg=16384, noverlap=8192, scaling='spectrum')

    # Renormalize to the range of 30 Hz to 200 Hz
    freq_mask = (f >= 30) & (f <= 200)
    Sxx = Sxx[freq_mask, :]

    # Create and save the spectrogram plot
    plt.figure(figsize=(10, 6))

    # Set the frequency range to display (30 Hz to 200 Hz)
    plt.pcolormesh(t, f[freq_mask], 10 * np.log10(Sxx + 1e-10),
                   shading='auto', cmap='gray_r', vmin=-50, vmax=50)
    plt.ylabel('Frequency (Hz)')
    plt.xlabel('Time (s)')
    plt.title(
        f'Spectrogram (30-200 Hz) - Inverse Grayscale - {os.path.basename(file_path)}')
    plt.colorbar(label='Power/Frequency (dB/Hz)')

    # Set y-axis limits to display only 30 Hz to 200 Hz
    plt.ylim([30, 200])

    plt.savefig(output_path)

    # Remove temporary WAV file
    os.remove(temp_wav_file)


# Output directory for saving spectrogram plots
output_directory = "./Spectrograms"
os.makedirs(output_directory, exist_ok=True)

# Loop through each file from 0.opus to 49.opus
for i in range(50):
    input_file = f"./Merged Audio (Music+Key)/{i}.opus"
    output_plot = f"{output_directory}/{i}_spectrogram_30-200Hz_inverse_gray.png"
    generate_spectrogram(input_file, output_plot)
