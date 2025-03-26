#!/bin/python3

import sys
import cv2

def main():

    vid = cv2.VideoCapture("/dev/video0", cv2.CAP_V4L2)
    if not (vid.isOpened()):
        sys.exit("Could not open video device")


    while(True):
        ret, frame = vid.read()

        cv2.imshow('frame', frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    vid.release()
    cv2.destroyAllWindows()

    return 0

if __name__ == "__main__":
    sys.exit(main())
