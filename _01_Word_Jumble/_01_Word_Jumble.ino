//-- LCD Variables --
#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"
Adafruit_LiquidCrystal lcd(0x27); // 0x20

// https://www.seetron.com/apps/app_cceditor.html
byte arrowBitmap[8] = {0x84, 0x8E, 0x9F, 0x84, 0x84, 0x84, 0x80, 0x80};
byte arrowASCII = 0;

String werd = "ALVIT";

void SetupLCD() {
  lcd.begin(16, 2); // 16 columns, 2 rows
  lcd.createChar(arrowASCII, arrowBitmap); // Tell LCD what ASCII 0 should look like
  lcd.clear(); // Clear screen
  lcd.print(werd); // Print the jumble on first line
  lcd.setCursor(0, 1); // Move cursor to beginning of second line
  lcd.setBacklight(HIGH); // Turn on backlight
}

void setup() {
  SetupLCD();
}

void loop() {
  MovingArrow();
}

void MovingArrow() {
  for (byte x = 0; x < 5; x++) {
    lcd.setCursor(x, 1); // Move cursor left to right
    lcd.write(arrowASCII); // Drawing our arrow ASCII character at each column
    delay(400); // Let humans see the arrow for a bit
    lcd.setCursor(x, 1); // Move cursor back...
    lcd.write(' '); // and draw a blank space to erase the arrow before we loop up.
  }
}
