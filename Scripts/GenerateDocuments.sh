#!/bin/bash
set -e

# Download doxygen
echo "Downloading doxygen..."
wget https://github.com/doxygen/doxygen/releases/download/Release_1_13_0/doxygen-1.13.0.linux.bin.tar.gz -O /tmp/doxygen.tar.gz

echo "Extracting doxygen..."
mkdir -p /tmp/doxygen
tar -xzf /tmp/doxygen.tar.gz -C /tmp/doxygen --strip-components=1

# add to PATH
export PATH=$PATH:/tmp/doxygen

# Fetch parent folder.
parent_dir=$(dirname "$(pwd)")

# Display parent directory for debugging
echo "Parent directory: $parent_dir"

# Initialize doxyfile_path
doxyfile_path=""

# Display doxyfile path for debugging
echo "Doxyfile path: $doxyfile_path"

# List files in the parent directory for debugging
ls -la "$parent_dir"

# Check if doxyfile exists in the parent directory
if [ -f "$parent_dir/Doxyfile" ]; then

    doxyfile_path="$parent_dir/Doxyfile"
    
    # Back to parent folder
    cd "$parent_dir"
        
else

    # Check if doxyfile exists in the current directory
    if [ -f "Doxyfile" ]; then
    
        doxyfile_path=$(pwd)/Doxyfile
        
    else
    
        echo "Doxyfile not found."
        exit 1
        
    fi
fi

# Call Python script to build solution
doxygen "$doxyfile_path"