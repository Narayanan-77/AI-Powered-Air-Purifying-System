#include "MQ135.h"

const int MQ135_1 = A0;
const int MQ135_2 = A1;
MQ135 gasSensor1(MQ135_1);
MQ135 gasSensor2(MQ135_2);

const float CALIBRATED_RZERO = 61.27; // Your calibrated value

void setup() {
  Serial.begin(9600);
  gasSensor1.setRZero(CALIBRATED_RZERO);
  gasSensor2.setRZero(CALIBRATED_RZERO);
}

void loop() {
  float ppm1 = gasSensor1.getPPM();
  float ppm2 = gasSensor2.getPPM();
  Serial.print(ppm1, 1); Serial.print(","); Serial.println(ppm2, 1);
  delay(2000);
}
