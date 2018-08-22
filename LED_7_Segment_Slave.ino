#include <I2C_Anything.h>
#include <SPI.h>
#include<Wire.h>

#define   LOAD  10
#define   MOSI  11
#define   MISO  12
#define   SCK   13
//#define   SERIAL_DEBUG

float i2cData = 0;
float pointValue = 0;
uint32_t integer = 0;
bool displayMinus = false, receivingData = false;
int counter = 0;
uint16_t selector = 1;
void disp(uint8_t, uint8_t);
void dispNoI2C(uint8_t, uint8_t);

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
  0b10000000,//DP
  0b01000001,//-
  0b00000000//NIL
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
};

uint8_t noI2C[] {
  0b01010100,// n
  0b11011100,//o
  0b00000110,//I
  0b01011011,//2
  0b10111001,//C
  0b01011110,//d
  0b01111001,//E
  0b10111110//V
};

int dataArray[] = {0, 0, 0, 0, 0, 0, 0, 0};
int noI2CMsg[] = {7, 6, 5, 4, 3, 2, 1, 0};
void dataToArray(uint32_t integer, uint32_t pointValue) {      //Splits the digits and feeds into dataArray
  int i = 0;
  int j = 3;
  while (i != 4) {
    if (pointValue <= 0)
      dataArray[i] = 0;

    dataArray[i] = pointValue % 10;
    pointValue /= 10;
    i++;
  }
  while (j != 8) {
    if (integer <= 0)
      dataArray[j] = 0;

    dataArray[j] = integer % 10;
    integer /= 10;
    j++;
  }
}

void reciveEvent(int howMany) {
  if (howMany >= (sizeof i2cData)) {
    I2C_readAnything (i2cData);
    //    Serial.print("in i2c");
    //    Serial.println(i2cData);
  }
}
void splitNum(float i2cData) {
  pointValue = i2cData;

  if (pointValue < 0)
    displayMinus = true;
  else
    displayMinus = false;
  pointValue = abs(pointValue);
  integer = pointValue;
  pointValue -= integer;
  pointValue *= 1000;
  //  Serial.println(integer);
  //  Serial.println(pointValue);
  //  Serial.println(displayMinus);
}

bool noI2CData() {
  if (!receivingData)
    return true;
  else
    return false;
}

void loop() {
  while (noI2CData()) {
    dispNoI2C(noI2CMsg[counter], selector);
    //    receivingData = false;
    selector <<= 1;
    counter++;
    counter %= 8;
    selector %= 255;
    delay(1);
    //    Serial.println("No I2C Devices Found!!!!!");
  }
  disp(dataArray[counter], selector); //0b10000000
  //  receivingData = false;
  selector <<= 1;
  counter++;
  counter %= 8;
  selector %= 255;
  splitNum(i2cData);
  dataToArray(integer, pointValue);
  delay(1);
}
void disp(uint8_t digit = 0, uint8_t place = 0) {
  digitalWrite(LOAD, LOW);
  SPI.setBitOrder(MSBFIRST);
  SPI.transfer(place);//transfer place
  if (counter == 7 && displayMinus == true ) {
    SPI.transfer(dg[11]);//transfer -
  }
  else if (counter == 7 && displayMinus == false  )
    SPI.transfer(dg[12]);//transfer NIL
  else if (counter == 3)
    SPI.transfer(dg1[digit]);//transfer digit with point
  else
    SPI.transfer(dg[digit]);//transfer digit
  digitalWrite(LOAD, HIGH);
}

void dispNoI2C(uint8_t digit = 0, uint8_t place = 0) {
  digitalWrite(LOAD, LOW);
  SPI.setBitOrder(MSBFIRST);
  SPI.transfer(place);//transfer place
  SPI.transfer(noI2C[digit]);
  digitalWrite(LOAD, HIGH);
}

