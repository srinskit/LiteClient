#!/bin/bash
pid=$(cat process.pid)
kill -s sigterm $pid
if [ $? -eq 0 ]; then
    echo Killed
fi