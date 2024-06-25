/* ------------------------------------------------------------------------------- */
// Weather Station v1.3
//Transmitter part

//CRC Update
#ifndef _UTIL_CRC16_H_
#define _UTIL_CRC16_H_


/** @ingroup util_crc16
    @param uint16_t crc (0x0000..0xFFFF)
    @param uint8_t a (0x00..0xFF)
    @return calculated CRC (0x0000..0xFFFF)
*/
static uint16_t crc16_update(uint16_t crc, uint8_t a)
{
  int i;
  crc ^= a;
  for (i = 0; i < 8; ++i)
  {
    if (crc & 1)
      crc = (crc >> 1) ^ 0xA001;
    else
      crc = (crc >> 1);
  }
  return crc;
}
#endif /* _UTIL_CRC16_H_ */


#include <Wire.h> //Management of wire connections
#include <VirtualWire.h> //Management of Virtual Wire for RF transmission
#include <Adafruit_BMP085.h> //Manage BMP180 temp, humidity, pressure sensor
#include <stdlib.h> //Library for string conversion

//BMP180
#define seaLevelPressure_hPa 1013.25
Adafruit_BMP085 bmp;
  
//DHT11 Setup
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTTYPE    DHT11     // DHT 11
#define DHTPIN 2
DHT_Unified dht(DHTPIN, DHTTYPE);

// // Create the BME280 object
// BME280_I2C bme;              // I2C using default 0x77 
// //BME280_I2C bme(0x76);  // I2C using address 0x76
// //int backLight = 13;    // pin 13 will control the backlight managed by button

const int TX_DIO_Pin = 7; //Definition of Transmission DATA PIN
static char outstr[10]="0.00"; //Buffer String for convert BME readed values
static char message[12]={0};

void setup()
{
  Serial.begin(9600); //Serial communication 9600
  if (!bmp.begin()) { //BMP180
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }
  initialize_transmitter();     //Initialize transmitter
  dht.begin(); //DHT11
  sensor_t sensor;
}


void loop()                     //Main looping code
{ 
dtostrf(bmp.readPressure(), 6, 2, outstr);                            //Read Pressure and convert in string

 strcat(message,"P");
 strcat(message,outstr);
  
 vw_send((int8_t *)message, strlen(message));                              //Transmitt Pressure
 //Serial.println(bme.getPressure_MB());
 message[0] = (char)0;
 delay(1000);                                                             //Delay for transmission
 
 //Humidity
 sensors_event_t event;
 dht.humidity().getEvent(&event);
 dtostrf(event.relative_humidity, 5, 2, outstr);                               //Read Humidity and convert in string

  strcat(message,"H");
  strcat(message,outstr);
  vw_send((int8_t *)message, strlen(message));                              //Transmitt Humidity
  //Serial.println(bme.getHumidity());
  message[0] = (char)0;
  delay(1000);                                                             //Delay for transmission
  
  dtostrf(bmp.readTemperature(), 5, 2, outstr);                        //Read Temperature in C and convert in string

  strcat(message,"T");
  strcat(message,outstr);
  vw_send((int8_t *)message, strlen(message));                              //Transmitt Temperature in C
  //Serial.println(bme.getTemperature_C());
  message[0] = (char)0;
  delay(1000);                                                             //Delay for transmission
  
  // dtostrf(bmp.readTemperature(), 5, 2, outstr);                    //Read Temperature in F and convert in string

  // strcat(message,"F");
  // strcat(message,outstr);
  // vw_send((int8_t *)message, strlen(message));                              //Transmitt Temperature in F
  // //Serial.println(bme.getTemperature_F());
  // message[0] = (char)0;
  // delay(1000);                                                             //Delay for transmission
}

void initialize_transmitter() {
  vw_set_tx_pin(TX_DIO_Pin);  // Initialises the DIO pin used to send data to the Tx module
  vw_set_ptt_inverted(true);  // Set the transmit logic level (LOW = transmit for this version of module) 
  vw_setup(2000);             // Transmit at 2000 bits per second 
}