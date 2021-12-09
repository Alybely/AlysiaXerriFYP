#include <MKRNB.h>

#define NB_APN     "ais.iot"

GPRS gprs;
NB nbAccess;
NBModem modem;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  Serial.println("Starting Arduino Connectivity Test");
  
  Serial.println("Starting modem test...");
  if (modem.begin()) {
    Serial.println("Modem connected!");
  } else {
    Serial.println("Error! No answer from modem");
  }
  
  boolean notConnected = true;
  Serial.println("Starting connection test...");

  while (notConnected) {
    if ((nbAccess.begin("", NB_APN) == NB_READY) && (gprs.attachGPRS() == GPRS_READY)) {
      notConnected = false;
      Serial.println("Connected!");
    } else {
      Serial.println("Error! No connection");
    }
  }
}

void loop() {
  delay(1000);
}
