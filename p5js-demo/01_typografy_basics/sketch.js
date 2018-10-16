var font;
var fontsize = 30;

function preload() {
  // Ensure the .ttf or .otf font stored in the assets directory
  // is loaded before setup() and draw() are called
  font = loadFont('RobotoMono-Bold.ttf');
}

function setup() {
  createCanvas(400, 400);
  textFont(font);
  textSize(fontsize);
  textAlign(CENTER, TOP);
}

function draw() {
  background('white');
  for(var i=0; i<13; i++){
    fill(255/13*i)
  	text("InteractiveEnvironmentInteractiveEnvironmentInteractiveEnvironment",width/2+(i*30),(i*30));
  }
}