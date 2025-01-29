//#include "S:\bot\MIPS\MIPSlab\MipsLib"
//#include "MipsLib.h"

int base = 6;
int arm = 7;
int claw = 5;

MipsLab ml;

void setup() {
  ml.Start();
  //ml.ControlStart();
  //ml.ControlServoUp("9", claw);
}

void loop() {
  //ml.ControlLoop();
  ml.ServoUp(claw);
  delay(1000);
  ml.ServoDown(claw);
  delay(1000);
  ml.ServoTo(base, 70);
  delay(1000);
  ml.ServoTo(base, 90);
}