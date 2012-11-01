var display = require('./display/build/Release/display');
var video = require('./build/Release/video');
console.log("libraries loaded successfully");
var primary = display.open(0);
console.log({width: primary.width, height: primary.height, handle: primary.handle})
var gl = video.getContext(primary);
console.log("retrieved valid context, trying to draw");

function color_fill_loop() {
    gl.clearColor(Math.random(), Math.random(), Math.random(), Math.random());
    gl.clear(gl.COLOR_BUFFER_BIT);
    gl.swapBuffers();
}

setInterval(color_fill_loop, 60);
