#include "arduino_secrets.h" //เรียกใช้ไฟล์ arduino_secrets.h
#include <TridentTD_LineNotify.h> //เรียกใช้ไลบรารีการแจ้งเตือนผ่านไลน์
#include "thingProperties.h" //เรียกใช้ไฟล์ thingProperties.h
#include <ArduinoIoTCloud.h> //เรียกใช้ Cloud ArduinoIOTCloud
#include <U8g2lib.h>

//-----------------------------------------------------------------

#define LINE_TOKEN "vgV21EKTQpNudqkPjXJrmdEnPc8V07dSkTcrOv6v7lq"
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE); // OLED Constructor
const int ledPin = 25;
unsigned long previousMillis = 0;  // เก็บเวลาที่ล่าสุดที่ข้อความถูกส่ง
const long interval = 5000;        // กำหนดช่วงเวลาสำหรับส่งข้อความ (5 วินาที)
const int buzzerPin = 26;
unsigned long currentMillis;

//-----------------------------------------------------------------

void setup() {
  Serial.begin(9600);
  u8g2.begin();
  LINE.setToken(LINE_TOKEN);
  delay(1500); 
  pinMode(buzzerPin, OUTPUT);

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

  currentMillis = millis();  // อัปเดตเวลาปัจจุบัน

  if (!led_sos) {
    // If LED SOS is not active, then check if it's time to send a notification
    linenotify();

    if (LED_re)
    {
      tone(buzzerPin, 1000);
      delay(250);

      noTone(buzzerPin);
      delay(250);
    }
  }

}

//-----------------------------------------------------------------

void linenotify() {

    if (LED_re && (currentMillis - previousMillis >= interval)) 
    {
      previousMillis = currentMillis;
      LINE.notify("Help Me!!");
    }
}

//-----------------------------------------------------------------

void onLEDReChange()  {
  // Add your code here to act upon LEDRe change
   digitalWrite(ledPin, LED_re ? HIGH : LOW); 
}

//-----------------------------------------------------------------

void onLedButtonChange()  {
  // Add your code here to act upon LedButton change
 if(led_button == 1)
  {
    digitalWrite(ledPin, HIGH);
    LED_re = true;
    u8g2.clearBuffer();                   // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr);   // set the font
    u8g2.drawStr(0, 10, "First line");    // draw string on the first line
    u8g2.drawStr(0, 20, "Second line");   // draw string on the second line
    u8g2.sendBuffer();                    // transfer internal memory to the display
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