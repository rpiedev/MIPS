int i = 0;
uint8_t x = 0;

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  i++;
  if(Serial1.available() > 0) {
    int thing = Serial1.read();
    if(thing == 255) return;
    Serial.println(thing);
  }
  delay(1);
}