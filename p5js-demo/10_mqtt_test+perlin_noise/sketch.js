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

//some settings for the grid
var gridW = 20;
var gridH = 20;
var gridSpaceX;
var gridSpaceY;

//to get new perlin noise values
var offset = 0;
var offsetRate = 0.001;


function setup() {
	createCanvas(windowWidth, windowHeight);

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
	drawTimeRemaining()

	stroke(0);
	strokeWeight(timeLeftPercentage * 10 * intensity / 100);

	gridW = map(intensity, 0, 100, 0, 20);
	gridH = map(intensity, 0, 100, 0, 20);

	gridSpaceX = width / gridW;
	gridSpaceY = height / gridH;

	var alignment = map(continuity, 0, 99, 0.00000001, 100000);

	offset += offsetRate;

	for (var i = 0; i < width; i += gridSpaceX) {
		for (var j = 0; j < height; j += gridSpaceY) {
			var n = noise(i / alignment + offset, j / alignment + offset) * 360;
			push();
			translate(i + gridSpaceX / 2, j + gridSpaceY / 2);
			rotate(n);
			line(0, 0, gridSpaceX / 2, gridSpaceY / 2);
			pop();
		}
	}
}

function onConnect() {
	// Once a connection has been made, make a subscription and send a message.
	console.log("connected to mosquitto");
	client.subscribe("/minorinteractive/studio/electroforest/3");
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
