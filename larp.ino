/* THIS CODE RUNS ON ARDUINO NANO */

/*
   Some code, including the below comment block, is adapted from
   public domain example code furnished with the MFRC522 library.

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
/*
   Some code is adapted from the FastLED examples
 */
#include <SPI.h>
#include <MFRC522.h>
#include "GameState.h"
#include "led.h"
#include "BeanChannel.h"
// We could also use A1
#define FLEX_PIN        A0
#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above


int data; // used to track data from Wire
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

Led led;
TagGameStateManager gameStateManager;
BeanChannel beanChannel;

int calibrate = 0;
void setup() {
    beanChannel.setup();
    pinMode(A0, INPUT_PULLUP);
    pinMode(3, OUTPUT);
    pinMode(6, OUTPUT);
    calibrate = analogRead(A0);
    led.led_setup();
    gameStateManager.setup();
    gameStateManager.addTagger(tagged);
    gameStateManager.addUnTagger(untagged);
    gameStateManager.addItter(it);
    gameStateManager.addQuitter(quit);
    Serial.begin(9600);		// Initialize serial communications with the PC
    SPI.begin();			// Init SPI bus
    mfrc522.PCD_Init();		// Init MFRC522
    mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
    Serial.println(F("Scan PICC to see UID, type, and data blocks..."));
}

bool tagged() {
    Serial.println(analogRead(FLEX_PIN) - calibrate);
    return analogRead(FLEX_PIN) - calibrate > 100;
}

bool untagged() {
    return mfrc522.PICC_IsNewCardPresent();
}

bool it () {
    return data == 1;
}
bool quit () {
    return data == 2;
}

int intensityLED;
void loop() {
    beanChannel.loop();
    if (beanChannel.hasData()) {
        data = beanChannel.getCurrentValue();
    } else {
        data = 0;
    }
    analogWrite(3, data % 1000);
    gameStateManager.loop();
    // Look for new cards
    led.led_intensity = intensityLED;
    GameState state = gameStateManager.getState();
    switch (state) {
        case NOT_IT:
            led.setColorRGB(0, 0, 255);
            break;
        case IT:
            led.setColorRGB(0, 255, 0);
            break;
        case FROZEN:
            led.setColorRGB(255, 255, 255);
            break;
        default:
            break;
    }
    beanChannel.sendByte(led.led_intensity);
    intensityLED = (intensityLED + 1) % 60;
    if (state == NOT_PLAYING) {
        led.led_loop(INTENSITY);
    } else {
        led.led_loop(COLOR);
    }
    /*
    if ( ! mfrc522.PICC_IsNewCardPresent()) {

    } else {
    }

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) {
        return;
    }

    // Dump debug info about the card; PICC_HaltA() is automatically called
      mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
      */
}
