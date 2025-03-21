#!/bin/bash

# Check if ImageMagick is installed
if ! command -v convert &> /dev/null; then
    echo "Error: ImageMagick is not installed. Please install it and try again."
    exit 1
fi

# Check for .ppm files in the current directory
shopt -s nullglob
ppm_files=(*.ppm)

if [ ${#ppm_files[@]} -eq 0 ]; then
    echo "No .ppm files found in the current directory."
    exit 1
fi

# Convert each .ppm file to .jpg
for input_file in "${ppm_files[@]}"; do
    output_file="${input_file%.ppm}.jpg"
    convert "$input_file" "$output_file"
    if [ $? -eq 0 ]; then
        echo "Converted: $input_file -> $output_file"
    else
        echo "Error converting: $input_file"
    fi
done

echo "Conversion completed."
