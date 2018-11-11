#pragma once

#include <time.h>

struct CountDown 
{
  time_t end { -1 };
  int speed;

  CountDown(int s = 4): speed(s) {}

  time_t OwnTime();

  void Time(char * msg, int);

  void Start(char * msg, int);
  void Rotate(char * msg, int);
  bool Tick(char * msg);
};

