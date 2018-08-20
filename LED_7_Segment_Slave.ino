#include <I2C_Anything.h>
#include <SPI.h>
#include<Wire.h>

#define   LOAD  10
#define   MOSI  11
#define   MISO  12
#define   SCK   13
//#define   SERIAL_DEBUG

void disp(uint8_t, uint8_t);
uint32_t i2c_data = 0;

void setup() {
  Wire.begin(5);
  Wire.onReceive(reciveEvent);
#ifdef SERIAL_DEBUG
  Serial.begin(9600);
  Serial.println("HEllo");
#endif

  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2);

  pinMode(LOAD, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(MISO, INPUT);
  digitalWrite(LOAD, HIGH);
}
uint8_t dg[] {
  0b00111111,//0
  0b00000110,//1
  0b01011011,//2
  0b01001111,//3
  0b01100110,//4
  0b01101101,//5
  0b01111101,//6
  0b00000111,//7
  0b01111111,//8
  0b01101111,//9
  0b10000000//DP
  //pgfedbca
};

int dataArray[] = {0, 0, 0, 0, 0, 0, 0, 0};
void dataToArray(uint32_t data) {       //Splits the digits and feeds into dataArray
  for (int i = 0; i <= 7; i++) {
    if (data <= 0)
      dataArray[i] = 0;
      
    dataArray[i] = data % 10;
    data /= 10;
  }
}

int counter = 0;
uint16_t selector = 1;

void reciveEvent(int howMany) {
  if (howMany >= (sizeof i2c_data)) {
    I2C_readAnything (i2c_data);
    //    Serial.print("in i2c");
    //    Serial.println(i2c_data);
  }
}

void loop() {
  disp(dataArray[counter], selector); //0b10000000
  selector <<= 1;
  counter++;
  counter %= 8;
  selector %= 255;
  dataToArray(i2c_data);
  delay(1);
}
void disp(uint8_t digit = 0, uint8_t place = 0) {
  digitalWrite(LOAD, LOW);
  SPI.setBitOrder(MSBFIRST);
  SPI.transfer(place);//transfer place
  SPI.transfer(dg[digit]);//transfer digit
  digitalWrite(LOAD, HIGH);
}
