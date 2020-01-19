// DEC KY11 Operators Console

#include <Arduino.h>

class KY11 {

  private:
    unsigned int value;
    unsigned int address;

  public:
    KY11(unsigned int address, unsigned int value);
    void init();
    void write(unsigned int address, unsigned int value);
    unsigned int read(unsigned int address);
    boolean here(unsigned int address);
    void event();
  
};
