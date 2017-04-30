
var gl;
var canvas;
var shaderProgram;
var vertexPositionBuffer;

// Create a place to store terrain geometry
var tVertexPositionBuffer;

//Create a place to store normals for shading
var tVertexNormalBuffer;

// Create a place to store the terrain triangles
var tIndexTriBuffer;

//Create a place to store the traingle edges
var tIndexEdgeBuffer;

var vertexColorBuffer;

// View parameters
var eyePt = vec3.fromValues(0.0,5.0,10.0);
var viewDir = vec3.fromValues(0.0,0.0,-1.0);
var up = vec3.fromValues(0.0,1.0,0.0);
var viewPt = vec3.fromValues(0.0,0.0,0.0);

// Create the normal
var nMatrix = mat3.create();

// Create ModelView matrix
var mvMatrix = mat4.create();

//Create Projection matrix
var pMatrix = mat4.create();

var mvMatrixStack = [];

var on = 1;
var started = 1;
var last = 0;


var vTerrain=[];
var fTerrain=[];
var nTerrain=[];
var eTerrain=[];
var colors=[];
var gridN=128;

var axis = [0, -1, 1];
var rotateAngle = 0;
var rotateQuat = quat.fromValues(Math.sin(rotateAngle / 2) * axis, Math.cos(rotateAngle / 2));
var velocity = [0, -1, 1];

/**
  * Setup vertices position, color, normal, etc. into the buffer
  */
function setupTerrainBuffers() {

    vTerrain=[];
    fTerrain=[];
    nTerrain=[];
    eTerrain=[];
    gridN=128;

    var numT = terrainFromIteration(gridN, -15,15,-15,15, vTerrain, fTerrain, nTerrain);
    console.log("Generated ", numT, " triangles");
    tVertexPositionBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, tVertexPositionBuffer);
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vTerrain), gl.STATIC_DRAW);
    tVertexPositionBuffer.itemSize = 3;
    tVertexPositionBuffer.numItems = (gridN+1)*(gridN+1);

    // Specify normals to be able to do lighting calculations
    tVertexNormalBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, tVertexNormalBuffer);
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(nTerrain),
                  gl.STATIC_DRAW);
    tVertexNormalBuffer.itemSize = 3;
    tVertexNormalBuffer.numItems = (gridN+1)*(gridN+1);

    // Specify faces of the terrain
    tIndexTriBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, tIndexTriBuffer);
    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(fTerrain),
                  gl.STATIC_DRAW);
    tIndexTriBuffer.itemSize = 1;
    tIndexTriBuffer.numItems = numT*3;

    //Setup Edges
     generateLinesFromIndexedTriangles(fTerrain,eTerrain);
     tIndexEdgeBuffer = gl.createBuffer();
     gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, tIndexEdgeBuffer);
     gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(eTerrain),
                  gl.STATIC_DRAW);
     tIndexEdgeBuffer.itemSize = 1;
     tIndexEdgeBuffer.numItems = eTerrain.length;

     //Setup colors
     setupColorBuffer(colors);
     vertexColorBuffer = gl.createBuffer();
     gl.bindBuffer(gl.ARRAY_BUFFER, vertexColorBuffer);
     gl.bufferData(gl.ARRAY_BUFFER, new Uint16Array(colors), gl.STATIC_DRAW);
     vertexColorBuffer.itemSize = 4;
     vertexColorBuffer.numItems = (gridN+1)*(gridN+1);
}

/**
 * Determine how to draw the terrain based on the options in HTML
 */
function drawTerrain(){
 gl.polygonOffset(0,0);
 gl.bindBuffer(gl.ARRAY_BUFFER, tVertexPositionBuffer);
 gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute, tVertexPositionBuffer.itemSize,
                         gl.FLOAT, false, 0, 0);

 // Bind normal buffer
 gl.bindBuffer(gl.ARRAY_BUFFER, tVertexNormalBuffer);
 gl.vertexAttribPointer(shaderProgram.vertexNormalAttribute,
                           tVertexNormalBuffer.itemSize,
                           gl.FLOAT, false, 0, 0);

 //Draw
 gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, tIndexTriBuffer);
 gl.drawElements(gl.TRIANGLES, tIndexTriBuffer.numItems, gl.UNSIGNED_SHORT,0);
}

/**
 * Draw edges of each triangle
 */
function drawTerrainEdges(){
 gl.polygonOffset(1,1);
 gl.bindBuffer(gl.ARRAY_BUFFER, tVertexPositionBuffer);
 gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute, tVertexPositionBuffer.itemSize,
                         gl.FLOAT, false, 0, 0);

 // Bind normal buffer
 gl.bindBuffer(gl.ARRAY_BUFFER, tVertexNormalBuffer);
 gl.vertexAttribPointer(shaderProgram.vertexNormalAttribute,
                           tVertexNormalBuffer.itemSize,
                           gl.FLOAT, false, 0, 0);

 //Draw
 gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, tIndexEdgeBuffer);
 gl.drawElements(gl.LINES, tIndexEdgeBuffer.numItems, gl.UNSIGNED_SHORT,0);
}

//-------------------------------------------------------------------------
function uploadModelViewMatrixToShader() {
  gl.uniformMatrix4fv(shaderProgram.mvMatrixUniform, false, mvMatrix);
}

//-------------------------------------------------------------------------
function uploadProjectionMatrixToShader() {
  gl.uniformMatrix4fv(shaderProgram.pMatrixUniform,
                      false, pMatrix);
}

//-------------------------------------------------------------------------
function uploadNormalMatrixToShader() {
  mat3.fromMat4(nMatrix,mvMatrix);
  mat3.transpose(nMatrix,nMatrix);
  mat3.invert(nMatrix,nMatrix);
  gl.uniformMatrix3fv(shaderProgram.nMatrixUniform, false, nMatrix);
}

//----------------------------------------------------------------------------------
function mvPushMatrix() {
    var copy = mat4.clone(mvMatrix);
    mvMatrixStack.push(copy);
}


//----------------------------------------------------------------------------------
function mvPopMatrix() {
    if (mvMatrixStack.length == 0) {
      throw "Invalid popMatrix!";
    }
    mvMatrix = mvMatrixStack.pop();
}

//----------------------------------------------------------------------------------
function setMatrixUniforms() {
    uploadModelViewMatrixToShader();
    uploadNormalMatrixToShader();
    uploadProjectionMatrixToShader();
}

//----------------------------------------------------------------------------------
function degToRad(degrees) {
        return degrees * Math.PI / 180;
}

//----------------------------------------------------------------------------------
function createGLContext(canvas) {
  var names = ["webgl", "experimental-webgl"];
  var context = null;
  for (var i=0; i < names.length; i++) {
    try {
      context = canvas.getContext(names[i]);
    } catch(e) {}
    if (context) {
      break;
    }
  }
  if (context) {
    context.viewportWidth = canvas.width;
    context.viewportHeight = canvas.height;
  } else {
    alert("Failed to create WebGL context!");
  }
  return context;
}

//----------------------------------------------------------------------------------
function loadShaderFromDOM(id) {
  var shaderScript = document.getElementById(id);

  // If we don't find an element with the specified id
  // we do an early exit
  if (!shaderScript) {
    return null;
  }

  // Loop through the children for the found DOM element and
  // build up the shader source code as a string
  var shaderSource = "";
  var currentChild = shaderScript.firstChild;
  while (currentChild) {
    if (currentChild.nodeType == 3) { // 3 corresponds to TEXT_NODE
      shaderSource += currentChild.textContent;
    }
    currentChild = currentChild.nextSibling;
  }

  var shader;
  if (shaderScript.type == "x-shader/x-fragment") {
    shader = gl.createShader(gl.FRAGMENT_SHADER);
  } else if (shaderScript.type == "x-shader/x-vertex") {
    shader = gl.createShader(gl.VERTEX_SHADER);
  } else {
    return null;
  }

  gl.shaderSource(shader, shaderSource);
  gl.compileShader(shader);

  if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
    alert(gl.getShaderInfoLog(shader));
    return null;
  }
  return shader;
}

//----------------------------------------------------------------------------------
function setupShaders() {
  vertexShader = loadShaderFromDOM("shader-vs");
  fragmentShader = loadShaderFromDOM("shader-fs");

  shaderProgram = gl.createProgram();
  gl.attachShader(shaderProgram, vertexShader);
  gl.attachShader(shaderProgram, fragmentShader);
  gl.linkProgram(shaderProgram);

  if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
    alert("Failed to setup shaders");
  }

  gl.useProgram(shaderProgram);

  shaderProgram.vertexPositionAttribute = gl.getAttribLocation(shaderProgram, "aVertexPosition");
  gl.enableVertexAttribArray(shaderProgram.vertexPositionAttribute);

  shaderProgram.vertexNormalAttribute = gl.getAttribLocation(shaderProgram, "aVertexNormal");
  gl.enableVertexAttribArray(shaderProgram.vertexNormalAttribute);

  shaderProgram.vertexColorAttribute = gl.getAttribLocation(shaderProgram, "aVertexColor");
  //gl.enableVertexAttribArray(shaderProgram.vertexColorAttribute);

  shaderProgram.mvMatrixUniform = gl.getUniformLocation(shaderProgram, "uMVMatrix");
  shaderProgram.pMatrixUniform = gl.getUniformLocation(shaderProgram, "uPMatrix");
  shaderProgram.nMatrixUniform = gl.getUniformLocation(shaderProgram, "uNMatrix");
  shaderProgram.uniformLightPositionLoc = gl.getUniformLocation(shaderProgram, "uLightPosition");
  shaderProgram.uniformAmbientLightColorLoc = gl.getUniformLocation(shaderProgram, "uAmbientLightColor");
  shaderProgram.uniformDiffuseLightColorLoc = gl.getUniformLocation(shaderProgram, "uDiffuseLightColor");
  shaderProgram.uniformSpecularLightColorLoc = gl.getUniformLocation(shaderProgram, "uSpecularLightColor");
}


//-------------------------------------------------------------------------
function uploadLightsToShader(loc,a,d,s) {
  gl.uniform3fv(shaderProgram.uniformLightPositionLoc, loc);
  gl.uniform3fv(shaderProgram.uniformAmbientLightColorLoc, a);
  gl.uniform3fv(shaderProgram.uniformDiffuseLightColorLoc, d);
  gl.uniform3fv(shaderProgram.uniformSpecularLightColorLoc, s);
}

//----------------------------------------------------------------------------------
function setupBuffers() {
    setupTerrainBuffers();
}

//----------------------------------------------------------------------------------
function draw() {
    var transformVec = vec3.create();

    gl.viewport(0, 0, gl.viewportWidth, gl.viewportHeight);
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    // We'll use perspective
    mat4.perspective(pMatrix,degToRad(45), gl.viewportWidth / gl.viewportHeight, 0.1, 200.0);

    // We want to look down -z, so create a lookat point in that direction
    vec3.add(viewPt, eyePt, viewDir);
    // Then generate the lookat matrix and initialize the MV matrix to that view
    mat4.lookAt(mvMatrix,eyePt,viewPt,up);
    //Draw Terrain
    mvPushMatrix();
    vec3.set(transformVec,0.0,-0.25,-3.0);
    mat4.translate(mvMatrix, mvMatrix,transformVec);
    mat4.rotateX(mvMatrix, mvMatrix, degToRad(-75));
    mat4.rotateZ(mvMatrix, mvMatrix, degToRad(25));
    setMatrixUniforms();

    if ((document.getElementById("polygon").checked) || (document.getElementById("wirepoly").checked))
    {
      uploadLightsToShader([0,1,1],[0.5,0.5,0.5],[0.1,0.1,0.1],[1.0,1.0,1.0]);
      drawTerrain();
    }

    if(document.getElementById("wirepoly").checked){
      uploadLightsToShader([0,1,1],[0.2,0.2,0.2],[0.0,0.0,0.0],[1.0,1.0,1.0]);
      drawTerrainEdges();
    }

    if(document.getElementById("wireframe").checked){
      uploadLightsToShader([0,1,1],[1.0,1.0,1.0],[0.0,0.0,0.0],[1.0,1.0,1.0]);
      drawTerrainEdges();
    }
    mvPopMatrix();

    vec3.add(eyePt, eyePt, [on * 0.01 * viewDir[0], on * 0.01 * viewDir[1], on * 0.01 * viewDir[2]])
}

//----------------------------------------------------------------------------------
function animate() {
  last += 0.1;
}

//----------------------------------------------------------------------------------
function startup() {
  handle_key();
  canvas = document.getElementById("myGLCanvas");
  gl = createGLContext(canvas);
  setupShaders();
  setupBuffers();
  gl.clearColor(0.0, 0.0, 0.0, 1.0);
  gl.enable(gl.DEPTH_TEST);
  tick();
}

//----------------------------------------------------------------------------------
function tick() {

}

//-------------------------------------------------------------------------
/**
  * Responce to the mouse click event on "Start" button to start the simulation
  */
function start_simulate()
{
  if (started == 1) {
    return;
  }
  started = 1;
  on = 1;
  document.getElementById('bt2').innerHTML = "Pause";
}

/**
  * Responce to the mouse click event on "Pause" button to pause or resume the simulation
  */
function pause_resume()
{
  if (started == 0) {
    console.log("on = " + on);
    return;
  } else {
    if (on == 1) {
      on = 0;
      document.getElementById('bt2').innerHTML = "Resume";
    } else {
      on = 1;
      document.getElementById('bt2').innerHTML = "Pause";
    }
  }
  console.log("on = " + on);
}

var myQuat = quat.create();
var rollLeftQuat = quat.fromValues(Math.sin(degToRad(1)) * 0, Math.sin(degToRad(1)) * 0, Math.sin(degToRad(1)) * 1, Math.cos(degToRad(1)));
var rollRightQuat = quat.fromValues(Math.sin(degToRad(-1)) * 0, Math.sin(degToRad(-1)) * 0, Math.sin(degToRad(-1)) * 1, Math.cos(degToRad(-1)));
var pitchUpQuat = quat.fromValues(Math.sin(degToRad(1)) * 1, Math.sin(degToRad(1)) * 0, Math.sin(degToRad(1)) * 0, Math.cos(degToRad(1)));
var pitchDownQuat = quat.fromValues(Math.sin(degToRad(-1)) * 1, Math.sin(degToRad(-1)) * 0, Math.sin(degToRad(-1)) * 0, Math.cos(degToRad(-1)));

/**
  * Handle keyboard input
  */
function handle_key() {
    document.addEventListener('keydown', function(event){
      event.preventDefault();
      switch (event.keyCode) {
        case 37:
          //quat.multiply(myQuat, myQuat, rollLeftQuat);
          //vec3.rotateY(up, up, eyePt, degToRad(0.5));
          //vec3.normalize(up, up);
          //console.log(up);
          vec3.transformQuat(up, up, rollLeftQuat);
          break;
        case 38:
          //quat.multiply(myQuat, myQuat, pitchUpQuat);
          //vec3.rotateX(viewDir, viewDir, eyePt, degToRad(0.5));
          //vec3.rotateX(up, up, eyePt, degToRad(0.5));
          vec3.transformQuat(viewDir, viewDir, pitchUpQuat);
          break;
        case 39:
          //quat.multiply(myQuat, myQuat, rollRightQuat);
          //vec3.rotateY(up, up, eyePt, degToRad(-0.5));
          //vec3.normalize(up, up);
          //console.log(up);
          vec3.transformQuat(up, up, rollRightQuat);
          break;
        case 40:
          //quat.multiply(myQuat, myQuat, pitchDownQuat);
          //vec3.rotateX(viewDir, viewDir, eyePt, degToRad(-0.5));
          //vec3.rotateX(up, up, eyePt, degToRad(-0.5));
          vec3.transformQuat(viewDir, viewDir, pitchDownQuat);
          break;
        default:
      }
    });
}
