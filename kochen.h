#pragma once

#include "module.h"
#include "countdown.h"

class Kochen: public Module
{
private:
  char msg[9];

  int chosen { 0 };
  int degs[4] { 100, 100, 100, 100 };
  CountDown counter[4];

  void Deg(int deg);

public:
  Kochen();
  void Tick() override;
  void Rotate(Knob, int) override;
  void Press(Knob) override;
  const char * GetText() override;
  void Chosen(int) override;
};
