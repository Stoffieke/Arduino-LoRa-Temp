#include <SPI.h>
#include <LoRa.h>
#include <SimpleDHT.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT,&Wire);

//define the pins used by the transceiver module
#define ss 5
#define rst 2
#define dio0 4

//define the pins used by the DHT11 sensor
#define DHTPIN 27


int counter = 0;
int pinDHT11 = 27;
//initialize DHT11 sensor
SimpleDHT11 dht11(pinDHT11);

void setup() {
  //initialize Serial Monitor
  Serial.begin(9600);
  while (!Serial);
  Serial.println("LoRa Sender");

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
  //read temperature and humidity data from DHT11 sensor
 

 

  Serial.print("Sending packet: ");
  Serial.print(counter);
  Serial.println(", Temperature: ");
  Serial.println(", Humidity: ");
  

  //Send LoRa packet to receiver
  LoRa.beginPacket();
  //LoRa.print("Temperature: ");
   LoRa.beginPacket();
 byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.print(SimpleDHTErrCode(err));
    Serial.print(","); Serial.println(SimpleDHTErrDuration(err)); delay(1000);
    return;
  }
  
  Serial.print("Sample OK: ");
  LoRa.print((int)temperature); LoRa.print(" *C, "); 
  LoRa.print((int)humidity);
  LoRa.endPacket();
//  LoRa.print(" Humidity: ");
//  LoRa.print(h);
  LoRa.endPacket();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Sending packet: ");
  display.print(counter++);
  display.setCursor(0 , 16);
  display.print("RSSI: ");
  display.println(LoRa.packetRssi());
  display.setCursor(0,32);
  display.print("LoRa Sender");
  display.display();
  delay(10000);
}
 