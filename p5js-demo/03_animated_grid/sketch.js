var gridW = 20;
var gridH = 20;

var gridSpaceX;
var gridSpaceY;

var offset = 0;

var alignment_factor=1;

function setup() {
  createCanvas(400, 400);
  gridSpaceX = width / gridW;
  gridSpaceY = height / gridH;
}

function draw() {
  background('#fff');

  offset += .0001;

  for (var i = 0; i < width; i += gridSpaceX) {
    for (var j = 0; j < height; j += gridSpaceY) {

      var alignment= map(alignment_factor,0,1,0,10000)
      var n=noise(i/alignment + offset, j/alignment + offset) * 360;
      push();
      translate(i+gridSpaceX/2, j+gridSpaceY/2);
      rotate(n);
      line(0, 0, gridSpaceX/2, gridSpaceY/2);
      pop();
    }
  }
}