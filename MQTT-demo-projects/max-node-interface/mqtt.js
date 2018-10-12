const maxApi = require("max-api");
const path = require('path');
var mqtt = require('mqtt')


// This will be printed directly to the Max console
maxApi.post(`Loaded the ${path.basename(__filename)} script`);

var username = "";
var password = "";
var host = "";

var connected = false;
var client;

var enableDebug = true;

maxApi.addHandler("connect", (h, u, p, c) => {
	host = h;
	username = u;
	password = p;
	clientId = c;

	client = mqtt.connect('mqtt://' + username + ':' + password + '@' + host, {
		clientId: clientId
	});

	client.on('connect', function() {
		debug('client has connected!');
		connected = true;
	});

	client.on('message', function(topic, message) {
		debug('received: ' + message.toString() + " on topic: " + topic, );

		var m = message.toString();
		var argumentsList = m.split(" ");

		for (var i = 0; i < argumentsList.length; i++) {
			if (!isNaN(Number(argumentsList[i]))) {
				argumentsList[i] = Number(argumentsList[i]);
			}
		}

		argumentsList.unshift(topic);
		maxApi.outlet(argumentsList);
	});

	client.on('close', () => {
		if (connected != false) {
			debug('client disconnected!');
			connected = false;
		}
	});

});

maxApi.addHandler("toggleDebug", () => {
	enableDebug = !enableDebug;
});

maxApi.addHandler("subscribe", (topic) => {
	if (connected == true) {
		debug('subscribed to: ' + topic)
		client.subscribe(topic);
	} else {
		debug('client not connected!')
	}
});

maxApi.addHandler("publish", (topic, ...args) => {
	if (connected == true) {
		var messages = "";
		for (var i = 0; i < args.length; i++) {
			messages += args[i];
			if (i != args.length - 1) {
				messages += " ";
			}
		}
		debug('sending: ' + messages.toString() + " on topic: " + topic, );
		client.publish(topic, messages.toString());
	} else {
		debug('client not connected!')
	}
});

function debug(msg) {
	if (enableDebug){
		console.log(msg);
		maxApi.post(msg);
	}
}
