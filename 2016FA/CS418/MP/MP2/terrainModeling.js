var heightMap = [];
var colorMap = [];
var size = 0;

/**
 *  Use Diamond-Square Algorithm to determine the height of each vertex
 *  @param scale -- size of the 2d-array
 */
function setHeightMap(scale) {
  for (var i = 0; i <= scale; i++) {
    var temp = new Array(scale + 1);
    for (var j = 0; j <= scale; j++) {
      temp[j] = 0;
    }
    heightMap.push(temp);
  }
  heightMap[0][0] = 5;
  heightMap[0][scale] = 5;
  heightMap[scale][0] = 5;
  heightMap[scale][scale] = 5;
  diamond(0, 0, scale, 2.0);
}

/**
  * Implementation of Diamond-Square Algorithm
  * @param x -- x coordinate
  * @param y -- y coordinate
  * @param length -- scale of the table
  * @param mag -- magnitude of randomized height variance
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
  mag /= 16;
  diamond(x - length / 2, y - length / 2, length / 2, mag);
  diamond(x - length / 2, y, length / 2, mag);
  diamond(x, y - length / 2, length / 2, mag);
  diamond(x, y, length / 2, mag);
}

/**
 * Calculate the average height of the 4 vertices around the point given
 * @param x -- x coordinate
 * @param y -- y coordinate
 * @param length == length of the small square to handle
 */
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
  return sum / count;
}

/**
 * Check if the given vertices is valid
 */
function valid(x, y) {
  if ((x >= 0 && x <= size) && (y >= 0 && y <= size)) {
    //console.log(x + "," + y + " is valid");
    return 1;
  } else {
    //console.log(x + "," + y + " is not valid");
    return 0;
  }
}

/**
 * Generate a random number
 */
function myRandom(mag) {
  var num = Math.random();
  if (num >= 0.7) {
    return (Math.random() * 2 - 1) * mag * 10 * num;
  }
  if (num >= 0.9) {
    return (Math.random() * 2 - 1) * mag * 50 * num;
  }
  if (num >= 0.999) {
    return (Math.random() * 2 - 1) * mag * 100 * num;
  }
  if (num <= -0.7) {
    return (Math.random() * 2 - 1) * mag * (-20) * num;
  }
  return (Math.random() * 2 - 1) * mag;
}

function normalizeHeight() {
  var max = 0;
  var min = 0;
  for (var i = 0; i < size; i++) {
    for (var j = 0; j < size; j++) {
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
    for (var j = 0; j < size; j++) {
      heightMap[j][i] = 0.1 * (heightMap[j][i] - min) / range - 10;
    }
  }
}

/**
 * Calculate the normals of each vertices
 */
function pushNormal(normalArray, x1, y1, z1, x2, y2, z2, x3, y3, z3) {
  var v1 = vec3.fromValues(x2 - x1, y2 - y1, z2 - z1);
  var v2 = vec3.fromValues(x3 - x1, y3 - y1, z3 - z1);
  var v3 = vec3.create();
  vec3.cross(v3, v1, v2);
  vec3.normalize(v3, v3);
  normalArray.push(v3[0]);
  normalArray.push(v3[1]);
  normalArray.push(v3[2]);

  normalArray.push(v3[0]);
  normalArray.push(v3[1]);
  normalArray.push(v3[2]);

  normalArray.push(v3[0]);
  normalArray.push(v3[1]);
  normalArray.push(v3[2]);
}

function pushColor(height)
{
  if (height >= 4) {
    colorMap.push(1.0);
    colorMap.push(1.0);
    colorMap.push(1.0);
    colorMap.push(1.0);
  }
  else if (height < 4 && height >= 2) {
    colorMap.push(0.8);
    colorMap.push(0.8);
    colorMap.push(0.8);
    colorMap.push(1.0);
  }
  else if (height < 2 && height >= 1) {
    colorMap.push(0.0);
    colorMap.push(1.0);
    colorMap.push(0.0);
    colorMap.push(1.0);
  }
  else
  {
    colorMap.push(0.0);
    colorMap.push(0.0);
    colorMap.push(1.0);
    colorMap.push(1.0);
  }
}

//-------------------------------------------------------------------------
function terrainFromIteration(n, minX,maxX,minY,maxY, vertexArray, faceArray,normalArray)
{
    size = n;
    setHeightMap(n);
    //normalizeHeight();

    var deltaX=(maxX-minX)/n;
    var deltaY=(maxY-minY)/n;
    for(var i=0;i<=n;i++)
    {
      for(var j=0;j<=n;j++)
      {
           vertexArray.push(minX+deltaX*j);
           vertexArray.push(minY+deltaY*i);
           vertexArray.push(heightMap[j][i]);

           pushColor(heightMap[j][i]);
       }
     }

     for (var i = 0; i < vertexArray.length; i += 9)
     {
       pushNormal(normalArray, vertexArray[i], vertexArray[i + 1], vertexArray[i + 2],
                               vertexArray[i + 3], vertexArray[i + 4], vertexArray[i + 5],
                               vertexArray[i + 6], vertexArray[i + 7], vertexArray[i + 8])
     }

    var numT=0;
    for(var i=0;i<n;i++)
       for(var j=0;j<n;j++)
       {
           var vid = i*(n+1) + j;
           faceArray.push(vid);
           faceArray.push(vid+1);
           faceArray.push(vid+n+1);

           faceArray.push(vid+1);
           faceArray.push(vid+1+n+1);
           faceArray.push(vid+n+1);
           numT+=2;
       }
    return numT;
}
//-------------------------------------------------------------------------
function generateLinesFromIndexedTriangles(faceArray,lineArray)
{
    numTris=faceArray.length/3;
    for(var f=0;f<numTris;f++)
    {
        var fid=f*3;
        lineArray.push(faceArray[fid]);
        lineArray.push(faceArray[fid+1]);

        lineArray.push(faceArray[fid+1]);
        lineArray.push(faceArray[fid+2]);

        lineArray.push(faceArray[fid+2]);
        lineArray.push(faceArray[fid]);
    }
}

//-------------------------------------------------------------------------

function setupColorBuffer(colorBuffer) {
  for (var i = 0; i < colorMap.length; i++) {
    colorBuffer.push(colorMap[i]);
  }
}
