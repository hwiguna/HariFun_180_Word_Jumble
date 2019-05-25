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

//-- JUMBLE VARIABLES --
String werd = "ALVIT";
int lastKnob = -1;

void WaitForButtonPress() {
  while (digitalRead(4) != LOW) {
    delay(100);
  }
  delay(500); // ignore switch bounce
}

void SetupLCD() {
  lcd.begin(16, 2); // 16 columns, 2 rows
  lcd.clear();
  lcd.print(werd);
  lcd.setBacklight(HIGH);

  lcd.createChar(upArrowChar, arrowBitmap);
}

void SetupInputs() {
  pinMode(button, INPUT_PULLUP);
}

void setup() {
  SetupLCD();
  Serial.begin(115200);
  SetupInputs();
}

void KnobArrow() {
  int knobValue = map(analogRead(knob), 0,1023, 0,15);
  if (knobValue != lastKnob) {
    if (lastKnob!=-1) {
      lcd.setCursor(lastKnob, 1);
      lcd.write(' ');
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
