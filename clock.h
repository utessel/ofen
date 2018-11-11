#pragma once

#include "module.h"

class Clock: public Module
{
private:
  char time[9];

public:
  Clock();
  void Tick() override;

  void Rotate(Knob, int) override;
  void Press(Knob) override;
  const char * GetText() override;
  void Chosen(int) override;

  static void GetTime(int & day, int & hour, int & min);
};
