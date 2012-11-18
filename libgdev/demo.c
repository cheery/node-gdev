#include <error.h>
#include <gdev.h>
#include <GLES2/gl2.h>
#include <unistd.h>

int main() {
    int kw[] = {
        GDEV_RED_SIZE,   8,
        GDEV_GREEN_SIZE, 8,
        GDEV_BLUE_SIZE,  8,
        GDEV_ALPHA_SIZE, 8,
        0
    };
    int e;

    GDevDisplay display;
    GDevLayer layer;
    GDevConfig config;
    GDevSurface surface;
    GDevContext context;

    e = gdevDisplayOpen(&display, 0);
    if (e != 0) error(1, 0, "demo:%i: 0x%04x", __LINE__, e);

    e = gdevConfigure(&config, kw);
    if (e != 0) error(1, 0, "demo:%i: 0x%04x", __LINE__, e);

    //e = gdevCreateLayer(&layer, 256, 256, config);
    e = gdevAddLayer(&layer, display, 0, 0, 256, 256, 0);
    if (e != 0) error(1, 0, "demo:%i: 0x%04x", __LINE__, e);

    e = gdevGetSurface(&surface, &layer, config);
    if (e != 0) error(1, 0, "demo:%i: 0x%04x", __LINE__, e);

    e = gdevCreateContext(&context, config, GDEV_OPENGL_ES_2);
    if (e != 0) error(1, 0, "demo:%i: 0x%04x", __LINE__, e);

    e = gdevMakeCurrent(surface, surface, context);
    if (e != 0) error(1, 0, "demo:%i: 0x%04x", __LINE__, e);

    glViewport(0, 0, 256, 256);
    glClearColor(0.15f, 0.25f, 0.35f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT );

    e = gdevSwapBuffers(surface);
    if (e != 0) error(1, 0, "demo:%i: 0x%04x", __LINE__, e);

    usleep(500 * 1000);

    e = gdevDiscardSurface(surface);
    if (e != 0) error(1, 0, "demo:%i: 0x%04x", __LINE__, e);

    /*
    unsigned int config[GDEV_CONFIG_HI];
    config[GDEV_CONFIG_LENGTH] = GDEV_CONFIG_HI;
    config[GDEV_CONFIG_API] = GDEV_API_GLESv2;

    GDEVDisplay primary = gdevDisplayOpen(0);

    int width, height;
    gdevDisplayGetSize(primary, &width, &height);

    char handle[32];
    gdevDisplayAddLayer(primary, 0, 0, width, height, 0, handle);
    GDEVContext ctx = gdevCreateNative(handle, config);
    if (ctx == 0) { error(1, 0, "cannot create opengl context"); }

    char handle2[32];
    gdevDisplayAddLayer(primary, width/4, height/4, width/2, height/2, 1, handle);
    GDEVContext ctx2 = gdevCreateNative(handle, config);
    if (ctx2 == 0) { error(1, 0, "cannot create opengl context"); }

    glViewport(0, 0, 256, 256);

    while(1) {
    gdevMakeCurrent(ctx);
    glClearColor(0.15f, 0.25f, 0.35f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT );
    gdevSwapBuffers(ctx);
    usleep(500 * 1000);

    gdevMakeCurrent(ctx2);
    glClearColor(0.35f, 0.15f, 0.85f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT );
    gdevSwapBuffers(ctx2);
    usleep(500 * 1000);

    gdevMakeCurrent(ctx);
    glClearColor(0.85f, 0.25f, 0.35f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT );
    gdevSwapBuffers(ctx);
    usleep(500 * 1000);

    gdevMakeCurrent(ctx2);
    glClearColor(0.9f, 0.8f, 0.45f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT );
    gdevSwapBuffers(ctx2);
    usleep(500 * 1000);
    }
    */

    return 0;
}
