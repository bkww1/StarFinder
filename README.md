# StarFinder
ESP8266 and Arduino UNO project of Star Finder using stepper motors and gyroscope.

Arduino is used to control stepper motors. It gets commands from ESP8266 using I2C.  ESP8266 uses AsyncWebSrv to send messages via webpage, ESP8266 connects to WiFi and gets IP addres from DHCP.
Webpage created using HTML and JS to run onclick functions that send messages via websocket.send()
