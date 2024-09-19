#!/bin/bash

#Directories
SHADER_DIR="./resources/shaders"
OUTPUT_DIR="./bin/shaders"

COMPILER_PATH="/home/luci/Code/VULKAN_SDK/1.3.290.0/x86_64/bin/glslc"

mkdir -p "$OUTPUT_DIR"

#compile loop
for shader_file in "$SHADER_DIR"/*.frag "$SHADER_DIR"/*.vert; do
  if [[ -f "$shader_file" ]]; then
    shader_name=$(basename "$shader_file")
    output_file="$OUTPUT_DIR/${shader_name}.spv"

    echo "compiling shader: $shader_file"
    "$COMPILER_PATH" -o "$output_file" "$shader_file"

    if [ $? -ne 0 ]; then
      echo "ERROR  COMPILING $shader_file!!"
    else
      echo "$shader_file compiled successfully"
    fi
  fi
done

echo "shader compilation completed"