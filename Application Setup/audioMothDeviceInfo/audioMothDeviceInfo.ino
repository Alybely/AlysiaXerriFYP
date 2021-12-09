#include <MKRNB.h>
#include "ThingsBoard.h"
#include <math.h>

#define TOKEN               "YtWxg4Vqudk7SwFvaY5o"  //change this accordingly
#define THINGSBOARD_SERVER  "demo.thingsboard.io"   //change this accordingly
#define NB_APN     "ais.iot"

// initialize the library instances
NBClient client(false); // use true for debug mode, use false to connect to the server
GPRS gprs;
NB nbAccess;
ThingsBoard tb(client);

int incomingByte = 0; // for incoming serial data
char deviceName;
int nextByte = 0;
void setup() {
  // initialization
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  Serial1.begin(9600); // opens serial port, sets data rate to 9600 bps

  Serial.println("Sending AudioMoth Device Information");
  Serial.println("Connecting...");

  // connection state
  boolean notConnected = true;

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
  int a = 0;
  int data[100];
  int samplerate = 0;
  int threshold = 0;

  if (Serial1.available() > 0) {
    incomingByte = Serial1.read();

    if (incomingByte == 38) {
      while (incomingByte != 36) {
        incomingByte = Serial1.read();

        if  (incomingByte > -1) {
          data[a] = incomingByte;
          a++;
        }
      }

      int size = a;
      int b = a - 1;

      for (int i = 0; i < size; i++) {
        if (data[i] != 0) {
          data[i] = data[i] * pow(10, (size - 2));
        }
        size--;
      }

      for (int j = 0; j < b; j++) {
        samplerate += data[j];
      }

      if (samplerate > 0) {
        Serial.print("Sample Rate is: ");
        Serial.println(samplerate);
        tb.sendTelemetryInt("Sample Rate", samplerate);
      }
    }

    else if (incomingByte == 42) {
      while (incomingByte != 36) {
        incomingByte = Serial1.read();

        if  (incomingByte > -1) {
          data[a] = incomingByte;
          a++;
        }
      }

      int size = a;
      int b = a - 1;

      for (int i = 0; i < size; i++) {
        if (data[i] != 0) {
          data[i] = data[i] * pow(10, (size - 2));
        }
        size--;
      }

      for (int j = 0; j < b; j++) {
        threshold += data[j];
      }

      if (threshold > 0) {
        Serial.print("Threshold is: ");
        Serial.println(threshold);
        tb.sendTelemetryInt("Threshold", threshold);
      }
    }
  }
}
