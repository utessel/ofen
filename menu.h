#pragma once

#include "module.h"
#include "input.h"

class Menu: public Module
{
private:
  int stack { 0 };

  struct StackItem
  {
     const struct MenuData * menu { nullptr };
     int current { -1 };
  };

  StackItem menuStack[8];


public:
  Menu();
  void Reset();

  void Tick() override;

  void Rotate(Knob, int) override;
  void Press(Knob) override;

  const char * GetText() override;

  void Chosen(int) override;
};
