int sensorValue;
int digitalValue;


#include "dht.h"
#define dht_apin A1
dht DHT;

//rain
#define sensorPower 7
#define sensorPin 8

//BM180
#include <Wire.h>
#include <SFE_BMP180.h>
SFE_BMP180 bmp180;

//OLED
#include <Adafruit_BMP085.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_BMP085 bmp;
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire, -1);

void setup()
{
  Serial.begin(9600); // sets the serial port to 9600
  //MQ135
  pinMode(13, OUTPUT);
  pinMode(2, INPUT);

 //rain
 pinMode(sensorPower, OUTPUT);
 digitalWrite(sensorPower, HIGH);

//BM180
bool success = bmp180.begin();

  if (success) {
    Serial.println("BMP180 init success");
  }
//OEDL
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
  Serial.println(F("SSD1306 allocation failed"));
  for(;;);
}
  delay(2000);
display.clearDisplay();
display.setTextColor(WHITE);
}

void loop()
{
 //MQ135
  sensorValue = analogRead(0); // read analog input pin 0
  digitalValue = digitalRead(2);
  if (sensorValue > 400)
  {
    digitalWrite(13, HIGH);
  }
  else
    digitalWrite(13, LOW);
    Serial.print("PPM-");
  Serial.println(sensorValue, DEC); // prints the value read
  Serial.println(digitalValue, DEC);


  //DHT
  DHT.read11(dht_apin);

    Serial.print("Current humidity = ");
    Serial.print(DHT.humidity);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(DHT.temperature); 
    Serial.println("C  ");

 //rain

    int val = readSensor();
  Serial.print("Digital Output: ");
  Serial.println(val);

  // Determine status of rain
  if (val) {
    Serial.println("Status: Clear");
  } else {
    Serial.println("Status: It's raining");
  }

  delay(1000);  // Take a reading every second
  Serial.println();

  //BM180
  char status;
  double T, P;
  bool success = false;

  status = bmp180.startTemperature();

  if (status != 0) {
    delay(1000);
    status = bmp180.getTemperature(T);

    if (status != 0) {
      status = bmp180.startPressure(3);

      if (status != 0) {
        delay(status);
        status = bmp180.getPressure(P, T);

        if (status != 0) {
          Serial.print("Pressure: ");
          Serial.print(P);
          Serial.println(" hPa");

          Serial.print("Temperature: ");
          Serial.print(T);
          Serial.println(" C");
        }
      }
    }
  }
display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Temperature: ");
  display.setTextSize(2);
  display.setCursor(0,10);
  display.print(DHT.temperature);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.print("C");
  
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Humidity: ");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(DHT.humidity);
  display.print(" %");
  
  display.display();
  delay(2500);
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Rainfall: ");
  display.setTextSize(2);
  display.setCursor(0,10);
  display.print(val);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.print("");

  display.display();
  delay(2500);
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("AQI: ");
  display.setTextSize(2);
  display.setCursor(0,10);
  display.print(sensorValue);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.print("PPM");

   display.display();
  delay(2500);
  display.clearDisplay();

    display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Pressure: ");
  display.setTextSize(2);
  display.setCursor(0,10);
  display.print(P);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.print("hPa");

    display.display();
  delay(2500);
  display.clearDisplay();
  
}

//  This function returns  sensor output
int readSensor() {
  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
  delay(10);              // Allow power to settle
  int val = digitalRead(sensorPin); // Read the sensor output
  digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
  return val;             // Return the value

  
}
