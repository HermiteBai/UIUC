var terranVertices = [];
var terranColors = [];
var terranNormals = [];

var heightMap = [];
var size = 0;

var on = 0;
var started = 0;

/**
  * Initiate terran generation environment, setup vertices positions and colors
  * @param scale of terran
  * @param size of each grid
  */
function initEnvi(scale, stepSize) {
  //console.log("scale: " + scale + ", " + "step: " + stepSize);
  size = scale;
  setHeightMap(scale);
  normalizeHeight();
  setTerranVetices(scale, stepSize);
}

/**
  * Setup height map
  * @param scale of terran
  */
function setHeightMap(scale) {
  for (var i = 0; i <= scale; i++) {
    var temp = new Array(scale + 1);
    for (var j = 0; j <= scale; j++) {
      temp[j] = 0;
    }
    heightMap.push(temp);
  }
  heightMap[0][0] = 10 * Math.random();
  heightMap[0][scale] = 10 * Math.random();
  heightMap[scale][0] = 10 * Math.random();
  heightMap[scale][scale] = 10 * Math.random();
  diamond(0, 0, scale, 0.01);
}

/**
  * Implementation of Diamond-Square Algorithm
  */
function diamond(x, y, length, mag) {
  if (length == 1) {
    return;
  }
  heightMap[x + length / 2][y + length / 2] = (heightMap[x][y]
                                        + heightMap[x + length][y]
                                        + heightMap[x][y + length]
                                        + heightMap[x + length][y + length]) / 4
                                        + myRandom(mag);
  square(x + length / 2, y + length / 2, length, mag);
}


function square(x, y, length, mag) {
  if (length == 1) {
    return;
  }
  heightMap[x - length / 2][y] = average(x - length / 2, y, length / 2) + myRandom(mag);
  heightMap[x + length / 2][y] = average(x + length / 2, y, length / 2) + myRandom(mag);
  heightMap[x][y - length / 2] = average(x, y - length / 2, length / 2) + myRandom(mag);
  heightMap[x][y + length / 2] = average(x, y + length / 2, length / 2) + myRandom(mag);
  mag /= 2;
  diamond(x - length / 2, y - length / 2, length / 2, mag / 2);
  diamond(x - length / 2, y, length / 2, mag / 2);
  diamond(x, y - length / 2, length / 2, mag / 2);
  diamond(x, y, length / 2, mag / 2);
}

function average(x, y, length) {
  var count = 0;
  var sum = 0.0;
  if (valid(x + length, y)) {
    sum += heightMap[x + length][y];
    count += 1;
  }
  if (valid(x - length, y)) {
    sum += heightMap[x - length][y];
    count += 1;
  }
  if (valid(x, y + length)) {
    sum += heightMap[x][y + length];
    count += 1;
  }
  if (valid(x, y - length)) {
    sum += heightMap[x][y - length];
    count += 1;
  }
  //if (count == 0) count += 1;
  return sum / count;
}

function valid(x, y) {
  if ((x >= 0 && x <= size) && (y >= 0 && y <= size)) {
    return 1;
  } else {
    return 0;
  }
}

function myRandom(mag) {
  return (Math.random()) * mag;
}

/**
  * Generate randomized terran
  * @param scale of terran, size of grids
  */
function setTerranVetices(scale, stepSize) {
  for (var i = 0; i < scale; i += 2) {
    for (var j = 0; j < scale; j += 2) {
      processGrid(j, i, stepSize);
    }
  }
}

function processGrid(x, y, stepSize) {
  //console.log("x: " + x + " y: " + y + " step: " + stepSize);
  addPoint(real(x, stepSize), real(y, stepSize), heightMap[x][y]);
  addPoint(real(x + 1, stepSize), real(y, stepSize), (heightMap[x][y] + heightMap[x + 2][y]) / 2);
  addPoint(real(x, stepSize), real(y + 1, stepSize), (heightMap[x][y] + heightMap[x][y + 2]) / 2);
  pushNormal(x, y, heightMap[x][y], x + 1, y, (heightMap[x][y] + heightMap[x + 2][y]) / 2, x, y + 1, (heightMap[x][y] + heightMap[x][y + 2]) / 2);

  addPoint(real(x + 1, stepSize), real(y, stepSize), (heightMap[x][y] + heightMap[x + 2][y]) / 2);
  addPoint(real(x, stepSize), real(y + 1, stepSize), (heightMap[x][y] + heightMap[x][y + 2]) / 2);
  addPoint(real(x + 1, stepSize), real(y + 1, stepSize), (heightMap[x][y] + heightMap[x][y + 2] + heightMap[x + 2][y] + heightMap[x + 2][y + 2]) / 4);
  pushNormal(x + 1, y, (heightMap[x][y] + heightMap[x + 2][y]) / 2, x, y + 1, (heightMap[x][y] + heightMap[x][y + 2]) / 2, x + 1, y + 1, (heightMap[x][y] + heightMap[x][y + 2] + heightMap[x + 2][y] + heightMap[x + 2][y + 2]) / 4);

  addPoint(real(x + 1, stepSize), real(y, stepSize), (heightMap[x][y] + heightMap[x + 2][y]) / 2);
  addPoint(real(x + 2, stepSize), real(y, stepSize), heightMap[x + 2][y]);
  addPoint(real(x + 1, stepSize), real(y + 1, stepSize), (heightMap[x][y] + heightMap[x][y + 2] + heightMap[x + 2][y] + heightMap[x + 2][y + 2]) / 4);
  pushNormal(x + 1, y, (heightMap[x][y] + heightMap[x + 2][y]) / 2, x + 2, y, heightMap[x + 2][y], x + 1, y + 1, (heightMap[x][y] + heightMap[x][y + 2] + heightMap[x + 2][y] + heightMap[x + 2][y + 2]) / 4);

  addPoint(real(x + 2, stepSize), real(y + 1, stepSize), (heightMap[x + 2][y] + heightMap[x + 2][y + 2]) / 2);
  addPoint(real(x + 2, stepSize), real(y, stepSize), heightMap[x + 2][y]);
  addPoint(real(x + 1, stepSize), real(y + 1, stepSize), (heightMap[x][y] + heightMap[x][y + 2] + heightMap[x + 2][y] + heightMap[x + 2][y + 2]) / 4);
  pushNormal(x + 2, y + 1, (heightMap[x + 2][y] + heightMap[x + 2][y + 2]) / 2, x + 2, y, heightMap[x + 2][y], x + 1, y + 1, (heightMap[x][y] + heightMap[x][y + 2] + heightMap[x + 2][y] + heightMap[x + 2][y + 2]) / 4);


  addPoint(real(x, stepSize), real(y + 1, stepSize), (heightMap[x][y] + heightMap[x][y + 2]) / 2);
  addPoint(real(x + 1, stepSize), real(y + 1, stepSize), (heightMap[x][y] + heightMap[x][y + 2] + heightMap[x + 2][y] + heightMap[x + 2][y + 2]) / 4);
  addPoint(real(x, stepSize), real(y + 2, stepSize), heightMap[x][y + 2]);
  pushNormal(x, y + 1, (heightMap[x][y] + heightMap[x][y + 2]) / 2, x + 1, y + 1, (heightMap[x][y] + heightMap[x][y + 2] + heightMap[x + 2][y] + heightMap[x + 2][y + 2]) / 4, x, y + 2, heightMap[x][y + 2]);

  addPoint(real(x + 1, stepSize), real(y + 1, stepSize), (heightMap[x][y] + heightMap[x][y + 2] + heightMap[x + 2][y] + heightMap[x + 2][y + 2]) / 4);
  addPoint(real(x, stepSize), real(y + 2, stepSize), heightMap[x][y + 2]);
  addPoint(real(x + 1, stepSize), real(y + 2, stepSize), (heightMap[x][y + 2] + heightMap[x + 2][y + 2]) / 2);
  pushNormal(x + 1, y + 1, (heightMap[x][y] + heightMap[x][y + 2] + heightMap[x + 2][y] + heightMap[x + 2][y + 2]) / 4, x, y + 2, heightMap[x][y + 2], x + 1, y + 2, (heightMap[x][y + 2] + heightMap[x + 2][y + 2]) / 2);

  addPoint(real(x + 1, stepSize), real(y + 1, stepSize), (heightMap[x][y] + heightMap[x][y + 2] + heightMap[x + 2][y] + heightMap[x + 2][y + 2]) / 4);
  addPoint(real(x + 2, stepSize), real(y + 1, stepSize), (heightMap[x + 2][y] + heightMap[x + 2][y + 2]) / 2);
  addPoint(real(x + 1, stepSize), real(y + 2, stepSize), (heightMap[x][y + 2] + heightMap[x + 2][y + 2]) / 2);
  pushNormal(x + 1, y + 1, (heightMap[x][y] + heightMap[x][y + 2] + heightMap[x + 2][y] + heightMap[x + 2][y + 2]) / 4, x + 2, y + 1,  (heightMap[x + 2][y] + heightMap[x + 2][y + 2]) / 2, x + 1, y + 2, (heightMap[x][y + 2] + heightMap[x + 2][y + 2]) / 2);

  addPoint(real(x + 2, stepSize), real(y + 2, stepSize), heightMap[x + 2][y + 2]);
  addPoint(real(x + 2, stepSize), real(y + 1, stepSize), (heightMap[x + 2][y] + heightMap[x + 2][y + 2]) / 2);
  addPoint(real(x + 1, stepSize), real(y + 2, stepSize), (heightMap[x][y + 2] + heightMap[x + 2][y + 2]) / 2);
  pushNormal(x + 2, y + 2, heightMap[x + 2][y + 2], x + 2, y + 1, (heightMap[x + 2][y] + heightMap[x + 2][y + 2]) / 2, x + 1, y + 2, (heightMap[x][y + 2] + heightMap[x + 2][y + 2]) / 2);
}

function pushNormal(x1, y1, z1, x2, y2, z2, x3, y3, z3) {
  var v1 = vec3.fromValues(x2 - x1, y2 - y1, z2 - z1);
  var v2 = vec3.fromValues(x3 - x1, y3 - y1, z3 - z1);
  var v3 = vec3.create();
  vec3.cross(v3, v1, v2);
  vec3.normalize(v3, v3);
  terranNormals.push(v3[0]);
  terranNormals.push(v3[1]);
  terranNormals.push(v3[2]);

  terranNormals.push(v3[0]);
  terranNormals.push(v3[1]);
  terranNormals.push(v3[2]);

  terranNormals.push(v3[0]);
  terranNormals.push(v3[1]);
  terranNormals.push(v3[2]);
}

function real(num, step) {
  return num * step - 1;
}

function addPoint(x, y, z) {
  pushPointPosition(x, y, z);
  pushPointColor(x, y, z);
}

function pushPointPosition(x, y, z) {
  terranVertices.push(x);
  terranVertices.push(y);
  terranVertices.push(z);
  //console.log("Pushed: " + x + "," + y + "," + z);
}

function pushPointColor(x, y, z) {
  getColorByHeight(z);
}

function getColorByHeight(height) {

  if (height > 0.5) {
    pushColor(0.2, 0.2, 0.2, 1.0);
  }
  else if (height > 0.10 && height <= 0.5) {
    pushColor(0.54, 0.27, 0.07, 1.0);
  }
  else if (height > -0.45 && height <= 0.10) {
    pushColor(0.0, 1.0, 0.0, 1.0);
  }
  else {
    pushColor(0.0, 0.0, 1.0, 1.0);
  }
}

function pushColor(r, g, b, a) {
  terranColors.push(r);
  terranColors.push(g);
  terranColors.push(b);
  terranColors.push(a);
}

function normalizeHeight() {
  var max = 0;
  var min = 0;
  for (var i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      if (heightMap[j][i] > max) {
        max = heightMap[j][i];
      }
      if (heightMap[j][i] < min) {
        min = heightMap[j][i];
      }
    }
  }
  var range = max - min;

  for (var i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      heightMap[j][i] = -1 + (2 * (heightMap[j][i] - min)) / range;
      if (heightMap[j][i] < -0.45) {
        heightMap[j][i] = -0.6
      }
    }
  }
}

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

/**
  * Handle keyboard input
  */
function handle_key() {
    //console.log("TRIGGERED");
    document.addEventListener('keydown', function(event){
      console.log(event.keyCode);
      switch (event.keyCode) {
        case 37:
          eyePt[0] -= 0.01;
          console.log(eyePt[0] + "," + eyePt[1] + "," + eyePt[2]);
          console.log(viewPt[0] + "," + viewPt[1] + "," + viewPt[2]);
          break;
        case 38:
          eyePt[1] += 0.01;
          console.log(eyePt[0] + "," + eyePt[1] + "," + eyePt[2]);
          console.log(viewPt[0] + "," + viewPt[1] + "," + viewPt[2]);
          break;
        case 39:
          eyePt[0] += 0.01;
          console.log(eyePt[0] + "," + eyePt[1] + "," + eyePt[2]);
          console.log(viewPt[0] + "," + viewPt[1] + "," + viewPt[2]);
          break;
        case 40:
          eyePt[1] -= 0.01;
          console.log(eyePt[0] + "," + eyePt[1] + "," + eyePt[2]);
          console.log(viewPt[0] + "," + viewPt[1] + "," + viewPt[2]);
          break;
        default:
      }
    });
}
