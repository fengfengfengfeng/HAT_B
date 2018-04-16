#include <Adafruit_CircuitPlayground.h>
#include <math.h>

float X, Y, Z;
float lastX = 0;
float lastY = 0;
float gap;

float value;
int currentSpeed = 1;
static int speeds[] = { 5, 10, 50, 100 };

bool pressState = false;

int num =2;
int mysensorpin = A1;
int sensorReading;
int ledPin = A7;

void setup() {
  // put your setup code here, to run once:
  CircuitPlayground.begin();
  Serial.begin(9600);
  pinMode(mysensorpin,INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorReading = analogRead(mysensorpin);
  Serial.print("Analog reading = ");
  Serial.println(sensorReading);
  
  X = CircuitPlayground.motionX();
  Y = CircuitPlayground.motionY();
  Z = CircuitPlayground.motionZ();
//  Serial.print("X: ");
//  Serial.print(X);
//  Serial.print("  Y: ");
//  Serial.print(Y);
//  Serial.print("  Z: ");
//  Serial.println(Z);

  if(X != lastX || Y != lastY){
    gap = (abs(lastX - X) + abs(lastY - Y))/2;
    Serial.print("gap:");
    Serial.println(gap);
    delay(100); 
  }
  lastX = X;
  lastY = Y;
   
/*************** here is the code for sensor***************/
  if(sensorReading > 80){
    Serial.println("led on");
    CircuitPlayground.redLED(HIGH);
    CircuitPlayground.playTone(500, 100);
    pressState = true; 
    for(int i=0; i<10; ++i) {
      CircuitPlayground.strip.setPixelColor(i, 37,18,225);
    }
    CircuitPlayground.strip.show();
    delay(1000);
  }else{
    Serial.println("led off");
    CircuitPlayground.redLED(LOW);
    CircuitPlayground.clearPixels();
    pressState = false;
  }

  if (gap >1 && pressState == false){
    Serial.println("blink");
    for (int i=0; i <num; i++){
      for(int i=0; i<10; ++i) {
      CircuitPlayground.strip.setPixelColor(i, 235,89,86);
      }
      CircuitPlayground.strip.show();
      delay(500);
      CircuitPlayground.clearPixels();
      delay(500);
      }
  }else{
    Serial.println("blink off");
    analogWrite(ledPin,0);
  }
 /*************** here is the code for sound detect and neon pixel***************/ 
 value = CircuitPlayground.mic.soundPressureLevel(10);
 Serial.print("Sound Sensor SPL: ");
 Serial.println(value);
 uint32_t offset = millis() / speeds[currentSpeed];
  if (value > 90){    
    for(int i=0; i<10; ++i) {
    CircuitPlayground.strip.setPixelColor(i, CircuitPlayground.colorWheel(((i * 256 / 10) + offset) & 255));
  }
  // Show all the pixels.
  CircuitPlayground.strip.show();
  delay(500);
  }else{
    CircuitPlayground.clearPixels();
  }
}
