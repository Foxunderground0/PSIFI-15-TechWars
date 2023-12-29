#!/bin/bash

# Set the source directory
src_directory="."

# Set the output directory
output_directory="./Merged Audio (Music+Key)"

# Create the output directory if it doesn't exist
mkdir -p "$output_directory"

# Change to the source directory
cd "$src_directory"

# Loop through each pair of files
for i in {0..49}; do
    input_music="Music audio/$i.wav"
    input_key="Keys Audio/$i.wav"
    output_file="$output_directory/$i.opus"

    # Run the ffmpeg command
    ffmpeg -i "$input_music" -i "$input_key" -filter_complex "[0]dynaudnorm[a];[a]highpass=f=160[b];[b][1]amix=inputs=2[aout]" -map "[aout]" -c:a libopus -b:a 25k -ar 24000 -ac 1 -t 60 "$output_file"
    
done

echo "Processing completed."
