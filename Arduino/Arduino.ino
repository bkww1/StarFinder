#include<Wire.h>
#include <Stepper.h>
#include <math.h>
const int stepsPerRevolution = 2038;
String direction;
String angle;
// Creates an instance of stepper class
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
Stepper stepperX = Stepper(stepsPerRevolution, 2, 4, 3, 5);
Stepper stepperY = Stepper(stepsPerRevolution, 8, 10, 9, 11);
String receivedValue;
int out_value = 0;
int numberofMoves = 0;
  void receiveEvent(int byteCount)
  {
    receivedValue = "";
  while (Wire.available())
  {
    char c = Wire.read();
    receivedValue += c;
    
    
  }
  int ampersand = receivedValue.indexOf('&');
  direction = receivedValue.substring(0, ampersand);
  angle = receivedValue.substring(ampersand + 1);
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
   if (receivedValue == "LEFT"){
     stepperX.step(-3);
   }
   else if (receivedValue == "RIGHT"){
     stepperX.step(3);
   }

   else if (receivedValue == "UP"){
     stepperY.step(-3);
   }

   else if (receivedValue == "DOWN"){
     stepperY.step(3);
     }
    else if (direction =="H"){
      int calculate_angle = ceil(2048*angle.toInt()/360);
      Serial.println(calculate_angle);
      stepperX.step(calculate_angle);
    }
    else if (direction == "V"){
      int calculate_angle = ceil(2048*angle.toInt()/360);
      Serial.println(calculate_angle);
      stepperY.step(calculate_angle);
       
    }
      numberofMoves = 0;
    
  }
}

