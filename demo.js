var display = require('./display/build/Release/display');
console.log('display loaded');
var video = require('./build/Release/video');

console.log(video);
var primary = display.open(0);
console.log(display);

console.log({width: primary.width, height: primary.height, handle: primary.handle})
var handle = primary.handle;
console.log(handle);

console.log("handle length:" + handle.length);

console.log("create context");
var ctx = video.getContext(handle);
console.log("make context current");
ctx.makeCurrent();

console.log("all right, now, lets demonstrate!");
video.demonstrate(0.5, 0.5, 0.5, 0.5);
console.log("swap buffers to show good will.");
ctx.swapBuffers();

console.log('process sleeps 2 seconds now');
setTimeout(function(){
    primary.close();
}, 2000);
