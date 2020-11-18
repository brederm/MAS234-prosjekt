#include <FlexCAN.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMono9pt7b.h>

#define OLED_DC     6
#define OLED_CS     10
#define OLED_RESET  5
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

static CAN_message_t msg, rxmsg;
int count = 0;
String CANStr(" ");

void setup() 
{
  //Intialiser canbus og velg hastighet.
  Can1.begin(250000); 
  //Intialiser display.
  display.begin(SSD1306_SWITCHCAPVCC);
  //Fjern det som er paa skjerm.
  display.clearDisplay();

  delay(1000);

  display.setTextSize(0);
  //Velger farge, monokromatisk skjerm benyttes.
  display.setTextColor(WHITE); 
  display.setCursor(14, 7);
  //Definerer yx-posisjonen til teksten.
  display.println("MAS2345-Gruppe 2");
  display.setCursor(5, 20);
  display.println("CAN-Statistikk");
  display.setCursor(5, 25);
  display.println("-----------------");
  display.setCursor(5, 30);
  display.println("Antall mottatt:");
  display.setCursor(5, 40);
  display.println("Mottok sist ID: ");
  display.setCursor(5, 45);
  display.println("------------------");

  //Definerer strl og radius paa rund-rektangel.
  uint16_t x = 0; 
  uint16_t y = 0;
  uint16_t g = 128;
  uint16_t d = 64;
  uint16_t r = 4;
  uint16_t t = WHITE;
  
  display.drawRoundRect(x, y, g, d, r, t);
  display.display();
}


void loop() 
{
  //Definerer can-melding som sendes, innhold, lengde og id.
  msg.len = 8;
  msg.id = 0x7DF;
  msg.buf[0] = 0x02;
  msg.buf[1] = 0x01;
  msg.buf[2] = 0x0c;
  msg.buf[3] = 0;
  msg.buf[4] = 0;
  msg.buf[5] = 0;
  msg.buf[6] = 0;
  msg.buf[7] = 0;

  delay(50);
  Can1.write(msg);

  while (Can1.read(rxmsg))
  {
    String CANStr("");
    for (int i = 0; i < 8; i++)
    {
      CANStr += String(rxmsg.buf[i], HEX);
      CANStr += (" ");
    }
    //Teller for antall meldinger mottatt.
    count++;
    
    //Loopen oppdaterer displayet naar meldinger mottas.
    display.clearDisplay();
    display.setTextSize(0);
    display.setTextColor(WHITE);
    display.setCursor(14, 7);
    display.println("MAS2345-Gruppe 2");
    display.setCursor(5, 20);
    display.println("CAN-Statistikk");
    display.setCursor(5, 25);
    display.println("-----------------");
    display.setCursor(5, 30);
    display.println("Antall mottatt:");
    display.setCursor(5, 40);
    display.println("Mottok sist ID: ");
    display.setCursor(5, 45);
    display.println("------------------");
    display.setCursor(102, 30);
    display.println(count);
    display.setCursor(102, 40);
    display.print(rxmsg.id, HEX);

    uint16_t x = 0;
    uint16_t y = 0;
    uint16_t g = 128;
    uint16_t d = 64;
    uint16_t r = 4;
    uint16_t t = WHITE;
    display.drawRoundRect(x, y, g, d, r, t);
    
    display.display();
    
    //Viser mottatte meldinger i seriell vindu.
    Serial.print(rxmsg.id, HEX); 
    Serial.print("  ");
    Serial.println(CANStr);
  }
}