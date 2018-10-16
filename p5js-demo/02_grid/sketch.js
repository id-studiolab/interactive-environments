var gridW = 20;
var gridH = 20;

var gridSpaceX;
var gridSpaceY;

function setup() {
  createCanvas(400, 400);
  gridSpaceX = width / gridW;
  gridSpaceY = height / gridH;
  
  background('#fff');
  for (var i = 0; i < gridW; i++) {
    for (var j = 0; j < gridH; j++) {
      rect(i * gridSpaceX, j * gridSpaceY, 10+random(10), 10);
    }
  }
}

function draw() {
  
}