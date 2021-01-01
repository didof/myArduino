#define ARRAY_SIZE(array) ((sizeof(array)) / (sizeof(array[0])))

const int buzzer = 3;

const int btns[] = { 2, 4, 7 };
int prevBtnStates[] = { 0, 0, 0, };
const int leds[] = { 8, 12, 13 };
const int freqs[] = { 440, 1760, 3520 };

const int recBtn = 11;
const int recLed = 10;

int prevRecState;
int currRecState;

bool isRecording;
int recIndexes[8] = { };
int counter = 0;

const int ptm = A0;

void setup() {
  Serial.begin(9600);
  
  pinMode(buzzer, OUTPUT);

  pinMode(recBtn, OUTPUT);
  pinMode(recLed, INPUT);

  pinMode(ptm, INPUT);

  for (int i = 0; i < 3; i++) {
    pinMode(btns[i], OUTPUT);
    pinMode(leds[i], INPUT);
  }
}

void player(int duration) {
  for(int i = 0; i < ARRAY_SIZE(recIndexes); i++) {
    tone(buzzer, freqs[recIndexes[i]]);
    digitalWrite(leds[recIndexes[i]], HIGH);
    delay(duration);
    noTone(buzzer);
    digitalWrite(leds[recIndexes[i]], LOW);
    delay(duration / 10);
  }
}

int calcVelocity(int input) {
  if(input <= 341) {
    return 500;
  }
  if(input <= 682) {
    return 300;
  }
  return 150;
}

void loop() {
  currRecState = digitalRead(recBtn);
  
  if(prevRecState && !currRecState) {
    if(counter < ARRAY_SIZE(recIndexes)) {
      isRecording = !isRecording;
    } else {
      int velocity = calcVelocity(analogRead(ptm));
        player(velocity);
    }
  }
  prevRecState = currRecState;

  if(isRecording) {
    analogWrite(recLed, 255);
  } else {
    analogWrite(recLed, 0);
  }
  
  for(int i = 0; i < 3; i++) {
   int currBtnState = digitalRead(btns[i]);
    if(currBtnState) {
      digitalWrite(leds[i], HIGH);    // led on
      tone(buzzer, freqs[i]);         // sound on
    } else {
      digitalWrite(leds[i], LOW);
      noTone(buzzer);
    }

    if(prevBtnStates[i] && !currBtnState && isRecording) {
      recIndexes[counter] = i;
      counter++;
      if(counter >= ARRAY_SIZE(recIndexes)) {
        isRecording = false;
      }
    }

    prevBtnStates[i] = currBtnState;
  }

  delay(1);
}