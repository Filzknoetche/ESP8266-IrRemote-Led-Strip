#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRtimer.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <TimedAction.h>

/* LED */
#define PIN            5
#define NUMPIXELS      60
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int RECV_PIN = 2;  //  Der Kontakt der am Infrarotsensor die Daten ausgibt, wird mit Pin 11 des Arduinoboards verbunden.

IRrecv irrecv(RECV_PIN);   // An dieser Stelle wird ein Objekt definiert, dass den Infrarotsensor an Pin 11 ausliest.

decode_results results;  // Dieser Befehl sorgt dafür, dass die Daten, die per Infrarot eingelesen werden unter „results“ abgespeichert werden.
bool customLoop = false; // Variable telling us we are in a custom animation loop
int currentColors[] = {0, 0, 0};
unsigned long previousMillis = 0; // variable for the delay function
int intensity = 10; // Intensity variable
void RGB_Remote(long code);
/* DEFINE IR CODES */
#define OFF_CODE            16712445
#define NEXT_ANIMATION      16745085
#define INTENSITY_DN_CODE   16759365
#define INTENSITY_UP_CODE   16726725
#define RED_CODE            16718565
#define GREEN_CODE          16751205
#define BLUE_CODE           16753245
#define WHITE_CODE          16720605
#define DIY1_CODE           16724175
#define DIY2_CODE           16756815
#define DIY3_CODE           16740495
#define DIY4_CODE           16716015
#define DIY5_CODE           16748655
#define DIY6_CODE           16732335

int BLACK_COLOR[3] = {0, 0, 0};
int RED_COLOR[3] = {255, 0, 0};
int GREEN_COLOR[3] = {0, 255, 0};
int BLUE_COLOR[3] =  {0, 0, 255};
int WHITE_COLOR[3] = {255, 255, 255};
int ORANGE_COLOR[3] = {255, 128, 0};
int TURQUOISE_COLOR[3] = {0, 255, 128};
int NAVY_COLOR[3] = {0, 76, 153};
int BROWN_COLOR[3] = {153, 76, 0};
int TEAL_COLOR[3] = {0, 102, 102};
int PURPLE_DARK_COLOR[3] = {102, 0, 51};
int ORANGE_LIGHT_COLOR[3] = {255, 153, 51};
int BLUE_LIGHT_COLOR[3] = {0, 255, 255};
int PINK_DARK_COLOR[3] = {255, 0, 128};
int YELLOW_COLOR[3] = {255, 255, 0};
int BLUE_BABY_COLOR[3] = {51, 153, 255};
int PINK_COLOR[3] = {255, 102, 178};

int AVAILABLE_COLORS[16][3] = {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}, {255, 255, 255}, {255, 128, 0}, {0, 255, 128}, {0, 76, 153}, {153, 76, 0}, {0, 102, 102}, {102, 0, 51}, {255, 153, 51}, {0, 255, 255}, {255, 0, 127}, {255, 255, 0}, {51, 153, 255}, {255, 102, 158}} ;

void setup()
{
  pixels.begin(); // This initializes the NeoPixel library.
  pixels.show();
  Serial.begin(115200);    //Im Setup wird die Serielle Verbindung gestartet, damit man sich die Empfangenen Daten der Fernbedienung per seriellen Monitor ansehen kann.
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
    RGB_Remote((int)results.value);
    irrecv.resume();  // Receive the next value
    if((int)results.value != DIY1_CODE && (int)results.value != DIY2_CODE && (int)results.value != DIY3_CODE && (int)results.value != DIY4_CODE && (int)results.value != DIY5_CODE && (int)results.value != DIY6_CODE){
      customLoop = false;
      sendColor();
    }
    
  }
}

void setColor(int colors[]) {
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
    }
    pixels.show();
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

void colorWipe() {
  int red = currentColors[0];
  int green = currentColors[1];
  int blue = currentColors[2];
  customLoop = true;
  
  while(customLoop){
    setOff();
    for(int i=0;i<NUMPIXELS;i++)
    {
      if (irrecv.decode(&results)) {
        if((int)results.value != DIY3_CODE){
          RGB_Remote((int)results.value);
          return; break;
          //irrecv.resume();  // Receive the next value
        }
        irrecv.resume();
      }
      pixels.setPixelColor(i, pixels.Color(red,green,blue));
      pixels.show();
      delay(50);
    }
  }
  
}

void RGB_Remote(int code){
  Serial.println(code);
  switch((long)code){
    case OFF_CODE: // An/Aus
      Serial.println("An/Aus");
      setColor(BLACK_COLOR);
      break; 
    case NEXT_ANIMATION: //nächste animation
      Serial.println("nächste animation");
      break; 
    case INTENSITY_DN_CODE: //Helligkeit runter
      Serial.println("Helligkeit runter"); 
      break; 
    case INTENSITY_UP_CODE: //Helligkeit hoch
      Serial.println("Helligkeit hoch");
      break; 
    case RED_CODE: //Rot
      Serial.println("Rot"); 
      setColor(RED_COLOR);
      break; 
    case GREEN_CODE: //Grün
      Serial.println("Grün"); 
      setColor(GREEN_COLOR); 
      break; 
    case BLUE_CODE: //Blau
      Serial.println("Blau"); 
      setColor(BLUE_COLOR);
      break; 
    case WHITE_CODE: //White
      Serial.println("White"); 
      setColor(WHITE_COLOR); 
      break; 
    case DIY1_CODE:
      Serial.println("DIY1");
      test1(40); 
      break;
    case DIY2_CODE:
      Serial.println("DIY2");
      break;
    case DIY3_CODE:
      Serial.println("ColorWipe"); 
      colorWipe(); // Red
      break;
    case DIY4_CODE:
      Serial.println("ColorWipe"); 
      break;
    case DIY5_CODE:
      Serial.println("ColorWipe"); 
      break;
    default:  break; //Bei unbekanntem Input //Serial.println((long)results.value);
    }
  }



