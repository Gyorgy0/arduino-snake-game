#include <LiquidCrystal_I2C.h>

#define pausestartbuttonPin A0
int loadingDelay = 50;
bool pause = false;
bool start = false;
bool startable = false;
int score = 1;
String scoreDisplay = "Pontszam: ";
String display = scoreDisplay + score;

LiquidCrystal_I2C lcd(0x27,20,4);

void setup() {
  lcd.init();
  lcd.clear();  
  lcd.backlight();
  Serial.begin(9600);

  pinMode(pauseStartButtonPin, INPUT_PULLUP);

  printWithLoadingEffect("Snake game", 3, 0);  
  printWithLoadingEffect("Jan & Gyorgy", 2, 1);
  startable = true;
}

void loop() {
  if (Serial.available()) {
    score = Serial.parseInt();
    display = scoreDisplay + String(score);
    lcd.clear();
  }
  if (digitalRead(pauseStartButtonPin) == HIGH && !start) {
    Serial.write(1);
    start = true;
    lcd.clear();
    delay(1000);
  }
  if (digitalRead(pauseStartButtonPin) == HIGH && start && !pause) {
    Serial.write(2);
    pause = true;
    lcd.clear();
    delay(1000);
  }
  if (digitalRead(pauseStartButtonPin) == HIGH && pause && start) {
    Serial.write(3);
    pause = false;
    lcd.clear();
    delay(1000);
  }
  if (start && !pause) {
    lcd.setCursor(2,0);
    lcd.print(display);
  }
}

void printWithLoadingEffect(const char* text, int column, int row) {
  int textLength = strlen(text);

  for (int i = 0; i < textLength; i++) {
    lcd.setCursor(column + i, row); 
    lcd.print(text[i]);  
    delay(loadingDelay);
  }
}
