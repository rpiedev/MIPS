#include "MipsLib.h"

int base = 7;
int arm = 6;
int claw = 5;

//MipsLab ml;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  //ml.Start();
}

void loop() {
  //ml.ElbowTo(claw, 60);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000);
  //ml.ElbowTo(claw, 90);
   digitalWrite(LED_BUILTIN, LOW);
  delay(2000);
}

