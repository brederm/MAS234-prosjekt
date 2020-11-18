#include <FlexCAN.h>
#include <SPI.h>

static CAN_message_t rxmsg;

void setup()
{
Can0.begin(500000);
}

void loop()
{
  while (Can0.read(rxmsg)) //Leser meldingen motatt paa can0
  {
  delay(1000);
  Can0.write(rxmsg); //Sender meldingen motatt paa can0, naar den mottar
    Serial.print(rxmsg.id,HEX); //Skriver meldingsid i seiral monitor
    Serial.print(' ');
    Serial.print(rxmsg.len,HEX); //Skriver lengden av meldingen seiral monitor
    Serial.print(' ');
    Serial.print(rxmsg.buf[0],HEX); //Skriver innholdet av buffer 0, i serial monitor
    Serial.println();
  }
}