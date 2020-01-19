////////////////////////////////////////////////////////////////////
// 1801VM2 RetroShield Code
// 2019/12/22
// Version 0.1
// Andrew Quinn

// The MIT License (MIT)

// Copyright (c) 2019 Andrew Quinn

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


// Compile Time Options

// Define to enable the LCD/Keyboard

//#define USE_LCDKEYPAD 1

// Define for No-ISR cycling

//#define NO_ISR 1

// Define to trace processor cycles - in conjunction with NO_ISR 

//#define CPUTRACE 1

// Define to enable traps

#define TRAP_BUS_ERROR 1

#ifdef USE_LCDKEYPAD
#include <LiquidCrystal.h>
#endif

// Peripherals

#include "M9312.h"
#include "M7856.h"
#include "KY11.h"


////////////////////////////////////////////////////////////////////
// Hardware Configuration
////////////////////////////////////////////////////////////////////

#ifdef USE_LCDKEYPAD

/*
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
*/

#define LCD_RS  8
#define LCD_EN  9
#define LCD_D4  4
#define LCD_D5  5
#define LCD_D6  6
#define LCD_D7  7
#define LCD_BL  10
#define LCD_BTN  0

#define NUM_KEYS   5
#define BTN_DEBOUNCE 10
#define BTN_RIGHT  0
#define BTN_UP     1
#define BTN_DOWN   2
#define BTN_LEFT   3
#define BTN_SELECT 4
const int adc_key_val[NUM_KEYS] = { 30, 180, 360, 535, 760 };
int key = -1;
int oldkey = -1;
boolean BTN_PRESS = 0;
boolean BTN_RELEASE = 0;
#endif

////////////////////////////////////////////////////////////////////
// initialize the library with the numbers of the interface pins
// LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);


////////////////////////////////////////////////////////////////////
// My Variables
////////////////////////////////////////////////////////////////////

int backlightSet = 25;

////////////////////////////////////////////////////////////////////
// ROMs
////////////////////////////////////////////////////////////////////

// Loop Test
 
const PROGMEM uint16_t ROM_LOOP[] = {
0165004,0000200,0000240,0000137,0165004
};

// Write B to Console Test

const PROGMEM uint16_t ROM_SEND_B[] = {
0165004,0000200,032737,000200,0177564,001774,012737,000102,0177566,000770
};

// Modified DEC M9312 ROMs

// Modified DEC M9312 23-248F1 console PROM

const PROGMEM uint16_t M9312_LOW_ROM[] = {
0165004,0000200,0010701,0000554,0010701,0000526,0010400,0000524,
0010600,0010701,0000521,0010500,0000517,0010605,0010701,0000540,
0112702,0000100,0010703,0000554,0010706,0000544,0000302,0000542,
0020227,0046040,0001450,0020402,0001001,0005725,0010204,0020227,
0042440,0001446,0020227,0042040,0001432,0020227,0051415,0001002,
0000005,0000115,0012704,0173000,0031427,0000200,0001323,0022402,
0001405,0061404,0020427,0174000,0001731,0000766,0010701,0000423,
0000005,0113705,0173024,0106105,0106105,0000164,0000010,0010701,
0000412,0010015,0000713,0010701,0000406,0010005,0005004,0000706,
0010506,0011505,0000675,0005000,0005002,0010703,0000453,0120227,
0000015,0001433,0162702,0000070,0062702,0000010,0103357,0006300,
0006300,0006300,0050200,0000760,0012702,0000030,0000261,0006100,
0106102,0010703,0000435,0012702,0020206,0006300,0001403,0106102,
0103774,0000765,0000302,0010703,0000423,0022121,0000161,0177776,
0012702,0014012,0010703,0000414,0061702,0003767,0105002,0152702,
0000015,0000770,0105737,0177560,0100375,0105002,0153702,0177562,
0105737,0177564,0100375,0110237,0177566,0142702,0100200,0022323,
0000163,0177776,0000000,0000000,0000000,0000000,0000000,0000000,
0000000,0000000,0000000,0000000,0000000,0000000,0000000,0000000,
0000000,0000000,0000000,0000000,0000000,0000000,0000000,0000000,
0000000,0000000,0000000,0000000,0000000,0000000,0000000,0000000,
0000000,0000000,0000000,0000000,0000000,0000000,0000000,0000000,
0000000,0000000,0000000,0000000,0000000,0000000,0000000,0000000,
0000000,0000000,0000000,0000000,0000000,0000000,0000000,0000000,
0000000,0000000,0000000,0000000,0000000,0000000,0000000,0000000,
0000000,0000000,0000000,0000000,0000000,0000000,0000000,0000000,
0000000,0000000,0000000,0000000,0000000,0000000,0000000,0000000,
0000000,0000000,0000000,0000000,0000000,0000000,0000000,0000000,
0000000,0000000,0000000,0000000,0000000,0000000,0000000,0000000,
0000000,0000000,0000000,0000000,0000000,0000000,0000000,0000000,
0000000,0000000,0000000,0000000,0000000,0000000,0000000,0000000,
0000000,0000000,0000000,0000000,0000000,0000000,0040460,0123162
};

// DEC M9312 23-765A9 TU-58 Tape Controller

const PROGMEM uint16_t M9312_HIGH1_ROM[] = {
0042104,0000176,0000261,0012700,0000000,0012701,0176500,0010704,
0103054,0000402,0173000,0000340,0012706,0002000,0005004,0012702,
0176504,0005212,0005003,0004767,0173120,0005012,0005737,0176502,
0012703,0004004,0004767,0173124,0010003,0004767,0173126,0005003,
0105711,0100376,0113723,0176502,0022703,0001000,0101371,0005007,
0004717,0004717,0004717,0105712,0100376,0110337,0176506,0000303,
0000207,0000261,0012700,0000001,0000720,0000137,0165564,0000000,
0000000,0000000,0000000,0000000,0000000,0000000,0000000,0125025
};

const PROGMEM uint16_t M9312_BLANK_ROM[] = {
0000000,0000000,0000000,0000000,0000000,0000000,0000000,0000000,
0000000,0000000,0000000,0000000,0000000,0000000,0000000,0000000,
0000000,0000000,0000000,0000000,0000000,0000000,0000000,0000000,
0000000,0000000,0000000,0000000,0000000,0000000,0000000,0000000,
0000000,0000000,0000000,0000000,0000000,0000000,0000000,0000000,
0000000,0000000,0000000,0000000,0000000,0000000,0000000,0000000,
0000000,0000000,0000000,0000000,0000000,0000000,0000000,0000000,
0000000,0000000,0000000,0000000,0000000,0000000,0000000,0000000
};

////////////////////////////////////////////////////////////////////
// MEMORY LAYOUT
////////////////////////////////////////////////////////////////////

// RAM (2 K Words with Mega2560)
#define RAM_START   0000000
#define RAM_END     0004000
unsigned int RAM[RAM_END-RAM_START+1];

// PDP-11 IO Page Base Address
#define IO_PAGE           0170000

// IO Devices Base Addresses
#define CONSOLE_BASE      0177560       // M7858/DL-11W Console SLU
#define KY11_BASE         0177570       // KY11-LB Switch Register
#define TU58_BASE         0176500       // M7858/DL-11W TU-58 SLU
#define BOOT_ADDRESS      0165000       // Address for boot PC/PSW from M9312 - JAQ's Hacked ROM

////////////////////////////////////////////////////////////////////
// Processor Control
////////////////////////////////////////////////////////////////////

/* Digital Pin Assignments */

#define ADDR_H   (PINC)
#define ADDR_L   (PINA)

#define DATA_H_OUT (PORTC)
#define DATA_L_OUT (PORTA)

#define DATA_H_IN (PINC)
#define DATA_L_IN (PINA)

#define DIR_IN  0x00
#define DIR_OUT 0xFF

#define DATA_H_DIR DDRC
#define DATA_L_DIR DDRA
#define ADDR_H_DIR DDRC
#define ADDR_L_DIR DDRA


#define ADDR     ((unsigned int) (ADDR_H << 8 | ADDR_L))
#define DATA_IN  ((unsigned int) (DATA_H_IN << 8 | DATA_L_IN))

// Digital IO Pin Numbers

#define uP_DOUT_N   40    // PG1
#define uP_DIN_N    41    // PG0
#define uP_SYNC_N   39    // PG2
#define uP_AR_N     44    // PL5
#define uP_RPLY_N   46    // PL3
#define uP_WTBT_N   47    // PL2
#define uP_INIT_N   48    // PL1
#define uP_SEL_N    49    // PL0
#define uP_DCLO_N   50    // PB3
#define uP_ACLO_N   51    // PB2
#define uP_CLCI     52    // PB1

// Digital IO Port Bit Masks

// Port L

#define _SEL_N  0x01
#define _INIT_N 0x02
#define _WTBT_N 0x04
#define _RPLY_N 0x08
#define _AR_N 0x20

// Port B

#define _CLCI 0x02
#define _ACLO_N 0x04
#define _DCLO_N 0x08

// Port G

#define _DIN_N 0x01
#define _DOUT_N 0x02
#define _SYNC_N 0x04

// Port D

#define _RPLY_N 0x08
#define _WTBT_N 0x10
// #define _SEL_N 0x40

// Macros for fast GPIO access 

#define CLK_HIGH      (PORTB = PORTB | _CLCI)
#define CLK_LOW       (PORTB = PORTB & (~_CLCI))

#define ACLO_N_HIGH   (PORTB = PORTB | _ACLO_N)
#define ACLO_N_LOW    (PORTB = PORTB & (~_ACLO_N))
#define DCLO_N_HIGH   (PORTB = PORTB | _DCLO_N)
#define DCLO_N_LOW    (PORTB = PORTB & (~_DCLO_N))

#define AR_N_HIGH     (PORTL = PORTL | _AR_N)
#define AR_N_LOW      (PORTL = PORTL & (~_AR_N))

#define RPLY_N_HIGH   (PORTL = PORTL | _RPLY_N)
#define RPLY_N_LOW    (PORTL = PORTL & ~(_RPLY_N))


// 1801VM2 States

#define STATE_RESET     !((PINL & _INIT_N) || (PINB & _ACLO_N) || (PINB & _DCLO_N))
#define STATE_START_1   !((PINL & _INIT_N) || (PINB & _ACLO_N) || !(PINB & _DCLO_N))
#define STATE_START_2   !(!(PINL & _INIT_N) || (PINB & _ACLO_N) || !(PINB & _DCLO_N))
#define STATE_STARTED   ((PINL & _INIT_N) && (PINB & _ACLO_N) && (PINB & _DCLO_N)) && (uP_last_state == START_2)

#define STATE_RESET_VECTOR ((PINL & _INIT_N) && (PINB & _ACLO_N) && (PINB & _DCLO_N)) && (!(PINL & _SEL_N) && (PING & _SYNC_N) && !(PING & _DIN_N))
#define STATE_SET_INIT_ADDRESS ((PINL & _INIT_N) && (PINB & _ACLO_N) && (PINB & _DCLO_N)) && (!(PINL & _SEL_N) && (PING & _DIN_N) && (PING & _DOUT_N) && (!(PING & _SYNC_N)))
#define STATE_SET_ADDRESS ((PINL & _INIT_N) && (PINB & _ACLO_N) && (PINB & _DCLO_N)) && ((PINL & _SEL_N) && (PING & _DIN_N) && (PING & _DOUT_N) && (!(PING & _SYNC_N)))
#define STATE_DIN ((PINL & _INIT_N) && (PINB & _ACLO_N) && (PINB & _DCLO_N)) && (!(PING & _SYNC_N) && !(PING & _DIN_N))
#define STATE_DOUT ((PINL & _INIT_N) && (PINB & _ACLO_N) && (PINB & _DCLO_N)) && (!(PING & _SYNC_N) && !(PING & _DOUT_N))
#define STATE_IDLE 0

#define STATE_SYNC (PING & _SYNC_N)

// State Management

#define DCLO_CYCLES 20
#define ACLO_CYCLES 40

enum uP_STATE {
  RESET,
  START_1,
  START_2,
  STARTED,
  RESET_VECTOR,
  SET_ADDRESS,
  SET_INIT_ADDRESS,
  DIN,
  DOUT,
  IDLE
};

char tmp[21];

unsigned long clock_cycle_count;
unsigned long uP_start_millis;
unsigned long uP_stop_millis;
unsigned int  uP_ADDR;
unsigned int  uP_BUSADDR;
unsigned int  uP_DATA;
unsigned int  uP_BUSDATA;
boolean address_valid;
uint8_t last_uP_SYNC_N = 1;
unsigned int offset;

uP_STATE uP_entry_state = IDLE;
uP_STATE uP_last_state = IDLE;
uP_STATE uP_current_state = IDLE;

M7856 console(1,CONSOLE_BASE,9600);
M7856 tu58(2,TU58_BASE,9600);
KY11 operatorconsole(KY11_BASE,0);
M9312 romterminator(M9312_LOW_ROM,M9312_BLANK_ROM,M9312_BLANK_ROM,M9312_BLANK_ROM,M9312_BLANK_ROM);

void uP_init()
{
  uP_IN();

  DCLO_N_LOW;
  ACLO_N_LOW;
  CLK_LOW;
  AR_N_HIGH;
  RPLY_N_HIGH;

  pinMode(uP_INIT_N, INPUT);
  pinMode(uP_DCLO_N, OUTPUT);
  pinMode(uP_ACLO_N, OUTPUT);
  pinMode(uP_CLCI, OUTPUT);
  pinMode(uP_DIN_N, INPUT);
  pinMode(uP_DOUT_N, INPUT);
  pinMode(uP_SYNC_N, INPUT);
  pinMode(uP_SEL_N, INPUT);
  pinMode(uP_AR_N, OUTPUT);
  pinMode(uP_RPLY_N, OUTPUT);
  pinMode(uP_WTBT_N, INPUT);

  uP_assert_reset();
  CLK_LOW;
  
  clock_cycle_count = 0;
  uP_start_millis = millis();
  last_uP_SYNC_N = 1;
}

void uP_assert_reset()
{
  DCLO_N_LOW;

  ACLO_N_LOW;
  
  AR_N_HIGH;
  
  RPLY_N_HIGH;
  
  uP_last_state = RESET;
  clock_cycle_count = 0;
}

void uP_IN()
{
  ADDR_H_DIR = DIR_IN;
  ADDR_L_DIR = DIR_IN;
}

void uP_OUT()
{
  ADDR_H_DIR = DIR_OUT;
  ADDR_L_DIR = DIR_OUT;
}

////////////////////////////////////////////////////////////////////
// Processor Control Loop
////////////////////////////////////////////////////////////////////

#ifndef NO_ISR
  ISR(TIMER1_COMPA_vect)
#endif
#ifdef NO_ISR
  void cycle()
#endif
{ 
  CLK_LOW;

  uP_entry_state = uP_current_state;
  if (STATE_RESET)
  {
    uP_current_state = RESET;
    if (uP_last_state != RESET)
    {
      uP_last_state = RESET;
    }
    if (clock_cycle_count > DCLO_CYCLES)
    {
      DCLO_N_HIGH;
    }
  } else if (STATE_START_1)
  {
    uP_current_state = START_1;
    if (uP_last_state != START_1)
    {
      uP_last_state = START_1;
    }
  } else if (STATE_START_2)
  {
    uP_current_state = START_2;
    if (uP_last_state != START_2)
    {
      uP_last_state = START_2;
    }
    if (clock_cycle_count > ACLO_CYCLES)
    {
      ACLO_N_HIGH;
    }
  } else if (STATE_STARTED)
  {
    uP_current_state = STARTED;
    if (uP_last_state != STARTED)
    {
      uP_last_state = STARTED;
    }
  } else if (STATE_RESET_VECTOR)
  {
    uP_current_state = RESET_VECTOR;
    if (uP_last_state != RESET_VECTOR)
    {
      uP_last_state = RESET_VECTOR;
      uP_DATA = romterminator.read(BOOT_ADDRESS);
      uP_BUSDATA = ~uP_DATA;
      uP_OUT();
      DATA_H_OUT = ((uP_BUSDATA & 0xFF00)>> 8);
      DATA_L_OUT = (uP_BUSDATA & 0x00FF);
      uP_DATA = 0;
    }
  } else if ((STATE_SET_ADDRESS) && (last_uP_SYNC_N == 1))
  {
    uP_current_state = SET_ADDRESS;
    if (uP_last_state != SET_ADDRESS)
    {
      uP_IN();
      uP_last_state = SET_ADDRESS;
      uP_BUSADDR = ADDR;
      uP_ADDR = ~uP_BUSADDR;
    }
  } else if ((STATE_SET_INIT_ADDRESS) && (last_uP_SYNC_N == 1))
  {
    uP_current_state = SET_INIT_ADDRESS;
    if (uP_last_state != SET_INIT_ADDRESS)
    {
      uP_IN();
      uP_last_state = SET_INIT_ADDRESS;
      uP_BUSADDR = ADDR;
      uP_ADDR = ~uP_BUSADDR;
    }
  } else if (STATE_DIN)
  {
    uP_current_state = DIN;
    if (uP_last_state != DIN)
    {
      address_valid = false;
      uP_OUT();

      if (romterminator.here(uP_ADDR))
      {
        // ROM

        address_valid = true;
        uP_DATA = romterminator.read(uP_ADDR);
      } else if ((uP_ADDR >= RAM_START) && (uP_ADDR < RAM_END))
      {
        // RAM
        
        address_valid = true;
        offset = (uP_ADDR - RAM_START) / 2;
        uP_DATA = RAM[offset];
      } else if (uP_ADDR >= IO_PAGE)
      {
        // Which IO device handles this

        if (console.here(uP_ADDR)) {
          address_valid = true;
          uP_DATA = console.read(uP_ADDR);
        } else if (tu58.here(uP_ADDR)) {
          address_valid = true;
          uP_DATA = tu58.read(uP_ADDR);
        } else if (operatorconsole.here(uP_ADDR)) {
          address_valid = true;
          uP_DATA = operatorconsole.read(uP_ADDR);
        }
      }
      
      if (address_valid)
      {
        uP_BUSDATA = ~uP_DATA;

        DATA_H_OUT = ((uP_BUSDATA & 0xFF00)>> 8);
        DATA_L_OUT = (uP_BUSDATA & 0x00FF);

        uP_last_state = DIN;

        RPLY_N_LOW;
      } else {
        // Bus Error Trap
#ifdef TRAP_BUS_ERROR
        trap_bus_error();
#endif
      }
    }
  } 
  else if (STATE_DOUT)
  {
    uP_current_state = DOUT;
    if (uP_last_state != DOUT)
    {
      address_valid = false;
      uP_IN();
      uP_BUSDATA = DATA_IN;
      uP_DATA = ~uP_BUSDATA;

      if ((uP_ADDR >= RAM_START) && (uP_ADDR < RAM_END))
      {
        // RAM

        address_valid = true;
        offset = (uP_ADDR - RAM_START) / 2;
        RAM[offset] = uP_DATA;
      } else if (uP_ADDR >= IO_PAGE)
      {
        // Which IO device handles this

        if (console.here(uP_ADDR)) {
          address_valid = true;
          console.write(uP_ADDR,uP_DATA);
        } else if (tu58.here(uP_ADDR)) {
          address_valid = true;
          tu58.write(uP_ADDR,uP_DATA);
        } else if (operatorconsole.here(uP_ADDR)) {
          address_valid = true;
          operatorconsole.write(uP_ADDR,uP_DATA);
        }
      }      

      if (address_valid)
      {
        uP_last_state = DOUT;

        RPLY_N_LOW;
      } else {
        // Bus Error Trap
#ifdef TRAP_BUS_ERROR
        trap_bus_error();
#endif
      }
    }
  }
  else
  {
   uP_current_state = IDLE;
   if (uP_last_state != IDLE)
    {
      uP_last_state = IDLE;
      uP_IN();
      uP_DATA = 000000;
      address_valid = false;
      RPLY_N_HIGH;
    }
  }

  // Last state is saved for the subsequent processor cycle
  
  last_uP_SYNC_N = (STATE_SYNC == _SYNC_N);
  
  if (last_uP_SYNC_N)  {
    AR_N_HIGH;
  } else {
    AR_N_LOW;
  }
 
  CLK_HIGH;

  clock_cycle_count ++;
}


////////////////////////////////////////////////////////////////////
// LCD/Keyboard functions
////////////////////////////////////////////////////////////////////

#ifdef USE_LCDKEYPAD

int getKey()
{
  key = get_key2();
  if (key != oldkey)
    {
      delay(BTN_DEBOUNCE);
      key = get_key2();
      if (key != oldkey) {
        oldkey = key;
        if (key == -1)
          BTN_RELEASE = 1;
        else
          BTN_PRESS = 1;
      }
    } else {
      BTN_PRESS = 0;
      BTN_RELEASE = 0;
    }
  return (key != -1);
}

int get_key2()
{
  int k;
  int adc_key_in;

  adc_key_in = analogRead( LCD_BTN );
  for( k = 0; k < NUM_KEYS; k++ )
  {
    if ( adc_key_in < adc_key_val[k] )
    {
      return k;
    }
  }
  if ( k >= NUM_KEYS )
    k = -1;
  return k;
}

////////////////////////////////////////////////////////////////////
// LCD/Keyboard callbacks
////////////////////////////////////////////////////////////////////

void btn_Pressed_Select()
{
}

void btn_Pressed_Left()
{
}

void btn_Pressed_Right()
{
}

void btn_Pressed_Up()
{
}

void btn_Pressed_Down()
{
}
#endif

////////////////////////////////////////////////////////////////////
// Setup
////////////////////////////////////////////////////////////////////

void setup() 
{

  // The primary ATMega2560 serial port is initialized for debug messages
  
  Serial.begin(115200);

  // Other serial ports are not initialized.  The M7856 class uses
  // direct access to the serial port registers

#ifdef USE_LCDKEYPAD
  if (USE_LCDKEYPAD)
  {
    pinMode(LCD_BL, OUTPUT);
    analogWrite(LCD_BL, backlightSet);  
    lcd.begin(16, 2);
  }
#endif

  // Initialize processor GPIO's
  uP_init();
  
  // Set up timer1 interrupt clock the processor and control state
  // For initial debugging define NO_ISR to run so the processor is
  // only clocked via the main loop

#ifndef NO_ISR
  cli();
  // Set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  
  // Set Compare Match Register for 1hz increments
  // OCR1A = 20; // 100kHz mode: 19;  // = (16*10^6) / (1*1024) - 1 (must be <65536)
  OCR1A = 20;
  // Turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  // CS12 - CS11 - CS10 Prescalar
  // 1=1/1, 2=1/8, 3=1/64, 4=1/256, 5=1/1024
  TCCR1B |= (1 << CS11);   // 95kHz
  
  // Enable Timer Compare Interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();
#endif

  // Delay 250ms for everything to settle on start
  
  delay(250);

  Serial.println("Init Complete");
}

void trap_bus_error()
{
  sprintf(tmp, "\nBus Error Trap A%0.6o", uP_ADDR);
  Serial.println(tmp);
  for (;;) {}
}

#ifdef NO_ISR

// Debug state prints a lot of information while running but cannot be used when the 
// processor is clocked via the timer interrupt

void debug_state()
{
  switch (uP_entry_state)
  {
    case RESET:
      Serial.print("RESET       /");
      break;
    case START_1:
      Serial.print("START_1     /");
      break;
    case START_2:
      Serial.print("START_2     /");
      break;
    case STARTED:
      Serial.print("STARTED     /");
      break;
    case RESET_VECTOR:
      Serial.print("RESET_VECTOR/");
      break;
    case SET_ADDRESS:
      Serial.print("SET_ADDRESS /");
      break;
    case SET_INIT_ADDRESS:
      Serial.print("INIT_ADDRESS/");
      break;
    case DIN:
      Serial.print("DIN         /");
      break;
    case DOUT:
      Serial.print("DOUT        /");
      break;
    case IDLE:
      Serial.print("IDLE        /");
      break;
    default:
      Serial.print("UNKNOWN     /");
      break;
  }

  switch (uP_current_state)
  {
    case RESET:
      Serial.print("RESET       :");
      break;
    case START_1:
      Serial.print("START_1     :");
      break;
    case START_2:
      Serial.print("START_2     :");
      break;
    case STARTED:
      Serial.print("STARTED     :");
      break;
    case RESET_VECTOR:
      Serial.print("RESET_VECTOR:");
      break;
    case SET_ADDRESS:
      Serial.print("SET_ADDRESS :");
      break;
    case SET_INIT_ADDRESS:
      Serial.print("INIT_ADDRESS:");
      break;
    case DIN:
      Serial.print("DIN         :");
      break;
    case DOUT:
      Serial.print("DOUT        :");
      break;
    case IDLE:
      Serial.print("IDLE        :");
      break;
    default:
      Serial.print("UNKNOWN     :");
      break;
  }

  Serial.print("INIT:");
  Serial.print(digitalRead(uP_INIT_N) ? "1" : "0");
  Serial.print(" DCLO:");
  Serial.print(digitalRead(uP_DCLO_N) ? "1" : "0");
  Serial.print(" ACLO:");
  Serial.print(digitalRead(uP_ACLO_N) ? "1" : "0");
  Serial.print(" SEL:");
  Serial.print(digitalRead(uP_SEL_N) ? "1" : "0");
  Serial.print(" SYNC:");
  Serial.print(digitalRead(uP_SYNC_N) ? "1" : "0");
  Serial.print(" LSYNC:");
  Serial.print(last_uP_SYNC_N ? "1" : "0");
  Serial.print(" AR:");
  Serial.print(digitalRead(uP_AR_N) ? "1" : "0");
  Serial.print(" DIN:");
  Serial.print(digitalRead(uP_DIN_N) ? "1" : "0");
  Serial.print(" DOUT:");
  Serial.print(digitalRead(uP_DOUT_N) ? "1" : "0");
  Serial.print(" RPLY:");
  Serial.print(digitalRead(uP_RPLY_N) ? "1" : "0");
  Serial.print(" ");
  Serial.print(clock_cycle_count);
  Serial.print(" ");
  
  sprintf(tmp, "A%0.6o D%0.6o", uP_ADDR, uP_DATA);
  Serial.println(tmp);
}
#endif

////////////////////////////////////////////////////////////////////
// Loop()
// * This function runs in parallel with timer interrupt handler.
//   i.e. simplest multi-threading.
// * try to be done as quickly as possible so processor does not
//   slow down.
////////////////////////////////////////////////////////////////////

void loop()
{

  // Check for M7856 state changes
  
  console.event();
  tu58.event();

#ifdef USE_LCDKEYPAD
  if (USE_LCDKEYPAD)
  {
    // Handle key presses
    //
    if ( getKey() ) {
      // button pressed
      if ( BTN_PRESS ) {
        if (key == BTN_SELECT) btn_Pressed_Select();
        if (key == BTN_UP)     btn_Pressed_Up();
        if (key == BTN_DOWN)   btn_Pressed_Down();
        if (key == BTN_LEFT)   btn_Pressed_Left();
        if (key == BTN_RIGHT)  btn_Pressed_Right();      
      }
    } else
    {
      float freq;
        
      lcd.setCursor(0, 0);
      sprintf(tmp, "A%0.6o D%0.6o", uP_ADDR, uP_DATA);
      lcd.print(tmp);
      lcd.setCursor(0,1);
      //freq = (float) clock_cycle_count / (millis() - uP_start_millis + 1);
      //lcd.print(freq);  lcd.print(" kHz   1801VM2");
      lcd.print(clock_cycle_count);
      lcd.print(" 1801VM2");
    }
  }
#endif

#ifdef NO_ISR
  cycle();
#ifdef CPUTRACE
  debug_state();
#endif
#endif

}
