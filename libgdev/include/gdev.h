#define GDEV_SUCCESS 0

#define GDEV_LAYER_BYTES (128 - sizeof(int))

#define GDEV_GLOBAL_LAYER 1
#define GDEV_NATIVE_LAYER 2

typedef struct {
    int type;
    union {
        char  b[GDEV_LAYER_BYTES];
        int   d[GDEV_LAYER_BYTES/sizeof(int)];
        void* p[GDEV_LAYER_BYTES/sizeof(void*)];
    };
} GDevLayer;

typedef void* GDevDisplay;
typedef void* GDevConfig;
typedef void* GDevContext;
typedef void* GDevImage;
typedef struct gdev_surface* GDevSurface;

#define GDEV_OPENGL_ES_2   0x204
#define GDEV_OPENVG        0x205

#define GDEV_NO_CONFIG     0x3001
#define GDEV_NO_DISPLAY    0x3002
#define GDEV_NO_CONTEXT    0x3003
#define GDEV_NO_IMAGE      0x3004
#define GDEV_NO_SURFACE    0x3005

#define GDEV_UNKNOWN_API        0x4001
#define GDEV_API_BIND_FAIL      0x4002
#define GDEV_UPDATE_LAYERS_FAIL 0x4003

#define GDEV_BAD_IMAGE 0x5001

#define GDEV_UNKNOWN_LAYER 0x6001


#define GDEV_INVALID_ARGUMENTS   0x8001
#define GDEV_SYSTEM_ERROR        0x8002
#define GDEV_UNKNOWN_ERROR       0x8003
#define GDEV_BAD_ACCESS          0x8004
#define GDEV_BAD_ALLOC           0x8005
#define GDEV_BAD_ATTRIBUTE       0x8006
#define GDEV_BAD_CONFIG          0x8007
#define GDEV_BAD_CONTEXT         0x8008
#define GDEV_BAD_CURRENT_SURFACE 0x8009
#define GDEV_BAD_MATCH           0x800a
#define GDEV_BAD_NATIVE_PIXMAP   0x800b
#define GDEV_BAD_PARAMETER       0x800c
#define GDEV_BAD_SURFACE         0x800d

#define GDEV_CONTEXT_LOST 0x9000

#define GDEV_RED_SIZE   0x301
#define GDEV_GREEN_SIZE 0x302
#define GDEV_BLUE_SIZE  0x303
#define GDEV_ALPHA_SIZE 0x304

int gdevDisplayOpen(GDevDisplay* dpy, unsigned int id);
int gdevDisplayGetSize(GDevDisplay dpy, int* width, int* height);
int gdevDisplayClose(GDevDisplay dpy);
int gdevAddLayer(GDevLayer* hwd, GDevDisplay dpy, int x, int y, int width, int height, int layer);
int gdevDestroyLayer(GDevLayer* hwd);
int gdevCreateLayer(GDevLayer* hwd, int width, int height, GDevConfig cfg);
int gdevGetSurface(GDevSurface* p, GDevLayer* hwd, GDevConfig cfg);
int gdevDiscardSurface(GDevSurface f);
int gdevSwapBuffers(GDevSurface f);
int gdevConfigure(GDevConfig* cfg, int* kw);
int gdevCreateContext(GDevContext* ctx, GDevConfig cfg, int api);
int gdevMakeCurrent(GDevSurface draw, GDevSurface read, GDevContext ctx);
int gdevGetCurrent(GDevContext* ctx);
int gdevDestroyContext(GDevContext ctx);
int gdevGetImage(GDevImage* img, GDevLayer* hwd);
int gdevDiscardImage(GDevImage img);
