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
		
		var m=message.toString();
		var argumentsList=m.split(" "); 
		
		for (var i=0; i<argumentsList.length;i++){
			if (!isNaN(Number(argumentsList[i]))){
				argumentsList[i]=Number(argumentsList[i]);			
			}	
		}

		argumentsList.unshift(topic);
		console.log('new message:', argumentsList);
		maxApi.outlet(argumentsList);
		
		
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
		var messages="";
		console.log(topic,args.length);
		for (var i=0; i<args.length;i++){
			messages+=args[i]+" ";
		}
		console.log(messages);
		client.publish(topic,messages.toString());
	}
	else {
		console.log('client not connected!');		
		maxApi.post('client not connected!');
	}
});
