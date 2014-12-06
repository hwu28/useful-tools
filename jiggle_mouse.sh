#!/bin/bash

while :; do 
	xdotool mousemove_relative -- -1 0;
	sleep 2;
	xdotool mousemove_relative -- 1 0;
	sleep 2;
done
