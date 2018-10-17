var duration=1000;
var timeStarted=0;
var timeLeftPercentage=1;

function setup() {
  createCanvas(400, 400);
}

function draw() {
	udateTime();
  background(0);
	fill (255);
	rect(0,0,width*timeLeftPercentage,height);
}

function mouseClicked(){
	timeStarted=millis();
	timeLeftPercentage=1;
}

function udateTime() {
	if (timeLeftPercentage > 0) {
		var timePassed = millis() - timeStarted;
		var timeRemaining = duration - timePassed;
		timeLeftPercentage = map(timeRemaining, 0, duration, 0, 1);
	}
}

function drawTimeRemaining() {
	noStroke()
	fill(0);
	rect(0, height - 5, width * timeLeftPercentage, 5);
}
