typedef struct display* DISPLAY_T;
#define NATIVEWINDOW_SIZE 3

DISPLAY_T displayOpen(uint32_t id);
void displayClose(DISPLAY_T p);
void displayGetSize(DISPLAY_T p, uint32_t* width, uint32_t* height);
void displayGetHandle(DISPLAY_T p, uint32_t* nativewindow);
