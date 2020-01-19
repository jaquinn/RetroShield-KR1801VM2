// DEC M9312 ROM/Terminator Card

#include "M9312.H"

#define M9312_ROM_LOW_START   0165000
#define M9312_ROM_LOW_END     0165776
#define M9312_ROM_HIGH1_START 0173000
#define M9312_ROM_HIGH1_END   0173177
#define M9312_ROM_HIGH2_START 0173200
#define M9312_ROM_HIGH2_END   0173377
#define M9312_ROM_HIGH3_START 0173400
#define M9312_ROM_HIGH3_END   0173577
#define M9312_ROM_HIGH4_START 0173600
#define M9312_ROM_HIGH4_END   0173777

M9312::M9312(const uint16_t (*ROML),const uint16_t (*ROMH1),const uint16_t (*ROMH2),const uint16_t (*ROMH3),const uint16_t (*ROMH4)) {
  this->_ROML = ROML;
  this->_ROMH1 = ROMH1;
  this->_ROMH2 = ROMH2;
  this->_ROMH3 = ROMH3;
  this->_ROMH4 = ROMH4;
  init();
}

void M9312::init() {
}

unsigned int M9312::read(unsigned int address) {
  unsigned int data;
  unsigned int offset;
  if ((address >= M9312_ROM_LOW_START) && (address <= M9312_ROM_LOW_END)) {
    offset = (address - M9312_ROM_LOW_START) / 2;
    data = pgm_read_word(_ROML+offset);    
  } else if ((address >= M9312_ROM_HIGH1_START) && (address <= M9312_ROM_HIGH1_END)) {
    offset = (address - M9312_ROM_HIGH1_START) / 2;
    data = pgm_read_word(_ROMH1+offset);    
  } else if ((address >= M9312_ROM_HIGH2_START) && (address <= M9312_ROM_HIGH2_END)) {
    offset = (address - M9312_ROM_HIGH2_START) / 2;
    data = pgm_read_word(_ROMH2+offset);    
  } else if ((address >= M9312_ROM_HIGH3_START) && (address <= M9312_ROM_HIGH3_END)) {
    offset = (address - M9312_ROM_HIGH3_START) / 2;
    data = pgm_read_word(_ROMH3+offset);    
  } else if ((address >= M9312_ROM_HIGH4_START) && (address <= M9312_ROM_HIGH4_END)) {
    offset = (address - M9312_ROM_HIGH4_START) / 2;
    data = pgm_read_word(_ROMH4+offset);    
  } else {
    data = 0;
  }
  return(data);
}

boolean M9312::here(unsigned int address) {
  return(((address >= M9312_ROM_LOW_START) && (address <= M9312_ROM_LOW_END)) || ((address >= M9312_ROM_HIGH1_START) && (address <= M9312_ROM_HIGH4_END)));
}

void M9312::event() {
}
