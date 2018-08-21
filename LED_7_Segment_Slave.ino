#include <I2C_Anything.h>
#include <SPI.h>
#include<Wire.h>

#define   LOAD  10
#define   MOSI  11
#define   MISO  12
#define   SCK   13
//#define   SERIAL_DEBUG

float decimal = 0;
void disp(uint8_t, uint8_t);
uint32_t integer = 0;

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

uint8_t dg1[] {
  0b10111111,//0.
  0b10000110,//1.
  0b11011011,//2.
  0b11001111,//3.
  0b11100110,//4.
  0b11101101,//5.
  0b11111101,//6.
  0b10000111,//7.
  0b11111111,//8.
  0b11101111,//9.
  0b10000000//DP
  //pgfedbca
};

int dataArray[] = {0, 0, 0, 0, 0, 0, 0, 0};
void dataToArray(uint32_t decimal, uint32_t integer) {      //Splits the digits and feeds into dataArray
  int i = 0;
  int j = 4;
  while (i != 4) {
    if (integer <= 0)
      dataArray[i] = 0;

    dataArray[i] = integer % 10;
    integer /= 10;
    i++;
  }
  while (j != 8) {
    if (decimal <= 0)
      dataArray[j] = 0;

    dataArray[j] = decimal % 10;
    decimal /= 10;
    j++;
  }
}

int counter = 0;
uint16_t selector = 1;

void reciveEvent(int howMany) {
  if (howMany >= (sizeof decimal)) {
    I2C_readAnything (decimal);
    integer=decimal;
    decimal-=integer;
    decimal*=10000;
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
  dataToArray(integer, decimal);
  delay(1);
}
void disp(uint8_t digit = 0, uint8_t place = 0) {
  digitalWrite(LOAD, LOW);
  SPI.setBitOrder(MSBFIRST);
  SPI.transfer(place);//transfer place
  if (counter == 4)
    SPI.transfer(dg1[digit]);//transfer digit
  else
    SPI.transfer(dg[digit]);//transfer digit
  digitalWrite(LOAD, HIGH);
}
