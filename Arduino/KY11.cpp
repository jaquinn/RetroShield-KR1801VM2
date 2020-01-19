// DEC KY11 Operators Console

#include "KY11.H"

KY11::KY11(unsigned int address,unsigned int value) {
  this->value=value;
  this->address=address;
  init();
}

void KY11::init() {
}

void KY11::write(unsigned int address, unsigned int value) {
    if (address == this->address) {
      this->value = value;         
    }
}

unsigned int KY11::read(unsigned int address) {
    if (address == this->address) {
      return this->value;      
    }
}

boolean KY11::here(unsigned int address) {
  return((this->address == address));
}

void KY11::event() {
}
