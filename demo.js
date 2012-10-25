var video = require('./build/Release/video');
var display = new video.Display(0);

console.log(display);

console.log('process sleeps 2 seconds now');
setTimeout(function(){
    display.close();
}, 2000);

