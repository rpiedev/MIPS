void setup() {
  pinMode(22, INPUT);
  pinMode(21, INPUT);
  Serial.begin(9600);
  Serial3.begin(9600, SERIAL_8N2);
}

void loop() {
  if(digitalRead(22) == 1) {
    Serial3.write('n');
  } else if(digitalRead(21) == 1) {
    Serial3.write('f');
  }
  delay(1);
  if(Serial3.available() > 1) {
    char thing = Serial3.read();
    Serial.println(thing);
  }
}