#define DEBUG
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940
#define FORWARD 10
#define LEFT 11
#define REVERSE 12
#define RIGHT 13

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
//#define ROZ     0xFD20
#define ROZ     0xFBE0
#define GRI     0xBDF7

#define BOXSIZE 20
#define PENRADIUS 3
#define MINPRESSURE 10
#define MAXPRESSURE 1000

void setup(void) {
#ifdef DEBUG
  Serial.begin(9600);
  Serial.println(F("RC CAR CONTROLS"));
#endif // DEBUG
  tft.reset();

  uint16_t identifier = tft.readID();

  if (identifier == 0x9325)
  {
#ifdef DEBUG
    Serial.println(F("Found ILI9325 LCD driver"));
  }
  else if (identifier == 0x9328) {

    Serial.println(F("Found ILI9328 LCD driver"));
  } else if (identifier == 0x7575) {

    Serial.println(F("Found HX8347G LCD driver"));
  } else if (identifier == 0x9341) {

    Serial.println(F("Found ILI9341 LCD driver"));
  } else if (identifier == 0x8357) {

    Serial.println(F("Found HX8357D LCD driver"));
#endif  DEBUG
  } else {
#ifdef DEBUG
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.print(F("I try use ILI9341 LCD driver "));
    Serial.println(F("If using the Adafruit 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
#endif  DEBUG
    identifier = 0x9341;
  }
  tft.begin(identifier);

  //Game Initialization
  tft.fillScreen(BLACK);
  tft.fillRect(0, 0, 320, 240, BLACK);
  tft.setTextColor(WHITE);
  tft.fillRect(0, 0, 320, 240, BLACK); //Resets Screen

  tft.setRotation(3);
  tft.drawRect(150, 60, BOXSIZE, BOXSIZE, WHITE); //Up Direction
  tft.drawRect(150, 200, BOXSIZE, BOXSIZE, WHITE);//Down Direction
  tft.drawRect(210, 130, BOXSIZE, BOXSIZE, WHITE);//Right Direction
  tft.drawRect(90, 130, BOXSIZE, BOXSIZE, WHITE);//Left Direction
  tft.setCursor(110, 5);
  tft.setTextSize(2);
  tft.println("Controls");
  pinMode(13, OUTPUT);
  pinMode(FORWARD, OUTPUT);
  pinMode(REVERSE, OUTPUT);
  pinMode(LEFT, OUTPUT);
  pinMode(RIGHT, OUTPUT);
}

void loop()
{
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  //if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
  {
#ifdef DEBUG
    Serial.print("X = "); Serial.print(p.x);
    Serial.print("\tY = "); Serial.print(p.y);
    Serial.print("\tPressure = "); Serial.println(p.z);
#endif  DEBUG
    if (p.y < (TS_MINY - 5))
      // scale from 0->1023 to tft.width
      p.x = tft.width() - (map(p.x, TS_MINX, TS_MAXX, tft.width(), 0));
    p.y = tft.height() - (map(p.y, TS_MAXY, TS_MINY, tft.height(), 0));
#ifdef DEBUG
    Serial.print("("); Serial.print(p.x);
    Serial.print(", "); Serial.print(p.y);
    Serial.println(")");
#endif DEBUG

  if ((p.x >= 340 && p.x <= 400) && (p.y >= 110 && p.y <= 130))//Forward (Digital Pin 10) 
  {
    tft.fillRect(150, 60, BOXSIZE, BOXSIZE, WHITE); 
    digitalWrite(FORWARD,HIGH);
    delay(2000);
    digitalWrite(FORWARD,LOW);
    tft.fillRect(150, 60, BOXSIZE, BOXSIZE, BLACK); 
    tft.drawRect(150, 60, BOXSIZE, BOXSIZE, WHITE); 
  }//end Digital Pin 10 
 
  else if (((p.x >= 575 && p.x <= 630) && (p.y >= 155 && p.y <= 180)))//Turn Left(Digital Pin 11)
  {
    tft.fillRect(90, 130, BOXSIZE, BOXSIZE, WHITE);  
    digitalWrite(LEFT,HIGH);
    delay(2000);
    digitalWrite(LEFT,LOW);
    tft.fillRect(90, 130, BOXSIZE, BOXSIZE, BLACK); 
    tft.drawRect(90, 130, BOXSIZE, BOXSIZE, WHITE);
  }//end Digital Pin 12

  else if (((p.x >= 790 && p.x <= 850) && (p.y >= 110 && p.y <= 130)))//Reverse (Digital Pin 12)
  {
    tft.fillRect(150, 200, BOXSIZE, BOXSIZE, WHITE); 
    digitalWrite(REVERSE,HIGH);
    delay(2000);
    digitalWrite(REVERSE,LOW);
    tft.fillRect(150, 200, BOXSIZE, BOXSIZE, BLACK); 
    tft.drawRect(150, 200, BOXSIZE, BOXSIZE, WHITE);
  }

  else if (((p.x >= 575 && p.x <= 630) && (p.y >= 70 && p.y <= 90)))//Turn Right(Digital Pin 13)
  {
    tft.fillRect(210, 130, BOXSIZE, BOXSIZE, WHITE); 
    digitalWrite(RIGHT,HIGH);
    delay(2000);
    digitalWrite(RIGHT,LOW);
    tft.fillRect(210, 130, BOXSIZE, BOXSIZE, BLACK); 
    tft.drawRect(210, 130, BOXSIZE, BOXSIZE, WHITE);
  }
   
  }
}


