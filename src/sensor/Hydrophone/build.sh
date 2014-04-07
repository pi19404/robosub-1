#! /bin/bash
gcc -Wall record.c -I/usr/include/alsa -lasound -lsndfile -o record.exe
