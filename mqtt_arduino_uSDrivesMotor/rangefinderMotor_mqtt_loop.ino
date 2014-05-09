/*
MQTT example using US Rangefinder (SRF05) and Stepper Motor rotation
  - file: "rangefinder_mqtt.ino", v0.99 (GitHub ready)
  - connects to an MQTT server, either providing username
    and password (MQTT v3.1) or w/o them (version <= MQTT v3.0)
  - publishes the US rangefinder reading to a user-specified topic,
      subscribes to it, and adjusts a stepper-motor rotation speed 
      according to how close an object is to the rangefinder
      
  - developed by German Kutsy, Litmus Automation LLC. (2014)
*/
#include <NewPing.h>
#include <PubSubClient.h>
#include <YunClient.h>
#include <Bridge.h>
#include <Stepper.h>

YunClient yun;
NewPing sonar;
Stepper stepper(STEPS, 8, 9);

void callback(char *topic, byte *payload, unsigned int length) {
    // separate application thread
}

uint8_t server[] = {107,170,104,14};  // ny1.litmusloop.com
uint16_t port = 1883;                 // default port

char* username = "rAnDoM_chars";      // user input reqd
char* password= "kgrem142";           // user input reqd
char* clientId = "Cliengratzi_2343";  // user input reqd
char* projectId = "Mosquitto";        // user input reqd
char* deviceName = "yun";             // user input reqd

char* jsonKV = "not used in this example";
boolean cleanSession = true;

// no username, no password (version <= MQTT v3.0)
PubSubClient litmus(server,port,callback,yun,clientId,projectId,deviceName,jsonKV,cleanSession);
// username/password used (MQTT v3.1)
//PubSubClient litmus(server,port,callback,yun,username,password,clientId,projectId,deviceName,jsonKV,cleanSession);
    

void setup() {
  Bridge.begin();
  litmus.loop();
  litmus.connect();
  stepper.setSpeed(30);
}

void loop() {
  delay(250);
  
  litmus.publish("stream", sonar.distToString()); // pub the dist in cm
  
  if (litmus.subscribe("stream")) { // can we subscribe to the same stream?
    stepper.step(sonar.distToInt()); // step the (distance * 1.8)deg rotations
  }
}
