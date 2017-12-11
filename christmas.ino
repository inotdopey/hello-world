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
#define NUM_LEDS    500
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
SimplePatternList gPatterns = { RedEater, redWithGlitter, solidRedGreen, SolidRedWhite, 
              RedAndGreenBars, RedAndWhiteBars, GreenAndWhiteBars, GreenEater, greenWithGlitter}; //blendwave, warmWhiteShimmer, confetti, sinelon, juggle};

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
  
void loop()
{
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
}

void redWithGlitter( uint8_t colorIndex) 
{
  fill_solid( leds, NUM_LEDS, CRGB(255,0,0));
  addGlitter(80);
}


void greenWithGlitter( uint8_t colorIndex) 
{
  fill_solid( leds, NUM_LEDS, CRGB(0,255,0));
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White; //CRGB::White;
    leds[ random16(NUM_LEDS) ] += CRGB::White; //CRGB::White;
    leds[ random16(NUM_LEDS) ] += CRGB::White; //CRGB::White;
    leds[ random16(NUM_LEDS) ] += CRGB::White; //CRGB::White;
    leds[ random16(NUM_LEDS) ] += CRGB::White; //CRGB::White;
    //delay(1000);
  }
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;

    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

void RedAndGreenBars( uint8_t colorIndex)
{
  fadeToBlackBy( leds, NUM_LEDS, 10);
  SetupRedAndGreenPalette();              currentBlending = LINEARBLEND;
  FillLEDsFromPaletteColors( colorIndex);
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
  FillLEDsFromPaletteColors( colorIndex);
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
  FillLEDsFromPaletteColors( colorIndex);
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
  colorRun( CRGB::Green, CRGB::Red, 1, 50);
}

void RedEater( uint8_t colorIndex)
{
  //fadeToBlackBy( leds, NUM_LEDS, 10);
  colorRun( CRGB::Red, CRGB::Green, 1, 50);
}

void colorRun(CRGB c, CRGB d, int width, int speed){
  // Slide the leds in one direction
  for(int i = 0; i <= NUM_LEDS-width; i++) {
    for(int j=0; j<width; j++){
      leds[i+j-1] = c;
      leds[i+j] = c;
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
  AlternateTwoColorExpandable(CRGB::Red,CRGB::Green); 
}

void SolidRedWhite( uint8_t colorIndex)
{
  fadeToBlackBy( leds, NUM_LEDS, 10);
  AlternateTwoColorExpandable(CRGB::White,CRGB::Red); 
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

void AlternateTwoColorExpandable(CRGB c1, CRGB c2){
  for(int i=0; i<NUM_LEDS-5; i++){
    leds[i] = c1;
    i++;
    leds[i] = c1;
    i++;
    leds[i] = c1;
    i++;
    leds[i] = c2;
    i++;
    leds[i] = c2;
    i++;
    leds[i] = c2;
  }
  FastLED.show();
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
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

void warmWhiteShimmer(unsigned char dimOnly)
{
  uint8_t randomBulb =random8(NUM_LEDS);
  Serial.begin(115200);
  Serial.println(randomBulb);
  for (int i = 0; i < 5; i += 2)
  {
    Serial.println(i);
    Serial.println(randomBulb);
    if (i = randomBulb) {
      uint8_t redBrightness = 100;
      uint8_t greenBrightness = 80;
      uint8_t blueBrightness = 12;
      
      // warm white: red = x, green = 0.8x, blue = 0.125x
      redBrightness = random8();
      greenBrightness = redBrightness*4/5;
      blueBrightness = redBrightness*8/10;
      leds[i] += CHSV( redBrightness, greenBrightness, blueBrightness);
      
      // every odd LED gets set to a quarter the brighness of the preceding even LED
      if (i + 1 < NUM_LEDS)
      {
        leds[i] += CHSV( redBrightness/4, greenBrightness/4, blueBrightness/4);
      }
    }
  }
}

void blendwave() {
  CRGB clr1;
  CRGB clr2;
  uint8_t loc1;
  uint8_t loc2;
  uint8_t speed;
  speed = beatsin8(6,0,255);

  clr1 = blend(CHSV(beatsin8(3,0,255),255,255), CHSV(beatsin8(4,0,255),255,255), speed);
  clr2 = blend(CHSV(beatsin8(4,0,255),255,255), CHSV(beatsin8(3,0,255),255,255), speed);

  loc1 = beatsin8(10,0,NUM_LEDS-1);
  
  fill_gradient_RGB(leds, 0, clr2, loc1, clr1);
  fill_gradient_RGB(leds, loc1, clr2, NUM_LEDS-1, clr1);

} // blendwave()
