#!/bin/bash
if [ "$1" == "up" ]; then
    git pull origin master
fi
nohup python3 main.py > /dev/null &
echo "$!" > process.pid
sleep 0.5