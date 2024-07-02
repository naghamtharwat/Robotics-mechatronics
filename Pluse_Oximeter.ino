//by ENG\Nagham Tharwat Ramadan

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"


#define MAX30100_REGISTERS_H
#define MAX30100_I2C_ADDRESS                    0x57
#define MAX30100_REG_SPO2_CONFIGURATION         0x04
#define MAX30100_SPC_SPO2_HI_RES_EN             (1 << 6)            
#define REPORTING_PERIOD_MS     5000
#define MAX30100_REG_LED_CONFIGURATION 

LiquidCrystal_I2C lcd(0x27, 16, 2);
const int buzzerPin = 9;

byte heart[] = {
  B00000,
  B00000,
  B11011,
  B11111,
  B01110,
  B00100,
  B00000,
  B00000
};

byte norate[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B00000,
  B00000,
  B00000
};

PulseOximeter pox;
uint32_t tsLastReport = 0;

void onBeatDetected()
{
  Serial.println("Beat!!!");
  tone(buzzerPin, 1000 ,100);
}

void setup()
{
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.createChar(1 , heart);
  lcd.createChar(2 , norate);
  lcd.setCursor(0, 0);
  lcd.print("     Pulse");
  lcd.setCursor(0, 1);
  lcd.print("    Oximeter");

  delay(3000);
  
  
  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;) ;
  } else {
    Serial.println("SUCCESS");
  }

  // Increase LED current for brighter output
  pox.setIRLedCurrent(MAX30100_LED_CURR_33_8MA);
  pox.begin(REPORTING_PERIOD_MS);
  pox.setOnBeatDetectedCallback(onBeatDetected);
  
}

void loop()
{
      
  pox.update();
   
  
    if (millis() - tsLastReport >= REPORTING_PERIOD_MS) {
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("HeartRate: ");
      lcd.print(pox.getHeartRate());
      lcd.setCursor(0, 1);
      lcd.print("SpO2: ");
      lcd.print(pox.getSpO2());
      
      tsLastReport = millis();
    
      
      // Display appropriate icon based on SpO2 level
      if (pox.getSpO2() >= 96) {
        lcd.setCursor(15, 1);
        lcd.write(1);
        
      }
      else if (pox.getSpO2() <= 95 && pox.getSpO2() >= 91) {
        lcd.setCursor(15, 1);
        lcd.write(1);
        
      }
      else if (pox.getSpO2() <= 90) {
        lcd.setCursor(15, 1);
        lcd.write(2);
        
      }
    }
  
}

