#include "arduino_secrets.h" //เรียกใช้ไฟล์ arduino_secrets.h
#include <TridentTD_LineNotify.h> //เรียกใช้ไลบรารีการแจ้งเตือนผ่านไลน์
#include "thingProperties.h" //เรียกใช้ไฟล์ thingProperties.h
#include <ArduinoIoTCloud.h> //เรียกใช้ Cloud ArduinoIOTCloud
#include <Wire.h> //เรียกใช้สำหรับการสื่อสารผ่านโพรโทคอล I2C
#include <LiquidCrystal_I2C.h> //เรียกใช้ไลบรารีสำหรับสื่อสารกับจอแสดงผล LCD ที่ใช้โพรโทคอล I2C

//-----------------------------------------------------------------

#define LINE_TOKEN "vgV21EKTQpNudqkPjXJrmdEnPc8V07dSkTcrOv6v7lq"
LiquidCrystal_I2C lcd(0x27, 16, 2);
const int ledPin = 25;
unsigned long previousMillis = 0;  // เก็บเวลาที่ล่าสุดที่ข้อความถูกส่ง
const long interval = 5000;        // กำหนดช่วงเวลาสำหรับส่งข้อความ (5 วินาที)

//-----------------------------------------------------------------

void setup() {
  Serial.begin(9600);
  LINE.setToken(LINE_TOKEN);
  lcd.init();                      
  lcd.backlight();
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