var canvas;


var team=0;
var gameStatus=0;

var redProgress=0.5;


var winner=red;

function setup() {
  createCanvas(windowWidth, windowHeight);
	noStroke();
}

function draw(){

	switch (gameStatus) {
		case 0:
			fill('red');
			rect(0,0,width/2,height);
			fill('blue')
			rect(width/2,0,width/2,height);

			fill('white')
			textSize(100);
			textAlign(CENTER, CENTER)
			text("CHOOSE YOUR SIDE", 50,50, width-100,height-100);
			break;

		case 1:{
			fill('red');
			rect(0,0,width*redProgress,height);
			fill('blue')
			rect(width*redProgress,0,width*redProgress,height);

			fill('white')
			textSize(100);
			textAlign(CENTER, CENTER)
			text("TAP", 50,50, width-100,height-100);

		}
		case 2:{
 		//todo victory screen
			if (winner=="red"){

			}else {

			}
			fill('red');
			rect(0,0,width*redProgress,height);
			fill('blue')
			rect(width*redProgress,0,width*redProgress,height);

			fill('white')
			textSize(100);
			textAlign(CENTER, CENTER)
			text("TAP", 50,50, width-100,height-100);

		}


		default:

	}

}

function mouseClicked(){

	if(gameStatus==0){
		gameStatus=1;
	}
	if (gameStatus==1){

		if (mouseX<width*redProgress){
			client.publish('/tapGame/team0', "+1");
		}else if (mouseX>width*redProgress){
			client.publish('/tapGame/team1', "+1");
		}
	}

}


function messageReceaved(topic,message){
	console.log(topic,message);
	if(topic=="/tapGame/redProgress"){
		redProgress=parseFloat(message);
	}else if(topic=="/tapGame/victory"){
		gameStatus=2;
		winner=message;
	}


	//if (topic)

}

function windowResized() {
  resizeCanvas(windowWidth, windowHeight);
}
