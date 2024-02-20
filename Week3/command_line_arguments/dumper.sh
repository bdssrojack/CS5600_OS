#!/bin/bash

name="./arger-luo"
make name=$name
$name $@
status=$?
if [ $status -eq 0 ]; then
    echo "0"
else
    echo "-1"
fi