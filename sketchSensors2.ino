#include <WiFiNINA.h> 
#include <PubSubClient.h>
#include <ArduinoJson.h>  
#include <Arduino_MKRENV.h>
#include <Wire.h>  
#include "SHT31.h" 
#include "SCD30.h"
#include "DFRobot_MultiGasSensor.h"

DFRobot_GAS_I2C gas(&Wire ,0x74);

SHT31 sht31 = SHT31();

WiFiClient wificlient; 
PubSubClient mqttClient(wificlient);

char WIFIssid [] = " "; // wifi name
char WIFIpass [] = " "; //  
char MQTThost [] = " "; // broker
int MQTTport = 1883; 

StaticJsonDocument<128> jsonDocument;
char jsonMessage[128]; //MQTT message max 128char


void setup() {
Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  mqttClient.setServer(MQTThost, MQTTport);

   sht31.begin(); 
   Wire.begin();
    
    scd30.initialize();


while(!gas.begin())
  {
    Serial.println("No device found");
    delay(1000);
  }
  Serial.println("The device connected successfully!");

  gas.changeAcquireMode(gas.PASSIVITY);
  delay(1000);

  gas.setTempCompensation(gas.ON);

}

void loop() {

  while (WiFi.status() != WL_CONNECTED || !mqttClient.connected()) {
    digitalWrite(LED_BUILTIN, LOW);  
    while(WiFi.status() != WL_CONNECTED) {
      WiFi.begin(WIFIssid, WIFIpass);
      delay(5000); 
    }

  if(!mqttClient.connect(""))
    delay(5000); 
  else 
    digitalWrite(LED_BUILTIN, HIGH);   
    
  } 

  float temp = sht31.getTemperature(); 
  publishMQTTJsonMessage("temperature", "°C", temp, " "); //last parameter topic

  float hum = sht31.getHumidity(); 
  publishMQTTJsonMessage("humidity", "%", hum, " ");


  float result[3] = {0};
  if (scd30.isAvailable()) {
        scd30.getCarbonDioxideConcentration(result);
    }

  publishMQTTJsonMessage("CO2", "ppm", result[0], " ");

  float CO = gas.readGasConcentrationPPM();


publishMQTTJsonMessage("CO", "ppm", CO, " "); 

  delay(5000); 

    
}

 void publishMQTTJsonMessage(char type[],char unit[], float value, char topic[]){
  
  StaticJsonDocument<128> jsonDocument; 
  char jsonMessage[128]; 

  jsonDocument["type"] = type; 
  jsonDocument["unit"] = unit;
  jsonDocument["value"] = value;

  serializeJson(jsonDocument, jsonMessage);
  mqttClient.publish(topic, jsonMessage); 

}
