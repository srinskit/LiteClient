#!/bin/bash
if [ $# -lt 1 ]; then
    echo 'More args'
    exit
fi
if [ "$2" == "up" ]; then
    git pull origin master
fi
echo "$$" > process.pid
python3 main.py "$1"