#! /bin/bash

# relative path to shader sources
cd ../../source/shaders;
# relative path to glslc FROM shader sources
runGLSLC="../../tools/binaries/glslc";
# relative path to output dir FROM shader sources
outputPath="../../build/source/shaders";
mkdir -p "${outputPath}";

# lists of shader files, add or change members as necessary.
fragFiles=(*.frag);
vertFiles=(*.vert);

# fragment shader compile loop, do not change.
# if not using fragment shaders then comment loop and list out.
for frag in "${fragFiles}"; do
    echo "compiling ${frag} into SPIRV format";
    "${runGLSLC}" "${frag}" -o "${outputPath}/${frag}.spv"
done;

# vertex shader compile loop, do not change.
# if not using vertex shaders then comment loop and list out.
for vert in "${vertFiles}"; do
    echo "compiling ${vert} into SPIRV format";
    "${runGLSLC}" "${vert}" -o "${outputPath}/${vert}.spv"
done;

