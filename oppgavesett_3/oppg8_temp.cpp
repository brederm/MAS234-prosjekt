#include <avr/io.h>
#include "util/delay.h"
#include <FlexCAN.h>
#include <SPI.h>
uint16_t CAN_ID = 21;
uint8_t bytes[sizeof(float)];
int reading;

void setup() 
{
  Can0.begin(250000);
  Can1.begin(250000);
}

void loop() 
{

  static CAN_message_t inMsg, msg1;
  while (Can0.available()) 
  {
    Can0.read(inMsg); 
    // Can0 leser verdier og legger verdiene inn i variabel inMsg
  }
  int RawData[inMsg.len];
  RawData[0] = inMsg.buf[0];

  float voltage = RawData[0] * (3.3 / 1023) - 0.02; 
  int temp = voltage * (1 / 0.01);
  // konverterer Analogverdien til volt, deretter til grader

  Serial.println();
  Serial.print("Temperatur verdi omgjort fra Volt = ");
  Serial.print (temp);
  Serial.println();
  delay(100);

  memcpy(bytes, &temp, sizeof(temp));  
  msg1.id = CAN_ID;
  msg1.len = 1;
  msg1.buf[0] = bytes[0];
  // legger temp verdien inn i bytes, burde egentlig bruke en array i memcpy.

  delay(100);
  Can0.write(msg1);
}