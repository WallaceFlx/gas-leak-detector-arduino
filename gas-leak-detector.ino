#include "WiFiEsp.h"
#include "SoftwareSerial.h"
#include <PubSubClient.h>

const byte btn = 2;
const byte buz = 13;
const byte sensor = A0;
const byte led = 4;

const char* mqttServer = "server-address"; //server address
const int mqttPort = 1234; //server port
const char* topic = "alarm/sounding";
const char* clientName = "client-name";//client's name
const int place = 2020;
int level = 0;

char ssid[] = "ssid"; //your network ssid
char pass[] = "your-password";//your network password

volatile byte status = 0;
byte isMsgSent = 0;
String format = "";
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
      Serial.print("failed, state=");
      Serial.print(client.state());
    }
  }
}

void messageController() {
  if (!isMsgSent && status) {//If the message is already sent, it won't do it again
    format = "{\"level\":" + String(level) + ", \"place\":" + String(place) + "}";
    sendMessage(topic, format.c_str());
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

  if (WiFi.status() == WL_NO_SHIELD) {
    while (true);
  }
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
  messageController();
  if (!client.connected()) {
    mqttReConnect();
  }  
}
