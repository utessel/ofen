#pragma once

#include "module.h"

struct LED {
    unsigned char green;
    unsigned char red;
    unsigned char blue;
    unsigned char white;
};

class LEDs: public Module
{
private:
  int fd { -1 };

  struct Brenner
  {
    enum State {
      Aus,
      Zuendet,
      BrenntA,
      BrenntB,
      BrenntC,
      BrenntD,
      Loescht,
      Anderes
    };

    State state { Aus };

    int pos { 0 };
    int offs { 0 };
    int factor { 100 };

    void Follow(int temp, LED * led, unsigned char * tga, int max);
  };

  Brenner brenner[6];

public:
  LEDs();
  ~LEDs();

  int brennerTemp[6];
  LED led[4*16 + 2*8];

  void Tick() override;
  void Rotate(Knob, int) override;
  void Press(Knob) override;
  const char * GetText() override;
  void Chosen(int) override;
};

extern LEDs leds;
