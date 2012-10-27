# video device library

Purpose of this library is to make access to graphics hardware really easy. Motivation is to get people experimenting with different wild desktop environments, improve conditions at the linux desktop and provide little bit more graphical freedom in small increments.

Beta version will support at least raspberry pi. We will implement C-API portability layer for other languages to implement same functionality.

## try it out

    node-waf configure build
    node demo.js

It draws a rectangle similar to `hello_dispmanx` -demo in raspberry pi's firmware repository.

## future

In future, you may open a context to a display simply by stating:

    var video = require('video');
    var display = require('video/display');
    var primary = display.open(0);

    var gl = dpy.getContext("webgl");

    gl.clearColor(0.5, 0.5, 0.5, 0.5);
    gl.clear(gl.COLOR_BUFFER_BIT);

In future, you may also pass the display handle to an another process with ease, details are still bit unclear but maybe it'll be something like this:

    var video = require('video');
    var display = require('video/display');
    var primary = display.open(0);
    
    client.on_fullscreen_request (response) ->
        response(primary.handle);

Desktop compositing will be supported in some platform-independent way. We'll try to keep it as simple as possible, depending
how flexible our early testing machines will be.
