// DEC M9312 ROM/Terminator Card

#include <Arduino.h>
#include <avr/pgmspace.h>

class M9312 {

  private:
    const uint16_t *_ROML;
    const uint16_t *_ROMH1;
    const uint16_t *_ROMH2;
    const uint16_t *_ROMH3;
    const uint16_t *_ROMH4;
    
  public:
    M9312(const uint16_t (*ROML),const uint16_t (*ROMH1),const uint16_t (*ROMH2),const uint16_t (*ROMH3),const uint16_t (*ROMH4));
    void init();
    unsigned int read(unsigned int address);
    boolean here(unsigned int address);
    void event();
  
};
