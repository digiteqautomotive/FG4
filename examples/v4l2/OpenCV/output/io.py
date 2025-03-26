#!/usr/bin/python3

import os
import sys
import cv2
import fcntl
from v4l2 import (
    v4l2_format, v4l2_fourcc, VIDIOC_G_FMT, V4L2_BUF_TYPE_VIDEO_OUTPUT,
    V4L2_FIELD_NONE, VIDIOC_S_FMT
)

VIDEO_IN = "/dev/video0"
VIDEO_OUT = "/dev/video2"

def main():

    # input device
    cam = cv2.VideoCapture(VIDEO_IN, cv2.CAP_V4L2)
    if not cam.isOpened():
        print("ERROR: could not open camera!")
        return -1
    width = int(cam.get(cv2.CAP_PROP_FRAME_WIDTH))
    height = int(cam.get(cv2.CAP_PROP_FRAME_HEIGHT))

    # output device
    try:
        output = os.open(VIDEO_OUT, os.O_RDWR)
    except Exception as ex:
        print("ERROR: could not open output device!")
        print(str(ex))
        return -1

    vid_format = v4l2_format()
    vid_format.type = V4L2_BUF_TYPE_VIDEO_OUTPUT
    if fcntl.ioctl(output, VIDIOC_G_FMT, vid_format) < 0:
        print("ERROR: unable to get video format!")
        return -1

    framesize = width * height * 3
    vid_format.fmt.pix.width = width
    vid_format.fmt.pix.height = height
    vid_format.fmt.pix.pixelformat = v4l2_fourcc('A', 'R', '2', '4')
    vid_format.fmt.pix.sizeimage = framesize
    vid_format.fmt.pix.field = V4L2_FIELD_NONE

    if fcntl.ioctl(output, VIDIOC_S_FMT, vid_format) < 0:
        print("ERROR: unable to set video format!")
        return -1

    # main loop
    while True:
        # grab frame
        if not cam.grab():
            print("ERROR: could not read from camera!")
            break
        frame = cam.retrieve()[1]

        # Add text overlay
        font = cv2.FONT_HERSHEY_SIMPLEX
        cv2.putText(frame, 'TOP SECRET', (60, int(height/2) + 60), font, 6, (0, 0, 255), 16, cv2.LINE_4)

        # Convert to the required output format
        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2BGRA)

        # write frame to output device
        written = os.write(output, frame.data)
        if written < 0:
            print("ERROR: could not write to output device!")
            os.close(output)
            break

    return 0

if __name__ == "__main__":
    sys.exit(main())
