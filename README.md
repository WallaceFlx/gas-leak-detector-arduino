# Smart Gas Leak Detector
## About
- **Authors**
	 - Wallace Felix
	 - Ezequiel Neves

### Description
- This a simple arduino Sketch designed to run on Arduino Uno R3 along with ESP8266 ESP-01 and MQ-6 sensor. It was developed as one of the college's assignments on Mackenzie Presbyterian University.
	The idea was to develop a simple, yet effective solution capable of monitoring the levels of LPG in the environment and notifying the users when it reaches certain levels.
	This solution uses [MQTT](https://mqtt.org/) protocol on its structure so that it can publish messages on a certain topic to notify the users about the incident on their phones. The notification takes place through [AWS SNS](https://aws.amazon.com/sns/?whats-new-cards.sort-by=item.additionalFields.postDateTime&whats-new-cards.sort-order=desc) and its API is called by a service that must be subscribed on the same topic used by the device to publish its messages.
	The broker used on this project was also the designed by the same authors and it can be found [here](https://github.com/nbezequiel/flamable-mqtt-broker).

### Usage
- **Circuit schematic setup**
![Esquema_montagem](https://user-images.githubusercontent.com/48225559/85324870-0253c480-b4a1-11ea-810a-09b9b19eb67e.png)

- **Materials**
	- Arduino Uno R3
	- ESP8266 ESP-01
	- MQ-6 Sensor
	- Buzzer 5V
	- Push-button
	- LED 5mm
	- 2 Resistors 1k ohm
	- 3 Resistor 330 ohm

- **Sketch Dependencies**
	- WiFiEsp.h -> [repository](https://github.com/bportaluri/WiFiEsp)
	- PubSubClient.h -> [repository](https://github.com/knolleary/pubsubclient)

- **Compatibility**
	- Make sure that the ESP-01 firmware supports AT serial commands and the BAUD rate is set to 9600.