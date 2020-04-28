import cv2
import numpy as np
from threading import Thread

def PSNR(new_frame, ref_frame):
    mse = np.mean((new_frame - ref_frame)**2)
    if mse == 0:
        return 100 #they be equal if mse == 0
    max_intensity = 255.0
    psnr =  10 * np.log10( (max_intensity**2) / mse )
    return psnr


def extract_psnr_videosnippets(filename, comp_video):
    #each video is prerolled with black frames, first find out where they end.
    cap = cv2.VideoCapture(filename)
    ret, frame = cap.read()
    black_frame = np.zeros((1080,1920,3),dtype=np.uint8)
    preroll_count = 0
    while ((frame ^ black_frame).sum() == 0 ):
        preroll_count+=1
        ret,frame = cap.read()
    print("Preroll consisted of: ",preroll_count," frames")
    video_psnr_collection = []
    lc = 0
    while cap.isOpened() and ret: #while video is available
        video = []
        while ret and (frame ^ black_frame).sum() != 0: #save video while input is not black screen
            video.append(frame)
            ret, frame = cap.read()
        psnr_res = get_psnr_from_vid(comp_video ,np.array(video),filename)
        print(psnr_res)
        video_psnr_collection.append(psnr_res)
        ret, frame = cap.read() #remove the black frame
        video.clear()
        lc+=1
        print(lc)
    return video_psnr_collection

def get_psnr_from_vid(ref, new_video, video_name):
    psnr_for_video = []
    num_frames = 0
    psnr_for_each_frame = []
    f = open("results/video_errors.txt",'a')
    if len(ref) != len(new_video):
        print("loss occured in ",video_name)
        print(len(new_video)/len(ref))
        if len(ref) > len(new_video):
            num_frames=len(new_video) 
        else:
            num_frames=len(ref)
    else:
        num_frames=len(ref)
    f.write(video_name + ": "+str(len(new_video)/len(ref))+"\n")
    for i in range(num_frames):
        psnr_for_each_frame.append(PSNR(new_video[i], ref[i]))
    psnr_for_video.append(psnr_for_each_frame)
    psnr_for_video = np.array(psnr_for_video)
    f.close()
    return psnr_for_video

def main():
    cap = cv2.VideoCapture("reference_videoslow-h264-1080p.mp4")
    ref_video=[]

    while(cap.isOpened()):
        ret, frame = cap.read()
        if ret == True:
            ref_video.append(frame)
            print("appended")
            cv2.waitKey(0)
        else:
            break
    for i in range(1,12):
        video = extract_psnr_videosnippets("4G/4gp"+str(i)+".mp4",ref_video)
        with open("results/4gp"+str(i)+"_psnr_result.csv", 'w') as outfile:
            for line in video:
                np.savetxt(outfile,line,fmt='%-7.4f',delimiter=',')
                #outfile.write('\n')
            
        print("Done with file nr ",i)
    print("all done with 4G...")
    for i in range(1,12):
        video = extract_psnr_videosnippets("5G/5gp"+str(i)+".mp4",ref_video)
        with open("results/5gp"+str(i)+"_psnr_result.csv", 'w') as outfile:
            for line in video:
                np.savetxt(outfile,line,fmt='%-7.4f',delimiter=',')
                #outfile.write('\n')
            
        print("Done with file nr ",i)
    print("all done with 5G...")
    print("finished")
    
if __name__ =='__main__':
    main()