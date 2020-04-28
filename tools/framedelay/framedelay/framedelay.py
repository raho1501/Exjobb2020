import cv2
import numpy as np
from time import time_ns

cap = cv2.VideoCapture("C:\\Users\\Rasmus\\desktop\\netcat_mplayer_win.mp4")

if (cap.isOpened()== False): 
  print("Error opening video stream or file")
 
# Read until video is completed
framecount = 0
while(cap.isOpened()):
  # Capture frame-by-frame
  ret, frame = cap.read()
  if ret == True:
 
    # Display the resulting frame
    cv2.imshow('Frame',frame)
    print(framecount)
    framecount += 1
    cv2.waitKey(0)


      
 
  # Break the loop
  else: 
    break
print(framecount)
# When everything done, release the video capture object

cap.release()
 
# Closes all the frames
cv2.destroyAllWindows()
