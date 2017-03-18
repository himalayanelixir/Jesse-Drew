#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// create screen object
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

int encoderState = 0;
int encoderStateHistory = 0;
int triggerCounter = 0;
unsigned long edgeOneTime = 0;
unsigned long edgeTwoTime = 0;
unsigned long timeDifference = 0;
int rps = 0;
int rpm = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(2, INPUT);
  Serial.begin(9600); 
  display.begin(SSD1306_SWITCHCAPVCC,  0x3C);
  display.clearDisplay();
  splashScreen();
  updateDisplay();
}

void loop() {
  encoderState = digitalRead(2);  
  //Serial.println(digitalRead(2));
  if (encoderState == 1 && encoderStateHistory == 0){ 
    if(triggerCounter == 0) {
      edgeOneTime = millis();
      triggerCounter = 1; 
    } else {
      edgeTwoTime = millis();  
      triggerCounter = 0; 
      timeDifference = edgeTwoTime - edgeOneTime;
      rps = 1000/timeDifference;
      rpm = rps * 60;
      //Serial.println(rpm);
      updateDisplay();
      edgeOneTime = 0;
      edgeTwoTime = 0;
      
    }
  }
  encoderStateHistory = encoderState;
}
void splashScreen(){
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Problems?");
  display.setTextSize(1);
  display.println("");
  display.setTextSize(2);
  display.println("Contact:");
  display.setTextSize(1);
  display.println("");
  display.println("harlenbains@ieee.org");
  display.display();
  delay(3000);
  display.clearDisplay();
}
void updateDisplay(){
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("RPM: ");
  display.println(rpm);
  display.display();
    display.print("RPS: ");
  display.println(rps);
  display.display();
  display.clearDisplay();

  }
