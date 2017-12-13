#include "FastLED.h"

FASTLED_USING_NAMESPACE

// Starting from FastLED DemoReal
// Updated to Christmas Design with mostly Red / Green / White
// Created Dec 2017 by Curtis Gustin
// Find more at cgustin.com

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    12
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER BRG
#define NUM_LEDS    100
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          255
#define FRAMES_PER_SECOND  60 //Default is 120
#define ROTATION_TIME_IN_SECOND 30

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

void setup() {
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  currentBlending = LINEARBLEND;
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])( uint8_t);
SimplePatternList gPatterns = { RedEater, redFading, redWithGlitter, randomFades, solidRedGreen, SolidRedWhite, randomFades,
              RedAndGreenBars, RedAndWhiteBars, GreenAndWhiteBars, randomFades, GreenEater, greenWithGlitter,
              randomFades}; //blendwave, warmWhiteShimmer, confetti, sinelon, juggle};

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
boolean moveOn = false;
  
void loop()
{
  //Check for manual next pattern
  if( moveOn) { nextPattern(); }
  // Create Loop Index for some functions
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
    
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]( startIndex);
  

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( ROTATION_TIME_IN_SECOND ) { nextPattern(); } // change patterns periodically
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
  moveOn=false;
}

void redWithGlitter( uint8_t colorIndex) 
{
  fill_solid( leds, NUM_LEDS, CRGB(255,0,0));
  addGlitter(80,5,CRGB::White,0);
}

void redFading( uint8_t colorIndex)
{
  fill_solid( leds, NUM_LEDS, CRGB(255,0,0));
  uint8_t redBrightness = random8(50,200);
  FastLED.setBrightness(redBrightness);
  delay(300);
}


void greenWithGlitter( uint8_t colorIndex) 
{
  fill_solid( leds, NUM_LEDS, CRGB(0,255,0));
  addGlitter(80,5,CRGB::White,50);
}

void addGlitter( fract8 chanceOfGlitter, int glitterCount, CRGB c1,int speeds) 
{
  if( random8() < chanceOfGlitter) {
    for(int i=0; i<glitterCount; i++) {
      leds[ random16(NUM_LEDS) ] += c1; //CRGB::White;
    }
    delay(speeds);
  }
}

void FillLEDsFromPaletteColors( uint8_t colorIndex, uint8_t maxBright, uint8_t minBright)
{
    for( int j = maxBright; j >= minBright; j=j-5) {
      for( int i = 0; i < NUM_LEDS; i++) {
          leds[i] = ColorFromPalette( currentPalette, colorIndex, j, currentBlending);
          colorIndex += 3;
      }
    }
}

void RedAndGreenBars( uint8_t colorIndex)
{
  fadeToBlackBy( leds, NUM_LEDS, 10);
  SetupRedAndGreenPalette();              currentBlending = LINEARBLEND;
  FillLEDsFromPaletteColors( colorIndex,255,255);
}

void SetupRedAndGreenPalette()
{
    CRGB red = CHSV( HUE_RED, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   red, red, black,  black,
                                   green,  green,  black,  black,
                                   red, red, black,  black );
}

void RedAndWhiteBars( uint8_t colorIndex)
{
  fadeToBlackBy( leds, NUM_LEDS, 10);
  SetupRedAndWhitePalette();              currentBlending = LINEARBLEND;
  FillLEDsFromPaletteColors( colorIndex,255,255);
}

void SetupRedAndWhitePalette()
{
    CRGB red = CHSV( HUE_RED, 255, 255);
    CRGB white  = CRGB::White;
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   white,  white,  black,  black,
                                   red, red, black,  black,
                                   white,  white,  black,  black,
                                   red, red, black,  black );
}

void GreenAndWhiteBars( uint8_t colorIndex)
{
  fadeToBlackBy( leds, NUM_LEDS, 10);
  SetupGreenAndWhitePalette();              currentBlending = LINEARBLEND;
  FillLEDsFromPaletteColors( colorIndex,255,255);
}

void SetupGreenAndWhitePalette()
{
    CRGB green = CHSV( HUE_GREEN, 255, 255);
    CRGB white  = CRGB::White;
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   white,  white,  black,  black,
                                   green, green, black,  black,
                                   white,  white,  black,  black,
                                   green, green, black,  black );
}

void GreenEater( uint8_t colorIndex)
{
  //fadeToBlackBy( leds, NUM_LEDS, 10);
  colorRun( CRGB::Green, CRGB::Red, 1, 50, true);
  moveOn = true;
}

void RedEater( uint8_t colorIndex)
{
  //fadeToBlackBy( leds, NUM_LEDS, 10);
  colorRun( CRGB::Red, CRGB::Green, 1, 50, false);
  moveOn = true;
}

void colorRun(CRGB c, CRGB d, int width, int speed, boolean bidirectional){
  // Slide the leds in one direction or 2 base on boolean value bidirectional
  for(int i = 0; i <= NUM_LEDS-width; i++) {
    for(int j=0; j<width; j++){
      leds[i+j-1] = c;
      leds[i+j] = c;
      if( bidirectional) {
        leds[NUM_LEDS-i-j-1] = c;
        leds[NUM_LEDS-i-j] = c;
      }
    }

    delay(speed);
    FastLED.show();
  }
}

// Display alternating stripes
void stripes(CRGB c1, CRGB c2, int width){
  for(int i=0; i<NUM_LEDS; i++){
    if(i % (width * 2) < width){
      leds[i] = c1;
    }
    else{
      leds[i] = c2;
    } 
  }
  FastLED.show();
}

void solidRedGreen( uint8_t colorIndex)
{
  fadeToBlackBy( leds, NUM_LEDS, 10);
  //AlternateTwoColor(CRGB::Red,CRGB::Green); 
  AlternateTwoColorExpandable(CRGB::Red,CRGB::Green,255,0); 
}

void SolidRedWhite( uint8_t colorIndex)
{
  fadeToBlackBy( leds, NUM_LEDS, 10);
  AlternateTwoColorExpandable(CRGB::White,CRGB::Red,255,255); 
}

void AlternateTwoColor(CRGB c1, CRGB c2){
  for(int i=0; i<NUM_LEDS; i++){
    if(i % 2 == 0 ){
      leds[i] = c1;
    } else{
      leds[i] = c2;
    }
  }
  FastLED.show();
}

void AlternateTwoColorExpandable(CRGB c1, CRGB c2, uint8_t maxBright, uint8_t minBright){
  for( int j = maxBright; j >= minBright; j=j-1) {
    FastLED.setBrightness(j);
    for(int i=0; i<NUM_LEDS; i++){
      leds[i] = c1;
      i++;
      if(i<NUM_LEDS) { leds[i] = c1; }
      i++;
      if(i<NUM_LEDS) { leds[i] = c1; }
      i++;
      if(i<NUM_LEDS) { leds[i] = c2; }
      i++;
      if(i<NUM_LEDS) { leds[i] = c2; }
      i++;
      if(i<NUM_LEDS) { leds[i] = c2; }
    }
    FastLED.show();
    delay(50);
  }
}

void confetti(boolean randomColors, CRGB setColor, int speeds) 
{
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  if( randomColors) {
    // random colored speckles that blink in and fade smoothly
    leds[pos] += CHSV( gHue + random8(64), 200, 255); 
  } else {
    // random white speckles that blink in and fade smoothly
    uint8_t redBrightness = random8(100,255);
    leds[pos] = setColor;
    FastLED.setBrightness(redBrightness);
  }
  delay(speeds);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void randomFades( uint8_t colorIndex) {
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int fadeType = random8(1,5);
  Serial.println(fadeType);
    Serial.println(fadeType);
    switch (fadeType) {
      case 1:
        confetti(false,CRGB::Red,100);
        //break;
      case 2:
        confetti(false,CRGB::Red,100); 
        //break;
      case 3:
        confetti(false,CRGB::Green,100);
        //break;
      case 4:
        confetti(false,CRGB::Green,100);
        //break;
      default:
        confetti(false,CRGB::White,100);
        //break;
    } 
}
