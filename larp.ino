/*
   --------------------------------------------------------------------------------------------------------------------
   Example sketch/program showing how to read data from a PICC to serial.
   --------------------------------------------------------------------------------------------------------------------
   This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid

   Example sketch/program showing how to read data from a PICC (that is: a RFID Tag or Card) using a MFRC522 based RFID
   Reader on the Arduino SPI interface.

   When the Arduino and the MFRC522 module are connected (see the pin layout below), load this sketch into Arduino IDE
   then verify/compile and upload it. To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M). When
   you present a PICC (that is: a RFID Tag or Card) at reading distance of the MFRC522 Reader/PCD, the serial output
   will show the ID/UID, type and any data blocks it can read. Note: you may see "Timeout in communication" messages
   when removing the PICC from reading distance too early.

   If your reader supports it, this sketch/program will read all the PICCs presented (that is: multiple tag reading).
   So if you stack two or more PICCs on top of each other and present them to the reader, it will first output all
   details of the first and then the next PICC. Note that this may take some time as all data blocks are dumped, so
   keep the PICCs at reading distance until complete.

   @license Released into the public domain.

   Typical pin layout used:
   -----------------------------------------------------------------------------------------
               MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
               Reader/PCD   Uno           Mega      Nano v3    Leonardo/Micro   Pro Micro
   Signal      Pin          Pin           Pin       Pin        Pin              Pin
   -----------------------------------------------------------------------------------------
   RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
   SPI SS      SDA(SS)      10            53        D10        10               10
   SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
   SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
   SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
*/
#include<FastLED.h>

#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>

#define LED_PIN  5

#define COLOR_ORDER GRB
#define CHIPSET     WS2811

#define BRIGHTNESS 128

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above
int data;
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
void receiveEvent(int howmany) {
  
    data = Wire.read();
}

const uint8_t kMatrixWidth = 16;
const uint8_t kMatrixHeight = 16;
const bool    kMatrixSerpentineLayout = false;
uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;

  if ( kMatrixSerpentineLayout == false) {
    i = (y * kMatrixWidth) + x;
  }

  if ( kMatrixSerpentineLayout == true) {
    if ( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (kMatrixWidth - 1) - x;
      i = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * kMatrixWidth) + x;
    }
  }

  return i;
}

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* leds( leds_plus_safety_pixel + 1);

uint16_t XYsafe( uint8_t x, uint8_t y)
{
  if ( x >= kMatrixWidth) return -1;
  if ( y >= kMatrixHeight) return -1;
  return XY(x, y);
}

void DrawOneFrame( byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8)
{
  byte lineStartHue = startHue8;
  for ( byte y = 0; y < kMatrixHeight; y++) {
    lineStartHue += yHueDelta8;
    byte pixelHue = lineStartHue;
    for ( byte x = 0; x < kMatrixWidth; x++) {
      pixelHue += xHueDelta8;
      leds[ XY(x, y)]  = CHSV( pixelHue, 255, 255);
    }
  }
}

void DrawNoise() {
  for (byte y = 0; y < kMatrixHeight; y++) {
    for (byte x = 0; x < kMatrixWidth; x++) {
      leds [ XY ( x, y)] = CHSV(random(0, 255), 255, 255);

    }
  }
}

void DrawScroll(int current) {
  int i = 0;
  for (byte y = 0; y < kMatrixHeight; y++) {
    for (byte x = 0; x < kMatrixWidth; x++) {
      leds[ XY(x, y)] = CHSV(0, 0, 0);
      if (i == current) {
        leds [ XY ( x, y)] = CHSV(255, 255, 255);
      }
      i++;
    }
  }
}

void DrawIntensity(int intensity) {
  int i = 0;
  for (int y = 0; y < kMatrixHeight; y++) {
    for (int x = 0; x < kMatrixWidth; x++) {
      if (i < intensity) {
        leds [ XY (x, y)] = CHSV(i * 5, 255, 255);
      } else {
        leds [ XY (x, y)] = CHSV(0, 0, 0);
      }
      i++;
    }
  }

}
int calibrate = 0;
void setup() {
  Wire.begin(5);
  Wire.onReceive(receiveEvent);
  pinMode(A0, INPUT_PULLUP);
  pinMode(3, OUTPUT);
  pinMode(6, OUTPUT);
  calibrate = analogRead(A0);
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
  Serial.begin(9600);		// Initialize serial communications with the PC
  SPI.begin();			// Init SPI bus
  mfrc522.PCD_Init();		// Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, type, and data blocks..."));
}

void led(int mode) {

  uint32_t ms = millis();
  int32_t yHueDelta32 = ((int32_t)cos16( ms * (27 / 1) ) * (350 / kMatrixWidth));
  int32_t xHueDelta32 = ((int32_t)cos16( ms * (39 / 1) ) * (310 / kMatrixHeight));
  switch (mode) {
    case 0:
      DrawIntensity((analogRead(A0) - calibrate) / 4);
      break;
    case 1:
      DrawNoise();
      break;
    default:
      break;
  }

  FastLED.setBrightness(BRIGHTNESS);

  FastLED.show();
}

void loop() {
  analogWrite(3, data % 1000);
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    led(0);

  } else {
    led(1);
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Dump debug info about the card; PICC_HaltA() is automatically called
//  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
}
