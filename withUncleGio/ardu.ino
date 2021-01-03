#define ARRAY_SIZE(array) ((sizeof(array)) / (sizeof(array[0])))

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

Servo servo;

const int buz = 2;
const int ptm = A0;
const int btn = 4;

int pause;

const int times = 4;
const int servoDeg[] = { 180, 180, 180, 180 };
const int buzHz[] = { 440, 440, 523, 294 };
const int buzPauseFactor[] = { 1, 1, 2, 1 };

// CONFIG
int useServo = false;
int useBuz = false;

LiquidCrystal_I2C lod(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup() {
  Serial.begin(9600);

  pinMode(btn, INPUT);

  // BUZZER
  if(useBuz) {
     pinMode(buz, OUTPUT);
  }

  // LCD
  lod.begin(16, 2); // x, y
  lod.clear();
  lod.setCursor(0, 0);
  lod.print("boot");

  // SERVO
  if(useServo) {
    servo.attach(8); 
    servo.write(0); 
  }

  Serial.println("use buzzer: " + String(useBuz));
  Serial.println("use servo: " + String(useServo));

  
}

float newPause;
const int maxServoDeg = 180;
void playServo(int deg, int pause) {
  newPause = pause / (maxServoDeg / deg);
  servo.write(deg);
  delay(newPause);
  servo.write(0);
  delay(newPause);
}

void playBuz(int hz, int duration) {
  Serial.println();
  tone(buz, hz);
  delay(duration);
  noTone(buz);
}

int note = 0;
void resetNote() {
  if(note == times - 1) {
     note = 0;
  } else {
    note++;
  }
}

void toggleReprodution() {
  useBuz = !useBuz;
}

String text1;
int prevDep1 = useBuz;
void printReproducingOnLCD(int dep) {
  if(prevDep1 != dep) {
    lod.setCursor(0, 0);
    lod.clear();
    if(useBuz) {
      text1 = "playing";
    } else {
      text1 = "pause";
    }
    lod.print(text1);
  }

  prevDep1 = dep;
}

String text2;
int prevDep2 = 0;
void printVelocityOnLCD(int dep) {
  lod.setCursor(6, 0);
  if(prevDep2 != dep) {
    lod.clear();
    lod.print("x" + String(dep));
  }

  prevDep2 = dep;
}

String text;
int prevIsPlaying = useBuz;
int prevVelocity = 0;
void printOnLCD(int isPlaying, int velocity) {
   
   if(prevIsPlaying != isPlaying) {
    lod.setCursor(0, 0);
    if(isPlaying) {
      text = "playing";
    } else {
      text = "pause";
    }
   }

   text += String(velocity);

   lod.print(text);

   
   prevIsPlaying = isPlaying;
   prevVelocity = velocity;
}

// CURRENT
int currentHz;
int currentPauseFactor;
int currentServoDeg;

// BUTTON
int isButtonPressed;
int prevButtonState;
int t0, t;

void loop() {
  isButtonPressed = digitalRead(btn);
  
  if(isButtonPressed) {
    if(!prevButtonState) {
      t0 = millis();
    }
    
  } else {
    if(prevButtonState) {
      t = millis() - t0;
      Serial.println("holded for " + String(t));
      if(t >= 1000) {
        toggleReprodution();
      } else {
        
      }
    }
  }


  prevButtonState = isButtonPressed;

  // PTM
  pause = analogRead(ptm) + 500;

  printOnLCD(useBuz, analogRead(ptm));

  // EXTRACT - BUZZER
  if(useBuz) {
     currentHz = buzHz[note];
    currentPauseFactor = buzPauseFactor[note];
  }

  // EXTRACT - SERVO
  if(useServo) {
    currentServoDeg = servoDeg[note];
  }
  
  // MAIN

  // PLAY - BUZZER
  if(useBuz) {
    playBuz(currentHz, currentPauseFactor * pause);
  }

  // PLAY - SERVO
  if(useServo) {
    playServo(currentServoDeg, pause);  
  }
  
  // END
  resetNote();
}