#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);

const int contrast = 9;

void setup() {
  // put your setup code here, to run once:
  pinMode(contrast, OUTPUT);
  analogWrite(contrast, 0);
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Weatherball");
  lcd.setCursor(0,1);
  lcd.print("Data loading...");
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
