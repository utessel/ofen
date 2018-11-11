#include <stdio.h>
#include "timer.h"
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>

Timer timer;

extern int inactive;
// -------------------------------------------------------------

Timer::Timer()
: counter(1)
{
}
// -------------------------------------------------------------

void Timer::Rotate(Knob k, int v)
{
  counter.Rotate(msg, v);
}
// -------------------------------------------------------------

void Timer::Press(Knob k)
{
  active = nullptr;
}
// -------------------------------------------------------------

void Timer::Tick()
{
  if (counter.Tick(msg))
  {
    system("ogg123 -q fertig.ogg &");
    
    if ((active==this) && (inactive>20)) active = nullptr;
 
    if (active != this)
    {
      Message("ZEIT RUM");
    }
  }
}
// -------------------------------------------------------------

const char * Timer::GetText()
{
  return msg;
}
// -------------------------------------------------------------

void Timer::Chosen(int code)
{
  counter.Start(msg, 300);
}
// -------------------------------------------------------------

bool Timer::Inactive()
{
  return (counter.end == -1);
}
// -------------------------------------------------------------
