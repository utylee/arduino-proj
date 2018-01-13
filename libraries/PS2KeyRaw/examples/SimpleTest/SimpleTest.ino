/*  PS2KeyRaw library example

  Stripped down version of PS2Keyboard to get every key code byte from a PS2 Keyboard
  for testing purposes. Enables capture of all bytes.

  IMPORTANT WARNING
 
    If using a DUE or similar board with 3V3 I/O you MUST put a level translator 
    like a Texas Instruments TXS0102 or FET circuit as the signals are 
    Bi-directional (signals transmitted from both ends on same wire).
 
    Failure to do so may damage your Arduino Due or similar board.

  Test History
    September 2014 Uno and Mega 2560 September 2014 using Arduino V1.6.0
    January 2016   Uno, Mega 2560 and Due using Arduino 1.6.7 and Due Board 
                    Manager V1.6.6
  
  The circuit:
   * KBD Clock (PS2 pin 1) to an interrupt pin on Arduino ( this example pin 3 )
   * KBD Data (PS2 pin 5) to a data pin ( this example pin 4 )
   * +5V from Arduino to PS2 pin 4
   * GND from Arduino to PS2 pin 3
   
   The connector to mate with PS2 keyboard is a 6 pin Female Mini-Din connector
   PS2 Pins to signal
    1       KBD Data
    3       GND
    4       +5V
    5       KBD Clock

   Keyboard has 5V and GND connected see plenty of examples and
   photos around on Arduino site and other sites about the PS2 Connector.
 
 Interrupts

   Clock pin from PS2 keyboard MUST be connected to an interrupt
   pin, these vary with the different types of Arduino

  PS2KeyRaw requires both pins specified for begin()

  keyboard.begin( data_pin, irq_pin );
  
  Valid irq pins:
     Arduino Uno:  2, 3
     Arduino Due:  All pins, except 13 (LED)
     Arduino Mega: 2, 3, 18, 19, 20, 21
     Teensy 2.0:   All pins, except 13 (LED)
     Teensy 2.0:   5, 6, 7, 8
     Teensy 1.0:   0, 1, 2, 3, 4, 6, 7, 16
     Teensy++ 2.0: 0, 1, 2, 3, 18, 19, 36, 37
     Teensy++ 1.0: 0, 1, 2, 3, 18, 19, 36, 37
     Sanguino:     2, 10, 11
  
  Like the Original library and example this is under LGPL license.

  Written by Paul Carpenter, PC Services <sales@pcserviceselectronics.co.uk>
*/
   
#include <PS2KeyRaw.h>
#include "Bluetooth.h"
#include "BTKeyCodes.h"
#include "KeyTable.h"


#define _DEBUG

#define DATAPIN 4
#define IRQPIN  3

uint8_t line = 0;

PS2KeyRaw keyboard;


#ifdef _DEBUG
#define HARDWARE_SERIAL_RATE 38400
#else
#define HARDWARE_SERIAL_RATE 115200
#endif

#ifdef _DEBUG
#define BLUETOOTH_DEBUG_RX 7
#define BLUETOOTH_DEBUG_TX 8
#define BLUETOOTH_DEBUG_BAUD 115200
Bluetooth bluetooth(BLUETOOTH_DEBUG_BAUD, true, BLUETOOTH_DEBUG_RX, BLUETOOTH_DEBUG_TX);
#else
Bluetooth bluetooth(HARDWARE_SERIAL_RATE, false, 0, 0);
#endif



void setup() {
  keyboard.begin( DATAPIN, IRQPIN );
  Serial.begin( 9600 );
#if defined(ARDUINO_ARCH_AVR)
  Serial.println( F( "PS2 Raw Test of PS2 Keyboard codes" ) );
#elif defined(ARDUINO_ARCH_SAM)
  Serial.println( "PS2 Raw Test of PS2 Keyboard codes" );
#endif
}

void loop()
{
if( keyboard.available() )
  {
  // read the next key
  int c = keyboard.read();
	uint8_t btCode = BTKC_NONE;

   
  if( c > 0 ) 
    {
    line = 1;  
    //Serial.print( c, HEX );
    //Serial.print( " " );
	btCode = KeyTable::getBTCode(c, false);
    Serial.print( btCode, HEX );

    delay( 30 );
	//bluetooth.sendKeyboard(btCode);
	//bluetooth.sendKeyboard('c');
	//bluetooth.sendKeyboard((uint8_t)8);
    }
  }
else  // When no bytes available start on a new line
  if( line > 0 )
    {
    line = 0;
    Serial.println( );
    }
}
