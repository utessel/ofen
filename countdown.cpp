#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "countdown.h"

time_t CountDown::OwnTime()
{
  struct timeval tv;

  if (gettimeofday(&tv, nullptr)) 
     return time(nullptr);

  tv.tv_sec *= speed;
  tv.tv_usec *= speed;

  int extra = tv.tv_usec / 1000000;
  return tv.tv_sec + extra;
}
// -------------------------------------------------------------

void CountDown::Start(char * msg, int diff)
{
  if (end==-1) 
  {
    end = OwnTime() + diff;
    Time(msg, diff);
  }
}
// -------------------------------------------------------------

void CountDown::Rotate(char * msg, int v)
{
  time_t t { OwnTime() };

  if (end==-1) 
     end = t + 1;

  time_t diff { end - t };

  int mod { diff % 10 };
  int f { 10 };

  if (diff > 600) { f = f*6; mod = diff % 60; }
  if (diff > 6000) { f = f*10; mod = diff % 600; }

  if (mod>0) v+=1;

  diff -= mod;
  diff += v*f;

  if (diff > 24*60*60) diff = 24*60*60;

  if (diff<10)
    end = -1;
  else
    end = t + diff;

  Time(msg, diff);
}
// -------------------------------------------------------------

bool CountDown::Tick(char * msg)
{
  if (end==-1) return false;

  time_t diff { end - OwnTime() };

  if (diff > 24*60*60)  { end = -1; }
  if (diff < -60)  { end = -1; }

  if (diff < 0)
  {
    end = -1;
    return true;
  }

  Time(msg, diff);
  return false;
}
// -------------------------------------------------------------

void CountDown::Time(char * msg, int diff)
{
  if (msg == nullptr) return; 

  if (end<0)
  {
    sprintf(msg, "--m --s");
    return;
  }

  if (diff > 1800) 
  {
    int sub = diff / 60;
    int hour = sub / 60;
    int min = sub % 60;

    if (hour>0)
      sprintf(msg, "%2dh %02dm", hour, min);
    else
      sprintf(msg, "    %02dm", min);

  } else
  {
    int min = diff / 60;
    int sec = diff % 60;
    sprintf(msg, "%2dm %02ds", min, sec);
  }
}
// -------------------------------------------------------------

