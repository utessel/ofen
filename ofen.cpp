#include <stdio.h>
#include <stdlib.h>
#include "ofen.h"
#include "leds.h"

Ofen ofen;
// -------------------------------------------------------------

Ofen::Ofen()
: counter(4)
{
  msg[0] = 0;
}
// -------------------------------------------------------------

void Ofen::Rotate(Knob k, int v)
{
  if (k==Knob::Left)
  {
    deg = deg + v*10;
    if (deg<30) deg=30;
    if (deg>250) deg=250;

    Deg();
  } else
  {
    counter.Rotate(msg, v);
  }
  leds.brennerTemp[4] = deg;
  leds.brennerTemp[5] = deg;
}
// -------------------------------------------------------------

void Ofen::Deg()
{
  sprintf(msg, "  %3d`C", deg);
}                
// -------------------------------------------------------------

void Ofen::Press(Knob k)
{
  active = nullptr;
  if (counter.end == -1)
  {
    leds.brennerTemp[4] = 0;
    leds.brennerTemp[5] = 0;
  }
}
// -------------------------------------------------------------

void Ofen::Tick()
{
  char * m { msg };

  if (msg[6]=='C') m = nullptr;

  if (counter.Tick(m))
  {
    if (active != this)
    {
      system("ogg123 -q fertig.ogg &");
      Message("BACKOFEN");
      leds.brennerTemp[4] = 0;
      leds.brennerTemp[5] = 0;
    }
  }
}
// -------------------------------------------------------------

const char * Ofen::GetText()
{
  return msg;
}
// -------------------------------------------------------------

void Ofen::Chosen(int code)
{
  if (code==0) 
  {
    if (counter.end>-1)
    {
       active = this;
    }
    return;
  } else
  {
    counter.Start(msg, (code/1000)*60 );
    deg = code%1000;
    leds.brennerTemp[4] = deg;
    leds.brennerTemp[5] = deg;
  }
}
// -------------------------------------------------------------
