
#include "arduino_secrets.h" //เรียกใช้ไฟล์ arduino_secrets.h
#include <TridentTD_LineNotify.h> //เรียกใช้ไลบรารีการแจ้งเตือนผ่านไลน์
#include "thingProperties.h" //เรียกใช้ไฟล์ thingProperties.h
#include <ArduinoIoTCloud.h> //เรียกใช้ Cloud ArduinoIoTCloud
#include <U8g2lib.h> //เรียกใช้จอคำสั่งจอแสดงผล

//-----------------------------------------------------------------

#define LINE_TOKEN "vgV21EKTQpNudqkPjXJrmdEnPc8V07dSkTcrOv6v7lq"
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE); 
const int ledPin = 25;
unsigned long previousMillis = 0;  
const long interval = 5000;        
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
  currentMillis = millis();  

  if (!led_sos) {
    linenotify();
    buzzer();
  }

}

//-----------------------------------------------------------------

void buzzer() {
    if (LED_re) {
            tone(buzzerPin, 1000); 
            delay(250); 

            noTone(buzzerPin); 
            delay(250);  
    }
}

//-----------------------------------------------------------------

void linenotify() {
    if (LED_re && (currentMillis - previousMillis >= interval)) 
    {
      previousMillis = currentMillis;
      LINE.notify("Help Me!!");

        tone(buzzerPin, 1000); 
        delay(250); 

        noTone(buzzerPin); 
        delay(250);   
    }
}

//-----------------------------------------------------------------

void onLEDReChange()  {
   digitalWrite(ledPin, LED_re ? HIGH : LOW); 
}

//-----------------------------------------------------------------

void onLedButtonChange()  {
 if(led_button == 1)
  {
    activateSOSMode();
  }
  else 
  {
    deactivateSOSMode();
  }
}

//-----------------------------------------------------------------

void activateSOSMode() {
  digitalWrite(ledPin, HIGH);
  LED_re = true;
  displaySOSMessage();
}

//-----------------------------------------------------------------

void deactivateSOSMode() {
  digitalWrite(ledPin, LOW);
  LED_re = false;
}

//-----------------------------------------------------------------

void displaySOSMessage() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_profont22_tr);
  u8g2.drawStr(50, 20, "SOS");
  u8g2.drawStr(40, 40, "ALERT");
  u8g2.sendBuffer();
}

//-----------------------------------------------------------------

void onLedSosChange()  {
}

//-----------------------------------------------------------------