#include <MKRNB.h>
#include <math.h>

#define NB_APN     "ais.iot"

// initialize the library instances
NBClient client(false); // use true for debug mode, use false to connect to the server
GPRS gprs;
NB nbAccess;

int incomingByte = 0; // for incoming serial data
int nextByte = 0;
void setup() {
  // initialization
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  Serial1.begin(9600); //open serial port for receiving data from AudioMoth

  Serial.println("Starting Data From AudioMoth Test");

  // connection state
  boolean notConnected = true;
  Serial.println("Connecting...");

  while (notConnected) {
    if ((nbAccess.begin("", NB_APN) == NB_READY) && (gprs.attachGPRS() == GPRS_READY)) {
      notConnected = false;
      Serial.println("Connected!");
    } else {
      Serial.println("Error! No connection");
      // delay(1000);
    }
  }
}

void loop () {
  if (Serial1.available() > 0) {
    incomingByte = Serial1.read();
     //   if  (incomingByte > -1) {
          Serial.print("I recieved:");
          Serial.println(incomingByte);
     //   }
  }
  delay(200);
}
