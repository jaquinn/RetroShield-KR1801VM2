// DEC M7856 DL11-W Serial IO Card

#include <Arduino.h>

class M7856 {

  private:
    unsigned int rxstatusaddress;
    unsigned int rxdataaddress;
    unsigned int txstatusaddress;
    unsigned int txdataaddress;
    unsigned int ltcaddress;
    
    unsigned int rxstatus;
    unsigned int rxdata;
    unsigned int txstatus;
    unsigned int txdata;

    uint8_t usart;

  public:
    M7856(uint8_t usart,unsigned int baseaddress, unsigned int baud);
    void init();
    void write(unsigned int address, unsigned int value);
    unsigned int read(unsigned int address);
    boolean here(unsigned int address);
    void event();
  
};
