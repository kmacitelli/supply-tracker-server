// #include "BluetoothSerial.h"
// #include <Arduino.h>


// #if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
// #error Bluetooth is not enabled! Please run `make menuconfig` to and enable it.
// #endif

// BluetoothSerial SerialBT; // Create a BluetoothSerial object

// int analogPin1 = A3; 
// int analogPin2 = A3; 
// int analogPin3 = A3; 
// int analogPin1 = A3; 


// void setup() {
//   // put your setup code here, to run once:

//   Serial.begin(115200); // Start serial communication for debugging
//   SerialBT.begin("ESP32_BT_TX"); // Initialize Bluetooth with a device name
//   Serial.println("Bluetooth device started. You can now pair with it.");

// }

// void loop() {
//   // put your main code here, to run repeatedly:
//   if (Serial.available()) {
//     // Send the data from the serial monitor over Bluetooth
//     SerialBT.write(Serial.read());
//   }
//   // Check if there is data from the Bluetooth device
//   if (SerialBT.available()) {
//     // Send the data from Bluetooth to the serial monitor
//     Serial.write(SerialBT.read());
//   }
//   delay(20); // Add a small delay to prevent overwhelming the system

// }




/*
    Go to thingspeak.com and create an account if you don't have one already.
    After logging in, click on the "New Channel" button to create a new channel for your data. This is where your data will be stored and displayed.
    Fill in the Name, Description, and other fields for your channel as desired, then click the "Save Channel" button.
    Take note of the "Write API Key" located in the "API keys" tab, this is the key you will use to send data to your channel.
    Replace the channelID from tab "Channel Settings" and privateKey with "Read API Keys" from "API Keys" tab.
    Replace the host variable with the thingspeak server hostname "api.thingspeak.com"
    Upload the sketch to your ESP32 board and make sure that the board is connected to the internet. The ESP32 should now send data to your Thingspeak channel at the intervals specified by the loop function.
    Go to the channel view page on thingspeak and check the "Field1" for the new incoming data.
    You can use the data visualization and analysis tools provided by Thingspeak to display and process your data in various ways.
    Please note, that Thingspeak accepts only integer values.

    You can later check the values at https://thingspeak.com/channels/2005329
    Please note that this public channel can be accessed by anyone and it is possible that more people will write their values.
 */

#include <WiFi.h>
#include <WiFiServer.h>
#include <WiFiClient.h>

const char *ssid = "Test";          // Change this to your WiFi SSID
const char *password = "12345678";  // Change this to your WiFi password

const char *host = "api.thingspeak.com";        // This should not be changed
const int httpPort = 80;                        // This should not be changed
const String channelID = "2005329";             // Change this to your channel ID
const String writeApiKey = "V6YOTILH9I7D51F9";  // Change this to your Write API key
const String readApiKey = "34W6LGLIFXD56MPM";   // Change this to your Read API key

// The default example accepts one data filed named "field1"
// For your own server you can ofcourse create more of them.
int field1 = 0;

int numberOfResults = 3;  // Number of results to be read
int fieldNumber = 1;      // Field number which will be read out

WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println("******************************************************");
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop(){
  WiFiClient client = server.available();

  //make sure to clear the webpage after each loop
  if (client) {
    Serial.println("Client is connected");
    String currentLine = ""; 
    while(client.connected()) {
      if (client.available()) {     // if there's bytes to read from the client,
      char c = client.read();     // read a byte, then
        Serial.write(c);            // print it out the serial monitor
        if (c == '\n') {            // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            float fakeWeight = 10.0;

            // the content of the HTTP response follows the header:
            client.print(fakeWeight);
            

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {  // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

      }
      //Serial.println("We are in guys");

      //delay(1000);
    }
    client.stop();
    Serial.println("Client disconnected");
  }

}