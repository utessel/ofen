#include <stdio.h>
#include <unistd.h>

#include <sys/time.h>
#include <time.h>
#include <string.h>

#include "clock.h"
#include "menu.h"

extern Menu menu;

extern int inactive;
static char message[9];

Clock uhr;
// -------------------------------------------------------------

Clock::Clock()
{
  time[0] = 0;
}
// -------------------------------------------------------------

const char * Day[8] = {
  "SO", "MO", "DI", "MI", "DO", "FR", "SA", "--"
};

void ClockTime( char * buf, int day, int hour, int min )
{
  if (day<0) day = 0;
  if (hour<0) hour = 0;
  if (min<0) min = 0;

  sprintf(buf, "%s %02d %02d",
    Day[day],
    hour,
    min
  );
}
// -------------------------------------------------------------

void Message(const char * msg)
{
  if (Module::active == nullptr)
    Module::active = &uhr;

  if (Module::active == &uhr) 
  {
    if (inactive > 0) inactive = 0;
    strncpy( message, msg, sizeof(message)-1 );
    message[8] = 0;
  }
}
// -------------------------------------------------------------

void Clock::Tick()
{
  if ((message[0]!=0) && 
      (inactive > 120)) message[0]=0;

  int day,hour,min;

  GetTime(day,hour,min);

  ClockTime(time, day, hour, min );

  if (active==nullptr) 
    active = this;
}
// -------------------------------------------------------------

void Clock::GetTime(int & day, int & hour, int & min)
{
  struct timeval tv;
  struct tm* ptm;

  gettimeofday (&tv, NULL);
  ptm = localtime(&tv.tv_sec);

  day = ptm->tm_wday;
  hour = ptm->tm_hour;
  min = ptm->tm_min;
}
// -------------------------------------------------------------

void Clock::Rotate(Knob, int)
{
  active = &menu;
  message[0] = 0;
  active->Chosen(0);
}
// -------------------------------------------------------------

void Clock::Press(Knob)
{
  active = &menu;
  message[0] = 0;
  active->Chosen(1);
}
// -------------------------------------------------------------

void Clock::Chosen(int)
{
}
// -------------------------------------------------------------

const char * Clock::GetText()
{
  if (message[0] != 0) return message;
  return time;
}
// -------------------------------------------------------------
