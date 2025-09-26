#!/bin/bash

if [ -z "$1" ]; then
  echo "Usage: $0 <PID>"
  exit 1
fi

PID=$1

OUTPUT=$(ps -la | awk -v pid="$PID" 'NR==1 || $2==pid')

if [ "$(echo "$OUTPUT" | wc -l)" -le 1 ]; then
  echo "no process found for $PID"
else
  echo "$OUTPUT"
fi
