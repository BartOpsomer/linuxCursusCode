#!/bin/bash

if[[! -f /sys/class/gpio/gpio67/value]]
then
	echo 67 > /sys/class/gpio/export
	echo in > /sys/class/gpio/gpio67/direction
fi

while IFS= read value
do
	if[["$value"==0]]
	then
		cat /proc/loadavg

	fi
done
