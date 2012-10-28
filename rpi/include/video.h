typedef struct surface* SURFACE_T;
#define NATIVEWINDOW_SIZE 3

SURFACE_T videoCreateNative(uint32_t* nativewindow);
void videoDestroySurface(SURFACE_T surface);
void videoMakeCurrent(SURFACE_T surface);
void videoSwapBuffers(SURFACE_T surface);
int videoIsCurrent(SURFACE_T surface);
