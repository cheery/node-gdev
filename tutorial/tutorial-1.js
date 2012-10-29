var display = require('../display/build/Release/display');
var video = require('../build/Release/video');
var fs = require("fs");
var primary = display.open(0);
var gl = video.getContext(primary.handle);
console.log("retrieved a context, lets initialize");

var shader = {
    fragment: fs.readFileSync(__dirname + "/tutorial-1.fragmentshader", "ascii"),
    vertex: fs.readFileSync(__dirname + "/tutorial-1.vertexshader", "ascii"),
};

function compile_shader(type, source) {
    var shader = gl.createShader(type);
    gl.shaderSource(shader, source);
    gl.compileShader(shader);
    if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
        throw gl.getShaderInfoLog(shader);
    }
    return shader;
}

console.log("compiling");
var program = gl.createProgram();
var frag = compile_shader(gl.FRAGMENT_SHADER, shader.fragment);
var vert = compile_shader(gl.VERTEX_SHADER, shader.vertex);

console.log("linking");
gl.attachShader(program, frag);
gl.attachShader(program, vert);
gl.linkProgram(program);

if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
    throw gl.getProgramInfoLog(program);
}

console.log("build an array buffer");
var tB = gl.createBuffer();
var vertices = [
     0.0,  1.0,  0.0,    1.0, 0.0, 0.0,
    -1.0, -1.0,  0.0,    0.0, 1.0, 0.0,
     1.0, -1.0,  0.0,    0.0, 0.0, 1.0
];

console.log("set viewport");
gl.viewport(0, 0, primary.width, primary.height);

console.log("enableVertexAttribArray");
var aP = gl.getAttribLocation(program, "position");
var aC = gl.getAttribLocation(program, "color");
if (aP == -1) { throw "position attribute not found"; }
if (aC == -1) { throw "color attribute not found"; }
console.log("position"+aP);
console.log("color"+aC);

console.log("set vertex pointers");
gl.bindBuffer(gl.ARRAY_BUFFER, tB);
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);
gl.vertexAttribPointer(aP, 3, gl.FLOAT, false, 6*4,   0);
gl.vertexAttribPointer(aC, 3, gl.FLOAT, false, 6*4, 3*4);
gl.enableVertexAttribArray(aP);
gl.enableVertexAttribArray(aC);

console.log("clear screen");
gl.clearColor(0.2, 0.3, 0.2, 0.8);
gl.clear(gl.COLOR_BUFFER_BIT);


console.log("using");
gl.useProgram(program);

console.log("drawing");
gl.drawArrays(gl.TRIANGLES, 0, 3);

console.log("show results");
gl.swapBuffers();

console.log("sleep 2 seconds");
setTimeout(function(){
}, 2000);
