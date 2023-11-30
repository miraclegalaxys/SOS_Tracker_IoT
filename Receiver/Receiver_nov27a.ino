#include "arduino_secrets.h"
#include <TridentTD_LineNotify.h>
#include "thingProperties.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//-----------------------------------------------------------------

#define LINE_TOKEN "vgV21EKTQpNudqkPjXJrmdEnPc8V07dSkTcrOv6v7lq"
LiquidCrystal_I2C lcd(0x27, 16, 2);
const int ledPin = 25;
unsigned long previousMillis = 0;  // จะเก็บเวลาที่ล่าสุดที่ข้อความถูกส่ง
const long interval = 5000;        // ช่วงเวลาสำหรับส่งข้อความ (5 วินาที)

//-----------------------------------------------------------------

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  LINE.setToken(LINE_TOKEN);
  lcd.init();                      
  lcd.backlight();
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
  pinMode(ledPin, OUTPUT);
}

//-----------------------------------------------------------------

void loop() {
  ArduinoCloud.update();

   unsigned long currentMillis = millis();  // อัปเดตเวลาปัจจุบัน

  if (!led_sos) {
    // If LED SOS is not active, then check if it's time to send a notification
    if (LED_re && (currentMillis - previousMillis >= interval)) 
    {
      previousMillis = currentMillis;
      LINE.notify("Help Me!!");
    }
  }

}

//-----------------------------------------------------------------

void onLEDReChange()  {
  // Add your code here to act upon LEDRe change
   digitalWrite(ledPin, LED_re ? HIGH : LOW); 
}


void onLedButtonChange()  {
  // Add your code here to act upon LedButton change
 if(led_button == 1)
  {
    digitalWrite(ledPin, HIGH);
    LED_re = true;
    lcd.setCursor(6, 0);
    lcd.print("SOS");
    lcd.setCursor(4, 1);
    lcd.print("Received");
  }
  else 
  {
    digitalWrite(ledPin, LOW);
    LED_re = false;
  }
}

//-----------------------------------------------------------------

void onLedSosChange()  {
  // Add your code here to act upon LedSos change
}

//-----------------------------------------------------------------