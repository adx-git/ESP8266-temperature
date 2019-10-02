This is an (easy) example project for the ESP8266 to measure temperatures and send the value to a MQTT server/broker.
It is based on the original script and instructions from https://www.smarthome-tricks.de/allgemein/nodemcu-esp8266-sensoren-an-iobroker-ueber-mqtt/
Since this script had some failures and was using an outdated and undefined DHT library, we changed the library, cleaned up the code (sill ongoing) and port the project to PlatformIO

## Usage
* modify the content of main_cfg.h.template to fit your configuration
* rename main_cfg.h.template to main_cfg.h
* build project
* enjoy 

## Current program flow
* initiate WLAN
* connect to MQTT broker
* read temperature and humidity
* send readings to MQTT broker
* repeat

## Missing and ToDO
* Code clean up
* add a more flexible configuration
* add further instructions and pictures about ESP8266 connectivity
* add instructions about MQTT broker settings
* profide example for IOBroker
* add error handling
* Send ESP8266 into deep sleep
* and more ...
