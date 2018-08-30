# TapGame

This example demonstrate how you can build a multi-player, multi-device, networked game using mqtt.

![tap game system diagram](./assets/schema_1.png "system diagram")

### Structure of the system:
- The webapp can be opened on user's browser, both on desktop or mobile phones.
- When the user will tap the screen they will send messages to the broker notifying that one color or an other has been tapped.

- The wemos will receive the tap information from all the devices connected; it will update the global count, display it on the ledstrip and send back - the global score to the broker.

- The webapp will get the data about the progress and update the visualization on the screen.

- Once the wemos detects that one team has won it will send out a new victort message to the broker, it will change the visualization and reset the counter to be ready fot a new mach.

- The web app will receive the victory message; it will display the win loose message and reset the game.


### MQTT topics
messages in MQTT are always sent to a BROKER which cannot perform anything else than forwarding the messages it receive on some topics to other clients who are subscribed to those topics.

- **/tapGame/team0** & **/tapGame/team1**: Whenever a tap is detected the message "+1" will be sent either on topic **/tapGame/team0** or on topic **/tapGame/team1**.

- **/tapGame/redProgress**: the wemos sends to this topic a number between 0 and 1 which is proportional to how much the red team is winning the game

- **/tapGame/victory**: the wemos will write to this topic every time one team manages to win the game. The name of the team who won will be sent as message.

### NOTES:
- You might need to change the wifi settings to allow the devices to connect to the network.
- Please find the credential to connect to the broker inside the arduino and js sketch.
