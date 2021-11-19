#!/bin/sh
PID=$(pidof ep2)
pmap -x $PID > output