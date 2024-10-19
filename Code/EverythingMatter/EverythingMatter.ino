/*

*/

#include <LiquidCrystal.h>
#include <Matter.h>
#include <MatterLightbulb.h>

MatterLightbulb matter_socket;

/* LCD Pins*/
const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


/* Relay */
#define RELAY_PIN                 (A7)
void inline Relay_Init(void) {pinMode(RELAY_PIN, OUTPUT);}
void inline Relay_Enable_Output(void) {digitalWrite(RELAY_PIN, LOW);}
void inline Relay_Disable_Output(void) {digitalWrite(RELAY_PIN, HIGH);}


/* Current Sensor */
#define CURRENT_SENS_LOG_ENABLE   (0)
#define CURRENT_SAMPLES           (20)
#define AREF_VOLTAGE              (3.3)
#define ADC_RESOLUTION            (4096.0)
#define CURRENT_ZERO_REF          (1.83)
#define VOLT_PER_AMP              (0.185)



void setup() {
  /* Initialize the LCD */
  lcd.begin(16, 2);
  lcd.print("**** Matter ****");

  /* Initialize Relay */
  Relay_Init();
  Relay_Disable_Output();
  delay(1000);

  Serial.begin(115200);

  Matter.begin();
  matter_socket.begin();


  Serial.println("Matter Socket");

  if (!Matter.isDeviceCommissioned()) {
    Serial.println("Matter device is not commissioned");
    Serial.println("Commission it to your Matter hub with the manual pairing code or QR code");
    Serial.printf("Manual pairing code: %s\n", Matter.getManualPairingCode().c_str());
    Serial.printf("QR code URL: %s\n", Matter.getOnboardingQRCodeUrl().c_str());
  }
  while (!Matter.isDeviceCommissioned()) {
    delay(200);
  }

  Serial.println("Waiting for Thread network...");
  while (!Matter.isDeviceThreadConnected()) {
    delay(200);
  }
  Serial.println("Connected to Thread network");

  Serial.println("Waiting for Matter device discovery...");
  while (!matter_socket.is_online()) {
    delay(200);
  }
  Serial.println("Matter device is now online");

}

void loop() {

  /* print Current value */
  lcd.setCursor(0, 1);
  lcd.print("Current: ");
  lcd.print(Calculate_Current(), 3);
  lcd.print(" A");

  static bool matter_socket_last_state = false;
  bool matter_socket_current_state = matter_socket.get_onoff();

  // If the current state is ON and the previous was OFF - turn on the Power
  if (matter_socket_current_state && !matter_socket_last_state) {
    matter_socket_last_state = matter_socket_current_state;
    Relay_Enable_Output();
    Serial.println("Power ON");
  }

  // If the current state is OFF and the previous was ON - turn off the Power
  if (!matter_socket_current_state && matter_socket_last_state) {
    matter_socket_last_state = matter_socket_current_state;
    Relay_Disable_Output();
    Serial.println("Power OFF");
  }

  delay(500);

}

float Calculate_Current(void)
{

  unsigned long long RawSum = 0;
  unsigned int RawValue = 0;
  float Voltage = 0.0;
  float current = 0.0;

  /* Get multiple  Current readings then take the average */
  for (int i = 0; i < CURRENT_SAMPLES; i++)
  {
    RawValue = analogRead(A0);
    RawSum += RawValue;
    delay(2);
  }
  RawValue = RawSum / CURRENT_SAMPLES;

  /* Convert the ADC Raw Counts to Current */
  Voltage = RawValue * AREF_VOLTAGE / ADC_RESOLUTION;
  current = (Voltage - CURRENT_ZERO_REF) / VOLT_PER_AMP;

#if CURRENT_SENS_LOG_ENABLE
  Serial.print(RawValue);
  Serial.print(" , ");
  Serial.print(Voltage);
  Serial.print(" , ");
  Serial.println(current);
#endif 

  /* Set the cut low current */
  if (current < 0.02)
  {
    current = 0.0;
  }
  return current;
}

