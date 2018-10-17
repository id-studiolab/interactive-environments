var values= [26,200,300,250,100,150,25,90,200];

function setup() {
  createCanvas(400, 400);
}

function draw() {
  background('#fff');

	var nSteps=values.length+2;
	stepSize=width/nSteps;

	for (var i=0; i< values.length-1; i++){
		line(stepSize*(i+1),values[i],stepSize*(i+2),values[i+1]);
	}
	for (var i=0; i< values.length; i++){
		ellipse(stepSize*(i+1),values[i],values[i]/10,values[i]/10);
	}

}

function mouseClicked(){
	values.push(mouseY);
}
