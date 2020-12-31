#define led 3
#define button 4
#define dimmer A0

int current;
int previous;

bool light;

int intensity;

int conversionFactor = 4.012;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(button, INPUT);
  pinMode(dimmer, INPUT);
}

void loop() {
  current = digitalRead(button);

  if(previous && !current) {
    light = !light;
  }

  if(light) {
    intensity = analogRead(dimmer);
    analogWrite(led, intensity / conversionFactor);
  } else {
    analogWrite(led, 0);
  }
  
  previous = current;
}