#!/bin/bash
set -e

# Update package list
echo "Updating package list..."
sudo apt-get update

# Install doxygen and graphviz
echo "Installing doxygen and graphviz..."
sudo apt-get install -y doxygen graphviz

echo "Installation complete."

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