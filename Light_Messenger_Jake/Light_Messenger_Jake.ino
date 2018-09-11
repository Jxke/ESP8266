// Adafruit IO Publish Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

#include "secrets.h"

/************************ Example Starts Here *******************************/

// set up the 'words' feed
AdafruitIO_Feed *words = io.feed("light-messenger.messages", "susu1112");

String readString;
String a_word;
String user = "Jake";

#define RED_PIN   4
#define GREEN_PIN 5
#define BLUE_PIN  2

void setup() {
  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

    #if defined(ARDUINO_ARCH_ESP32) // ESP32 pinMode
    // assign rgb pins to channels
    ledcAttachPin(RED_PIN, 1);
    ledcAttachPin(GREEN_PIN, 2);
    ledcAttachPin(BLUE_PIN, 3);
    // init. channels
    ledcSetup(1, 12000, 8);
    ledcSetup(2, 12000, 8);
    ledcSetup(3, 12000, 8);
  #else
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
  #endif

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  words->onMessage(handleMessage);

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

    // set analogWrite range for ESP8266
  #ifdef ESP8266
    analogWriteRange(255);
  #endif

      analogWrite(RED_PIN, 255);
      analogWrite(GREEN_PIN, 255);
      analogWrite(BLUE_PIN, 255);
  
}

void loop() {
  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

   while (Serial.available()) 
 {
   char c = Serial.read();
   readString += c;
   delay(2); 
 }
 if (readString.length() >1) 
 {
   Serial.println(readString); 
   delay(500);
   a_word = readString;
   sendWord();
    if (readString == "box"){
      analogWrite(RED_PIN, 0);
      analogWrite(GREEN_PIN, 255);
      analogWrite(BLUE_PIN, 255);
    }
    if (readString == "white"){
      analogWrite(RED_PIN, 0);
      analogWrite(GREEN_PIN, 0);
      analogWrite(BLUE_PIN, 0);
    }
    if (readString == "reset"){
      analogWrite(RED_PIN, 255);
      analogWrite(GREEN_PIN, 255);
      analogWrite(BLUE_PIN, 255);
    }
   readString = "";
 } 



  // Adafruit IO is rate limited for publishing, so a delay is required in
  // between feed->save events. In this example, we will wait three seconds
  // (1000 milliseconds == 1 second) during each loop.
  delay(3000);
}

void sendWord() {
//  Serial to confirm send
  Serial.print("Sending ->: ");
  Serial.println(a_word);

  words->save(user + ": " + a_word);
}

void handleMessage(AdafruitIO_Data *data) {

  Serial.print("Received <-: ");
  Serial.println(data->value());

}
