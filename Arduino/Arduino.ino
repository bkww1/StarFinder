#include <Wire.h>
#include <Stepper.h>
#include <math.h>
#include <stdlib.h>
#include <SkyMap.h>

// Creates an instance of stepper class
const int stepsPerRevolution = 2038;
Stepper stepperX = Stepper(stepsPerRevolution, 2, 4, 3, 5);
Stepper stepperY = Stepper(stepsPerRevolution, 8, 10, 9, 11);



//SkyMap Stars Definitions and Location Definitions
ObserverPosition observation_location{ 52.391 , 16.922}; 
DateTimeValues dt{ 2023/*year*/,6 /*month*/,2/*day*/,21.60 /*time utc*/ }; //-0.35h + 3M 
double Sirius_right_ascension = 101.52, Sirius_Declination = -16.7424;
double Polaris_right_ascension = 44.875, Polaris_Declination = 89.358;
double Betelgeza_right_ascension = 89.1125, Betelgeza_Declination = 7.411;
double Vega_right_ascension = 279.4375, Vega_Declination = 38.802;

double Azimuth, Altitude;                              

const Star Sirius{ Sirius_right_ascension, Sirius_Declination };
const Star Polaris{ Polaris_right_ascension, Polaris_Declination };
const Star Betelgeza{ Betelgeza_right_ascension, Betelgeza_Declination };
const Star Vega{ Vega_right_ascension, Vega_Declination };

SearchResult search_result{};   

String direction;
String angle;
String command;
String receivedValue;
int numberofMoves = 0;
double DeltaAZ;
double currentAZposition;
double currentALTposition;
double destinationAZposition, destinationALTposition;

  void receiveEvent(int byteCount){
    receivedValue = "";
  while (Wire.available()){
    char c = Wire.read();
    receivedValue += c;
    }
  int firstAmpersand = receivedValue.indexOf('&');
  int secondAmpersand = receivedValue.lastIndexOf('&');
  direction = receivedValue.substring(0, firstAmpersand);
  angle = receivedValue.substring(firstAmpersand + 1);
  command = receivedValue.substring(secondAmpersand + 1);
  numberofMoves = 1;
  Serial.println(receivedValue);
  }

 void requestEvent(int out_value){
    Wire.write(out_value);
  }



void setup()
{  
  //I2C communication between ESP8266 and Arduino
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  Serial.begin(9600);
  //Steppers speed definition
  stepperX.setSpeed(4);	
  stepperY.setSpeed(4);
}

void loop()
{ 
  Serial.println(currentAZposition);
  Serial.println(currentALTposition);
  delay(500);

  while (numberofMoves > 0){
    if (command =="SAVE"){
    Skymap.update(observation_location, Polaris, dt);
    search_result = Skymap.get_search_result();
    currentAZposition = search_result.GetAzimuth();
    currentALTposition = search_result.GetAltitude();
  }
   else if (receivedValue == "LEFT"){
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
    else if (command == "Sirius") {
      Skymap.update(observation_location, Sirius, dt);
      search_result = Skymap.get_search_result();
      destinationAZposition = search_result.GetAzimuth();
      destinationALTposition = search_result.GetAltitude();
      double DeltaAZ = calculate_deltaAZ(currentAZposition, destinationAZposition);
      double DeltaALT = max(destinationALTposition, currentALTposition) - min(destinationALTposition, currentALTposition);
      String route = pick_route(currentAZposition, destinationAZposition);

      if ((destinationAZposition-currentAZposition) < 0 and DeltaAZ > 0) {
        int calculate_angle = ceil(2048*DeltaAZ/360);
        DeltaAZ = DeltaAZ - 360*calculate_angle/2048;
        stepperX.step(calculate_angle);
        if (DeltaALT > 0) {
          if (destinationALTposition > currentALTposition){
            int calculate_angle = ceil(2048*DeltaALT/360);
            DeltaALT = DeltaALT - 360*calculate_angle/2048;
            stepperY.step(-calculate_angle);
            currentAZposition = destinationAZposition;
            currentALTposition = destinationALTposition;
          }
        else if(destinationALTposition < currentALTposition){
          int calculate_angle = ceil(2048*DeltaALT/360);
          DeltaALT = DeltaALT - 360*calculate_angle/2048;
          stepperY.step(calculate_angle);
          currentAZposition = destinationAZposition;
          currentALTposition = destinationALTposition;
        }
      }
      }
      else if ((destinationAZposition-currentAZposition) > 0 and DeltaAZ > 0) {
        int calculate_angle = ceil(2048*DeltaAZ/360);
        DeltaAZ = DeltaAZ - 360*calculate_angle/2048;
        stepperX.step(-calculate_angle);
        if (DeltaALT > 0) {
          if (destinationALTposition > currentALTposition){
            int calculate_angle = ceil(2048*DeltaALT/360);
            DeltaALT = DeltaALT - 360*calculate_angle/2048;
            stepperY.step(-calculate_angle);
             currentAZposition = destinationAZposition;
          currentALTposition = destinationALTposition;
          }
        else if(destinationALTposition < currentALTposition){
          int calculate_angle = ceil(2048*DeltaALT/360);
          DeltaALT = DeltaALT - 360*calculate_angle/2048;
          stepperY.step(calculate_angle);
          currentAZposition = destinationAZposition;
          currentALTposition = destinationALTposition;
        }
      }
      }
    }
        else if (command == "Betelgeza") {
          Skymap.update(observation_location, Betelgeza, dt);
          search_result = Skymap.get_search_result();
          destinationAZposition = search_result.GetAzimuth();
          destinationALTposition = search_result.GetAltitude();
          double DeltaAZ = calculate_deltaAZ(currentAZposition, destinationAZposition);
          double DeltaALT = max(destinationALTposition, currentALTposition) - min(destinationALTposition, currentALTposition);
          String route = pick_route(currentAZposition, destinationAZposition);
      
      if ((destinationAZposition-currentAZposition) < 0 and DeltaAZ > 0) {
        int calculate_angle = ceil(2048*DeltaAZ/360);
        DeltaAZ = DeltaAZ - 360*calculate_angle/2048;
        stepperX.step(calculate_angle);
        if (DeltaALT > 0) {
          if (destinationALTposition > currentALTposition){
            int calculate_angle = ceil(2048*DeltaALT/360);
            DeltaALT = DeltaALT - 360*calculate_angle/2048;
            stepperY.step(-calculate_angle);
            currentAZposition = destinationAZposition;
            currentALTposition = destinationALTposition;
          }
        else if(destinationALTposition < currentALTposition){
          int calculate_angle = ceil(2048*DeltaALT/360);
          DeltaALT = DeltaALT - 360*calculate_angle/2048;
          stepperY.step(calculate_angle);
          currentAZposition = destinationAZposition;
          currentALTposition = destinationALTposition;
        }
      }
      }
      else if ((destinationAZposition-currentAZposition) > 0 and DeltaAZ > 0) {
        int calculate_angle = ceil(2048*DeltaAZ/360);
        DeltaAZ = DeltaAZ - 360*calculate_angle/2048;
        stepperX.step(-calculate_angle);
        if (DeltaALT > 0) {
          if (destinationALTposition > currentALTposition){
            int calculate_angle = ceil(2048*DeltaALT/360);
            DeltaALT = DeltaALT - 360*calculate_angle/2048;
            stepperY.step(-calculate_angle);
            currentAZposition = destinationAZposition;
            currentALTposition = destinationALTposition;
          }
        else if(destinationALTposition < currentALTposition){
          int calculate_angle = ceil(2048*DeltaALT/360);
          DeltaALT = DeltaALT - 360*calculate_angle/2048;
          stepperY.step(calculate_angle);
          currentAZposition = destinationAZposition;
          currentALTposition = destinationALTposition;
        }
      }
      }
    }
        else if (command == "Vega") {
      Skymap.update(observation_location, Vega, dt);
      search_result = Skymap.get_search_result();
      destinationAZposition = search_result.GetAzimuth();
      destinationALTposition = search_result.GetAltitude();
      double DeltaAZ = calculate_deltaAZ(currentAZposition, destinationAZposition);
      double DeltaALT = max(destinationALTposition, currentALTposition) - min(destinationALTposition, currentALTposition);
      String route = pick_route(currentAZposition, destinationAZposition);
      
      if ((destinationAZposition-currentAZposition) < 0 and DeltaAZ > 0) {
        int calculate_angle = ceil(2048*DeltaAZ/360);
        DeltaAZ = DeltaAZ - 360*calculate_angle/2048;
        stepperX.step(calculate_angle);
        if (DeltaALT > 0) {
          if (destinationALTposition > currentALTposition){
            int calculate_angle = ceil(2048*DeltaALT/360);
            DeltaALT = DeltaALT - 360*calculate_angle/2048;
            stepperY.step(-calculate_angle);
            currentAZposition = destinationAZposition;
            currentALTposition = destinationALTposition;
          }
        else if(destinationALTposition < currentALTposition){
          int calculate_angle = ceil(2048*DeltaALT/360);
          DeltaALT = DeltaALT - 360*calculate_angle/2048;
          stepperY.step(calculate_angle);
          currentAZposition = destinationAZposition;
          currentALTposition = destinationALTposition;
        }
      }
      }
      else if ((destinationAZposition-currentAZposition) > 0 and DeltaAZ > 0) {
        int calculate_angle = ceil(2048*DeltaAZ/360);
        DeltaAZ = DeltaAZ - 360*calculate_angle/2048;
        stepperX.step(-calculate_angle);
        if (DeltaALT > 0) {
          if (destinationALTposition > currentALTposition){
            int calculate_angle = ceil(2048*DeltaALT/360);
            DeltaALT = DeltaALT - 360*calculate_angle/2048;
            stepperY.step(-calculate_angle);
            currentAZposition = destinationAZposition;
            currentALTposition = destinationALTposition;
          }
        else if(destinationALTposition < currentALTposition){
          int calculate_angle = ceil(2048*DeltaALT/360);
          DeltaALT = DeltaALT - 360*calculate_angle/2048;
          stepperY.step(calculate_angle);
          currentAZposition = destinationAZposition;
          currentALTposition = destinationALTposition;
        }
      }
      }
    }
        else if (command == "Polaris") {
      Skymap.update(observation_location, Polaris, dt);
      search_result = Skymap.get_search_result();
      destinationAZposition = search_result.GetAzimuth();
      destinationALTposition = search_result.GetAltitude();
      double DeltaAZ = calculate_deltaAZ(currentAZposition, destinationAZposition);
      double DeltaALT = max(destinationALTposition, currentALTposition) - min(destinationALTposition, currentALTposition);
      String route = pick_route(currentAZposition, destinationAZposition);
      
      if ((destinationAZposition-currentAZposition) < 0 and DeltaAZ > 0) {
        int calculate_angle = ceil(2048*DeltaAZ/360);
        DeltaAZ = DeltaAZ - 360*calculate_angle/2048;
        stepperX.step(calculate_angle);
        if (DeltaALT > 0) {
          if (destinationALTposition > currentALTposition){
            int calculate_angle = ceil(2048*DeltaALT/360);
            DeltaALT = DeltaALT - 360*calculate_angle/2048;
            stepperY.step(-calculate_angle);
            currentAZposition = destinationAZposition;
            currentALTposition = destinationALTposition;
          }
        else if(destinationALTposition < currentALTposition){
          int calculate_angle = ceil(2048*DeltaALT/360);
          DeltaALT = DeltaALT - 360*calculate_angle/2048;
          stepperY.step(calculate_angle);
          currentAZposition = destinationAZposition;
          currentALTposition = destinationALTposition;
        }
      }
      }
      else if ((destinationAZposition-currentAZposition) > 0 and DeltaAZ > 0) {
        int calculate_angle = ceil(2048*DeltaAZ/360);
        DeltaAZ = DeltaAZ - 360*calculate_angle/2048;
        stepperX.step(-calculate_angle);
        if (DeltaALT > 0) {
          if (destinationALTposition > currentALTposition){
            int calculate_angle = ceil(2048*DeltaALT/360);
            DeltaALT = DeltaALT - 360*calculate_angle/2048;
            stepperY.step(-calculate_angle);
            currentAZposition = destinationAZposition;
            currentALTposition = destinationALTposition;
          }
        else if(destinationALTposition < currentALTposition){
          int calculate_angle = ceil(2048*DeltaALT/360);
          DeltaALT = DeltaALT - 360*calculate_angle/2048;
          stepperY.step(calculate_angle);
          currentAZposition = destinationAZposition;
          currentALTposition = destinationALTposition;
        }
      }
      }

    }

      numberofMoves = 0;
    
  }

}

String pick_route(double currentAZposition, double destinationAZposition) {
  String route;
  double left_route = 360 - currentAZposition - destinationAZposition;
  double right_route = destinationAZposition - currentAZposition;
  
  if (left_route < right_route) {
    route = "left";
  } else {
    route = "right";
  }
  
  return route;
}

double calculate_deltaAZ(double currentAZposition, double destinationAZposition) {
    if ((destinationAZposition <= 180 and currentAZposition <= 180) or (destinationAZposition > 180 and currentAZposition >180)) {
      DeltaAZ = max(destinationAZposition, currentAZposition) - min(destinationAZposition, currentAZposition);
    }
    else if(destinationAZposition <= 180 and currentAZposition > 180) {
      DeltaAZ = destinationAZposition - currentAZposition;
      if (DeltaAZ < 0){
        DeltaAZ += 360;
      }
    }
      
    else if(destinationAZposition > 180 and currentAZposition <= 180) {
      DeltaAZ = currentAZposition -destinationAZposition;
      if (DeltaAZ < 0){
        DeltaAZ += 360;
      }
    }
    return DeltaAZ;
}

