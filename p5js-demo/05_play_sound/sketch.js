var song;

function preload() {
  song = loadSound('noise.wav');
}

function setup() {
  //song.loop()
	createCanvas(400, 400);
	song.playMode('restart');
}

function draw() {
  background(200);

  // Set the rate to a range between 0.1 and 4
  // Changing the rate also alters the pitch
  var speed = map(mouseX, 0.1, height, 0, 2);
  speed = constrain(speed, 0.01, 4);
  song.rate(speed);

	song.amp(map(mouseY,0,height,0,1));

  // Draw a circle to show what is going on
  stroke(0);
  fill(51, 100);
  ellipse(mouseX, mouseY, 48, 48);
}

function mouseClicked(){
	song.play();
}
