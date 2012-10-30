# video device library

Purpose of this library is to make access to graphics hardware really easy. Motivation is to get people experimenting with different wild desktop environments, improve conditions at the linux desktop and provide little bit more graphical freedom in small increments.

Beta version will support at least raspberry pi. We will implement C-API portability layer for other languages to implement same functionality.

## try it out

Verify you are in debian wheezy on your raspberry pi and do exactly these steps. I haven't written proper build files yet.

    cd /home/pi
    git clone git@github.com:cheery/node-video.git
    cd node-video/rpi/
    sh build-api.sh
    cd ../display
    node-waf configure build
    cd ..
    node-waf configure build
    ./node-video tutorial/tutorial-1.js

Tutorial-1 draws a triangle to the screen. You can also try demo.js, which just flips colors. The webgl API is still a stub, but features glClear and glClearColor already.

## future

In future, you may open a context to a display simply by stating:

    var video = require('video');
    var display = require('video/display');
    var primary = display.open(0);

    var gl = dpy.getContext("webgl");

    gl.clearColor(0.5, 0.5, 0.5, 0.5);
    gl.clear(gl.COLOR_BUFFER_BIT);
    gl.swapBuffers();

In future, you may also pass the display handle to an another process with ease, details are still bit unclear but maybe it'll be something like this:

    var video = require('video');
    var display = require('video/display');
    var primary = display.open(0);
    
    client.on_fullscreen_request (response) ->
        response(primary.handle);

Desktop compositing will be supported in some platform-independent way. We'll try to keep it as simple as possible, depending
how flexible our early testing machines will be.
