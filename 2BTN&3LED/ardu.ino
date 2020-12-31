// input
int rightBtn = 2;
int leftBtn = 4;
int isRightOn;
int isLeftOn;


// output
int rightLed = 7;
int leftLed = 8;
int middleLed = 12;

void setup() {
  Serial.begin(9600);
  pinMode(leftBtn, INPUT);
  pinMode(rightBtn, INPUT);
  pinMode(rightLed, OUTPUT);
  pinMode(leftLed, OUTPUT);
  pinMode(middleLed, OUTPUT);
}

void loop() {
  isRightOn = digitalRead(rightBtn);
  isLeftOn = digitalRead(leftBtn);
  Serial.println(isRightOn);
  Serial.println(isLeftOn);

  if(isRightOn && isLeftOn) {
    // only middleLed should be on
    digitalWrite(leftLed, LOW);
    digitalWrite(rightLed, LOW);
    digitalWrite(middleLed, HIGH);
  } else {
    // middleLed should be off, whilst one of the other two can eventualy be on.
    digitalWrite(middleLed, LOW);
    
    if(isRightOn) {
      digitalWrite(rightLed, HIGH);
    } else {
      digitalWrite(rightLed, LOW);
    }

    if(isLeftOn) {
      digitalWrite(leftLed, HIGH);
    } else {
      digitalWrite(leftLed, LOW);
    }
  }
  delay(100);
}