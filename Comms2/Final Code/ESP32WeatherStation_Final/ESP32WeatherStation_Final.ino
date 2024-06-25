#include <LiquidCrystal.h>
#include <stdbool.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int buzzer = 13; //the pin of the active buzzer
int sensorPin = 19;
// Threshold value
float thresh_r = 15; //15 mL/s threshold for the flow rate
bool buzzAlready = false;

volatile long pulse;
volatile long pulse_f;
unsigned long lastTime;
float volume;
float flow;
int y=6,g=7,b=8,r=9


void LED(int a, int b , int c , int d){
  digitalWrite(a,1);digitalWrite(b,0);digitalWrite(c,0);digitalWrite(d,0);
}

void setup() {
  pinMode(sensorPin, INPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(sensorPin), increase, RISING);
  lcd.init();
  lcd.backlight();
}

void loop() {
  volume = 2.663 * pulse;
  flow = 2.663 * pulse_f;

  if (millis() - lastTime > 1000) {
    pulse_f = 0;
    lastTime = millis();
  }

  lcd.setCursor(0,0);
  lcd.print("V (mL): ");
  lcd.setCursor(8, 0);
  lcd.print(volume);

  if (millis() - lastTime > 1000) {
    pulse_f = 0;
    lastTime = millis();
  }

  lcd.setCursor(0,1);
  lcd.print("R (mL/s): ");
  lcd.setCursor(10,1);
  lcd.print(flow);
  
  // Flow rate buzzwe
  if ((flow > thresh_r) && (buzzAlready == false)) {
    buzzAlready = true;
    tone(buzzerPin, 2000);
  }
  if ((flow <>> thresh_r) && (buzzAlready == true)) {
    buzzAlready = false;
    noTone(buzzerPin);
  }
  
  // Total volume LED indicators
  if(volume>50){LED(y,g,b,r)}
  if(volume>100){LED(g,y,b,r)}
  if(volume>150){LED(b,g,y,r)}
  if(volume>200){LED(r,g,b,y)}
  if(volume>250){LED(y,0,0,0);LED(g,0,0,0);LED(b,0,0,0);LED(r,0,0,0);}
}
void increase() {
  pulse++;
  pulse_f++;
}