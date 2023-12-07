#include "arduino_secrets.h"
//เรียกใช้ไฟล์ arduino_secrets.h
#include <TridentTD_LineNotify.h> //เรียกใช้ไลบรารีการแจ้งเตือนผ่านไลน์
#include "thingProperties.h" //เรียกใช้ไฟล์ thingProperties.h
#include <ArduinoIoTCloud.h> //เรียกใช้ Cloud ArduinoIoTCloud
//#include <U8g2lib.h> //เรียกใช้จอคำสั่งจอแสดงผล
#include <Wire.h> //ที่ใช้ในการสื่อสารผ่านโปรโตคอล I2C หรือ TWI (Two-Wire Interface) SDA SCL
//SDA สำหรับการส่งข้อมูลไปและกลับระหว่างอุปกรณ์ SCL ส่งสัญญาณนาฬิกาเพื่อกำหนดจังหวะของการส่งข้อมูลในโปรโตคอล I2C
#include <LiquidCrystal_I2C.h>  //เรียกใช้จอคำสั่งจอแสดงผล

//-----------------------------------------------------------------

#define LINE_TOKEN "vgV21EKTQpNudqkPjXJrmdEnPc8V07dSkTcrOv6v7lq" //กำหนดโทเค็นสำหรับ LINE Notify.
//U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE); //สร้างอ็อบเจ็คต์ u8g2 สำหรับการควบคุมหน้าจอ OLED รุ่น SH1106 ขนาด 128x64 ผ่าน I2C
LiquidCrystal_I2C lcd(0x27, 16, 2); // ตั้งค่า I2C address 0x27 สำหรับ 16 char 2 line LCD
const int ledPin = 25; //กำหนดพินของ LED
unsigned long previousMillis = 0; // กำหนดตัวแปรเพื่อจัดเก็บเวลาล่าสุดที่เหตุการณ์เกิดขึ้น.
const long interval = 5000; //กำหนดช่วงเวลาที่ต้องรอ 5000 (มิลลิวินาที) = 5 วินาที
const int buzzerPin = 26; //กำหนดพินของบัสเซอร์
unsigned long currentMillis; //ประกาศตัวแปรสำหรับจัดเก็บเวลาปัจจุบัน

//-----------------------------------------------------------------

void setup() {
  Serial.begin(9600); //เริ่มต้นการสื่อสารผ่านพอร์ต Serial ด้วยอัตรา 9600
  //u8g2.begin(); //กำหนดการเริ่มต้นหน้าจอ OLED.
  lcd.init();                      // กำหนดการเริ่มต้น LCD
  lcd.backlight();                 // เปิดไฟหลัง LCD
  LINE.setToken(LINE_TOKEN); //กำหนดโทเค็นที่ใช้สำหรับ LINE Notify = LINE_TOKEN
  delay(1500); //รอ 1.5 วินาที
  pinMode(buzzerPin, OUTPUT); //ตั้งค่าพินของบัสเซอร์เพื่อใช้งานเป็น OUTPUT.

  
  initProperties();

 
  ArduinoCloud.begin(ArduinoIoTPreferredConnection); //เริ่มต้นการเชื่อมต่อกับ Arduino IoT Cloud
  
  setDebugMessageLevel(2); //ตั้งค่าระดับของข้อความดีบัก
  ArduinoCloud.printDebugInfo(); //พิมพ์ข้อมูลดีบัก
  pinMode(ledPin, OUTPUT); //ตั้งค่าพินของ LED เพื่อใช้งานเป็น OUTPUT
}

//-----------------------------------------------------------------

void loop() {

  ArduinoCloud.update(); // อัปเดตสถานะการเชื่อมต่อกับ Arduino IoT Cloud
  currentMillis = millis(); // นับเวลาปัจจุบัน (ใน milliseconds) และเก็บค่าไว้ในตัวแปร currentMillis

  if (!led_sos) { //ตรวจสอบว่า led_sos เป็นเท็จหรือไม่
    linenotify(); //เรียกใช้ฟังก์ชัน linenotify
    buzzer(); //เรียกใช้ฟังก์ชัน buzzer
  }

}

//-----------------------------------------------------------------

void buzzer() {  // ฟังก์ชัน buzzer
    if (LED_re) {  // ตรวจสอบว่า LED_re เป็นจริงหรือไม่
        tone(buzzerPin, 1000);  // ส่งสัญญาณเสียงผ่าน Buzzer ที่ pin ที่กำหนด (buzzerPin) ที่ความถี่ 1000 Hz
        delay(250);  // รอเวลา 250 milliseconds

        noTone(buzzerPin);  // ปิดเสียง Buzzer ที่ pin ที่กำหนด
        delay(250);  // รอเวลา 250 milliseconds
    }
}

//-----------------------------------------------------------------

void linenotify() {  // ฟังก์ชัน linenotify
    if (LED_re && (currentMillis - previousMillis >= interval)) 
    {  // ตรวจสอบว่า LED_re เป็นจริง และผ่านเงื่อนไขเวลา
        previousMillis = currentMillis;  // อัปเดตค่า previousMillis เป็นค่า currentMillis

        LINE.notify("Help Me!!");  // ส่งข้อความ "Help Me!!" ผ่าน LINE Notify
    }
}


//-----------------------------------------------------------------

void onLEDReChange()  {  // ฟังก์ชัน onLEDReChange
    digitalWrite(ledPin, LED_re ? HIGH : LOW);  // เปลี่ยนสถานะของ LED ตามค่า LED_re
}

//-----------------------------------------------------------------

void onLedButtonChange()  
{  // ฟังก์ชัน onLedButtonChange
    if (led_button == 1) 
    {  // ตรวจสอบค่าของ led_button
        activateSOSMode();  // เรียกใช้ฟังก์ชัน activateSOSMode
    } 
    else 
    {
        deactivateSOSMode();  // เรียกใช้ฟังก์ชัน deactivateSOSMode
    }
}

//-----------------------------------------------------------------

void activateSOSMode() 
{  // ฟังก์ชัน activateSOSMode
    digitalWrite(ledPin, HIGH);  // เปิด LED ที่ pin ที่กำหนด
    LED_re = true;  // ตั้งค่า LED_re เป็นจริง
    displaySOSMessage();  // เรียกใช้ฟังก์ชัน displaySOSMessage
}

//-----------------------------------------------------------------

void deactivateSOSMode() 
{  // ฟังก์ชัน deactivateSOSMode
    digitalWrite(ledPin, LOW);  // ปิด LED ที่ pin ที่กำหนด
    LED_re = false;  // ตั้งค่า LED_re เป็นเท็จ
}


//-----------------------------------------------------------------

void displaySOSMessage() 
{  // ฟังก์ชัน displaySOSMessage
    /*u8g2.clearBuffer();  // ลบข้อมูลบัฟเฟอร์ของหน้าจอ
    u8g2.setFont(u8g2_font_profont22_tr);  // เลือก font ขนาด 22
    u8g2.drawStr(50, 20, "SOS");  // สร้างข้อความ "SOS" ที่ตำแหน่ง (50, 20)
    u8g2.drawStr(40, 40, "ALERT");  // สร้างข้อความ "ALERT" ที่ตำแหน่ง (40, 40)
    u8g2.sendBuffer();  // ส่งข้อมูลบัฟเฟอร์ไปยังหน้าจอ*/

    lcd.clear();                     // ล้างหน้าจอ LCD
    lcd.setCursor(7, 0);             // ตั้งตำแหน่งเคอร์เซอร์
    lcd.print("SOS");                // พิมพ์ข้อความบน LCD
    lcd.setCursor(6, 1);             // ตั้งตำแหน่งเคอร์เซอร์บนแถวที่สอง
    lcd.print("ALERT");              // พิมพ์ข้อความบน LCD
}


//-----------------------------------------------------------------

void onLedSosChange()  {
}

//-----------------------------------------------------------------