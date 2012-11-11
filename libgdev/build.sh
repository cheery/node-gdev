CFLAGS="-I./include/ -I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -DSTANDALONE -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS -DTARGET_POSIX -D_LINUX -fPIC -DPIC -D_REENTRANT -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -U_FORTIFY_SOURCE -Wall -g -DHAVE_LIBOPENMAX=2 -DOMX -DOMX_SKIP64BIT -ftree-vectorize -pipe -DUSE_EXTERNAL_OMX -DHAVE_LIBBCM_HOST -DUSE_EXTERNAL_LIBBCM_HOST -DUSE_VCHIQ_ARM -Wno-psabi -DEGL_EGLEXT_PROTOTYPES -DGL_GLEXT_PROTOTYPES"
LDFLAGS="-L/opt/vc/lib/ -shared"
CC=gcc

$CC $CFLAGS $LDFLAGS src/*.c -o libgdev.so -lEGL -lGLESv2 -lopenmaxil -lbcm_host -lvcos -lvchiq_arm

CFLAGS="-I./include/ -I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads"
$CC $CFLAGS -g demo.c -o demo libgdev.so

# for temporary testing
#LD_LIBRARY_PATH=`pwd` gdb ./demo
