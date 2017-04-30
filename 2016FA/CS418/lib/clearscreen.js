var VSHADER_SOURCE_SOURCE = 
    'void main() {\n' +
    'gl_Position = vec4(0.0,0.0,0.0,1.0);\n' +
    'gl_PointSize = 5.0;\n' +
    '}\n';

var FSHADER_SOURCE = 
    'void main() {\n' +
    'gl_FragColor = vec4(1.0, 0.5, 0.0, 1.0);\n' +
    '}\n';

function main() {
    var canvas = document.getElementById('example');
    var gl = getWebGLContext(canvas);
    if  (!gl) {
        console.log('Fail to get gl');
        return;
    }
    
    if (!initShaders(gl, VSHADER_SOURCE_SOURCE, FSHADER_SOURCE)) {
        console.log('Fail to init shaders.');
        return;
    }
    
    gl.clearColor(0.0,0.0,0.0,1.0);
    gl.clear(gl.COLOR_BUFFER_BIT);
    gl.drawArrays(gl.POINTS, 0, 1);
}