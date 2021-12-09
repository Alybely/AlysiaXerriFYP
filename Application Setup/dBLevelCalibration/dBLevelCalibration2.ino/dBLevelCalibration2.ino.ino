#include <MKRNB.h>
#include "ThingsBoard.h"

#define TOKEN               "YtWxg4Vqudk7SwFvaY5o"  //change this accordingly
#define THINGSBOARD_SERVER  "demo.thingsboard.io"   //change this accordingly
#define NB_APN     "ais.iot"

#define interceptConst   -23145.82582
#define dbConst   1154.514113

// initialize the library instances
NBClient client(false); // use true for debug mode, use false to connect to the server
GPRS gprs;
NB nbAccess;
ThingsBoard tb(client);
NBModem modem;

int8_t incomingByte = 0; // for incoming serial data
int i = 0;
void setup() {
  // initialization
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  Serial1.begin(9600); // opens serial port, sets data rate to 9600 bps

  Serial.println("Calculating AudioMoth dB Level");
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

  int dbValues[10];
  uint8_t data[2];
  uint8_t MSB = 0;
  uint8_t LSB = 0;
  int a = 0;
  float dbAvrg = 0;
  float totDB = 0;
  float finalValue;
  int firstByte = 0;

  if (Serial1.available() > 0) {

    firstByte = Serial1.read();
    if (firstByte == 35) {
      do {
        uint8_t x = Serial1.read();

        if (x > 0) {
          if (x != 10) {
            data[a] = x;
            a++;
          }
        }
      } while (a < 2);

      MSB = data[0];
      LSB = data[1];

      int join = (MSB << 8) | LSB;

      if (i < 10) {
        dbValues[i] = join;
      }

      if (i == 9) {
        for (int k = 0; k < 10; k++) {
          totDB = totDB + dbValues[k];
          dbAvrg = totDB / 10;
        }

        Serial.print("Average ADC Value: ");
        Serial.println(dbAvrg);

        finalValue = (dbAvrg - interceptConst) / dbConst;
        Serial.print("dB Level: ");
        Serial.println(finalValue);

        if  (finalValue > 0 && finalValue < 400) {
          tb.sendTelemetryFloat("Average dB Level", (finalValue));
        }
      }
      i++;
    }

    if (i == 10) {
      for (int j = 0; j < 10; j++) {
        dbValues[j] = 0;
      }
      dbAvrg = 0;
      totDB = 0;
      i = 0;
      Serial.println("RESET");
    }

  }
  delay(50);
}
