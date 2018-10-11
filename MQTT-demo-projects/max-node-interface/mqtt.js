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

maxApi.addHandler("connect", (h, u, p, c) => {
	host = h;
	username = u;
	password = p;
	clientId= c;

	client = mqtt.connect('mqtt://' + username + ':' + password + '@' + host, {
		clientId: clientId
	});

	client.on('connect', function() {
		console.log('client has connected!');
		connected = true;
	});

	client.on('message', function(topic, message) {
		console.log('new message:', topic, message.toString());
		maxApi.outlet(topic, message.toString());
	});
	
	client.on('close', () => {
		if (connected != false) {
			maxApi.post('client disconnected!');
			connected = false;
		}
	});

});

maxApi.addHandler("subscribe", (topic) => {
	if (connected == true) {
		client.subscribe(topic);
	}
	else {
		console.log('client not connected!');		
		maxApi.post('client not connected!');
	}
		
});

maxApi.addHandler("publish", (topic, ...message) => {
	if (connected == true) {
		client.publish(topic, message.toString());
	}
	else {
		console.log('client not connected!');		
		maxApi.post('client not connected!');
	}
});
