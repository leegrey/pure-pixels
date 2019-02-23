const exec = require('child_process').execFile;
const path = require('path');
const fs = require('fs');

// get the platform from the arguments
var platform = "win";
if (process.argv.length >= 3) {
    platform = process.argv[2].substr(1);
}

console.log("Ase Export for Platform:", platform);

// get the platform specific path to aseprite binary
var pathToAse = "";
if (platform == "win") {
    pathToAse = path.normalize(__dirname + "/../tools/win/aseprite/Aseprite.exe");
} else if (platform == "osx") {
    pathToAse = path.normalize(__dirname + "/../tools/osx/aseprite/aseprite");
} else {
    console.log("Bad platform name supplied. Use either -win or -osx");
    process.exit();
}

// NOTE: This version of the script just has hard-wired names
// TODO: expand script to accept arguments for input and output path

// get paths to source ase file and output dir
var pathToLevel = path.normalize(__dirname + "/art/world1.ase");
var outputPath = path.normalize(__dirname + "/build_" + platform + "/assets/world1/world_1_.png");

console.log("pathToAse:", pathToAse);
console.log("pathToLevel:", pathToLevel);
console.log("outputPath:", outputPath);

// call out to aseprite to export the layers
exec(
    pathToAse,
    [
        "-b", 
        "--split-layers", 
        pathToLevel,    //"../art/pure_level1.ase",
        "--save-as", 
        outputPath      //"../build/assets/world1/world_1_.png"   
    ],
    
    function(err, data) {  
        console.log(err)
        console.log(data.toString()); 
        // rename the files to the correct form
        renameFiles(__dirname + "/build_" + platform + "/");
    }
);  

function renameFiles(basePath) {

    fs.renameSync(
        basePath + "assets/world1/world_1_ (foreground).png",
        basePath + "assets/world1/foreground.png");

    fs.renameSync(
        basePath + "assets/world1/world_1_ (solid).png",
        basePath + "assets/world1/solid.png");

    fs.renameSync(
        basePath + "assets/world1/world_1_ (background_objects).png",
        basePath + "assets/world1/background_objects.png");

    fs.renameSync(
        basePath + "assets/world1/world_1_ (background).png",
        basePath + "assets/world1/background.png");

}
