//-- JUMBLE VARIABLES --
String randomWords[] = {
  "VITAL", "KANGAROO", "OYSTER", "INTELLIGENT", "DRUNK",  
};

String werd;
String jumble;
int lastKnob = -1;
bool lastPressState;

//-- LCD Variables --
#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"
Adafruit_LiquidCrystal lcd(0x20);

// https://www.seetron.com/apps/app_cceditor.html
byte underlineBitmap[] = {0x9F, 0x9F, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80};
byte arrowBitmap[] = {0x84, 0x8E, 0x9F, 0x84, 0x84, 0x84, 0x80, 0x80};
byte underlineChar = 0;
byte upArrowChar = 1;

//--I/O pins --
const byte button = 4; // Digital pin 4
const byte knob = A0;
#define pressedState LOW


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
  for (byte i = 0; i < wordLen; i++) {
    int randomIndex = random(wordLen);
    byte tmp = jumble[i];
    jumble[i] = jumble[randomIndex];
    jumble[randomIndex] = tmp;
  }
  lcd.print(jumble);
}

void SetupLCD() {
  lcd.begin(16, 2); // 16 columns, 2 rows
  lcd.clear();
  lcd.setBacklight(HIGH);

  lcd.createChar(underlineChar, underlineBitmap);
  lcd.createChar(upArrowChar, arrowBitmap);
}

void SetupInputs() {
  pinMode(button, INPUT_PULLUP);
}

void setup() {
  Serial.begin(115200);
  SetupInputs();
  SetupLCD();
  NewGame();
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

String PickRandomWord() {
  int numberOfRandomWords = sizeof(randomWords) / sizeof(String);
  Serial.print("numberOfRandomWords=");
  Serial.println(numberOfRandomWords);
  int randomWordIndex = random(numberOfRandomWords);
  return randomWords[randomWordIndex];
}

void NewGame() {
  lcd.clear();
  werd = PickRandomWord();
  SetupJumble();
  lastKnob = -1;
}

void Win() {
  lcd.setCursor(0, 1);
  lcd.print("** GOOD JOB!! **");
  if (digitalRead(button) == pressedState) NewGame();
  delay(500);
  lcd.setCursor(0, 1);
  lcd.print("                ");
  if (digitalRead(button) == pressedState) NewGame();
  delay(500);
}

void KnobArrow() {
  bool isPressed = digitalRead(button) == pressedState;
  bool pressChanged = isPressed != lastPressState;
  if (!isPressed && jumble == werd) Win();

  int knobValue = map(analogRead(knob), 0, 1023, 0, 15);
  bool knobMoved = knobValue != lastKnob;
  if (knobMoved || pressChanged) {
    if (lastKnob != -1) {
      lcd.setCursor(lastKnob, 1);
      lcd.write(' ');
      if (isPressed) {
        SwapLetters(knobValue, lastKnob);
      }
    }
    lcd.setCursor(knobValue, 1);
    byte cursorChar = isPressed ? upArrowChar : underlineChar;
    lcd.write(cursorChar);
    lastKnob = knobValue;
    lastPressState = isPressed;
  }
  delay(50);
}

void loop() {
  KnobArrow();
}
