#include <dummy.h>

#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL64PxYQyLh"
#define BLYNK_TEMPLATE_NAME "Weather Station"

#include <WiFi.h>  // importing all the required libraries
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "Arduino.h"
#include "DHT.h"
#include "BMP085.h"
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
float temperature;  // parameters
float humidity;
float pressure;
float mbar;

char auth[] = "fZQmfqI5nESBUTMIo50llWBP_xW6mzWt";  // replace this with your auth token
char ssid[] = "Virus";                    // replace this with your wifi name (SSID)
char pass[] = "password";                          // replace this with your wifi password

BMP085 myBarometer;  // initialise pressure sensor

#define DHTPIN 5           // dht sensor is connected to D5
#define DHTTYPE DHT11      // DHT 11
DHT dht(DHTPIN, DHTTYPE);  // initialise dht sensor
BlynkTimer timer;
#define AO_PIN 4  // ESP32's pin GPIO34 connected to AO pin of the rain sensor
#define DO_PIN 13  // ESP32's pin GPIO34 connected to AO pin of the rain sensor

void sendSensor()  // function to read sensor values and send them to Blynk
{
  humidity = dht.readHumidity();
  temperature = myBarometer.bmp085GetTemperature(myBarometer.bmp085ReadUT());
  pressure = myBarometer.bmp085GetPressure(myBarometer.bmp085ReadUP());  // read pressure value in pascals
  mbar = pressure / 100;                                                 // convert millibar to pascals

  Blynk.virtualWrite(V0, temperature);  // send all the values to their respective virtual pins
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V2, mbar);
}

void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(115200);
  pinMode(AO_PIN, INPUT);
  myBarometer.init();
  dht.begin();
  delay(1000);
  Blynk.begin(auth, ssid, pass);
  delay(1000);
  timer.setInterval(1000L, sendSensor);  // sendSensor function will run every 1000 milliseconds
}

void loop() {
  Blynk.run();
  timer.run();

  int rain_detect = digitalRead(DO_PIN);
  int rain_value = analogRead(AO_PIN);  // Sensor pin for rainfall

  if (rain_detect = LOW) {
    Blynk.logEvent("rainfall_alert", "Rainfall Detected!");
  }
  // Rainfall detection
  if (rain_value < 1700) {
    // Threshold value 1500 for actaul rainfall detection based
    Blynk.logEvent("heavy", "Heavy Rainfall Detected!");
  }

  Serial.println(rain_value);  // print out the analog value
  delay(1000); // pause for 1 sec to avoid reading sensors frequently to prolong the sensor lifetime
  //LCD I2C
  // delay(1000);
  // lcd.setCursor(0, 0);
  // lcd.print("T :");
  // lcd.print(temperature);
  //  Serial.print("Temperature : ");
  // Serial.println(temperature);
  // lcd.setCursor(9, 0);
  // lcd.print("H :");
  // lcd.print(humidity);
  // Serial.print("Humidity : ");
  // Serial.println(humidity);
  // lcd.setCursor(0, 1);
  // lcd.print("P:");
  // lcd.print(mbar);
  // lcd.print(" ");
  // Serial.print("Pressure : ");
  // Serial.println(mbar);
  // delay(1000);
}
