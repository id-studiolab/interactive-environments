var client = mqtt.connect('wss://a0e78aaf:2626bb47aaf15e04@broker.shiftr.io', {
  clientId: 'javascript'
});

client.on('connect', function(){
  console.log('client has connected!');
  client.subscribe('/tapGame/#');
  // client.unsubscribe('/example');
});

client.on('message', function(topic, message) {
	messageReceaved(topic,message.toString());
});


var form = document.getElementById("form-id");

// document.getElementById("your-id").addEventListener("click", function () {
// 	var message=document.getElementById("message").value;
// 	client.publish('/display', message);
//
// });
