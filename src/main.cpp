#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include "main_cfg.h"
//#include <DHT_U.h>

/* Initiate sensor */
DHT dht(CFG_DHTPIN, CFG_DHTTYPE);


// Update these with values suitable for your network.
const char* ssid = CFG_SSID;
const char* password = CFG_PWD;
const char* mqtt_server = CFG_MQTT_SERVER;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {
   delay(100);
  // We start by connecting to a WiFi network
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() 
{
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    //if you MQTT broker has clientID,username and password
    //please change following line to    if (client.connect(clientId,userName,passWord))
    //if (client.connect(clientId.c_str()))
    if (client.connect(clientId.c_str(),CFG_MQTT_USER,CFG_MQTT_PWD))
    {
      Serial.println("connected");
     //once connected to MQTT broker, subscribe command if any
      client.subscribe("OsoyooCommand");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 6 seconds before retrying
      delay(6000);
    }
  }
}

void setup() 
{
  Serial.begin(115200);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() 
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  // read DHT11 sensor every 6 seconds
  if (now - lastMsg > 10000) {
     lastMsg = now;
    
     String msg="";
     char MsgTemp[25];
     char MsgFeutigkeit[25];
     float temperature, humidity;    
 
     temperature = dht.readTemperature();
     msg = String(temperature);
     msg.toCharArray(MsgTemp,25); 
         
     humidity = dht.readHumidity();
     msg = String(humidity) + "%";
     msg.toCharArray(MsgFeutigkeit,25);     
          
     client.publish("Temp", MsgTemp);
     Serial.print(MsgTemp);
     client.publish("Feuchtigkeit", MsgFeutigkeit);
     Serial.print(MsgFeutigkeit);

  }
}