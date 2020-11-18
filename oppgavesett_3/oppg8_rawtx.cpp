#include <FlexCAN.h>
#include <SPI.h>
static CAN_message_t msg, tempvalue;
uint16_t CAN_ID = 0x700;
uint8_t bytes[sizeof(float)];
int Reading;

void setup() {
  Can1.begin(250000);
  Can0.begin(250000);
  msg.id = 0x7DF;
  msg.flags.extended = 0;
  msg.flags.remote = 0;
}

void loop() {
  pinMode(A9, INPUT);
  Reading = analogRead(A9);
  msg.len = 1;
  Serial.print("Analogverdi fra temp sensor = ");
  Serial.print(Reading);
  Serial.println();
  delay(100);
  memcpy(bytes, &Reading, sizeof(Reading));
  msg.id = CAN_ID;
  msg.buf[0] = bytes[0];

  Can1.write(msg); // sender can melding msg 
  delay(100);
  while (Can1.available()) 
  {
    Can1.read(tempvalue); 
    // Can1 leser melding og legger verdiene inn i variabel tempvalue
  }

  Serial.print("Temperatur = ");
  Serial.print(tempvalue.buf[0]);
  Serial.println();
}