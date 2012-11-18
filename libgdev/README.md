# Yet another crappy graphics context device library

Major purpose of this library is to abstract away some really hairy details. It does a bad job at abstracting away dispmanx and egl. A considerably better library would require better access to the graphics hardware and I can't get that from raspberry pi at the moment.

Thing I particularly don't understand is the necessity of EGLConfig everywhere. Why does my Context or EGLImage need to be limited for certain framebuffer configuration?

If I could, I'd have done something equivalent to this:

    /* display_id would be retrievable from udev.
     * Any layer could be attached to any display device or detached from it.
     */
    GetDisplay(display_id) -> display
    DiscardDisplay(display)
    AttachLayer(display, layer, x, y, width, height, z)
    DetachLayer(display, layer)

    /* Layers would be resizable handles.
     */
    CreateLayer(width, height) -> layer
    ResizeLayer(layer, width, height)
    DiscardLayer(layer)

    /* Layers could be shared between processes.
     */
    GetLayer(layer_id) -> layer
    LayerId(layer) -> layer_id
    LayerAccess(pid, flags) // flags: READ_FLAG | DRAW_FLAG

    /* Surfaces would use those handles and things drawn to them would go to the layer.
     */
    GetSurface(layer, attributes) -> surface
    DiscardSurface(surface)
    SwapBuffers(surface)

    /* For context it'd be enough to know which API is in use.
     */
    CreateContext(api) -> context
    MakeCurrent(draw, read, context)
    GetCurrent() -> (draw, read, context)
    DestroyContext(context)

    /* Layers could be used as textures.
     */
    GetImage(layer) -> image
    DiscardImage(image) -> image

Instead, now you have to pass config everywhere and everything keeps staying harder than it should be. Anyway I'll store the idea of what I were looking for as I might get chance to do it later.
