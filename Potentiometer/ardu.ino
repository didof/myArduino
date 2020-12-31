// input
int ptm = A1; // Potentiometer; its output goes from 0 up to 1023

// output
int led = 11;

// utils
int factor = 4.012; // since ptm range is 0:1023 whilst led range is 0:255

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(ptm, INPUT);
}

void loop() {
  int ptmOutput = analogRead(ptm);
  analogWrite(led, floor(ptmOutput / factor));
  delay(100);
}