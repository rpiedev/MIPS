int flag = 1;
int packetLen = 0;

unsigned char buffer[10];
int bufferIndex = 0;
int bufferFull = 0;

struct Module {
  uint32_t address;
  uint16_t type;
  uint16_t version;
};

void setup() {
  pinMode(22, INPUT);
  pinMode(21, INPUT);
  Serial.begin(38400);
  Serial3.begin(38400, SERIAL_8N2);
}

void loop() {
  if(digitalRead(22) == 1) {
    Serial3.write(7);
    Serial3.write(0x25);
    Serial3.write(0);
    Serial3.write(0);
    Serial3.write(0);
    Serial3.write(0x2);
    Serial3.write(90);
  } else if(digitalRead(21) == 1) {
    Serial3.write(7);
    Serial3.write(0x25);
    Serial3.write(0);
    Serial3.write(0);
    Serial3.write(0);
    Serial3.write(0x2);
    Serial3.write(120);
  }
  if(Serial3.available() > 1) {
    int rec = Serial3.read();
    if(packetLen == 0) 
      packetLen = rec;
      Serial.println(rec);
    buffer[bufferIndex++] = rec;
    if(bufferIndex == packetLen) {
      bufferFull = 1;
      bufferIndex = 0;
    }
  }
  
}
