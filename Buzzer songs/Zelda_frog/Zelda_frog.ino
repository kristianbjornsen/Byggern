
int sensorPin = 5; 	// light sensor is connected to analog pin 5
int sensorValue; 	// variable to store the value coming from the sensor
int speakerPin = 11; 	// speaker connected to digital pin 17
int speakerGnd = 12;    // spearker - is connectted to digital pin 18
 
int ledPin = 13;        // LED onboard is connected digital pin 13
int redPin = 18; 	// R petal on RGB LED module connected to digital pin 11
int greenPin = 9; 	// G petal on RGB LED module connected to digital pin 9
int bluePin = 10; 	// B petal on RGB LED module connected to digital pin 10
 
int NOTE_B2  = 123;
int NOTE_C3  = 131;
  int NOTE_C3s = 139;
  int NOTE_D3b = 139;
int NOTE_D3  = 147;
  int NOTE_D3s = 156;
  int NOTE_E3b = 156;
int NOTE_E3  = 165;
int NOTE_F3  = 175;
  int NOTE_F3s = 185;
  int NOTE_G3b = 185;
int NOTE_G3  = 196;
  int NOTE_G3s = 208;
  int NOTE_A3b = 208;
int NOTE_A3  = 220;
  int NOTE_A3s = 233;
  int NOTE_B3b = 233;
int NOTE_B3  = 247;
int NOTE_C4  = 262;
  int NOTE_C4s = 277;
  int NOTE_D4b = 277;
int NOTE_D4  = 294;
  int NOTE_D4s = 311;
  int NOTE_E4b = 311;
int NOTE_E4  = 330;
int NOTE_F4  = 349;
  int NOTE_F4s = 370;
  int NOTE_G4b = 370;
int NOTE_G4  = 392;
  int NOTE_G4s = 415;
  int NOTE_A4b = 415;
int NOTE_A4  = 440;
  int NOTE_A4s = 466;
  int NOTE_B4b = 466;
int NOTE_B4  = 494;
int NOTE_C5  = 523;
  int NOTE_C5s = 554;
  int NOTE_D5b = 554;
int NOTE_D5  = 587;
  int NOTE_D5s = 622;
  int NOTE_E5b = 622;
int NOTE_E5  = 659;
int NOTE_F5  = 698;
  int NOTE_F5s = 740;
  int NOTE_G5b = 740;
int NOTE_G5  = 784;
  int NOTE_G5s = 831;
  int NOTE_A5b = 831;
int NOTE_A5  = 880;
  int NOTE_A5s = 932;
  int NOTE_B5b = 932;
int NOTE_B5  = 988;
int NOTE_C6  = 1047;
  int NOTE_C6s = 1109;
  int NOTE_D6b = 1109;
int NOTE_D6  = 1175;
  int NOTE_D6s = 1245;
  int NOTE_E6b = 1245;
int NOTE_E6  = 1319;
int NOTE_F6  = 1397;
  int NOTE_F6s = 1480;
  int NOTE_G6b = 1480;
int NOTE_G6  = 1568;
  int NOTE_G6s = 1661;
  int NOTE_A6b = 1661;
int NOTE_A6  = 1760;
  int NOTE_A6s = 1865;
  int NOTE_B6b = 1865;
int NOTE_B6  = 1976;
int NOTE_C7  = 2093;
  int NOTE_C7s = 2217;
  int NOTE_D7b = 2217;
int NOTE_D7  = 2349;
  int NOTE_D7s = 2489;
  int NOTE_E7b = 2489;
int NOTE_E7  = 2637;
int NOTE_F7  = 2794;
  int NOTE_F7s = 2960;
  int NOTE_G7b = 2960;
int NOTE_G7  = 3136;
  int NOTE_G7s = 3322;
  int NOTE_A7b = 3322;
int NOTE_A7  = 3520;
  int NOTE_A7s = 3729;
  int NOTE_B7b = 3729;
int NOTE_B7  = 3951;
int NOTE_C8  = 4186;
  int NOTE_C8s = 4435;
  int NOTE_D8b = 4435;
int NOTE_D8  = 4699;
  int NOTE_D8s = 4978;
  int NOTE_E8b = 4978;
 
void setup() 	 
{ 	 
  pinMode(speakerGnd, OUTPUT); 	        // sets the speakerGnd to be an output
  digitalWrite(speakerGnd, LOW); 	// sets speakerGnd to be LOW
  pinMode(speakerPin, OUTPUT); 	        // sets the speakerPin to be an output
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  pinMode(redPin, OUTPUT); 	// sets the redPin to be an output
  pinMode(greenPin, OUTPUT); 	// sets the greenPin to be an output
  pinMode(bluePin, OUTPUT); 	// sets the bluePin to be an output
  
} 	 
 
void loop() 	// run over and over again
{ 	  
   zelda(); 	        // call the scale() function
   delay(100); 	// delay for 1 secon
} 	 
 
void color (unsigned char red, unsigned char green, unsigned char blue)     // the color generating function
{ 	 
          analogWrite(redPin, 255-red); 	 
          analogWrite(bluePin, 255-blue);
          analogWrite(greenPin, 255-green);
} 	
 
void beep (unsigned char speakerPin, int frequencyInHertz, long timeInMilliseconds)     // the sound producing function
{ 	 
  int x; 	 
  long delayAmount = (long)(1000000/frequencyInHertz);
  long loopTime = (long)((timeInMilliseconds*1000)/(delayAmount*2));
  for (x=0;x<loopTime;x++) 	 
  { 
    digitalWrite(speakerPin,HIGH);
    delayMicroseconds(delayAmount);
    digitalWrite(speakerPin,LOW);
    delayMicroseconds(delayAmount);
  } 	 
} 	 
 
void scale ()
{ 	 
  digitalWrite(ledPin,HIGH); 	//turn on the LED
  beep(speakerPin,2093,500); 	//C: play the note C (C7 from the chart linked to above) for 500ms
  beep(speakerPin,2349,500); 	//D
  beep(speakerPin,2637,500); 	//E
  beep(speakerPin,2793,500); 	//F
  beep(speakerPin,3136,500); 	//G
  beep(speakerPin,3520,500); 	//A
  beep(speakerPin,3951,500); 	//B
  beep(speakerPin,4186,500); 	//C
  digitalWrite(ledPin,LOW); 	//turn off the LED
} 
 
void two_beep (unsigned int tone_1, unsigned int tone_2, long duration)
{
  int chunk = 30;
  int t = 0;
  while (t < duration){
    beep(speakerPin,tone_1,chunk);
    beep(speakerPin,tone_2,chunk);
    t += (2 * chunk);
  }
}
 
void zelda ()
{
  int d = 300;
  int r = 255;
  int g = 0;
  int b = 0;
 
  // 1
  beep(speakerPin,NOTE_G7,d); r = 240; g += 16; color(r,g,b);
  beep(speakerPin,NOTE_G6,d); r -= 16; g += 16; color(r,g,b);
  beep(speakerPin,NOTE_C7,d); r -= 16; g += 16; color(r,g,b);
  beep(speakerPin,NOTE_E7,d); r -= 16; g += 16; color(r,g,b);
 
  // 2
  beep(speakerPin,NOTE_E7b,d); r -= 16; g += 16; color(r,g,b);
  beep(speakerPin,NOTE_G6,d); r -= 16; g += 16; color(r,g,b);
  beep(speakerPin,NOTE_B7,d); r -= 16; g += 16; color(r,g,b);
  beep(speakerPin,NOTE_B6,d); r -= 16; g += 16; color(r,g,b);
  
  // 3
  beep(speakerPin,NOTE_A7,d); r -= 16; g += 16; color(r,g,b);
  beep(speakerPin,NOTE_C7,d); r -= 16; g += 16; color(r,g,b);
  beep(speakerPin,NOTE_E7,d); r -= 16; g += 16; color(r,g,b);
  beep(speakerPin,NOTE_A7,d); r -= 16; g += 16; color(r,g,b);
  
  // 4
  beep(speakerPin,NOTE_G7,d); r -= 16; g += 16; color(r,g,b);
  beep(speakerPin,NOTE_C7,d); r -= 16; g += 16; color(r,g,b);
  beep(speakerPin,NOTE_D7,d); r -= 16; g += 16; color(r,g,b);
  beep(speakerPin,NOTE_E7,d); r -= 16; g = 255; color(r,g,b);
  
  // 5
  beep(speakerPin,NOTE_A7,d); g = 240; b += 16; color(r,g,b);
  beep(speakerPin,NOTE_C7,d); g -= 16; b += 16; color(r,g,b);
  beep(speakerPin,NOTE_F7,d); g -= 16; b += 16; color(r,g,b);
  beep(speakerPin,NOTE_A7,d); g -= 16; b += 16; color(r,g,b);
  
  // 6
  beep(speakerPin,NOTE_A7b,d); g -= 16; b += 16; color(r,g,b);
  beep(speakerPin,NOTE_C7,d); g -= 16; b += 16; color(r,g,b);
  beep(speakerPin,NOTE_D7,d); g -= 16; b += 16; color(r,g,b);
  beep(speakerPin,NOTE_F7,d); g -= 16; b += 16; color(r,g,b);
  
  // 7
  beep(speakerPin,NOTE_E7,d); g -= 16; b += 16; color(r,g,b);
  beep(speakerPin,NOTE_G6,d); g -= 16; b += 16; color(r,g,b);
  beep(speakerPin,NOTE_C7,d); g -= 16; b += 16; color(r,g,b);
  beep(speakerPin,NOTE_E7,d); g -= 16; b += 16; color(r,g,b);
  
  // 8
  beep(speakerPin,NOTE_D7,d); g -= 16; b += 16; color(r,g,b);
  two_beep(NOTE_G4,NOTE_A6,d); g -= 16; b += 16; color(r,g,b);
  two_beep(NOTE_A4,NOTE_B6,d); g -= 16; b += 16; color(r,g,b);
  two_beep(NOTE_B4,NOTE_D7,d); g -= 16; b = 255; color(r,g,b);
  
  // Bring in the bass line...
 
  // 1
  two_beep(NOTE_C5,NOTE_G7,d); b = 240; r += 16; color(r,g,b);
  two_beep(NOTE_G5,NOTE_G6,d); b -= 16; r += 16; color(r,g,b);
  two_beep(NOTE_G4,NOTE_C7,d); b -= 16; r += 16; color(r,g,b);
  two_beep(NOTE_G5,NOTE_E7,d); b -= 16; r += 16; color(r,g,b);
  
  // 2
  two_beep(NOTE_B4,NOTE_E7b,d); b -= 16; r += 16; color(r,g,b);
  two_beep(NOTE_G5,NOTE_G6,d); b -= 16; r += 16; color(r,g,b);
  two_beep(NOTE_G4,NOTE_B7,d); b -= 16; r += 16; color(r,g,b);
  two_beep(NOTE_G5,NOTE_B6,d); b -= 16; r += 16; color(r,g,b);
 
  // 3  
  two_beep(NOTE_C5,NOTE_A7,d); b -= 16; r += 16; color(r,g,b);
  two_beep(NOTE_G5,NOTE_C7,d); b -= 16; r += 16; color(r,g,b);
  two_beep(NOTE_G4,NOTE_E7,d); b -= 16; r += 16; color(r,g,b);
  two_beep(NOTE_G5,NOTE_A7,d); b -= 16; r += 16; color(r,g,b);
  
  // 4
  two_beep(NOTE_C5,NOTE_G7,d); b -= 16; r += 16; color(r,g,b);
  two_beep(NOTE_C5,NOTE_C7,d); b -= 16; r += 16; color(r,g,b);
  two_beep(NOTE_D5,NOTE_D7,d); b -= 16; r += 16; color(r,g,b);
  two_beep(NOTE_E5,NOTE_E7,d); b -= 16; r = 255; color(r,g,b);
  
  // 5
  two_beep(NOTE_F5,NOTE_A7,d); r = 16; g += 16; b += 16; color(r,g,b);
  two_beep(NOTE_C6,NOTE_C7,d); r += 16; g += 16; b += 16; color(r,g,b);
  two_beep(NOTE_C5,NOTE_F7,d); r += 16; g += 16; b += 16; color(r,g,b);
  two_beep(NOTE_C6,NOTE_A7,d); r += 16; g += 16; b += 16; color(r,g,b);
  
  // 6
  two_beep(NOTE_F5,NOTE_A7b,d); r += 16; g += 16; b += 16; color(r,g,b);
  two_beep(NOTE_C6,NOTE_C7,d); r += 16; g += 16; b += 16; color(r,g,b);
  two_beep(NOTE_C5,NOTE_D7,d); r += 16; g += 16; b += 16; color(r,g,b);
  two_beep(NOTE_C6,NOTE_F7,d); r += 16; g += 16; b += 16; color(r,g,b);
  
  // 7
  two_beep(NOTE_E5,NOTE_E7,d); r += 16; g += 16; b += 16; color(r,g,b);
  two_beep(NOTE_C6,NOTE_G6,d); r += 16; g += 16; b += 16; color(r,g,b);
  two_beep(NOTE_G4,NOTE_C7,d); r += 16; g += 16; b += 16; color(r,g,b);
  two_beep(NOTE_G5,NOTE_E7,d); r += 16; g += 16; b += 16; color(r,g,b);
  
  // 8
  two_beep(NOTE_B4,NOTE_D7,d); r += 16; g += 16; b += 16; color(r,g,b);
  two_beep(NOTE_G4,NOTE_A6,d); r += 16; g += 16; b += 16; color(r,g,b);
  two_beep(NOTE_A4,NOTE_B6,d); r += 16; g += 16; b += 16; color(r,g,b);
  two_beep(NOTE_B4,NOTE_D7,d); color(255,255,255);
  
  // Final note!
  two_beep(NOTE_E7,NOTE_G7,2*d);
  
}
