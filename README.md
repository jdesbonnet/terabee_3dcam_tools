# terabee_3dcam_tools
Tools relating to Terabee 3Dcam 80x60 ToF Time-of-Flight camera

Copying raw data frames from 3Dcam:

```
ffmpeg -f video4linux2 -input_format yuyv422 -i /dev/video0 -c:v copy raw.avi
```

Extract frames:

```
ffmpeg -i myfile.avi -f image2 image-%05d.raw
```

Make a PGM (gray map):

```
./terabee_3dcam < ../image-0001.raw  > 3d.pgm
```

Question: how to flip between close range mode (0.2 - 1.2m) and standard mode (1-4m)?

