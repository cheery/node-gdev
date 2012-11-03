# video device library

Purpose of this library is to make access to graphics hardware really easy. Motivation is to get people experimenting with different wild desktop environments, improve conditions at the linux desktop and provide little bit more graphical freedom in small increments.

Beta version will support at least raspberry pi. We will implement C-API portability layer for other languages to implement same functionality.

This software depends on node.js and can be compiled directly on the target machine due to it's small size.

## try it out

Verify you are in debian wheezy on your raspberry pi and do exactly these steps. I haven't written proper build files yet, but I will do it soon once you'll have more reasons to try it out.

    cd /home/pi
    git clone git@github.com:cheery/node-video.git
    cd node-video/rpi/
    sh build-api.sh
    cd ../display
    node-waf configure build
    cd ..
    node-waf configure build
    ./node-video tutorial/tutorial-1.js

Tutorial-1 draws a triangle to the screen. You can also try demo.js, which just flips colors. The webgl API is mostly complete, but still lacks few vital functions. There will be more tutorials and examples, which are simultaneously being used to make sure everything works and is accessible like it should.

This project is still unstable. If you get this working and there's a bug, write an issue about it asap. I'll try to fix it within 24 hours so you can continue!

API of this project is still changing and incomplete. Keep this in mind and remember to inform author of any issues you might encounter.

## now

Now, you may open a context to a display by stating:

    var video = require('video');
    var display = require('video/display');
    var primary = display.open(0);

    var gl = video.getContext(primary);

    gl.clearColor(0.5, 0.5, 0.5, 0.5);
    gl.clear(gl.COLOR_BUFFER_BIT);
    gl.swapBuffers();

You should also be able to make composite surfaces like this:

    var video = require('video');
    var display = require('video/display');
    var primary = display.open(0);

    var gl0 = video.getContext(primary);
    var gl1 = video.getContext({width: 256, height: 256});

    // once tested, there will be an example about the feature.
    var tex1 = gl0.createTexture();
    gl0.bindTexture(gl0.TEXTURE2D, tex1);
    gl0.texImage2D(gl0.TEXTURE2D, gl1.handle);

Both display handles and composite handles are Buffer -objects, that can be transmitted over network connection.

Desktop compositing is supported in platform-dependent way now. We'll try to keep it as simple as possible, depending on how flexible our early testing machines will be.
