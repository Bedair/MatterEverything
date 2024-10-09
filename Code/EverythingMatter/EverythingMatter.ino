/*

*/

#include <LiquidCrystal.h>

/* LCD Pins*/
const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


/* Relay */
#define RELAY_PIN                 (A7)
void inline Relay_Init(void) {pinMode(RELAY_PIN, OUTPUT);}
void inline Relay_Enable_Output(void) {digitalWrite(RELAY_PIN, LOW);}
void inline Relay_Disable_Output(void) {digitalWrite(RELAY_PIN, HIGH);}


void setup() {
  /* Initialize the LCD */
  lcd.begin(16, 2);
  lcd.print("**** Matter ****");

  /* Initialize Relay */
  Relay_Init();
  Relay_Disable_Output();
  delay(1000);

}

void loop() {

  /* print Current value */
  lcd.setCursor(0, 1);
  lcd.print("Current: ");
  lcd.print(1.525412, 3);
  lcd.print(" A");

  Relay_Enable_Output();

}

