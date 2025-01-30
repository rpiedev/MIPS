#include "MipsLib.h"

int base = 6;
int arm = 7;
int claw = 5;

MipsLab ml;

void setup() {
ml.Start();
ml.ServoTo(arm,90);
ml.ControlStart();

}

void loop() {
  ml.ControlServoUp("1", arm);
  ml.ControlServoUp("2", base);
  ml.ControlServoUp("3", claw);
  ml.ControlServoDown("4", arm);
  ml.ControlServoDown("5", base);
  ml.ControlServoDown("6", claw);
  ml.ControlLoop();
//  ml.ControlServoUp("1", arm);
}