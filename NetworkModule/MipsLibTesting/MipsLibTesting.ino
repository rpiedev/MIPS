#include "MipsLib.h"

int base = 7;
int arm = 9;
int claw = 5;

MipsLab ml;

void setup() {
  ml.Start();
}

void loop() {
  ml.ElbowTo(claw, 100);
  delay(500);
  ml.ElbowTo(claw, 80);
  delay(500);
}

