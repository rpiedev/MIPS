#include "MipsLib.h"

int base = 7;
int arm = 6;
int claw = 5;

MipsLab ml;

void setup() {
  ml.Start();
  ml.ControlStart();

  ml.ControlElbowDown("1", base);
  ml.ControlElbowUp("3", base);
  
  ml.ControlElbowDown("4", arm);
  ml.ControlElbowUp("6", arm);
  
  ml.ControlElbowDown("7", claw);
  ml.ControlElbowUp("9", claw);
}

void loop() {
  ml.ControlLoop();
}