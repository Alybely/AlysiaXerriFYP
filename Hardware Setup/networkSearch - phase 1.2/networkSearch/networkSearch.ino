#include <MKRNB.h>
#include "Modem.h"

NB nbAccess;
NBScanner scannerNetworks;
GPRS gprs;
int count = 0;
void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  Serial.println("NB-IoT Signal Strength Test");
  
  boolean notConnected = true;
  Serial.println("Connecting...");

  while (notConnected) {
    if ((nbAccess.begin("", "ais.iot") == NB_READY) && (gprs.attachGPRS() == GPRS_READY)) {
      notConnected = false;
    Serial.println("Connected!");
    } else {
      Serial.println("Error! No connection");
    }
  }

  Serial.print("Current carrier: ");
  Serial.println(scannerNetworks.getCurrentCarrier());
}

void loop() {
  delay(3000);
  
//  Serial.print("Signal strength: ");
  Serial.println(scannerNetworks.getSignalStrength());
  count++;

  if (count == 400) {
      Serial.println("Data Collected!");
    }
}
