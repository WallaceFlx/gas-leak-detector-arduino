#include "WiFiEsp.h"
#include "SoftwareSerial.h"
#include <PubSubClient.h>

const byte btn = 2;//Push-button pin. Make sure to select a pin that supports interruption
const byte buz = 13;//Buzzer output pin
const byte sensor = A0;//MQ-6 sensor input pin
const byte led = 4;//LED output pin

const char* mqttServer = "server-address"; //server address
const int mqttPort = 1234; //server port
const char* topic = "alarm/sounding";//topic on broker
const char* clientName = "client-name";//client's name
const int place = 2020;//facility Id
int level = 0;//gas level

char ssid[] = "ssid"; //your network ssid
char pass[] = "your-password";//your network password

volatile byte status = 0;//status of considerable gas level detection
byte isMsgSent = 0;
String format = "";//formated message to be published on topic
byte statusConnection = WL_IDLE_STATUS;

SoftwareSerial Serial2(6, 7); //pins to emulate serial and communicate with ESP8266, RX/TX

WiFiEspClient wifi;

PubSubClient client(wifi);

void blink() {
  digitalWrite(led, HIGH);
  delay(500);
  digitalWrite(led, LOW);
  delay(500);
}

void monitorEnvironment() {
  if (analogRead(sensor) > 300) {
    level = analogRead(sensor);
    turnOnBuzzer();
  }
}

void mqttReConnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(clientName)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, state=");//prints connect state
      Serial.print(client.state());
    }
  }
}

void messageController() {
  if (!isMsgSent && status) {//If the message is already sent, it won't do it again
    format = "{\"level\":" + String(level) + ", \"place\":" + String(place) + "}";
    sendMessage(topic, format.c_str());//Publishes the gas level and the facility Id
  }
  else if (isMsgSent && !status)
    isMsgSent = !isMsgSent;
}

void sendMessage(char* topic, char* message) {
  if (!client.publish(topic, message))
    Serial.println("Publish failed!");
  else
    isMsgSent = 1;
}

void turnOffBuzzer() {
  status = 0;
  digitalWrite(buz, status);
}

void turnOnBuzzer() {
  status = 1;
  digitalWrite(buz, status);
}

void setup()
{
  pinMode(buz, OUTPUT);
  pinMode(btn, INPUT_PULLUP);
  pinMode(sensor, INPUT);
  pinMode(led, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(btn), turnOffBuzzer, FALLING);

  Serial.begin(9600);
  Serial2.begin(9600);
  WiFi.init(&Serial2); //Initializes serial communication with ESP8266

  while (statusConnection != WL_CONNECTED) {
    statusConnection = WiFi.begin(ssid, pass);
  }
  Serial.println(WiFi.localIP());
  client.setServer(mqttServer, mqttPort);
}

void loop()
{  
  monitorEnvironment();

  if (status) {
    blink();
  }

  if (!client.connected()) {
    mqttReConnect();
  }
    
  messageController();
}
