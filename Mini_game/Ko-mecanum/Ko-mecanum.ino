#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

String command;
int speedCar = 800;

const char* ssid = "heyy hey";
const char* password = "11111111";
ESP8266WebServer server(80);

#define ENA 14
#define ENB 12
#define IN_1 15 // motor 1 va 2 nam ben phai
#define IN_2 13
#define IN_3 2 // motor 3 va 4 nam ben trai
#define IN_4 0

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);

  Serial.begin(115200);

  // Connecting WiFi
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid,password);
  

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // Starting WEB-server
  server.on("/", HTTP_handleRoot);
  server.onNotFound(HTTP_handleRoot);
  server.begin();
}

void goBack(){ 

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar);
  }

void goAhead(){ 

      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
  }

void goRight(){ 

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
  }

void goLeft(){

      digitalWrite(IN_1,HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar);
  }



void stopRobot(){  

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
 }

void loop() {
    server.handleClient();
    
      command = server.arg("State");
      if (command == "B") goAhead();
      else if (command == "F") goBack();
      else if (command == "R") goLeft();
      else if (command == "L") goRight();

      else if (command == "0") speedCar = 400;
      else if (command == "1") speedCar = 890;
      else if (command == "2") speedCar = 1040;
      else if (command == "3") speedCar = 1290;
      else if (command == "4") speedCar = 1400;
      else if (command == "5") speedCar = 1590;
      else if (command == "6") speedCar = 1890;
      else if (command == "7") speedCar = 2290;
      else if (command == "8") speedCar = 2690;
      else if (command == "9") speedCar = 3000 ;
      else if (command == "S") stopRobot();
}

void HTTP_handleRoot(void) {
  String html = "<!DOCTYPE html>";
  html += "<html>";
  html += "<head>";
  html += "<title>Tank Control</title>";
  html += "<script>";
  html += "var xhr = new XMLHttpRequest();";
  html += "function sendCommand(command) {";
  html += "xhr.open('GET', '/?State=' + command, true);";
  html += "xhr.send();";
  html += "}";
  html += "function startCommand(command) {";
  html += "sendCommand(command);";
  html += "repeatCommand = setInterval(function () {";
  html += "sendCommand(command);";
  html += "}, 100);";
  html += "}";
  html += "function stopCommand() {";
  html += "clearInterval(repeatCommand);";
  html += "sendCommand('S');";
  html += "}";
  html += "function updateSpeed() {";
  html += "var speedSlider = document.getElementById('speedSlider');";
  html += "var speedValue = speedSlider.value;";
  html += "document.getElementById('speedLabel').innerHTML = 'Speed: ' + speedValue;";
  html += "sendCommand(speedValue);";
  html += "}";
  html += "function speedUp() {";
  html += "var speedSlider = document.getElementById('speedSlider');";
  html += "speedSlider.stepUp();";
  html += "updateSpeed();";
  html += "}";
  html += "function speedDown() {";
  html += "var speedSlider = document.getElementById('speedSlider');";
  html += "speedSlider.stepDown();";
  html += "updateSpeed();";
  html += "}";
  html += "function resetSpeed() {";
  html += "var speedSlider = document.getElementById('speedSlider');";
  html += "speedSlider.value = 800;"; // Set it to your default speed
  html += "updateSpeed();";
  html += "}";
  html += "</script>";
  html += "</head>";
  html += "<body>";
  html += "<h1>Tank Control</h1>";
  html += "<p id='speedLabel'>Speed: 800</p>";
  html += "<input type='range' id='speedSlider' min='1' max='9' step='1' value='5' oninput='updateSpeed()' />";
  html += "<br />";
  html += "<button onmousedown='startCommand(\"B\")' onmouseup='stopCommand()' ontouchstart='startCommand(\"B\")' ontouchend='stopCommand()'>Forward</button>";
  html += "<button onmousedown='startCommand(\"F\")' onmouseup='stopCommand()' ontouchstart='startCommand(\"F\")' ontouchend='stopCommand()'>Backward</button>";
  html += "<button onmousedown='startCommand(\"R\")' onmouseup='stopCommand()' ontouchstart='startCommand(\"R\")' ontouchend='stopCommand()'>Left</button>";
  html += "<button onmousedown='startCommand(\"L\")' onmouseup='stopCommand()' ontouchstart='startCommand(\"L\")' ontouchend='stopCommand()'>Right</button>";
  html += "<button onclick='speedUp()'>Speed Up</button>";
  html += "<button onclick='speedDown()'>Speed Down</button>";
  html += "<button onclick='resetSpeed()'>Reset Speed</button>";
  html += "<button onmousedown='stopCommand()' onmouseup='stopCommand()' ontouchstart='stopCommand()' ontouchend='stopCommand()'>Stop</button>";
  html += "</body>";
  html += "</html>";

  server.send(200, "text/html", html);
  delay(1);
}
