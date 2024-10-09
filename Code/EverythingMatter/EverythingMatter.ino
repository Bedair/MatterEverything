/*

*/

#include <LiquidCrystal.h>

// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  /* Initialize the LCD */
  lcd.begin(16, 2);
  lcd.print("**** Matter ****");
}

void loop() {

  /* print Current value */
  lcd.setCursor(0, 1);
  lcd.print("Current: ");
  lcd.print(1.525412, 3);
  lcd.print(" A");

}

