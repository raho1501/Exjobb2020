import cv2
import numpy as np

cap = cv2.VideoCapture("C:\\Users\\Rasmus\\desktop\\PSNR\\bbb_sunflower_1080p_60fps_normal.mp4")
if (cap.isOpened()== False): 
    print("Error opening video stream or file")
    #Read until video is completed
framecount = 0
for i in range(0,24*60):
    ret, frame = cap.read()
    if ret == True:
       #cv2.imshow('Frame',frame)
        #print(framecount)
        framecount += 1
        cv2.waitKey(0)
fourcc = cv2.VideoWriter_fourcc(*'XVID')
out = cv2.VideoWriter('C:\\Users\\Rasmus\\desktop\\PSNR\\output1080p.mp4', fourcc, 60.0, (1920,  1080))
print("letsago")
frames = []
for i in range(0,3*60):
    ret, frame = cap.read()
    out.write(frame)
    frames.append(frame)
print("done")
cap.release()
out.release()
cv2.destroyAllWindows()

out = cv2.VideoWriter('C:\\Users\\Rasmus\\desktop\\PSNR\\video1080p.mp4', fourcc, 60.0, (1920,  1080))

for i in range(0,30):
    for frame in frames:
        out.write(frame)

out.release()