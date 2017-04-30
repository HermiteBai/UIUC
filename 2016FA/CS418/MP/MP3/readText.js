function readTextFile(objectLoaded)
{
    console.log("reading "+ "teapot.obj");
    var rawFile = new XMLHttpRequest();
    var allText = [];
    rawFile.open("GET", "teapot.obj", true);

    rawFile.onreadystatechange = function ()
    {
        if(rawFile.readyState === 4)
        {
            if(rawFile.status === 200 || rawFile.status == 0)
            {
                 //callbackFunction(rawFile.responseText);
                 console.log("Got text file!");


                 var lines = rawFile.responseText.split("\n");
                 console.log("Lines splited");
                 for (var i = 0; i < lines.length; i++)
                 {
                    var line = lines[i];
                    var tokens = line.split("  ");
                    if (tokens[0] == "v") {
                      objectLoaded.vertexPositions.push(tokens[1]);
                      objectLoaded.vertexPositions.push(tokens[2]);
                      objectLoaded.vertexPositions.push(tokens[3]);
                    }
                    else if (tokens[0] == "f") {
                      objectLoaded.indices.push(tokens[1]);
                      objectLoaded.indices.push(tokens[2]);
                      objectLoaded.indices.push(tokens[3]);
                    }
                    else if (tokens[0] == "vn") {
                      objectLoaded.vertexNormals.push(tokens[1]);
                      objectLoaded.vertexNormals.push(tokens[2]);
                      objectLoaded.vertexNormals.push(tokens[3]);
                    }
                    else {
                      console.log("Cannot recognise!");
                    }
                 }

                 objectLoaded.vertexPositions = new Float32Array(objectLoaded.vertexPositions);
                 objectLoaded.indices = new Uint16Array(objectLoaded.indices);
                 if (objectLoaded.vertexNormals.length == 0) {
                   for (var i = 0; i < objectLoaded.vertexPositions.length; i += 9) {

                     var vertices = new Array(9);

                     for (var j = 0; j < 9; j++) {
                       vertices[j] = objectLoaded.vertexPositions[objectLoaded.indices[i + j]];
                     }
                     var vec31 = vec3.fromValues(vertices[0], vertices[1], vertices[2]);
                     var vec32 = vec3.fromValues(vertices[3], vertices[4], vertices[5]);
                     var vec33 = vec3.fromValues(vertices[6], vertices[7], vertices[8]);

                     var vec3a = vec3.create();
                     var vec3b = vec3.create();
                     vec3.subtract(vec3a, vec32, vec31);
                     vec3.subtract(vec3b, vec33, vec31);

                     var vec3c = vec3.create();
                     vec3.cross(vec3c, vec3a, vec3b);
                     vec3.normalize(vec3c, vec3c);

                     objectLoaded.vertexNormals.push(vec3c[0]);
                     objectLoaded.vertexNormals.push(vec3c[1]);
                     objectLoaded.vertexNormals.push(vec3c[2]);

                     objectLoaded.vertexNormals.push(vec3c[0]);
                     objectLoaded.vertexNormals.push(vec3c[1]);
                     objectLoaded.vertexNormals.push(vec3c[2]);

                     objectLoaded.vertexNormals.push(vec3c[0]);
                     objectLoaded.vertexNormals.push(vec3c[1]);
                     objectLoaded.vertexNormals.push(vec3c[2]);

                   }
                 }

                 console.log("File reading complete");

                 objectLoaded.vertexNormals = new Float32Array(objectLoaded.vertexNormals);
                 console.log("Converted");

                 console.log(objectLoaded.vertexPositions.length);
                 console.log(objectLoaded.vertexNormals.length);
                 console.log(objectLoaded.indices.length);
            }
        }
    }
    rawFile.send(null);
}
