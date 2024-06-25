/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "**************"      //Paste your credentials here
#define BLYNK_DEVICE_NAME "*****************************"  //Paste your credentials here

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

//DHT
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTTYPE    DHT11     // DHT 11
#define DHTPIN 2
DHT_Unified dht(DHTPIN, DHTTYPE);

//DSB1820
#include <OneWire.h>
#include <DallasTemperature.h>
// Data wire is conntec to the Arduino digital pin 4
#define ONE_WIRE_BUS 4
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

//BMP180
#include <Wire.h>
#include <Adafruit_BMP085.h>
#define seaLevelPressure_hPa 1013.25
Adafruit_BMP085 bmp;

// You should get Auth Token in the Blynk App.
char auth[] = "*************************5m8a";//Paste auth token you copied

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "******************";///Enter your wifi name
char pass[] = "******************";// Enter wifi password

// #define DHTPIN 32         // What digital pin we're connected to select yours accordingly

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h); // select your virtual pins accordingly
  Blynk.virtualWrite(V6, t); // select your virtual pins accordingly
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  delay(1000);
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
  if (!bmp.begin()) { //BMP180
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }
  dht.begin();
  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
  sensor_t sensor;
}

void loop()
{
  Blynk.run();
  timer.run();
}