//setup MQTT client
client = new Paho.MQTT.Client("broker.shiftr.io", Number(443), "controlpanel");
client.onConnectionLost = onConnectionLost;
client.onMessageArrived = onMessageArrived;

//some placeholders for the data received trough MQTT
var intensity = 0;
var duration = 0;
var continuity = 0;

//some placeholders to keep track of time
var messagereceivedTime = 0;
var timeLeftPercentage = 0;

var font;
var sentence = "The clearest way into the universe is through a forest wilderness."
var words = sentence.split(' ');

function preload() {
	font = loadFont('RobotoMono-Bold.ttf');
}

function setup() {
	createCanvas(windowWidth, windowHeight);
	textFont(font);
	textSize(30);
	textAlign(CENTER);

	client.connect({
		onSuccess: onConnect,
		userName: "connected-object",
		password: "c784e41dd3da48d4",
		useSSL: true,
	});
}

function draw() {
	background(255);
	udateTime();
	drawTimeRemaining();

	if (timeLeftPercentage > 0) {
		var indexToDraw= floor(map(timeLeftPercentage,0,1,words.length,0));

		var variationX=random(-width/2,width/2)*map(continuity,0,100,0,1);
		var variationY=random(-height/2,height/2)*map(continuity,0,100,0,1);

		textSize(map(intensity,0,100,10,100));

		text(words[indexToDraw], width/2+variationX, height/2+variationY)
	} else {
		fill(255);
		rect(0, 0, width, height);
	}
}


function onConnect() {
	// Once a connection has been made, make a subscription and send a message.
	console.log("connected to mosquitto");
	client.subscribe("/minorinteractive/studio/electroforest/1");
}

function onConnectionLost(responseObject) {
	if (responseObject.errorCode !== 0) {
		console.log("onConnectionLost:" + responseObject.errorMessage);
	}
}

function onMessageArrived(message) {
	console.log(message.destinationName + " -> " + message.payloadString);

	var payload = message.payloadString;

	try {
		valuesArray = payload.split(' ');

		duration = Number(valuesArray[0]);
		intensity = Number(valuesArray[1]);
		continuity = Number(valuesArray[2]);

		messagereceivedTime = millis();
		timeLeftPercentage = 1;



	} catch (e) {
		console.log("error parsing data!")
		console.log(e);
	}

}


function sendMessage(topic, message) {
	message = new Paho.MQTT.Message(message);
	message.destinationName = topic;
	client.send(message);
}

function udateTime() {
	if (timeLeftPercentage > 0) {
		var timePassed = millis() - messagereceivedTime;
		var timeRemaining = duration - timePassed;
		timeLeftPercentage = map(timeRemaining, 0, duration, 0, 1);
	}
}

function drawTimeRemaining() {
	noStroke()
	fill(0);
	rect(0, height - 5, width * timeLeftPercentage, 5);
}
