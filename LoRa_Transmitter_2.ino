
//Include the necessary libraries
#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT,&Wire);
String temp;
String hum;

//define the pins used by the transceiver module
#define ss 10
#define rst 9
#define dio0 2

int counter = 0;

void setup() {
  pinMode(26,OUTPUT);
  delay(100);
  digitalWrite(26,HIGH);
  delay(100);
  //initialize Serial Monitor
  Serial.begin(9600);
  while (!Serial);
  Serial.println("LoRa Receiver");

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000);
  display.clearDisplay();

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  
  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(866E6)) {
    Serial.println(".");
    delay(500);
  }
   // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
  LoRa.setTxPower(23,true);
}

void loop() {
   digitalWrite(26,HIGH);
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
//    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      String LoRaData = LoRa.readString();
//      Serial.print(LoRaData); 
      temp = LoRaData.substring(0,5);
      Serial.println(temp);
      hum = LoRaData.substring(6,11);
      Serial.println(hum);
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Temperature: ");
  display.print(temp);
  display.setCursor(0,16);
  display.print("Humidity: ");
  display.print(hum);
  display.println(" %");
  display.setCursor(0,32);
  display.print("RSSI: ");
  display.println(LoRa.packetRssi());
  display.setCursor(0,43);
  display.print("LoRa Transmitter");
  display.display();
}