#include "MipsLib.h"

int base = 6;
int arm = 7;
int claw = 5;

MipsLab ml;
int startTime;

void setup() {
  ml.Start();
  ml.ControlStart();
  ml.ControlElbowUp("7", claw);
  ml.ControlElbowDown("9", claw);
  ml.ControlElbowUp("4", arm);
  ml.ControlElbowDown("5", arm);
  startTime = millis();
}

void loop() {
  if((millis() - startTime) > 10000) {
    Serial3.write(0x2);
    Serial3.write(0x9);
    startTime = millis();
  }
  ml.ControlLoop();
}