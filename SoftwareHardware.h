#ifndef SOFTWARE_HARDWARE_H_
#define SOFTWARE_HARDWARE_H_

#include <Arduino.h>
#include "SoftwareSerial.h"

class SoftwareHardware {
public:
  SoftwareHardware(){}
  
  void init()
  {
    serial_ = SoftwareSerial(10, 11);
    baud_ = 115200;
    serial_.begin(baud_);
  }
  
  int read(){
    return serial_.read();
  }

  void write(uint8_t* data, int length)
  {
    serial_.write(&data);
  }

  unsigned long time()
  {
    return millis();
  }

protected:
  SoftwareSerial& serial_;
  unsigned long baud_;
};

#endif
