#pragma once

#include "module.h"
#include "countdown.h"

class Ofen: public Module
{
private:
  char msg[9];

  int deg { 100 };
  CountDown counter;

  void Deg();
public:
  Ofen();
  void Tick() override;
  void Rotate(Knob, int) override;
  void Press(Knob) override;
  const char * GetText() override;
  void Chosen(int) override;
};
