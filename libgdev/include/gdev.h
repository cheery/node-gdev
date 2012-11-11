typedef struct gdevcontext* GDEVContext;

extern void gdevDestroy(GDEVContext ctx);
void gdevMakeCurrent(GDEVContext ctx);
void gdevSwapBuffers(GDEVContext ctx);
int gdevIsCurrent(GDEVContext ctx);
GDEVContext gdevCreateNative(const char handle[32], unsigned int *config);
GDEVContext gdevCreateGlobal(char handle[32], int width, int height, unsigned int *cfg);

// configuration fields
#define GDEV_CONFIG_LENGTH  0
#define GDEV_CONFIG_API     1


// some predefined values
#define GDEV_CONFIG_HI      2
#define GDEV_API_GLESv2     0x1001
#define GDEV_API_OPENVG     0x1002

// FIXME: this might not belong here.
#ifdef GL_GDEV_API
void glGDEVGlobalTargetTexture2D(GLenum target, char handle[32]);
#endif

// FIXME: grr...
typedef struct gdevdisplay* GDEVDisplay;
GDEVDisplay gdevDisplayOpen(unsigned int id);
void gdevDisplayGetSize(GDEVDisplay dpy, int* width, int* height);
void gdevDisplayClose(GDEVDisplay dpy);
void gdevDisplayAddLayer(GDEVDisplay dpy, int x, int y, int width, int height, int layer, char handle[32]);
void gdevDisplayRemoveLayer(GDEVDisplay dpy, char handle[32]);
