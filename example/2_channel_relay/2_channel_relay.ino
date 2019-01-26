#include <ESP8266WiFi.h>                   // This library helps to provide the functions of the ESP8266 
const char* ssid = "Dicosta";                 // Declare your Wi-Fi name here within the double quotes
const char* password = "ramo011911";             // Declare your Wi-Fi password here within the double quotes
WiFiServer server(80);                     // Create an instance of the server and specify the port to listen on as an argument

void setup() 
{
  Serial.begin(9600);                           // set buad rate 
  delay(10);
  pinMode(0, OUTPUT);                           // set GPIO 0 as OUTPUT
  pinMode(2, OUTPUT);                           // set GPIO 2 as OUTPUT
  digitalWrite(0, 0);                           // initially set GPIO 0 as Low
  digitalWrite(2, 0);                           // initially set GPIO 2 as Low
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");               // Connect to WiFi network
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected"); 
  server.begin();                              // Start the server
  Serial.println("Server started");
  Serial.println(WiFi.localIP());              // Print the IP address
}

void loop() 
{ 
  WiFiClient client = server.available();       // Check if a client has connected
  if (!client) 
  {
    return;                                     // Wait until the client sends some data
  }
  Serial.println("new client");
  while(!client.available())
  {
    delay(1);
  }
  String req = client.readStringUntil('\r');   // Read the first line of the request
  Serial.println(req);
  client.flush();
  int val1;
  int val2;
  if (req.indexOf("/gpio1/0") != -1)           // Match the request
    val1 = 1;
  else if (req.indexOf("/gpio1/1") != -1)
    val1 = 0;
  else if (req.indexOf("/gpio2/0") != -1)
    val2 = 1;
  else if (req.indexOf("/gpio2/1") != -1)
    val2 = 0;  
  else 
  {
    Serial.println("invalid request");
    client.stop();
    return;
  }
  digitalWrite(2, val1);               // Set GPIO 2 according to the request
  digitalWrite(0, val2);               // Set GPIO 0 according to the request
  client.flush();
  
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>";        // Prepare the response
  s += "<body><h1>ESP8266 Smallest Home Automation</h1>\r\nGPIO1 is now ";
  s += (val1)?"high":"low";
  s += "\n GPIO2 is now ";
  s += (val2)?"high":"low";
  s += "</html>\n";
  client.print(s);                           // Send the response to the client
  delay(1);
  Serial.println("Client disonnected");      // The client will actually be disconnected and when the function returns and 'client' object is detroyed
}
