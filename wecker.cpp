#include <stdio.h>
#include <stdlib.h>
#include "wecker.h"
#include "clock.h"

Wecker wecker;
extern void ClockTime( char * buf, int day, int hour, int min );
extern int inactive;
// -------------------------------------------------------------


void Wecker::Rotate(Knob k, int v)
{
  switch (k)
  {
  case Knob::Left:
  {
    change += v;
    if (change<0) change = 0;
    if (change>2) change = 2;

    Show();
    msg[change*3]='_';
    msg[change*3+1]='_'; 
    return;
  }
  case Knob::Right:
  {
    Wake & w = wecker[chosen];
    w.happened = true;
    switch (change)
    {
    case 0:
       w.day += v;
       if (w.day<0) w.day=0;
       if (w.day>7) w.day=7;
       break;    
    case 1:
       w.hour += v;
       if (w.hour<0) w.hour=0;
       if (w.hour>23) w.hour=23;
       break;    
    case 2:
       w.min += v;
       if (w.min<0) w.min=0;
       if (w.min>59) w.min=59;
       break;    
    }
    Show();
    return;
  }
  default:;
  };
}
// -------------------------------------------------------------

void Wecker::Press(Knob k)
{
  if (change < 2) 
    change++;
  else
  {
    active = nullptr;
  }
}
// -------------------------------------------------------------

void Wecker::Tick()
{
  int day, hour, min;
  Clock::GetTime(day,hour,min);
  for (auto & w: wecker)
  {
    if (((w.day!=day) && (w.day!=7)) ||
        (w.hour != hour) ||
        (w.min != min))
        {
          w.happened = false;
          continue;
        }
    if (w.happened) continue;

    w.happened = true;
    Message("WECKER");

    if ((hour < 4) || (hour > 12))
      system("ogg123 -q fertig.ogg &");
    else
      system("ogg123 -q day.ogg &");
  }

  if (active != this) return;
  
  Show();
  if ((inactive % 10)>5) 
  { 
     msg[change*3]='_';
     msg[change*3+1]='_'; 
  }
}
// -------------------------------------------------------------

void Wecker::Show()
{
  Wake & w = wecker[chosen];
  
  ClockTime( msg, w.day, w.hour, w.min );
}
// -------------------------------------------------------------

const char * Wecker::GetText()
{
  return msg;
}
// -------------------------------------------------------------

void Wecker::Chosen(int code)
{
  chosen = code;
  change = 0;

  Show();
}
// -------------------------------------------------------------
