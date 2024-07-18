
#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Arduino_MKRENV.h>



WiFiClient wificlient;
PubSubClient mqttClient(wificlient);

char WIFIssid[] = " ";  //wifi name
char WIFIpass[] = " ";
char MQTThost[] = " ";  //broker
int MQTTport = 1883;

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  if(!ENV.begin()) {while(1);}

  mqttClient.setServer(MQTThost, MQTTport);

 
}

void loop() {

  while (WiFi.status() != WL_CONNECTED || !mqttClient.connected()) {
    digitalWrite(LED_BUILTIN, LOW);
    while (WiFi.status() != WL_CONNECTED) {  //if arduino cant connect, waits 5 seconds and try again
      WiFi.begin(WIFIssid, WIFIpass);
      delay(5000);
    }
    if (!mqttClient.connect(""))
      delay(5000);
    else
      digitalWrite(LED_BUILTIN, HIGH);  //successfull connection
  }
  mqttClient.loop(); //keep connection alive

  float raggiuva = ENV.readUVA();
  float raggiuvb = ENV.readUVB();
  float indraggiuv = ENV.readUVIndex();
  float pressure = ENV.readPressure();
  
  Serial.println(raggiuva);
  publishMQTTJsonMessage("raggiuva","W/m^2",raggiuva," "); //empty parameter for topic
  Serial.println(raggiuvb);
  publishMQTTJsonMessage("raggiuvb","W/m^2",raggiuvb," ");
  Serial.println(indraggiuv);
  publishMQTTJsonMessage("indraggiuv","W/m^2",indraggiuv," ");
  Serial.println(pressure);
  publishMQTTJsonMessage("pressione","kPa",pressure," "); 
  delay(5000);
  
}
void publishMQTTJsonMessage(char type[], char unit[], float value,char topic[])
{StaticJsonDocument<128> jsonDocument;
 char jsonMessage[128];
 jsonDocument["type"]=type;
 jsonDocument["unit"]=unit;
 jsonDocument["value"]=value;
 serializeJson(jsonDocument,jsonMessage);
 mqttClient.publish(topic,jsonMessage);
  
}
