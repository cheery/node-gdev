var display = require('../display/build/Release/display');
var video = require('../build/Release/video');
var fs = require('fs');
var primary = display.open(0);
var gl = video.getContext(primary);

var get = function(path) {
    return fs.readFileSync(__dirname + '/' + path, "ascii");
};

var ShaderProgram = function(options) {
    function compile(type, source) {
        var shader = gl.createShader(type);
        gl.shaderSource(shader, source);
        gl.compileShader(shader);
        if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
            throw gl.getShaderInfoLog(shader);
        }
        return shader;
    }

    this.program = gl.createProgram();
    this.fragment_shader = compile(gl.FRAGMENT_SHADER, options.fragment);
    this.vertex_shader   = compile(gl.VERTEX_SHADER,   options.vertex);

    gl.attachShader(this.program, this.fragment_shader);
    gl.attachShader(this.program, this.vertex_shader);
    gl.linkProgram(this.program);
    if (!gl.getProgramParameter(this.program, gl.LINK_STATUS)) {
        throw gl.getProgramInfoLog(this.program);
    }
};

ShaderProgram.prototype.use = function() {
    gl.useProgram(this.program);
};

ShaderProgram.prototype.attribLoc = function(name) {
    return gl.getAttribLocation(this.program, name);
};

ShaderProgram.prototype.loc = function(name) {
    return gl.getUniformLocation(this.program, name);
};

var texprg = new ShaderProgram({
    fragment: get("tutorial-2.fragmentshader"),
    vertex: get("tutorial-2.vertexshader")
});

var vertices = [
    -1.0,  1.0,  0.0,    1.0, 1.0, 1.0,   -1.0,  1.0,
     1.0,  1.0,  0.0,    1.0, 0.0, 0.0,    1.0,  1.0,
    -1.0, -1.0,  0.0,    0.0, 1.0, 0.0,   -1.0, -1.0,
     1.0, -1.0,  0.0,    0.0, 0.0, 1.0,    1.0, -1.0
];

var r = "\xFF\x00\x00\x80";
var b = "\x00\x00\xFF\x20";
var texture_data = new Buffer(8*8*4);
texture_data.write(
    r+b+r+b+r+b+r+b +
    b+r+b+r+b+r+b+r +
    r+b+r+b+r+b+r+b +
    b+r+b+r+b+r+b+r +
    r+b+r+b+r+b+r+b +
    b+r+b+r+b+r+b+r +
    r+b+r+b+r+b+r+b +
    b+r+b+r+b+r+b+r, 'binary'
);
var texture = gl.createTexture();
gl.activeTexture(gl.TEXTURE0);
gl.bindTexture(gl.TEXTURE_2D, texture);
gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, 8, 8, 0, gl.RGBA, gl.UNSIGNED_BYTE, texture_data);
gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);

var aP = texprg.attribLoc("position");
var aC = texprg.attribLoc("color");
var aT = texprg.attribLoc("texcoords");

var tB = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, tB);
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);
gl.vertexAttribPointer(aP, 3, gl.FLOAT, false, 8*4,   0);
gl.enableVertexAttribArray(aP);
if (aC != -1) {
    gl.vertexAttribPointer(aC, 3, gl.FLOAT, false, 8*4, 3*4);
    gl.enableVertexAttribArray(aC);
}
if (aT != -1) {
    gl.vertexAttribPointer(aT, 2, gl.FLOAT, false, 8*4, 6*4);
    gl.enableVertexAttribArray(aT);
}

gl.viewport(0, 0, primary.width, primary.height);

var draw = function() {
    var now = Date.now();
    texprg.use();
    var texloc = texprg.loc("texture");
    if (texloc != -1) gl.uniform1i(texloc, 0);
    var scloc = texprg.loc("sincos");
    if (scloc != -1) gl.uniform2f(scloc, Math.sin(now/1000), Math.cos(now/1000));

    gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);
    gl.swapBuffers();
};

draw();
setInterval(function() {
    draw();
}, 1000/60);
