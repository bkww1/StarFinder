#include<Wire.h>
#include <Stepper.h>
const int stepsPerRevolution = 2038;

// Creates an instance of stepper class
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
Stepper stepperX = Stepper(stepsPerRevolution, 2, 4, 3, 5);
Stepper stepperY = Stepper(stepsPerRevolution, 8, 10, 9, 11);
int receivedValue = 0;
int out_value = 0;
int numberofMoves = 0;
  void receiveEvent(int byteCount)
{
  while (Wire.available())
  {
    receivedValue = Wire.read();
    Serial.println(receivedValue);
    
  }
  numberofMoves = 1;
}

 void requestEvent(int out_value)
{
  Wire.write(out_value);
}



void setup()
{  
   Wire.begin(8);
   Wire.onReceive(receiveEvent);
   Wire.onRequest(requestEvent);
   Serial.begin(9600);

   stepperX.setSpeed(10);	
 
   stepperY.setSpeed(5);	
}

void loop()
{  
  Serial.println(receivedValue);
  Serial.println(numberofMoves);
  delay(500);
  while (numberofMoves > 0){
  Serial.println(receivedValue);
   if (receivedValue == 1){
     stepperX.step(-3);
   }
   else if (receivedValue == 2){
     stepperX.step(3);
   }

   else if (receivedValue == 3){
     stepperY.step(-3);
   }

   else if (receivedValue == 4){
     stepperY.step(3);}
      numberofMoves = 0;
    
  }
}

