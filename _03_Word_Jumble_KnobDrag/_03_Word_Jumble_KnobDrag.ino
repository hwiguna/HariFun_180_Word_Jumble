//-- LCD Variables --
#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"
Adafruit_LiquidCrystal lcd(0x20);

// https://www.seetron.com/apps/app_cceditor.html
byte arrowBitmap[8] = {0x84, 0x8E, 0x9F, 0x84, 0x84, 0x84, 0x80, 0x80};
byte upArrowChar = 0;

//--I/O pins --
const byte button = 4; // Digital pin 4
const byte knob = A0;
#define ISPRESSED LOW

//-- JUMBLE VARIABLES --
String werd = "VITAL";
String jumble;
int lastKnob = -1;

void WaitForButtonPress() {
  while (digitalRead(4) != LOW) {
    delay(100);
  }
  delay(500); // ignore switch bounce
}

void SetupJumble() {
  randomSeed(analogRead(A1));
  jumble = werd;
  byte wordLen = jumble.length();
  for (byte i=0; i<wordLen; i++) {
    int randomIndex = random(wordLen);
    byte tmp = jumble[i];
    jumble[i] = jumble[randomIndex];
    jumble[randomIndex] = tmp;
  }
}

void SetupLCD() {
  lcd.begin(16, 2); // 16 columns, 2 rows
  lcd.clear();
  lcd.print(jumble);
  lcd.setBacklight(HIGH);

  lcd.createChar(upArrowChar, arrowBitmap);
}

void SetupInputs() {
  pinMode(button, INPUT_PULLUP);
}

void setup() {
  Serial.begin(115200);
  SetupInputs();
  SetupJumble();
  SetupLCD();
}

void SwapLetters(byte index1, byte index2) {
  byte wordLen = werd.length();
  if (index1 >= 0 && index1 < wordLen && index2 >= 0 && index2 < wordLen) {
    byte letter1 = jumble[index1];
    jumble[index1] = jumble[index2];
    jumble[index2] = letter1;
    lcd.setCursor(0, 0);
    lcd.print(jumble);
  }
}

void KnobArrow() {
  int knobValue = map(analogRead(knob), 0, 1023, 0, 15);
  if (knobValue != lastKnob) {
    if (lastKnob != -1) {
      lcd.setCursor(lastKnob, 1);
      lcd.write(' ');
      if (digitalRead(button) == ISPRESSED) SwapLetters(knobValue, lastKnob);
    }
    lcd.setCursor(knobValue, 1);
    lcd.write((byte)upArrowChar);
    lastKnob = knobValue;
  }
  delay(50);
}

void loop() {
  KnobArrow();
}
