gst-launch-1.0 v4l2src !  video/x-raw,width=640,height=480 !  x264enc tune=zerolatency byte-stream=true  bitrate=3000 threads=3 !  h264parse config-interval=1 !  rtph264pay ! udpsink host=192.168.254.170 port=5000