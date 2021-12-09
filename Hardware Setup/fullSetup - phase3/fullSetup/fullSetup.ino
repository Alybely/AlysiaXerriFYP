#include <MKRNB.h>
#include "ThingsBoard.h"

#define TOKEN               "YtWxg4Vqudk7SwFvaY5o"  //change this accordingly
#define THINGSBOARD_SERVER  "demo.thingsboard.io"   //change this accordingly
#define NB_APN     "ais.iot"

// initialize the library instances
NBClient client(false); // use true for debug mode, use false to connect to the server
GPRS gprs;
NB nbAccess;
ThingsBoard tb(client);
NBModem modem;
int incomingByte = 0; // for incoming serial data

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  Serial1.begin(9600); //open serial port for receiving data from AudioMoth

  Serial.println("Starting Full System Test");

  boolean notConnected = true;
  Serial.println("Connecting...");

  while (notConnected) {
    if ((nbAccess.begin("", NB_APN) == NB_READY) && (gprs.attachGPRS() == GPRS_READY)) {
      notConnected = false;
      Serial.println("Connected!");
    } else {
      Serial.println("Error! No connection");
    }
  }

  if ((tb.connect(THINGSBOARD_SERVER, TOKEN)) == 1) {
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN);
  }

  else {
    // no connection to the server
    Serial.println("Connection failed");
  }
}

void loop () {
  if (Serial1.available() > 0) {
    incomingByte = Serial1.read();
    Serial.print("I recieved:");
    Serial.println(incomingByte);

    tb.sendTelemetryInt("Data", (incomingByte));
    delay(2000);
  }
}
