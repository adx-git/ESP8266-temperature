#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include "main_cfg.h"
//#include <DHT_U.h>

/* Initiate sensor */
DHT dht(CFG_DHTPIN, CFG_DHTTYPE);


/* WLAN configuration */
const char* ssid = CFG_SSID;
const char* password = CFG_PWD;

/*  MQTT Server 
    Note: The implementation is expecting the MQTT broker at port 1883
*/
const char* mqtt_server = CFG_MQTT_SERVER;

WiFiClient espClient;
PubSubClient mqtt_client(espClient);
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

void mqtt_connect() 
{
  // Loop until we're reconnected
  while (!mqtt_client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = CFG_CLIENT_PREFIX;
    clientId += "_ESP8266-DHT22";

#ifdef CFG_MQTT_USER
    if (mqtt_client.connect(clientId.c_str(),CFG_MQTT_USER,CFG_MQTT_PWD))
#else
    if (mqtt_client.connect(clientId.c_str()))
#endif
    {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 2 seconds");
      // Wait 6 seconds before retrying
      delay(2000);
    }
  }
}

void setup() 
{
  Serial.begin(115200);
  dht.begin();
  setup_wifi();
  mqtt_client.setServer(mqtt_server, 1883);
}

void loop() 
{
  if (!mqtt_client.connected()) {
    mqtt_connect();
  }
  
  mqtt_client.loop();
  long now = millis();
  
  // read DHT sensor every 10 seconds
  /* TODO: this looks like a potential overflow, will be different once we send the device into deep sleep */
  if (now - lastMsg > 10000) {
    lastMsg = now;
    
    String msg="";
    const char TempTopic[] = {CFG_TOPIC_PREFIX  "Temperature"};
    const char HumidityTopic[] = {CFG_TOPIC_PREFIX  "Humidity"};
    char MsgTemp[25];
    char MsgHumidity[25];
    float temperature, humidity;    
 
    temperature = dht.readTemperature();
    msg = String(temperature);
    msg.toCharArray(MsgTemp,25); 
         
    humidity = dht.readHumidity();
    msg = String(humidity);
    msg.toCharArray(MsgHumidity,25);     
          
    mqtt_client.publish(TempTopic, MsgTemp);
    Serial.print(MsgTemp);
    mqtt_client.publish(HumidityTopic, MsgHumidity);
    Serial.print(MsgHumidity);
  }
}