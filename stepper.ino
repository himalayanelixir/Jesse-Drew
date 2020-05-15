#include <AccelStepper.h>
AccelStepper stepper(1, 9, 8);
int stepperSpeed = 100;
int count = 0;
const int pushButton1 = 2; 
const int pushButton2 = 3;
int buttonCounter1 = 0;
int buttonCounter2 = 0;
void setup(){ 
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(8, LOW);
  pinMode(pushButton1, INPUT_PULLUP);
  pinMode(pushButton2, INPUT_PULLUP);
  while (count <= 10000)
  {
    digitalWrite(9, LOW);  // This LOW to HIGH change is what creates the
    digitalWrite(9, HIGH); // "Rising Edge" so the easydriver knows to when to step.
    delayMicroseconds(500);      // This delay time is close to top speed for this
    count++;
  }
  count = 0;
  
    while (count <= 10000)
  {
    digitalWrite(9, LOW);  // This LOW to HIGH change is what creates the
    digitalWrite(9, HIGH); // "Rising Edge" so the easydriver knows to when to step.
    delayMicroseconds(200);      // This delay time is close to top speed for this
    count++;
  }

    count = 0;
  
    while (count <= 10000)
  {
    digitalWrite(9, LOW);  // This LOW to HIGH change is what creates the
    digitalWrite(9, HIGH); // "Rising Edge" so the easydriver knows to when to step.
    delayMicroseconds(150);      // This delay time is close to top speed for this
    count++;
  }

      count = 0;
  
    while (count <= 10000)
  {
    digitalWrite(9, LOW);  // This LOW to HIGH change is what creates the
    digitalWrite(9, HIGH); // "Rising Edge" so the easydriver knows to when to step.
    delayMicroseconds(125);      // This delay time is close to top speed for this
    count++;
  }

   count = 0;
  
    while (count <= 10000)
  {
    digitalWrite(9, LOW);  // This LOW to HIGH change is what creates the
    digitalWrite(9, HIGH); // "Rising Edge" so the easydriver knows to when to step.
    delayMicroseconds(100);      // This delay time is close to top speed for this
    count++;
  }
     count = 0;
  

}
void loop() {

    int pushButton1Val = digitalRead(pushButton1);
  int pushButton2Val = digitalRead(pushButton2);
  
    digitalWrite(9, LOW);  // This LOW to HIGH change is what creates the
    digitalWrite(9, HIGH); // "Rising Edge" so the easydriver knows to when to step.
    delayMicroseconds(stepperSpeed);      // This delay time is close to top speed for this

    // button 1 *************************************************

  if (pushButton1Val == LOW) {
    buttonCounter1 = buttonCounter1 + 1;
  } else {
    buttonCounter1 = 0;
  }

  if (buttonCounter1 > 5000) {
    buttonCounter1 = 0; 
    // increment variable
    if (stepperSpeed > 50) {
      stepperSpeed = stepperSpeed - 10;
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
    if (stepperSpeed < 20000) {
      stepperSpeed = stepperSpeed + 10;
    } 
  }
}


