#!/bin/bash
for (( counter=30; counter>0; counter-- ))
do
    cp backup a
    { time ./ep3 < commands_rm ; } 2>> 830dir50Ocupados.txt
    printf "\n"
    rm a
done