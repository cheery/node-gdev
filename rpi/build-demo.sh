CFLAGS="-I/opt/vc/include/"

gcc $CFLAGS demo.c /home/pi/node-video/rpi/libdisplay.so /home/pi/node-video/rpi/libvideo.so -o demo
