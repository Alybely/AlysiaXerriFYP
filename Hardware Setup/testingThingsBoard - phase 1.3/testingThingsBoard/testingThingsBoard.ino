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

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  Serial.println("Starting ThingsBoard Test");

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
  int i = 0;
  for (i=0; i< 10; i++){
      tb.sendTelemetryInt("Integer", (i));
      Serial.print("Sending integer: ");
      Serial.println(i);
      delay(4000);
    }
}
