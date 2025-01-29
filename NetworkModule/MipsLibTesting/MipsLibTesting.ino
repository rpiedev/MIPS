#include "MipsLib.h"

MipsLab ml;

void setup() {
  ml.MipsStart();
  ml.ControlStart();
}

void loop() {
  ml.ControlLoop();
}