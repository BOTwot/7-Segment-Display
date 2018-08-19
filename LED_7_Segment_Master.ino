#include <I2C_Anything.h>
#include<Wire.h>

const int slaveAddress = 5;
void setup() {
  Wire.begin();
  //  Serial.begin(9600);
}

void loop() {
  uint32_t i = 0;
  for (i = 0; i < 10000000; i++) {
    Wire.beginTransmission(slaveAddress);
    I2C_writeAnything (i);
    Wire.endTransmission();
  }
}
