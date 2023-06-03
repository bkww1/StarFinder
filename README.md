# StarFinder
ESP8266 and Arduino UNO project of Star Finder using stepper motors and gyroscope.

Arduino is used to control stepper motors. It gets commands from ESP8266 using I2C bus.  ESP8266 uses AsyncWebSrv to send messages via webpage, ESP8266 connects to WiFi and gets IP address from DHCP.
Webpage created using HTML and JS to run onclick functions that send messages via websocket.send()


![350853968_1182332239834337_8374165952614250802_n](https://github.com/bkww1/StarFinder/assets/42109824/bfd40f5e-d297-426a-b049-817142ed4196)
![350281876_165334759649827_64694715805772859_n](https://github.com/bkww1/StarFinder/assets/42109824/7b2d1e42-2467-47ef-943c-e2f542d5bf11)
