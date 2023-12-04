
#include "arduino_secrets.h" //เรียกใช้ไฟล์ arduino_secrets.h
#include <TinyGPS++.h> //เรียกใช้ไลบรารีสำหรับการจัดการข้อมูล GPS
#include "thingProperties.h" //เรียกใช้ไฟล์ thingProperties.h
#include <ArduinoIoTCloud.h> //เรียกใช้ Cloud ArduinoIOTCloud
#include <SoftwareSerial.h> //เรียกใช้ไลบรารีสำหรับการจัดการสร้างพอร์ต Serial เสมือน (RX TX)

//-----------------------------------------------------------------

TinyGPSPlus gps;  // สร้างอ็อบเจกต์ TinyGPSPlus สำหรับใช้ในการจัดการข้อมูล GPS
const int buttonPin = 27;  // กำหนดหมายเลขขาของปุ่ม SOS
const int ledPin = 25;  // กำหนดหมายเลขขาของ LED
bool lastButtonState = LOW;  // เก็บสถานะปุ่มของรอบที่แล้ว
double Lat;  // ตัวแปรเก็บค่าละติจูด
double Long;  // ตัวแปรเก็บค่าลองจิจูด
#define gpsRxPin 16  // กำหนดหมายเลขขาของ RX สำหรับการติดต่อกับโมดูล GPS
#define gpsTxPin 17  // กำหนดหมายเลขขาของ TX สำหรับการติดต่อกับโมดูล GPS
SoftwareSerial neo6m(gpsTxPin, gpsRxPin);  // สร้างอ็อบเจกต์ SoftwareSerial สำหรับการติดต่อกับโมดูล GPS

//-----------------------------------------------------------------

void setup() {
  Serial.begin(9600);  // เริ่มการใช้งาน Serial ที่ความเร็ว 9600 bps
  neo6m.begin(9600);  // เริ่มการใช้งาน SoftwareSerial ที่ความเร็ว 9600 bps
  delay(1500);  // รอ 1500 milliseconds 1.5 วินาที
  initProperties();  // เรียกใช้ฟังก์ชัน initProperties ที่ไว้สำหรับกำหนดค่าเริ่มต้น properties
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);  // เริ่มการเชื่อมต่อกับ Arduino IoT Cloud
  setDebugMessageLevel(2);  // ตั้งระดับการแสดงข้อมูล debug
  ArduinoCloud.printDebugInfo();  // แสดงข้อมูล debug ของ Arduino IoT Cloud
  pinMode(buttonPin, INPUT_PULLUP);  // กำหนดปุ่ม SOS เป็น INPUT แบบ Pull-up
  pinMode(ledPin, OUTPUT);  // กำหนด LED เป็น OUTPUT
}

//-----------------------------------------------------------------

void loop() 
{

  bool currentButtonState = digitalRead(buttonPin);  // อ่านสถานะปุ่มปัจจุบัน

  if (currentButtonState == LOW && lastButtonState == HIGH) {  // ตรวจสอบว่าปุ่มถูกกดหรือไม่
    Serial.println("SOS Button pressed!!");  // แสดงข้อความบน Serial Monitor
    led_button = 1;  // กำหนดค่า led_button เป็น 1
    while(true) {  // ทำลูปไปเรื่อยๆ
      smartdelay_gps(1000);  // อัพเดตข้อมูล GPS ทุก 1000 milliseconds 1 วินาที
      if (gps.location.isValid()) {  // ตรวจสอบว่าข้อมูล GPS ถูกต้องหรือไม่
        Serial.println("New SOS tracking location: ");  // แสดงข้อความบน Serial Monitor
        latitude = String(gps.location.lat(), 6);  // ดึงข้อมูลละติจูด
        longitude = String(gps.location.lng(), 6);  // ดึงข้อมูลลองจิจูด
        Lat = latitude.toDouble();  // แปลงข้อมูลละติจูดเป็น double
        Long = longitude.toDouble();  // แปลงข้อมูลลองจิจูดเป็น double
        maptrack = {Lat, Long};  // กำหนดค่า maptrack
        ArduinoCloud.update();  // อัพเดตข้อมูลไปยัง Arduino IoT Cloud
        Serial.print("Lat: ");
        Serial.print(latitude + " , ");
        Serial.println(Lat, 6);
        Serial.print("Long: ");
        Serial.print(longitude + " , ");
        Serial.println(Long, 6);
      }
    }
  }

  lastButtonState = currentButtonState;  // อัพเดตสถานะปุ่มของรอบที่แล้ว

  ArduinoCloud.update();  // อัพเดตข้อมูลไปยัง Arduino IoT Cloud
}


//-----------------------------------------------------------------

static void smartdelay_gps(unsigned long ms)
{
  unsigned long start = millis();  // เวลาปัจจุบัน
  do 
  {
    while (neo6m.available())  // วนลูปตรวจสอบข้อมูลที่มีอยู่ใน SoftwareSerial
      gps.encode(neo6m.read());  // ถอดรหัสข้อมูล GPS และนำเข้าใน TinyGPSPlus
  } while (millis() - start < ms);  // ทำวงวนไปเรื่อยๆ จนกว่าเวลาที่ผ่านมาจะมีค่ามากกว่าหรือเท่ากับ ms
}

//-----------------------------------------------------------------

void onMaptrackChange()  {
  Serial.println("SOS Maptrack Is Ready");  // แสดงข้อความบน Serial Monitor
}

//-----------------------------------------------------------------

void onLatitudeChange() {


}

//-----------------------------------------------------------------

void onLongitudeChange() {


}

//-----------------------------------------------------------------

void onLEDMapChange()  {
  if (LED_map) {
    digitalWrite(ledPin, HIGH);  // ถ้า LED_map เป็นจริง ให้เปิด LED
    Serial.println("SOS has already received");  // แสดงข้อความบน Serial Monitor
  } else {
    digitalWrite(ledPin, LOW);  // ถ้า LED_map เป็นเท็จ ให้ปิด LED
    Serial.println("SOS has not received");  // แสดงข้อความบน Serial Monitor
  }
}

//-----------------------------------------------------------------

void onLedButtonChange()  {

}

//-----------------------------------------------------------------