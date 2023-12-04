
#include "arduino_secrets.h" 
#include <TinyGPS++.h> 
#include "thingProperties.h" 
#include <ArduinoIoTCloud.h> 
#include <SoftwareSerial.h> 

//-----------------------------------------------------------------

TinyGPSPlus gps;  
const int buttonPin = 27;  
const int ledPin = 25;  
bool lastButtonState = LOW;  
double Lat;  
double Long;  
#define gpsRxPin 16  
#define gpsTxPin 17  
SoftwareSerial neo6m(gpsTxPin, gpsRxPin); 

//-----------------------------------------------------------------

void setup() {
  Serial.begin(9600);  
  neo6m.begin(9600);  
  delay(1500);  
  initProperties();  
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);  
  setDebugMessageLevel(2);  
  ArduinoCloud.printDebugInfo();  
  pinMode(buttonPin, INPUT_PULLUP);  
  pinMode(ledPin, OUTPUT);  
}

//-----------------------------------------------------------------

void loop() 
{

  bool currentButtonState = digitalRead(buttonPin);  

  if (currentButtonState == LOW && lastButtonState == HIGH) {  
    Serial.println("SOS Button pressed!!");  
    led_button = 1;  
    while(true) {  
      smartdelay_gps(1000);  
      if (gps.location.isValid()) {  
        Serial.println("New SOS tracking location: ");  
        latitude = String(gps.location.lat(), 6);  
        longitude = String(gps.location.lng(), 6);  
        Lat = latitude.toDouble();  
        Long = longitude.toDouble();  
        maptrack = {Lat, Long}; 
        ArduinoCloud.update();  
        Serial.print("Lat: ");
        Serial.print(latitude + " , ");
        Serial.println(Lat, 6);
        Serial.print("Long: ");
        Serial.print(longitude + " , ");
        Serial.println(Long, 6);
      }
    }
  }

  lastButtonState = currentButtonState;  

  ArduinoCloud.update();  
}


//-----------------------------------------------------------------

static void smartdelay_gps(unsigned long ms)
{
  unsigned long start = millis();  
  do 
  {
    while (neo6m.available())  
      gps.encode(neo6m.read());  
  } while (millis() - start < ms);  
}

//-----------------------------------------------------------------

void onMaptrackChange()  {
  Serial.println("SOS Maptrack Is Ready");  
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
    digitalWrite(ledPin, HIGH);  
    Serial.println("SOS has already received"); 
  } else {
    digitalWrite(ledPin, LOW);  
    Serial.println("SOS has not received");  
  }
}

//-----------------------------------------------------------------

void onLedButtonChange()  {

}

//-----------------------------------------------------------------