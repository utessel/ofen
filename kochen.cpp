#include <stdio.h>
#include <stdlib.h>
#include "kochen.h"
#include "leds.h"

Kochen kochen;
// -------------------------------------------------------------

Kochen::Kochen()
{
  msg[0] = 0;
}
// -------------------------------------------------------------

void Kochen::Rotate(Knob k, int v)
{
  if (chosen<0) return;
  int & deg = degs[chosen];

  if (k==Knob::Left)
  {
    deg = deg + v*10;
    if (deg<20) deg=20;
    if (deg>100) deg=100;

    Deg(deg);

    leds.brennerTemp[chosen] = deg;
  } else
  {
    counter[chosen].Rotate(msg, v);
  }
}
// -------------------------------------------------------------

void Kochen::Deg(int deg)
{
  sprintf(msg, "  %3d`C", deg);
}                
// -------------------------------------------------------------

void Kochen::Press(Knob k)
{
  if (chosen<0) return;
  if (counter[chosen].end==-1)
    leds.brennerTemp[chosen] = 0;
  active = nullptr;
  chosen = -1;
}
// -------------------------------------------------------------

void Kochen::Tick()
{
  for (int i=0; i<4; i++)
  {
    char * m = nullptr;
    if ((i==chosen) && (msg[6]!='C')) m = msg;

    if (counter[i].Tick(m))
    {
      if (active != this)
      {
         sprintf(msg, "%d FERTIG", i+1);
         Message(msg);
      }
      if (i != chosen)
      {
        system("ogg123 -q fertig.ogg &");
        leds.brennerTemp[i] = 0;
      }
    }
  }
}
// -------------------------------------------------------------

const char * Kochen::GetText()
{
  return msg;
}
// -------------------------------------------------------------

void Kochen::Chosen(int code)
{
  if ((code>=1) && 
      (code<=4)) 
  {
    chosen = code-1;

    if (counter[chosen].end>-1)
    {
      active = this;
    }
    return;
  }

  if (chosen < 0) return;

  if (code==0) 
  {
    if (counter[chosen].end==-1)
    {
       counter[chosen].Start(msg, 300);
    }
  } else
  {
    counter[chosen].Start(msg, (code/1000)*60 );
    degs[chosen] = code%1000;
  }

  leds.brennerTemp[chosen] = degs[chosen];
}
// -------------------------------------------------------------
