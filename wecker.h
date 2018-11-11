#pragma once

#include "module.h"

class Wecker: public Module
{
private:
  char msg[9];

  int chosen;
  int change;

  struct Wake {
    int day { -1 };
    int hour { -1 };
    int min { 0 };
    bool happened { true };
  };

  Wake wecker[8];

  void Show();

public:
  void Tick() override;
  void Rotate(Knob, int) override;
  void Press(Knob) override;
  const char * GetText() override;
  void Chosen(int) override;
};
