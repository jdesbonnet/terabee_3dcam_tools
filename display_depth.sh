#!/bin/bash
ffmpeg -f video4linux2 -input_format yuyv422 -i /dev/video1 -c:v copy -f image2pipe - \
 | ./terabee_3dcam -c 1 -f pgm \
 | ffplay -probesize 10000 -loglevel quiet  -f image2pipe -vf scale=800:-1 pipe:.pgm
