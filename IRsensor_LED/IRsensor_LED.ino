// libraries for screen 
#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// create screen object
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

// pin layout 
const int ledPin = 15;
const int pushButton1 = 2; 
const int pushButton2 = 3;
const int pushButton3 = 4;
const int pushButton4 = 5;

// other varibales 
volatile int irInputVal = 0;
int irPin = 14; 
int irThreshold = EEPROM.get(10, irThreshold);
long ledDuration = EEPROM.get(20, ledDuration);
int ledDurationCount = 0;
int ledDelay = EEPROM.get(30, ledDelay); 
volatile int ledDelayCount = 0;

int buttonCounter1 = 0;
int buttonCounter2 = 0;
int buttonCounter3 = 0; 
int buttonCounter4 = 0;


int trigger = 0; 
int triggerHistory = 0;
// for IR sensor 

int ledTrigger = 0;
int ledDelayTemp = 0;
long ledDurationTemp = 0;

// use these to see if we need to update the screen
// we don't do this all the time because it is an expensive operation 
int oldVariableSum = 0;
int variableSum = 0; 
// 
int screenState = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(ledPin, OUTPUT);
  pinMode(pushButton1, INPUT_PULLUP);
  pinMode(pushButton2, INPUT_PULLUP);
  pinMode(pushButton3, INPUT_PULLUP);
  pinMode(pushButton4, INPUT_PULLUP);
  
  display.begin(SSD1306_SWITCHCAPVCC,  0x3C);
  display.clearDisplay();
  splashscreen();
  updatedisplay();

  digitalWrite(ledPin, LOW);
  
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A); 
}

void loop() {
  
  int pushButton1Val = digitalRead(pushButton1);
  int pushButton2Val = digitalRead(pushButton2);
  int pushButton3Val = digitalRead(pushButton3);
  int pushButton4Val = digitalRead(pushButton4);

  if(ledDelayTemp > 0) {
    ledDelayTemp = ledDelayTemp - 1;
  } else if (ledDurationTemp > 0){
    ledDurationTemp = ledDurationTemp - 1;
    digitalWrite(ledPin,HIGH);
  } else {
      digitalWrite(ledPin,LOW);   
  }
 
  
// button 1 *************************************************

  if (pushButton1Val == LOW) {
    buttonCounter1 = buttonCounter1 + 1;
  } else {
    buttonCounter1 = 0;
  }

  if (buttonCounter1 > 5000) {
    buttonCounter1 = 0; 
    // increment variable
    if (screenState < 2) {
      screenState = screenState + 1;
    } else {
      screenState = 0;
    }    
  }
//// button 2 *************************************************
  if (pushButton2Val == LOW) {
    buttonCounter2 = buttonCounter2 + 1;
  } else {
    buttonCounter2 = 0;
  }

  if (buttonCounter2 > 5000) {
    buttonCounter2 = 0; 
    // increment variable
    if (screenState > 0) {
      screenState = screenState - 1;
    } else {
      screenState = 2;
    }
  }
//// button 3 *************************************************
  if (pushButton3Val == LOW) {
    buttonCounter3 = buttonCounter3 + 1;
  } else {
    buttonCounter3 = 0;
  }

  if (buttonCounter3 > 2000) {
    buttonCounter3 = 0; 
    // increment variable
    switch (screenState) {
      case 0: 
        irThreshold = irThreshold - 10;          
        break;
      case 1: 
        ledDuration = ledDuration - 100; 
        break; 
      case 2: 
        ledDelay = ledDelay - 100; 
        break;
    }
  }
//// button 4 *************************************************
  if (pushButton4Val == LOW) {
    buttonCounter4 = buttonCounter4 + 1;
  } else {
    buttonCounter4 = 0;
  }

  if (buttonCounter4 > 2000) {
    buttonCounter4 = 0; 
    // increment variable
    switch (screenState) {
      case 0: 
        irThreshold = irThreshold + 10;          
        break;
      case 1: 
        ledDuration = ledDuration + 100; 
        break; 
      case 2: 
        ledDelay = ledDelay + 100; 
        break;
    }
  }
// end buttons 
// bounds for variables 
  if(irThreshold < 0){
    irThreshold = 0;
  } else if (irThreshold > 1020){
    irThreshold = 1020;
  }

  if(ledDuration < 0){
    ledDuration = 0;
  } else if (ledDuration > 20000){
    ledDuration = 20000;
  }

  if(ledDelay < 0){
    ledDelay = 0;
  } else if (ledDelay > 20000){
    ledDelay = 20000;
  }


  variableSum = irThreshold + ledDuration + ledDelay + screenState;


  if (variableSum != oldVariableSum) {
    updatedisplay();
    EEPROM.put(10, irThreshold);
    EEPROM.put(20, ledDuration);
    EEPROM.put(30, ledDelay); 
  }
  oldVariableSum = variableSum;
}

//***************************************//
// Funcitons 
//***************************************//

SIGNAL(TIMER0_COMPA_vect) {
  
  irInputVal = analogRead(irPin);  
  
  if (irInputVal < irThreshold && triggerHistory == 0){
    trigger = 1;
  }
  
  
  if(irInputVal < irThreshold && ledDurationTemp == 0 && trigger == 1) {      
    ledDelayTemp = ledDelay;
    ledDurationTemp = ledDuration;
    trigger = 0;
  }  
  
  if (irInputVal < irThreshold) {
    triggerHistory = 1;
  } else {
    triggerHistory = 0;
  }

  
} 

void splashscreen(){
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

void updatedisplay(){
  switch (screenState){
    case 0:
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.print("-> Threshold: ");
      display.println(irThreshold);
      display.println("");
      display.print("   Duration: ");
      display.println(ledDuration);
      display.println("");
      display.print("   Delay: ");
      display.println(ledDelay);
      display.println("");
      display.display();
      display.clearDisplay();
      break;
    case 1:
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.print("   Threshold: ");
      display.println(irThreshold);
      display.println("");
      display.print("-> Duration: ");
      display.println(ledDuration);
      display.println("");
      display.print("   Delay: ");
      display.println(ledDelay);
      display.println("");
      display.display();
      display.clearDisplay();
      break;
    case 2: 
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.print("   Threshold: ");
      display.println(irThreshold);
      display.println("");
      display.print("   Duration: ");
      display.println(ledDuration);
      display.println("");
      display.print("-> Delay: ");
      display.println(ledDelay);
      display.println("");
      display.display();
      display.clearDisplay();
      break;
  }
}

