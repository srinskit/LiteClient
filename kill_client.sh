#!/bin/bash
pid=$(cat process.pid)
kill $pid
echo Killed