#include <MKRNB.h>
#include "ThingsBoard.h"

#define TOKEN               "YtWxg4Vqudk7SwFvaY5o"  //change this accordingly
#define THINGSBOARD_SERVER  "demo.thingsboard.io"   //change this accordingly
#define NB_APN     "ais.iot"

#define interceptConst   -59344.0386
#define dbConst   2934.170122

// initialize the library instances
NBClient client(false); // use true for debug mode, use false to connect to the server
GPRS gprs;
NB nbAccess;
ThingsBoard tb(client);
NBModem modem;

 int hist_count;

  float tot_hist;
  float avrg_hist;

int8_t incomingByte = 0; // for incoming serial data
int count = 0;
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
  uint8_t splitValue[2];
  uint8_t MSB = 0;
  uint8_t LSB = 0;
  int z = 0;
  float dbAvrg = 0;
  float totDB = 0;
  float finalValue;
  int firstByte = 0;
  int data[100];
  int samplerate = 0;
  int threshold = 0;
  float hist_arr[1000];
  int avrgCount = 0;
  float totAvrg = 0;
  float totTimeAvrg = 0;
 
                          
  if (Serial1.available() > 0) {

    firstByte = Serial1.read();
    // Serial.println(firstByte);
    if (firstByte == 35) {
      //   Serial.println("audio");
      do {
        uint8_t x = Serial1.read();

        if (x > 0) {
          if (x != 10) {
            splitValue[z] = x;
            z++;
          }
        }
      } while ( z < 2);

      MSB = splitValue[0];
      LSB = splitValue[1];

      int join = (MSB << 8) | LSB;

      if (count < 8) {
        dbValues[count] = join;
      }

      if (count == 7) {
        for (int k = 0; k < 8; k++) {
          totDB = totDB + dbValues[k];
          dbAvrg = totDB / 8;
        }

        // Serial.print("Average ADC Value: ");
        //  Serial.println(dbAvrg);

        finalValue = (dbAvrg - interceptConst) / dbConst;
        Serial.print("Average dB Level: ");
        Serial.println(finalValue);

        if  (finalValue > 0 && finalValue < 400) {
          tb.sendTelemetryFloat("Average dB Level", (finalValue));

            if (finalValue >= 30) {
              hist_arr[hist_count] = finalValue;
              hist_count++;
              Serial.print("Hysteresis value: ");
              Serial.println(finalValue);
              Serial.println(hist_count);

              if (hist_count == 10){
                for (int h = 0; h < 10; h++) {
                          tot_hist = tot_hist + hist_arr[h];
                          avrg_hist = tot_hist / 10;
                        }
                tb.sendTelemetryFloat("Average Hysteresis value", (avrg_hist));
                Serial.print("Final Hysteresis value: ");
                Serial.println(avrg_hist);
                hist_count = 0;
                hist_arr[1000] = 0;
                tot_hist = 0;
                avrg_hist = 0;
                }
            }

            else {
              hist_arr[1000] = 0;
              hist_count = 0;
            }
          }
        
      }
      count++;
    }

    if (count == 8) {
      for (int l = 0; l < 8; l++) {
        dbValues[l] = 0;
      }
      dbAvrg = 0;
      totDB = 0;
      count = 0;
      //  Serial.println("RESET");
    }

    else if (firstByte == 38) {
      // Serial.println("sample");

      int a = 0;

      while (firstByte != 36) {
        firstByte = Serial1.read();
        // Serial.println(firstByte);

        if  (firstByte > -1) {
          data[a] = firstByte;
          // Serial.println(data[a]);
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
      //  Serial.print("Sample Rate is: ");
      //  Serial.println(samplerate);
        tb.sendTelemetryInt("Sample Rate", samplerate);
      }
    }

    else if (firstByte == 42) {
      // Serial.println("threshold");

      int a = 0;

      while (firstByte != 36) {
        firstByte = Serial1.read();

        if  (firstByte > -1) {
          data[a] = firstByte;
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
      //  Serial.print("Threshold is: ");
      //  Serial.println(threshold);
        tb.sendTelemetryInt("Threshold", threshold);
      }
    }

  }
  delay(50);
}
