#include <stdio.h>
#include <unistd.h>

#include <sys/select.h>
#include <fcntl.h>
#include <linux/input.h>
#include <sys/time.h>
#include <time.h>

#include "display.h"
#include "input.h"
#include "module.h"

Display display;

Module * Module::first { nullptr };
Module * Module::last { nullptr };

Module * Module::active { nullptr };

Module::Module()
{
  if (last)
  {
     prev = last;
     last->next = this;
  } else
  {
     first = this;
  }
  last = this;
}

Module::~Module()
{
  if (next)
     next->prev = prev;
  else
     last = next->prev;

  if (prev)
     prev->next = next;
  else
     first = prev->next;
}

static unsigned int lastTick;

static unsigned int GetTick()
{
  struct timeval tv;
  gettimeofday(&tv, nullptr);

  unsigned int result = tv.tv_usec;
  result /= 1000;
  result += tv.tv_sec * 1000;

  return result;
}


void Tick()
{

  Module * it = Module::first;
  while (it)
  {
     it->Tick();
     it = it->next;
  }

  unsigned int tick = GetTick();
  int diff = tick - lastTick;

  if ((diff<0) || (diff>200)) lastTick = tick;

  if (diff>=100)
  {
    lastTick += 100;
    ChangeState( Event::Tick, Knob::Unknown, 0 );
  }
}

int main()
{
  display.SetBrightness(8);

  display.ShowText("");

  InputDev * idev[4];

  idev[0] = new InputDev("/dev/input/by-path/platform-button@10-event", Knob::Right);
  idev[1] = new InputDev("/dev/input/by-path/platform-rotary@1a-event", Knob::Right);
  idev[2] = new InputDev("/dev/input/by-path/platform-button@6-event", Knob::Left);
  idev[3] = new InputDev("/dev/input/by-path/platform-rotary@5-event", Knob::Left);

  lastTick = GetTick();

  for (;;)
  {
     int maxfd = -1;
     struct timeval tv;
     fd_set rfds;
     FD_ZERO(&rfds);

     tv.tv_sec = 0;
     tv.tv_usec = 20*1000;

     fflush(stdout);
     fflush(stderr);

     for (auto dev: idev) dev->FdSet(maxfd, rfds );

     int ret = select(maxfd, &rfds, NULL, NULL, &tv);

     if (ret==-1)
     {
        perror("select");
        break;
     } else if (ret==0)
     {
       Tick();
     } else
     {
       for (auto dev: idev) dev->Check(rfds);
     }

     if (Module::active)
        display.ShowText( Module::active->GetText() );
  }

  for (auto &dev: idev) { delete dev; dev=nullptr; }

  return 0;
}
