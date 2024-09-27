int i = 0;
uint8_t x = 0;

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial1.write('c');

  delay(35);
}