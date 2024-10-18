int flag = 1;
int packetlen = 0;

void setup() {
  pinMode(22, INPUT);
  pinMode(21, INPUT);
  Serial.begin(38400);
  Serial3.begin(38400, SERIAL_8N2);
}

void loop() {
  if(digitalRead(22) == 1) {
    flag = 1;
  } else if(flag && digitalRead(21) == 1) {
    Serial3.write(2);
    Serial3.write(0x9);
    flag = 0;
  }
  if(Serial3.available() > 1) {
    int rec = Serial3.read();
    if(packetLen == 0) 
      packetLen = rec;
    Serial.println(rec);
  }
  
}
