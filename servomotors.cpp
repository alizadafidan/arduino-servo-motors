#include <SPI.h>
#include <Ethernet.h>
#include <DHT.h>
#include "Servo.h"
#include <microDS18B20.h>

// "..." means private info

Servo servo;
Servo servoSecond;
Servo servoThird;
DHT dht(2, DHT11); 

boolean newInfo = false; 

#define DHTPIN 2       
#define DHTTYPE DHT22  

#define DS_PIN 3
uint8_t s1_addr[] = {"..."};
uint8_t s2_addr[] = { "..." };

int pos;  

//  ~ MAC address of your Ethernet module
byte mac[] = "....";
// ~ Your IP address
IPAddress "....";
EthernetServer server(80); 
int lightSensor = 0;

void setup() {
  pinMode(8, OUTPUT); 
  pinMode(7, OUTPUT);
  Ethernet.begin(mac, ip); 
  server.begin();
  Serial.begin(9600);
  dht.begin();  
  servo.attach(9);
  servoSecond.attach(5);
  servoThird.attach(6);
}

void loop() {
  EthernetClient client = server.available();  
  lightSensor = analogRead(A1);
  Serial.println(lightSensor);
  delay(500);

  if (client) {
    if (lightSensor < 600) {
      digitalWrite(7, HIGH);
    } else {
      digitalWrite(7, LOW);
    }
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (newInfo && c == ' ') {
          newInfo = false;  
        }
        if (c == '$') {
          newInfo = true;  
        }
        if (newInfo == true) { 
          Serial.println(c);
          if (c == '1') {
            Serial.println("ON");
            digitalWrite(9, HIGH);
            servo.write(0);
          }
          if (c == '2') {
            Serial.println("OFF");
            digitalWrite(9, LOW);
            servo.write(90);
          }
          if (c == '3') {
            Serial.println("ON");
            digitalWrite(5, HIGH);
            servoSecond.write(0);
          }
          if (c == '4') {
            Serial.println("OFF");
            digitalWrite(5, LOW);
            servoSecond.write(90);
          }
          if (c == '5') {
            Serial.println("ON");
            digitalWrite(6, HIGH);
            servoThird.write(0);
          }
          if (c == '6') {
            Serial.println("OFF");
            digitalWrite(6, LOW);
            servoThird.write(90);
          }
          if (c == '7') {
            Serial.println("ON");
            digitalWrite(8, HIGH);
          }
          if (c == '8') {
            Serial.println("OFF");
            digitalWrite(8, LOW);
          }
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
        if (c == '\n' && currentLineIsBlank) {
          float temperature = dht.readTemperature();
          float humidity = dht.readHumidity();
          MicroDS18B20<DS_PIN, s1_addr> sensor1;
          MicroDS18B20<DS_PIN, s2_addr> sensor2;
          sensor1.requestTemp();
          sensor2.requestTemp();
          delay(2000);
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println("Refresh: 180");  
          client.println();
          client.println(F("<!DOCTYPE HTML>"));
          client.println(F("<html>"));
          client.println(F("<head>"));
          client.println(F("<style>"));
          client.println(F("body { font-family: Arial, sans-serif; background-color: #a3b18a; }"));
          client.println(F("h1 { color: #132a13; text-align:center; font-size:80px }"));
          client.println(F(".button { background-color: #132a13; border: none; color: #31572c; padding: 15px 32px;"));
          client.println(F("text-align: center; text-decoration: none; display: inline-block; font-size: 16px;"));
          client.println(F("margin: 4px 2px; cursor: pointer; }"));
          client.println(F(".box { background-color: #fff; border: 1px dashed #132a13; color: #31572c; padding: 15px 32px;width:300px}"));
          client.println(F("#temperature1 { color: #132a13; font-size:22px;text-decoration: underline; text-align:center}"));
          client.println(F("#temperatureNumber { color: #00b4d8; font-size:55px; text-align: center; }"));
          client.println(F(".boxContainer { display:flex; gap:30px;margin-bottom:50px}"));
          client.println(F(".buttonsContainer { display:flex; gap:30px;margin-bottom:50px;align-items:center; justify-content:center;}"));
          client.println(F(".buttonContainer { display:flex; flex-direction:column; align-items:center; justify-content:center; gap:30px; background-color:#fff; width:300px; height:300px; border:1px dashed #000}"));
          client.println(F("#buttonHeading { color: #000; font-size:30px; text-align: center;}"));
          client.println(F(".onOffButtonContainer { display:flex; align-items:center; justify-content:center; gap:30px;padding: 15px 32px;}"));
          client.println(F(".onButton { background-color: #48cae4; border: none; color: #31572c; padding: 15px 32px;color:#fff }"));
          client.println(F("input {margin-top:20px; background-color: #a3b18a; border: none; padding: 15px 32px;color:#fff }"));
          client.println(F(".offButton { background-color: #588157; border: none; color: #31572c; padding: 15px 32px; color:#fff}"));
          client.println(F("</style>"));
          client.println(F("<script>"));
          client.println(F("function updateSensorData() {"));
          client.println(F("var xhttp = new XMLHttpRequest();"));
          client.println(F("xhttp.onreadystatechange = function() {"));
          client.println(F("if (this.readyState == 4 && this.status == 200) {"));
          client.println(F("document.getElementById('wholepage').innerHTML = this.responseText;"));
          client.println(F("console.log(this.responseText)"));
          client.println(F("}};"));
          client.println(F("xhttp.open('GET', '/temperature', true);"));
          client.println(F("xhttp.send();"));
          client.println(F("}"));
          client.println(F("setInterval(updateSensorData, 1000);"));  // Update sensor data every 5 seconds
          client.println(F("</script>"));
          client.println(F("</head>"));
          client.println(F("<body>"));
          client.println(F("<div id='wholepage'>"));
          client.println(F("<h1>Greenhouse</h1>"));
          client.println(F("<div class=\"boxContainer\">"));
          client.println(F("<div class=\"box\">"));
          client.println(F("<p id='temperature1'>Temperature "));                               
          client.println("<p id='temperatureNumber'> " + String(temperature) + " &#8451;</p>"); 
          client.println(F("</div>"));
          client.println(F("<div class=\"box\">"));
          client.println(F("<p id='temperature1'>Humidity "));                        
          client.println("<p id='temperatureNumber'> " + String(humidity) + "%</p>"); 
          client.println(F("</div>"));
          client.println(F("<div class=\"box\">"));
          client.println(F("<p id='temperature1'>Light Sensor "));                      
          client.println("<p id='temperatureNumber'> " + String(lightSensor) + "</p>"); 
          client.println(F("</div>"));
          client.println(F("<div class=\"box\">"));
          client.println(F("<p id='temperature1'>Temperature from Sensor 1:"));                       
          client.println("<p id='temperatureNumber'> " + String(sensor1.getTemp()) + " &#8451;</p>"); 
          client.println(F("</div>"));
          client.println(F("<div class=\"box\">"));
          client.println(F("<p id='temperature1'>Temperature from Sensor 2:"));                       
          client.println("<p id='temperatureNumber'> " + String(sensor2.getTemp()) + " &#8451;</p>"); 
          client.println(F("</div>"));
          client.println(F("</div>"));
          client.println(F("<div class=\"buttonsContainer\">"));
          client.println(F("<div class=\"buttonContainer\">"));
          client.println(F("<p id='temperature1'>SERVO MOTOR1"));
          client.println(F("<div class=\"onOffButtonContainer\">"));
          client.println(F("<a href=\"/$1\"><button class=\"onButton\">Turn on</button></a>"));
          client.println(F("<a href=\"/$2\"><button class=\"offButton\">Turn off</button></a>")); 
          client.println(F("</div>"));
          client.println(F("</div>"));
          client.println(F("<div class=\"buttonContainer\">"));
          client.println(F("<p id='temperature1'>SERVO MOTOR 2"));
          client.println(F("<div class=\"onOffButtonContainer\">"));
          client.println(F("<a href=\"/$3\"><button class=\"onButton\">Turn on</button></a>"));
          client.println(F("<a href=\"/$4\"><button class=\"offButton\">Turn off</button></a>")); 
          client.println(F("</div>"));
          client.println(F("</div>"));
          client.println(F("<div class=\"buttonContainer\">"));
          client.println(F("<p id='temperature1'>SERVO MOTOR 3"));
          client.println(F("<div class=\"onOffButtonContainer\">"));
          client.println(F("<a href=\"/$5\"><button class=\"onButton\">Turn on</button></a>"));
          client.println(F("<a href=\"/$6\"><button class=\"offButton\">Turn off</button></a>")); 
          client.println(F("</div>"));
          client.println(F("</div>"));
          client.println(F("<div class=\"buttonContainer\">"));
          client.println(F("<p id='temperature1'>LED"));
          client.println(F("<div class=\"onOffButtonContainer\">"));
          client.println(F("<a href=\"/$7\"><button class=\"onButton\">Turn on</button></a>"));
          client.println(F("<a href=\"/$8\"><button class=\"offButton\">Turn off</button></a>")); 
          client.println(F("</div>"));
          client.println(F("</div>"));
          client.println(F("</div>"));
          client.println(F("</div>"));
          client.println(F("</body>"));
          client.println(F("</html>"));

          break;
        }
      }
    }
    delay(1);
    client.stop();
  }
}