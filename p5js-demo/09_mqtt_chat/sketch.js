client = new Paho.MQTT.Client("broker.shiftr.io", Number(443), "controlpanel");
client.onConnectionLost = onConnectionLost;
client.onMessageArrived = onMessageArrived;

var receivedMessage="";

var message, address, button;

var destinationAddress="/minorinteractive/studio/chat/1"
var personalAddress="/minorinteractive/studio/chat/1"

function setup() {
	createCanvas(400, 400);

	message = createInput();
  message.position(10, 65);

	address = createInput();
	address.position(message.x + message.width+10, 65);
	address.value("/minorinteractive/studio/chat/1");

  button = createButton('submit');
  button.position(message.x + message.width + address.width+20, 65);
  button.mousePressed(sendToChat);

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

function sendToChat(){
	var m=message.value();
	destinationAddress=address.value();
	sendMessage(destinationAddress,m);
}

function onConnect() {
  // Once a connection has been made, make a subscription and send a message.
  console.log("connected to mosquitto");
  client.subscribe(personalAddress);
}

function onConnectionLost(responseObject) {
  if (responseObject.errorCode !== 0) {
    console.log("onConnectionLost:" + responseObject.errorMessage);
  }
}

function onMessageArrived(message) {
  console.log(message.destinationName + " -> " + message.payloadString);
  receivedMessage = message.payloadString;
}

function sendMessage(topic, message) {
  m = new Paho.MQTT.Message(message);
  m.destinationName = topic;
	console.log("sending "+message+" to address:"+topic);
  client.send(m);
}
