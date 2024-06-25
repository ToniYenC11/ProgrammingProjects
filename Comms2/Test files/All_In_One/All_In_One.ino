//System Test Code
//BMP180 Setup
#include <Wire.h>
#include <Adafruit_BMP085.h>
#define seaLevelPressure_hPa 1013.25
Adafruit_BMP085 bmp;

//DSB1820 Setup
#include <OneWire.h>
#include <DallasTemperature.h>
// Data wire is conntec to the Arduino digital pin 4
#define ONE_WIRE_BUS 4
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);
  
//DHT11 Setup
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTTYPE    DHT11     // DHT 11
#define DHTPIN 2
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;


void setup() {
  Serial.begin(9600);
  sensors.begin(); //DSB180
  if (!bmp.begin()) { //BMP180
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }
  dht.begin(); //DHT11
  sensor_t sensor;
  delayMS = sensor.min_delay / 1000;
}
  
void loop() {
    sensors.requestTemperatures(); 
  
    Serial.print("Air Temperature: ");
    Serial.print(sensors.getTempCByIndex(0)); 
    Serial.print("\nAverage Air Temperature = ");
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");
     
    Serial.print("Pressure = ");
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");

    Serial.print("Pressure at sealevel (calculated) = ");
    Serial.print(bmp.readSealevelPressure());
    Serial.println(" Pa");

    //DHT11
    sensors_event_t event;
    dht.humidity().getEvent(&event);
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%\n"));
    delay(1000);
}