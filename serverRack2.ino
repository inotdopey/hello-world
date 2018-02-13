#include "FastLED.h"
#include <Ethernet.h>

  //LED SETUP
  #define DATA_PIN    8
  #define LED_TYPE    WS2811
  #define COLOR_ORDER BRG
  #define NUM_LEDS    100
  CRGB leds[NUM_LEDS];

  int defaultr = 0;
  int defaultg = 255;
  int defaultb = 0;
  int r;
  int g;
  int b;
  String readString(20);
  boolean doorOpen = false;

  //SENSOR SETUP
  #define SENSOR_PIN 2
  int state; // 0 close - 1 open switch

  //Ethernet Setup
  
  // Enter a MAC address for your controller below.
  // Newer Ethernet shields have a MAC address printed on a sticker on the shield
  byte mac[] = {0x32, 0xCE, 0xBE, 0xAE, 0xD23, 0x12};
  IPAddress ip(X,X,X,X);
  EthernetServer server(80);
  
void setup() {
	// sanity check delay - allows reprogramming if accidently blowing power w/leds
   	delay(2000);
    Serial.begin(9600);
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  colorCycle(100,100,100,50);
  FastLED.show();

  // start the Ethernet connection:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());

  // Door Setup
  pinMode(SENSOR_PIN, INPUT_PULLUP);
 
}

void loop() {
  //Check Sensor Data
  state = digitalRead(SENSOR_PIN);
  //Serial.println(state);
  int count = 0;
  
  
  if (state == HIGH){
    r = 255;
    g = 255;
    b = 255;
    colorCycle(r,g,b,255);
    doorOpen = true;
  } else{
    //clear doorOpen
      if (doorOpen) {
        doorOpen = false;
        r = defaultr;
        g = defaultg;
        b = defaultb;
      }
    //listen for incoming clients
    EthernetClient client = server.available();
    //Serial.println(client.available());
    while (client.available() > 0 && count < 5) {
          char c = client.read();
          readString += c;
          count++;
      }
      //Serial.print("Inbound String: ");
      //Serial.print(readString);
      //Serial.println();
      if (readString == "GET /") {
        readString = "";
        while (client.available() > 0 && count < 8) {
            char c = client.read();
            readString += c;
            count++;
        }
        Serial.print("Red: ");
        Serial.print(readString);
        Serial.println();
        r = readString.toInt();
        defaultr = r;
        readString = "";
        while (client.available() > 0 && count < 11) {
            char c = client.read();
            readString += c;
            count++;
        }
        Serial.print("Green: ");
        Serial.print(readString);
        Serial.println();
        g = readString.toInt();
        defaultg = g;
        readString = "";
        while (client.available() > 0 && count < 14) {
            char c = client.read();
            readString += c;
            count++;
        }
        Serial.print("Blue: ");
        Serial.print(readString);
        Serial.println();
        b = readString.toInt();
        defaultb = b;
      }
      readString = "";
      client.stop();
      colorCycle(r,g,b,50);
    }
}

void colorCycle(int r, int g, int b, int bright) 
{
  FastLED.setBrightness(bright);
  fill_solid( leds, NUM_LEDS, CRGB(r,g,b));
  FastLED.show();
  delay(100);
}

void printIPAddress()
{
  Serial.print("My IP address: ");
  Serial.print(Ethernet.localIP());
  Serial.println();
}
