int flag = 1;
int packetLen = 0;

struct Module {
  uint32_t address;
  uint16_t type;
  uint16_t version;
};
Module mod1 = { 1, 3, 1 };
Module mod2 = { 2, 2, 1 };
Module mod3 = { 3, 3, 1 };

String in;

unsigned char command[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int commandLen = 0;

Module modules[10];
uint8_t modulesLen;

void setup() {
  pinMode(22, INPUT);
  pinMode(21, INPUT);
  pinMode(20, INPUT);
  pinMode(19, INPUT);
  pinMode(18, INPUT);
  pinMode(17, INPUT);
  Serial.begin(38400);
  Serial3.begin(38400, SERIAL_8N2);

  modules[0] = mod1;
  modules[1] = mod3;
  modules[2] = mod2;
  modulesLen = 3;
}

void sendACT(uint32_t address, String comm) {
  uint8_t *b = (uint8_t *)&address;
  Serial3.write(7);
  Serial3.write(0x25);
  Serial3.write(b[3]);
  Serial3.write(b[2]);
  Serial3.write(b[1]);
  Serial3.write(b[0]);
  int type = 0;
  int num = comm.toInt();
  uint8_t *ang = (uint8_t *)&num;
  for (int i = 0; i < modulesLen; i++) {
    if (modules[i].address == address)
      type = modules[i].type;
  }

  switch (type) {
    case 1:
      if (comm == "on")
        Serial3.write(1);
      else
        Serial3.write(2);
      break;
    case 2:
    case 3:
      Serial3.write(ang[0]);
      break;
    default:
      break;
  }
}

void loop() {
  if (Serial3.available() > 1) {
    int rec = Serial3.read();
    Serial.println(rec);
  }
  if(digitalRead(22) == 1) {
    sendACT(2, 250);
    delay(5);
  } else if(digitalRead(21) == 1) {
    sendACT(2, 240);
    delay(5);
  } else if(digitalRead(20) == 1) {
    sendACT(1, 250);
    delay(5);
  } else if(digitalRead(19) == 1) {
    sendACT(1, 240);
    delay(5);
  } else if(digitalRead(18) == 1) {
    sendACT(3, 250);
    delay(5);
  } else if(digitalRead(17) == 1) {
    sendACT(3, 240);
    delay(5);
  }
  if (Serial.available() > 1) {
    in = Serial.readString();
    in = in.substring(0, in.length() - 1);
    int spacePos = in.indexOf(' ');
    sendACT(in.substring(0, spacePos).toInt(), in.substring(spacePos + 1, in.length()));
  }
}
