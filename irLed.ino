#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRtimer.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

/* LED */
#define PIN            4
#define NUMPIXELS      60
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int RECV_PIN = 5;  //  Der Kontakt der am Infrarotsensor die Daten ausgibt, wird mit Pin 11 des Arduinoboards verbunden.

IRrecv irrecv(RECV_PIN);   // An dieser Stelle wird ein Objekt definiert, dass den Infrarotsensor an Pin 11 ausliest.

decode_results results;  // Dieser Befehl sorgt dafür, dass die Daten, die per Infrarot eingelesen werden unter „results“ abgespeichert werden.
decode_results results2;
bool customLoop = false; // Variable telling us we are in a custom animation loop
int currentColors[] = {0, 0, 0};
unsigned long previousMillis = 0; // variable for the delay function
int intensity = 10; // Intensity variable
int speedValue = 5; // Speed Variable
void RGB_Remote(long code);

/* DEFINE IR CODES */
#define OFF_CODE            16712445
#define NEXT_ANIMATION      16745085
#define INTENSITY_DN_CODE   16759365
#define INTENSITY_UP_CODE   16726725
#define RED_CODE            16718565
#define RED2_CODE           16722645
#define RED3_CODE           16714485
#define RED4_CODE           16726215
#define RED5_CODE           16718055
#define GREEN_CODE          16751205
#define GREEN2_CODE         16755285
#define GREEN3_CODE         16747125
#define GREEN4_CODE         16758855
#define GREEN5_CODE         16750695
#define BLUE_CODE           16753245
#define BLUE2_CODE          16749165
#define BLUE3_CODE          16757325
#define BLUE4_CODE          16742535
#define BLUE5_CODE          16734375
#define WHITE_CODE          16720605
#define WHITE2_CODE         16716525
#define WHITE3_CODE         16724685
#define WHITE4_CODE         16775175
#define WHITE5_CODE         16767015
#define RED_UP_CODE         16722135
#define RED_DOWN_CODE       16713975
#define GREEN_UP_CODE       16754775
#define GREEN_DOWN_CODE     16746615
#define BLUE_UP_CODE        16738455
#define BLUE_DOWN_CODE      16730295
#define QUICK_CODE          16771095
#define SLOW_CODE           16762935
#define DIY1_CODE           16724175
#define DIY2_CODE           16756815
#define DIY3_CODE           16740495
#define DIY4_CODE           16716015
#define DIY5_CODE           16748655
#define DIY6_CODE           16732335

int BLACK_COLOR[3] = {0, 0, 0};
int RED_COLOR[3] = {255, 0, 0};
int RED2_COLOR[3] = {100, 0, 0};
int RED3_COLOR[3] = {255, 128, 0};
int RED4_COLOR[3] = {255, 208, 0};
int RED5_COLOR[3] = {250, 250, 0};
int GREEN_COLOR[3] = {0, 255, 0};
int GREEN2_COLOR[3] = {0, 200, 0};
int GREEN3_COLOR[3] = {0, 150, 0};
int GREEN4_COLOR[3] = {0, 100, 0};
int GREEN5_COLOR[3] = {0, 50, 0};
int BLUE_COLOR[3] =  {0, 0, 255};
int BLUE2_COLOR[3] =  {0, 5, 79};
int BLUE3_COLOR[3] =  {102, 0, 255};
int BLUE4_COLOR[3] =  {47, 0, 119};
int BLUE5_COLOR[3] =  {46, 15, 94};
int WHITE_COLOR[3] = {255, 255, 255};
int WHITE2_COLOR[3] = {255, 102, 178};
int WHITE3_COLOR[3] = {255, 0, 128};
int WHITE4_COLOR[3] = {0, 255, 255};
int WHITE5_COLOR[3] = {0, 140, 140};

int AVAILABLE_COLORS[16][3] = {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}, {255, 255, 255}, {255, 128, 0}, {0, 255, 128}, {0, 76, 153}, {153, 76, 0}, {0, 102, 102}, {102, 0, 51}, {255, 153, 51}, {0, 255, 255}, {255, 0, 127}, {255, 255, 0}, {51, 153, 255}, {255, 102, 158}} ;

void setup()
{
  pixels.begin(); // This initializes the NeoPixel library.
  pixels.show();
  Serial.begin(9600);    //Im Setup wird die Serielle Verbindung gestartet, damit man sich die Empfangenen Daten der Fernbedienung per seriellen Monitor ansehen kann.
  irrecv.enableIRIn();   //Dieser Befehl initialisiert den Infrarotempfänger.

}

void loop()
{
  
  unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 100) {
      //Serial.println("test");
      previousMillis = currentMillis;
      findCode();
    }
}


void findCode(){
  if (irrecv.decode(&results)) {
    int lul = results.value;
    RGB_Remote(lul);
    irrecv.resume();  // Receive the next value
    if((int)results.value != DIY1_CODE && (int)results.value != DIY2_CODE && (int)results.value != DIY3_CODE && (int)results.value != DIY4_CODE && (int)results.value != DIY5_CODE && (int)results.value != DIY6_CODE){
      customLoop = false;
      sendColor();
    }
    
  }
}

void setColor(int colors[]) {
  if(colors[0] == currentColors[0] && colors[1] == currentColors[1] && colors[2] == currentColors[2]){
    return;
  }
  currentColors[0] = colors[0];
  currentColors[1] = colors[1];
  currentColors[2] = colors[2];
}

void sendColor()
{
  if (customLoop == false) {
    int red = currentColors[0];
    int green = currentColors[1];
    int blue = currentColors[2];

    int mappedIntensity = map(intensity, 0, 10, 0, 255);
    int redComputed = map(red, 0, 255, 0, mappedIntensity);
    int blueComputed = map(blue, 0, 255, 0, mappedIntensity);
    int greenComputed = map(green, 0, 255, 0, mappedIntensity);
    
    for (int i = 0; i < NUMPIXELS; i++ ) {
      pixels.setPixelColor(i, pixels.Color(redComputed,greenComputed,blueComputed));
      pixels.show();
      delay(5);
    }
    
  }
}

// raise the intensity of light or the speed of animation
void raiseIntensity() {
  if (!customLoop) {
    if (intensity <= 9) {
      intensity++;
    }
  } else if (speedValue <= 9) {
    speedValue++;
  }
}

// lower the intensity of light or the speed of animation
void lowerIntensity() {
  if (!customLoop) {
    if (intensity >= 2) {
      intensity--;
    }
  } else if (speedValue >= 2) {
    speedValue--;
  }
}

void setOff() {
  for (int i = 0; i < NUMPIXELS; i++ ) {
    pixels.setPixelColor(i, pixels.Color(0,0,0));
  }
  pixels.show();
}

void test1(int timer){
  customLoop = true;
  while(customLoop){
    setOff();
  for(int i=0;i<NUMPIXELS + 7;i++)
  {
    if (irrecv.decode(&results)) {
        if((int)results.value != DIY1_CODE){
          RGB_Remote((int)results.value);
          return; break;
          //irrecv.resume();  // Receive the next value
        }
        irrecv.resume();
      }
    pixels.setPixelColor(i - 7, pixels.Color(0,0,0));
    pixels.setPixelColor(i - 6, pixels.Color(0,1,1));
    pixels.setPixelColor(i - 5, pixels.Color(0,5,5));
    pixels.setPixelColor(i - 4, pixels.Color(0,10,10));
    pixels.setPixelColor(i - 3, pixels.Color(0,50,50));
    pixels.setPixelColor(i - 2, pixels.Color(0,100,100));
    pixels.setPixelColor(i - 1, pixels.Color(0,150,150));
    pixels.setPixelColor(i, pixels.Color(0,250,250));
    pixels.show(); 
    delay(timer);
  }
    for(int i=0;i<NUMPIXELS + 7;i++)
  {
    if (irrecv.decode(&results)) {
      
        if((int)results.value != DIY1_CODE){
          RGB_Remote((int)results.value);
          return; break;
          //irrecv.resume();  // Receive the next value
        }
        irrecv.resume();
      }
    pixels.setPixelColor(i - 7, pixels.Color(0,0,0));
    pixels.setPixelColor(i - 6, pixels.Color(1,1,0));
    pixels.setPixelColor(i - 5, pixels.Color(5,5,0));
    pixels.setPixelColor(i - 4, pixels.Color(10,10,0));
    pixels.setPixelColor(i - 3, pixels.Color(50,50,0));
    pixels.setPixelColor(i - 2, pixels.Color(100,100,0));
    pixels.setPixelColor(i - 1, pixels.Color(150,150,0));
    pixels.setPixelColor(i, pixels.Color(250,250,0));
    pixels.show(); 
    delay(timer);
  }
    for(int i=0;i<NUMPIXELS + 7;i++)
  {
    if (irrecv.decode(&results)) {
        if((int)results.value != DIY1_CODE){
          RGB_Remote((int)results.value);
          return; break;
          //irrecv.resume();  // Receive the next value
        }
        irrecv.resume();
      }
    pixels.setPixelColor(i - 7, pixels.Color(0,0,0));
    pixels.setPixelColor(i - 6, pixels.Color(1,0,1));
    pixels.setPixelColor(i - 5, pixels.Color(5,0,5));
    pixels.setPixelColor(i - 4, pixels.Color(10,0,10));
    pixels.setPixelColor(i - 3, pixels.Color(50,0,50));
    pixels.setPixelColor(i - 2, pixels.Color(100,0,100));
    pixels.setPixelColor(i - 1, pixels.Color(150,0,150));
    pixels.setPixelColor(i, pixels.Color(250,0,250));
    pixels.show(); 
    delay(timer);
  }
  for(int i=0;i<NUMPIXELS + 7;i++)
  {
    if (irrecv.decode(&results)) {
        if((int)results.value != DIY1_CODE){
          RGB_Remote((int)results.value);
          return; break;
          //irrecv.resume();  // Receive the next value
        }
        irrecv.resume();
      }
    pixels.setPixelColor(i - 7, pixels.Color(0,0,0));
    pixels.setPixelColor(i - 6, pixels.Color(0,1,0));
    pixels.setPixelColor(i - 5, pixels.Color(0,5,0));
    pixels.setPixelColor(i - 4, pixels.Color(0,10,0));
    pixels.setPixelColor(i - 3, pixels.Color(0,50,0));
    pixels.setPixelColor(i - 2, pixels.Color(0,100,0));
    pixels.setPixelColor(i - 1, pixels.Color(0,150,0));
    pixels.setPixelColor(i, pixels.Color(0,250,0));
    pixels.show(); 
    delay(timer);
  }
  }
 }

void TwinkleRandom(int Count, int SpeedDelay) {
  setColor(BLACK_COLOR);

  for (int i = 0; i < Count; i++) {
    pixels.setPixelColor(random(NUMPIXELS), pixels.Color(random(0, 255),random(0, 255),random(0, 255)));
    pixels.show();
    delay(SpeedDelay);
  }

  delay(SpeedDelay);
}

void colorWipe() {
  customLoop = true;
  while(customLoop){
    setOff();
    for(int i=0;i<NUMPIXELS;i++)
    {
      if (irrecv.decode(&results2)) {
        int lul = (int)results2.value;
        Serial.print("Neuer IRCode: ");
        Serial.println(lul);
        RGB_Remote(lul);
        return; break;
        //irrecv.resume();
      }
      irrecv.resume();
      pixels.setPixelColor(i, pixels.Color(currentColors[0],currentColors[1],currentColors[2]));
      pixels.show();
      delay(50);
    }
  }
  
}

void RGB_Remote(int code){
  //Serial.println(code);
  switch(code){
    case OFF_CODE: // An/Aus
      //Serial.println("An/Aus");
      setColor(BLACK_COLOR);
      break; 
    case NEXT_ANIMATION: //nächste animation
      //Serial.println("nächste animation");
      break; 
    case INTENSITY_DN_CODE: //Helligkeit runter
      //Serial.println("Helligkeit runter"); 
      break; 
    case INTENSITY_UP_CODE: //Helligkeit hoch
      //Serial.println("Helligkeit hoch");
      break; 
    case RED_CODE: //Rot
      //Serial.println("Rot"); 
      setColor(RED_COLOR);
      break; 
    case RED2_CODE: //Rot
      //Serial.println("Rot"); 
      setColor(RED2_COLOR);
      break; 
    case RED3_CODE: //Rot
      //Serial.println("Rot"); 
      setColor(RED3_COLOR);
      break; 
    case RED4_CODE: //Rot
      //Serial.println("Rot"); 
      setColor(RED4_COLOR);
      break; 
    case RED5_CODE: //Rot
      //Serial.println("Rot"); 
      setColor(RED5_COLOR);
      break; 
    case GREEN_CODE: //Grün
      //Serial.println("Grün"); 
      setColor(GREEN_COLOR); 
      break; 
    case GREEN2_CODE: //Grün
      //Serial.println("Grün"); 
      setColor(GREEN2_COLOR); 
      break; 
    case GREEN3_CODE: //Grün
      //Serial.println("Grün"); 
      setColor(GREEN3_COLOR); 
      break;
    case GREEN4_CODE: //Grün
      //Serial.println("Grün"); 
      setColor(GREEN4_COLOR); 
      break;  
    case GREEN5_CODE: //Grün
      //Serial.println("Grün"); 
      setColor(GREEN5_COLOR); 
      break; 
    case BLUE_CODE: //Blau
      //Serial.println("Blau"); 
      setColor(BLUE_COLOR);
      break; 
    case BLUE2_CODE: //Blau
      //Serial.println("Blau"); 
      setColor(BLUE2_COLOR);
      break; 
    case BLUE3_CODE: //Blau
      //Serial.println("Blau"); 
      setColor(BLUE3_COLOR);
      break; 
    case BLUE4_CODE: //Blau
      //Serial.println("Blau"); 
      setColor(BLUE4_COLOR);
      break;
    case BLUE5_CODE: //Blau
      //Serial.println("Blau"); 
      setColor(BLUE5_COLOR);
      break; 
    case WHITE_CODE: //White
      //Serial.println("White"); 
      setColor(WHITE_COLOR); 
      break;
    case WHITE2_CODE: //White
      //Serial.println("White"); 
      setColor(WHITE2_COLOR); 
      break;
    case WHITE3_CODE: //White
      //Serial.println("White"); 
      setColor(WHITE3_COLOR); 
      break;
    case WHITE4_CODE: //White
      //Serial.println("White"); 
      setColor(WHITE4_COLOR); 
      break;
    case WHITE5_CODE: //White
      //Serial.println("White"); 
      setColor(WHITE5_COLOR); 
      break;
    case DIY1_CODE:
      Serial.println("DIY1");
      test1(40); 
      break;
    case DIY2_CODE:
      Serial.println("DIY2");
      TwinkleRandom(20, 100);
      break;
    case DIY3_CODE:
      //Serial.println("ColorWipe"); 
      irrecv.resume();
      colorWipe(); // Red
      break;
    case DIY4_CODE:
      Serial.println("ColorWipe"); 
      break;
    case DIY5_CODE:
      Serial.println("ColorWipe"); 
      break;
    default: 
      Serial.print("Code nicht gefunden: ");
      Serial.println((int)results.value);  
      break; 
    }
  }



