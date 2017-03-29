/*
  WiFi Web Server LED Blink

 A simple web server that lets you blink an LED via the web.
 This sketch will print the IP address of your WiFi (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 9.

 If the IP address of your WiFi is yourAddress:
 http://yourAddress/H turns the LED on
 http://yourAddress/L turns it off

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 Circuit:
 * CC3200 WiFi LaunchPad or CC3100 WiFi BoosterPack
   with TM4C or MSP430 LaunchPad

 */
#ifndef __CC3200R1M1RGC__
// Do not include SPI for CC3200 LaunchPad
#include <SPI.h>
#endif
#include <WiFi.h>

#define trigpin 19
#define echopin 18
//int s1=12;
//int s2=11;
int mn1=0;
int u=0;

// your network name also called SSID
char ssid[] = "iot";
// your network password
char password[] = "yadav1995";
// your network key Index number (needed only for WEP)
int keyIndex = 0;

WiFiServer server(80);

void setup() {
  Serial.begin(9600);      // initialize serial communication
  pinMode(RED_LED, OUTPUT);      // set the LED pin mode
  pinMode(GREEN_LED, OUTPUT);
  pinMode(trigpin,OUTPUT);
  pinMode(echopin,INPUT);
  pinMode(37,OUTPUT);     //pwm pin for motor
  //pinMode(11,OUTPUT);
  //pinMode(13,OUTPUT);
  //pinMode(s1,INPUT);
  //digitalWrite(s1,LOW);
  //pinMode(s2,INPUT);
  //digitalWrite(s2,LOW);
  digitalWrite(RED_LED,LOW);
  digitalWrite(GREEN_LED,LOW);

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to Network named: ");
  // print the network name (SSID);
  Serial.println(ssid); 
  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED) {
    // print dots while we wait to connect
    Serial.print(".");
    delay(300);
  }
  
  Serial.println("\nYou're connected to the network");
  Serial.println("Waiting for an ip address");
  
  while (WiFi.localIP() == INADDR_NONE) {
    // print dots while we wait for an ip addresss
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nIP Address obtained");
  
  // you're connected now, so print out the status  
  printWifiStatus();

  Serial.println("Starting webserver on port 80");
  server.begin();                           // start the web server on port 80
  Serial.println("Webserver started!");
}

void loop() {
  int i = 0;
  
  int x;  // for loop
 int y;   // comdition for led to check
 //int sw2;
 //int sw1;
 //sw1=digitalRead(s1);
 //sw2=digitalRead(s2);
 
 
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    char buffer[150] = {0};                 // make a buffer to hold incoming data
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (strlen(buffer) == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.println("<html><head><title>Energia CC3200 WiFi Web Server</title></head><body align=center>");
            client.println("<h1 align=center><font color=\"red\">Welcome to the CC3200 WiFi Web Server</font></h1>");
            client.print("RED LED <button onclick=\"location.href='/H'\">START</button>");
            client.println(" <button onclick=\"location.href='/L'\">STOP</button><br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear the buffer:
            memset(buffer, 0, 150);
            i = 0;
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          buffer[i++] = c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (endsWith(buffer, "GET /H")) {
          digitalWrite(GREEN_LED, HIGH);               // GET /H turns the LED on
          //digitalWrite(s1, HIGH);          // train on
        }
        
        if (endsWith(buffer, "GET /L")) {
          digitalWrite(GREEN_LED, LOW);           // GET /L turns the LED off
          //digitalWrite(s2, HIGH);         
        }
      }     // condition  if (client.available())
    }    //  condition while (client.connected())
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  } //    condition- if clint condition
  
  
  
  
  int duration,distance;
        digitalWrite(trigpin,HIGH);
        delayMicroseconds(1000);
        digitalWrite(trigpin,LOW);
        duration=pulseIn(echopin,HIGH);
        distance=(duration/2)/29.1;
        y=digitalRead(GREEN_LED);
        
        if(distance>=150 && distance>=0){
        if(y==HIGH){
         Serial.println("out of range");
        if(mn1==0){
         for(x=0;x<=255;x++){
          analogWrite(37,x); // check code here
           //analogWrite(11,x);
             Serial.println("Train speed is increasing");
         //delay(50);    
           }
         }
        Serial.println("Train is running at full speed");
        digitalWrite(37,HIGH);
       mn1=2;
       u=0;
       }
      }
      
      else if(distance>=0 && distance<=50){
               Serial.println(distance);
               Serial.println(" cm");
              if(u==0){
                Serial.println("obstacle is present");
                  for(x=255;x>0;x--){
                  analogWrite(37,x);
                  //analogWrite(11,x);
                    Serial.println("Train speed is decreasing");
                  delay(10);
                   }

                 u=2;
                  mn1=0;
                 }
               Serial.println("Train is stopped");
              digitalWrite(37,LOW);

         }
         
         if(y==LOW){
            Serial.println("Train reaches the destination");
         // digitalWrite(GREEN_LED, LOW);          // GET /L turns the LED off
          for(x=255;x<0;x--){
          analogWrite(37,x);
          //analogWrite(11,x);
          }
           digitalWrite(37,LOW);
           mn1=0;
           u=0; 
         }
  
  
}  // void loop

//
//a way to check if one array ends with another array
//
boolean endsWith(char* inString, char* compString) {
  int compLength = strlen(compString);
  int strLength = strlen(inString);
  
  //compare the last "compLength" values of the inString
  int i;
  for (i = 0; i < compLength; i++) {
    char a = inString[(strLength - 1) - i];
    char b = compString[(compLength - 1) - i];
    if (a != b) {
      return false;
    }
  }
  return true;
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}
