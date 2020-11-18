#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include <FlexCAN.h>
#include <Fonts/FreeMono9pt7b.h>
#include <Adafruit_NeoPixel.h>

Adafruit_SSD1306 display(6, 5, 10);

// sier hvilke pins joysticken er paa

const int JOY_DOWN  = 23;
const int JOY_UP  = 22;
const int JOY_LEFT  = 18;
const int JOY_RIGHT = 17;
const int JOY_CLICK  = 19;

// definerer bolkse variabler for joysticken
bool  joyUpIsclicked  = true;
bool  joyDownIsClicked  = true;
bool  joyClick  = true;

// startkoordinat i y retning for veggene
int rightLinePos = 22;
int leftLinePos = 22;

//posisjon og retning til ballen i x og y
int16_t ballXPos;
int8_t  ballYPos;

int8_t  ballXDir  = -1;
int8_t  ballYDir  = 1;

int16_t newPosX = ballXPos  + ballXDir;
int8_t  newPosY = ballYPos  + ballYDir;


//definerer can meldinger master message, slave message.
CAN_message_t mmsg, smsg;
CAN_message_t rxmsg1;

//boolske variabler for retning til ball i x og y
bool moveRight = false;
bool moveUp = true;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  display.display ();

  // oppforselen til joysticken
  pinMode(JOY_CLICK, INPUT_PULLUP);
  pinMode(JOY_UP, INPUT_PULLUP);
  pinMode(JOY_DOWN, INPUT_PULLUP);

  // starter can porten med en bit rate paa 500kBit/s
  Can0.begin(500000);
  Can1.begin(500000);

  // tegner rammen rundt brettet, og veggene for aa forberede spilet.
  display.drawRect(0, 0, 128, 64, WHITE);
  display.drawFastVLine(6, rightLinePos, 20, WHITE);
  display.drawFastVLine(123, leftLinePos, 20, WHITE);
  display.display();


}

// funksjonen som sier hva veggene skal gjore
int wallWall() {
  joyUpIsclicked = digitalRead(JOY_UP);
  joyDownIsClicked = digitalRead(JOY_DOWN);
  if ( joyUpIsclicked == false ) {
    if ( rightLinePos < 0) {
      rightLinePos = 0;
    }
    else if ( rightLinePos > (64 - 20 )) {
      rightLinePos = (64 - 20);
    }
    else {
      rightLinePos --;
    }
  }
  if ( joyDownIsClicked == false ) {
    if ( rightLinePos < 0) {
      rightLinePos = 0;
    }
    else if ( rightLinePos > (64 - 20 )) {
      rightLinePos = (64 - 20);
    }
    else {
      rightLinePos ++;
    }
  }


}
// funskjonen som styrer ballen
int moveBall() {

  if (moveRight == false ) {
    newPosX = newPosX - 1;
  } else {
    newPosX = newPosX + 1;
  }
  // Hvis ballen treffer en av veggene skal den bytte x retning
  if ((newPosX <= 8) && (newPosY <= (leftLinePos + 20)) 
       && (newPosY > leftLinePos)) {
    moveRight = true;
  }
  else if ((newPosX >= 120) && (newPosY <= (rightLinePos + 20))
           && (newPosY > rightLinePos)) {
    moveRight = false;
  }
  // Hvis ballen treffer taket eller gulvet skal den bytte y retning.
  if (moveUp == true) {
    newPosY = newPosY - 1;
  }
  else {
    newPosY = newPosY + 1;
  }
  if (newPosY >= 63) {
    moveUp = true;
  }
  else if (newPosY <= 1) {
    moveUp = false;
  }
}
/ 7 Programmet master kjorer
void myBall() 
{
  bool kjor = true;
  while ( kjor == true ) 
  {
    // melding som blir sendt til slaven. 
    // Inneholder ballposisjon og masterens vegg.
    mmsg.len = 8;
    mmsg.id = 22;
    mmsg.buf[0] = 0;
    mmsg.buf[1] = rightLinePos;
    mmsg.buf[2] = newPosX;
    mmsg.buf[3] = newPosY;
    mmsg.buf[4] = 0;
    mmsg.buf[5] = 0;
    mmsg.buf[6] = 0;
    mmsg.buf[7] = 0;


    Can1.read(rxmsg1);
    Can0.write(mmsg);

    display.clearDisplay();
    // bruker delay for aa ikke cleare displayet og printe det samtidig. 
    // Denne gjor og at can meldingen med posisjon blir sendt hvert 10ms.
    delay(10);

    wallWall();
    moveBall();


    // Tegner rammen, din egen vegg med posisjon du har bestemt, 
    // slavens vegg hentet fra can melding, og ballen som du har generert.
    display.drawRect(0, 0, 128, 64, WHITE);
    display.drawFastVLine(123, rightLinePos, 20, WHITE);
    display.drawFastVLine(6, rxmsg1.buf[1], 20, WHITE);
    display.fillCircle(newPosX, newPosY, 2, WHITE);
    display.display();

    // hvis ballen har kommet bak en av veggene saa skal programmet stoppes, 
    // og sendes tilbake til void loop.
    if ( newPosX > 124 ) {
      kjor = false;
      return loop();
    }
    else if ( newPosX < 6 ) {
      kjor = false;
      return loop();
    }
  }
}

// Funksjonen slaven kjorer
void yourBall() {
  bool kjor = true;
  while ( kjor == true ) 
 {
    display.clearDisplay();

    // meldingen slaven sender til master. inneholder bare veggposisjon
    smsg.len = 8;
    smsg.id = 52;
    smsg.buf[0] = 0;
    smsg.buf[1] = rightLinePos;
    smsg.buf[2] = 0;
    smsg.buf[3] = 0;
    smsg.buf[4] = 0;
    smsg.buf[5] = 0;
    smsg.buf[6] = 0;
    smsg.buf[7] = 0;

    Can1.read(rxmsg1);
    Can0.write(smsg);



    delay(10);

    // kjorer bare funskjonen for vegg, siden det er master som generer ball
    wallWall();

    // Tegner rektangel, din egen vegg, 
    // motstanderens vegg med posisjon fra can melding,
    // og ball fra can melding med speilvendt x koordinat.
    display.drawRect(0, 0, 128, 64, WHITE);
    display.drawFastVLine(123, rightLinePos, 20, WHITE);
    display.drawFastVLine(6, rxmsg1.buf[1], 20, WHITE);
    display.fillCircle((128 - rxmsg1.buf[2]), rxmsg1.buf[3] , 2, WHITE);
    display.display();

    if ( newPosX > 124 ) {
      kjor = false;
      return loop();
    }
    else if ( newPosX < 6 ) {
      kjor = false;
      return loop();
    }
  }
}

void loop() 
{
  // put your main code here, to run repeatedly:
  /*Serial.println ("Click Joystick to start game");
    display.clearDisplay();*/

  //nullstiller posisjoner
  ballYPos = 32;
  newPosX = ballXPos  + ballXDir;
  newPosY = ballYPos  + ballYDir;
  rightLinePos = 22;
  leftLinePos = 22;

  // definerer aa trykke inn joysticken
  joyClick = digitalRead(JOY_CLICK);
  Serial.print("Press joystick to become Master");

  Can1.read(rxmsg1);



  // trykker du inn joysticken blir du master og starter med ballen.
  if  (joyClick == false) {
    Serial.println ("Player1 is master");
    ballXPos = 122;
    bool moveRight = false;
    bool moveUp = true;

    display.clearDisplay();
    display.drawRect(0, 0, 128, 64, WHITE);
    display.drawFastVLine(123, rightLinePos, 20, WHITE);
    display.drawFastVLine(6, leftLinePos, 20, WHITE);
    display.fillCircle(newPosX, newPosY, 2, WHITE);
    display.display();
    //Bruker delay for aa gi spilleren tid til aa bli klar
    delay(1500);
    // master gaar inn i myBall funksjonen
    return myBall();


  }
  // mottar du can melding blir du slave og motstanderen starter med ballen.
  else if (rxmsg1.id == 22) {
    Serial.println ("Player2 is master");
    bool moveRight = true;
    bool moveUp = true;
    ballXPos = 9;
    display.clearDisplay();
    display.drawRect(0, 0, 128, 64, WHITE);
    display.drawFastVLine(123, 22, 20, WHITE);
    display.drawFastVLine(5, leftLinePos, 20, WHITE);
    display.fillCircle(newPosX, newPosY, 2, WHITE);
    display.display();
    //slaven gaar inn i yourBall funksjonen.
    return yourBall();
  }
}