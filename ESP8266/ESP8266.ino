#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebSrv.h>
#include "LittleFS.h"
#include<Wire.h>
char c;
String direction;
String angle;
String chosen_motor;
String sendCommand;
String sendInfo;
bool processed = true;
String message = "";
String objects[3] = {"MOON", "SUN", "START"};
// Replace with your network credentials
const char* ssid = "CGA2121";
const char* password = "Joanna123";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create a WebSocket object
AsyncWebSocket ws("/ws");
bool notifyStop = false;

// Initialize LittleFS
void initFS() {
  if (!LittleFS.begin()) {
    Serial.println("An error has occurred while mounting LittleFS");
  }
  else{
    Serial.println("LittleFS mounted successfully");
  }
}

// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void notifyClients(String state) {
  ws.textAll(state);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    bool isFound = false;
    data[len] = 0;
    message = (char*)data;
    Serial.println(message);
    int firstAmpersand = message.indexOf('&');
    int secondAmpersand = message.lastIndexOf('&');
  // Extract the substrings
    direction = message.substring(0, firstAmpersand);
    angle = message.substring(firstAmpersand + 1, secondAmpersand);
    chosen_motor = message.substring(secondAmpersand + 1);

    
    for (int i = 0; i < 3; i++) {
    if (objects[i] == message) {
      isFound = true;
      break;
      }
    }
    if (isFound){
      Serial.println(message);
    }
    else if (message == "SAVE"){
      Serial.println(message);
    }
    else{
      Serial.println("Direction");
      Serial.println(direction);
      Serial.println("angle");
      Serial.println(angle);
      Serial.println("Motor");
      Serial.println(chosen_motor);
      notifyClients(direction);
      notifyStop = true;
      if (chosen_motor == "0"){
        sendCommand = "GO_ANGLE";
        sendInfo = direction + "&" + angle;
        processed = false;
      }
      else if (chosen_motor == "1"){
        sendCommand = "GO_LEFT";
        processed = false; 
      }
      else if (chosen_motor == "2") {
        sendCommand = "GO_RIGHT";
        processed = false;
      }
      else if (chosen_motor == "3") {
        sendCommand = "GO_UP";
        processed = false;
      }
      else if (chosen_motor == "4" ) {
        sendCommand = "GO_DOWN";
        processed = false;
      }
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      //Notify client of motor current state when it first connects
      notifyClients(direction);
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
        handleWebSocketMessage(arg, data, len);
        break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
     break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}


void setup() {
  Serial.begin(9600);
  Wire.begin(D1,D2);

  initWiFi();
  initWebSocket();
  initFS();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", "text/html");
  });
  server.serveStatic("/", LittleFS, "/");
  server.begin();
}

void loop() {
  if (!processed){
    Serial.print(sendCommand);
    if (sendCommand == "GO_LEFT"){
      Wire.beginTransmission(8);
      Wire.write("LEFT");
      Wire.endTransmission();
   } 
    else if (sendCommand == "GO_RIGHT"){
      Wire.beginTransmission(8);
      Wire.write("RIGHT");
      Wire.endTransmission();
   } 
    else if (sendCommand == "GO_UP"){
      Wire.beginTransmission(8);
      Wire.write("UP");
      Wire.endTransmission();
    } 
    else if (sendCommand == "GO_DOWN"){
      Wire.beginTransmission(8);
      Wire.write("DOWN");
      Wire.endTransmission();
    } 
    else if (sendCommand = "GO_ANGLE"){
      Serial.println(sendInfo);
      Wire.beginTransmission(8);
      Wire.write(sendInfo.c_str());
      Wire.endTransmission();
    }
    processed = true;
  }

  Serial.println();
  delay(1000);
}
