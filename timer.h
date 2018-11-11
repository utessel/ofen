#pragma once

#include "module.h"
#include "countdown.h"

class Timer: public Module
{
private:
  char msg[9];

  CountDown counter;

public:
  Timer();

  void Tick() override;
  void Rotate(Knob, int) override;
  void Press(Knob) override;
  const char * GetText() override;
  void Chosen(int) override;

  bool Inactive() override;
};
