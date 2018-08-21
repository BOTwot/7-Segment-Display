#include <I2C_Anything.h>
#include<Wire.h>
float voltage = 0;
const int slaveAddress = 5;
void setup() {
  Wire.begin();
  pinMode(A1, INPUT);
  //  Serial.begin(9600);
}
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
void loop() {
  float reading = analogRead(A1);
  voltage = mapfloat(reading, 0, 1023, 0, 12);
  Wire.beginTransmission(slaveAddress);
  I2C_writeAnything (voltage);
  Wire.endTransmission();

}
