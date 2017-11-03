#!/bin/bash
if [ $# -lt 1 ]; then
    echo 'More args'
    exit
fi
if [ "$2" == "up" ]; then
    git pull origin master
fi
nohup python3 main.py "$1" > /dev/null &
echo "$!" > process.pid
sleep 0.5