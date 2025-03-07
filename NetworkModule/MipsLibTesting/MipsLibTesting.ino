#include "MipsLib.h"

int base = 7;
int arm = 6;
int claw = 5;

MipsLab ml;

void setup() {
  ml.Start();
}

void loop() {
  ml.ElbowTo(claw, 60);
  delay(1000);
  ml.ElbowTo(claw, 90);
  delay(1000);
}

