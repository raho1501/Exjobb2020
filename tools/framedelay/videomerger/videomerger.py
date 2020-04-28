import cv2
import numpy as np

cap = cv2.VideoCapture("C:\\Users\\Rasmus\\desktop\\PSNR\\output1080p.mp4")
if (cap.isOpened()== False): 
    print("Error opening video stream or file")
    #Read until video is completed
frames = []
while(cap.isOpened()):
    # Capture frame-by-frame
    ret, frame = cap.read()
    if ret == True:
        frames.append(frame)
        print("appended")
        cv2.waitKey(0)
    else:
        break
cap.release()
print("input done")
fourcc = cv2.VideoWriter_fourcc(*'XVID')
out = cv2.VideoWriter('C:\\Users\\Rasmus\\desktop\\PSNR\\1080p_video_to_be_streamed.mp4', fourcc, 60.0, (1920,  1080))
blank_frame = np.zeros((1080,1920,3),dtype=np.uint8)

for i in range(0,60*15):
    out.write(blank_frame)
print("writing_video")
for i in range(0,30):
    for frame in frames:
        out.write(frame)
    out.write(blank_frame)
    print("loop done", str(i))

print("done")

out.release()