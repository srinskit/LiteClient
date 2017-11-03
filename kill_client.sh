#!/bin/bash
pid=$(cat process.pid)
kill -s sigterm $pid
echo Killed