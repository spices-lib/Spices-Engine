#!/bin/bash

# Fetch parent folder.
parent_dir=$(dirname "$(pwd)")

# Display parent directory for debugging
echo "Parent directory: $parent_dir"

# Display doxyfile path for debugging
echo "Doxyfile path: $doxyfile_path"

# List files in the parent directory for debugging
ls "$parent_dir"

# Check if doxyfile exists in the parent directory
if [ -f "$parent_dir/Doxyfile" ]; then

    doxyfile_path="$parent_dir/Doxyfile"
    
    doxygen="$parent_dir/vendor/doxygen/1.10.0/doxygen"
    
    # Back to parent folder
    cd "$parent_dir"
        
else

    # Check if doxyfile exists in the current directory
    if [ -f "Doxyfile" ]; then
    
        doxyfile_path="$(pwd)/Doxyfile"
        
        doxygen="$(pwd)/vendor/doxygen/1.10.0/doxygen"
        
    else
    
        echo "Doxyfile not found."
        read -p "Press [Enter] key to continue..."
        exit 1
        
    fi
fi

# Set doxygen executable
chmod +x "$doxygen"

# Display parent directory for debugging
echo "doxygen: $doxygen"

# Display doxyfile path for debugging
echo "Doxyfile path: $doxyfile_path"

# Display GLIBCXX version
strings /usr/lib64/libstdc++.so.6 | grep GLIBCXX

# Display GCC version
gcc -v

# Call Python script to build solution
"$doxygen" "$doxyfile_path"

#read -p "Press [Enter] key to continue..."
