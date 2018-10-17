client = new Paho.MQTT.Client("broker.shiftr.io", Number(443), "controlpanel");
client.onConnectionLost = onConnectionLost;
client.onMessageArrived = onMessageArrived;

var receivedMessage="";

function setup() {
  createCanvas(400, 400);

  client.connect({
    onSuccess: onConnect,
    userName: "connected-object",
    password: "c784e41dd3da48d4",
    useSSL: true,
  });

	textAlign(CENTER);
}

function draw() {
  background(220);
	text(receivedMessage, width/2,height/2);
}

function mouseClicked(){
	sendMessage("/minorinteractive/studio/test/1", "got a message");
}

function onConnect() {
  // Once a connection has been made, make a subscription and send a message.
  console.log("connected to mosquitto");
  client.subscribe("/minorinteractive/studio/test/1");
}

function onConnectionLost(responseObject) {
  if (responseObject.errorCode !== 0) {
    console.log("onConnectionLost:" + responseObject.errorMessage);
  }
}

function onMessageArrived(message) {
  console.log(message.destinationName + " -> " + message.payloadString);
  receivedMessage = message.payloadString;

	setTimeout(function(){
		receivedMessage="";
	},500);
}

function sendMessage(topic, message) {
  message = new Paho.MQTT.Message(message);
  message.destinationName = topic;
  client.send(message);
}
