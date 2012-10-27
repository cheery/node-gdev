console.log("loading library");
var display = require("./build/Release/display");
console.log("opening primary display");
var primary = display.open(0);
console.log("primary display opened");
console.log({
    width: primary.width,
    height: primary.height,
    handle: primary.handle,
});
console.log("closing primary display");
primary.close();
console.log("all good");
