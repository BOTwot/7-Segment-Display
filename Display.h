#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <Arduino.h>
#include <Wire.h>

template <typename T> unsigned int I2C_writeAnything (const T& value)
  {
    const byte * p = (const byte*) &value;
    unsigned int i;
    for (i = 0; i < sizeof value; i++)
          Wire.write(*p++);
    return i;
  }  // end of I2C_writeAnything

template <typename T> unsigned int I2C_readAnything(T& value)
  {
    byte * p = (byte*) &value;
    unsigned int i;
    for (i = 0; i < sizeof value; i++)
          *p++ = Wire.read();
    return i;
  }  // end of I2C_readAnything

  class Display
  {
  private:
    uint8_t _addr;
  public:
    Display(uint8_t addr){
        _addr=addr;
    }

    bool begin(){
        Wire.begin();
        return true;
    }

    void write(int32_t value){

        Wire.begintransmission(_addr);
        I2C_writeAnything(value);
        Wire.endTransmission();
    
    }
    void write(float value){
        
        Wire.begintransmission(_addr);
        I2C_writeAnything(value);
        Wire.endTransmission();
            
        }
    }
