#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL64PxYQyLh"
#define BLYNK_TEMPLATE_NAME "Weather Station"
#define seaLevelPressure_hPa 1013.25
#include <WiFi.h> // importing all the required libraries
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "Arduino.h"
#include <Adafruit_BMP085.h> //Manage BMP180 temp, humidity, pressure sensor
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>


float temperature; // parameters
float humidity;
Adafruit_BMP085 bmp;
float pressure;
float mbar;

char auth[] = "fZQmfqI5nESBUTMIo50llWBP_xW6mzWt"; // replace this with your auth token
char ssid[] = "Wifi_ni_Timo_2"; // replace this with your wifi name (SSID)
char pass[] = "XDMqdFeG"; // replace this with your wifi password

#define DHTPIN 5 // dht sensor is connected to D5
#define DHTTYPE DHT11     // DHT 11

DHT dht(DHTPIN, DHTTYPE); // initialise dht sensor
BlynkTimer timer;

void sendSensor() // function to read sensor values and send them to Blynk
{
  humidity = dht.readHumidity();
  temperature = bmp.readTemperature();
  pressure = bmp.readPressure(); // read pressure value in pascals
  mbar = pressure / 100; // convert millibar to pascals

  Blynk.virtualWrite(V0, temperature); // send all the values to their respective virtual pins
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V2, mbar);
}

void setup()
{
  Serial.begin(115200);
  sensor_t sensor;
  dht.begin();
  delay(1000);
  Blynk.begin(auth, ssid, pass);
  delay(1000);
  timer.setInterval(1000L, sendSensor); // sendSensor function will run every 1000 milliseconds
}

void loop()
{
  Blynk.run();
  timer.run();
}
