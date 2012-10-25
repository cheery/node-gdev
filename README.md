# video device library

Purpose of this library is to make access to graphics hardware really easy.

Beta version will support at least raspberry pi. We will implement C-API portability layer for other languages to implement same functionality.

## try it out

    node-waf configure build
    node demo.js

It draws a rectangle similar to `hello_dispmanx` -demo in raspberry pi's firmware repository.

## future

In future, you may open a context to a display simply by stating:

    var video = require('video');
    var dpy = video.openDisplay(0);

    var gl = dpy.getContext("webgl");

    gl.clearColor(0.5, 0.5, 0.5, 0.5);
    gl.clear(gl.COLOR_BUFFER_BIT);

In future, you may also pass the display handle to an another process with ease, details are still bit unclear but maybe it'll be something like this:

    var video = require('video');
    var dpy = video.openDisplay(0);
    
    client.on_fullscreen_request (response) ->
        response(dpy.handle);

Desktop compositing will be supported in some platform-independent way. We'll try to keep it as simple as possible, depending
how flexible our early testing machines will be.
