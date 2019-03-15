//-- LCD Variables --
#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"
Adafruit_LiquidCrystal lcd(0);

// https://www.seetron.com/apps/app_cceditor.html
byte arrow1[8] = {0x84, 0x8E, 0x9F, 0x84, 0x84, 0x84, 0x80, 0x80};
byte underline[8] = {0x9F, 0x9F, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80};

//-- BUTTON --
byte button = 4; // Digital pin 4


//-- JUMBLE VARIABLES --
String werd = "ALVIT";

void WaitForButtonPress() {
  while (digitalRead(4) != LOW) {
    delay(100);
  }
  delay(500); // ignore switch bounce
}

void SetupLCD() {
  lcd.begin(16, 2); // 16 columns, 2 rows
  lcd.createChar(0, underline);
  lcd.createChar(1, arrow1);
  lcd.clear();
  lcd.print(werd);
  lcd.setCursor(0, 1);
  lcd.setBacklight(HIGH);

  delay(1000);
  lcd.write(byte(1));

  //lcd.setCursor(3, 0);
  //lcd.blink();
}

void SetupInputs() {
  pinMode(button, INPUT_PULLUP);
}

void setup() {
  SetupLCD();
  Serial.begin(115200);
  SetupInputs();
}

void MovingArrow() {
  for (byte x=0; x<5; x++) {
    lcd.setCursor(x, 1);
    lcd.write(byte(1));
    delay(500);
    lcd.setCursor(x, 1);
    lcd.write(' ');
  }
}
void loop() {
// -- Blinking arrow --
//  lcd.setCursor(0, 1);
//  lcd.write(byte(1));
//  delay(500);
//  lcd.setCursor(0, 1);
//  lcd.write(' ');
//  delay(500);

MovingArrow();
}
