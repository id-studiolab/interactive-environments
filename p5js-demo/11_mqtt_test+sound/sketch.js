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

var song, bird;

var randombird;

var backgroundColor = 0;

function preload() {
	//soundFormats('mp3', 'ogg');
	song = loadSound('noise.wav');
	bird = loadSound('bird.mp3');
}

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
	background(backgroundColor);
	udateTime();
	drawTimeRemaining();
	if (timeLeftPercentage > 0) {
		drawRandomStripes();

		song.amp(timeLeftPercentage);

		song.rate(map(intensity, 0, 100, 0.2, 4));

	} else {
		song.stop();
		clearTimeout(randombird);

	}
}

function onConnect() {
	// Once a connection has been made, make a subscription and send a message.
	console.log("connected to mosquitto");
	client.subscribe("/minorinteractive/studio/electroforest/2");
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

		song.play();
		playRandomBird();

	} catch (e) {
		console.log("error parsing data!")
		console.log(e);
	}

}

function playRandomBird() {
	bird.rate(random(0,1));

	bird.play();
	clearTimeout(randombird);

	//calculate a random interval after wich re-play the chirp sound
	var intervalNextChirp=map(intensity,0,100,500,100)+random(map(continuity,0,100,500,100)*-1,map(continuity,0,100,500,100))

	//recall the function after a specified amount of time
	randombird = setTimeout(function() {
		playRandomBird();
	}, intervalNextChirp);

	//set the background to white
	backgroundColor = 255;
	//and set it back to black after 10
	setTimeout(function() {
		backgroundColor = 0;
	}, 100);
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

function drawRandomStripes() {
	stroke(255);
	for (var i = 0; i < intensity * 10; i++) {
		var r = random(width)
		line(r, 0, r, height);
	}
}
