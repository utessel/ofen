#include "input.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <fcntl.h>
#include <linux/input.h>

void InputDev::FdSet(int & max, fd_set & fds)
{
   if (fd==-1) return;

   if (fd>=max) max = fd+1;
   FD_SET( fd, &fds );
}

void InputDev::Check( const fd_set & fds )
{
   if (fd==-1) return;

   if (FD_ISSET(fd, &fds))
     DoRead();
}

InputDev::InputDev(const char * name, Knob k)
: knob(k)
{
  fd = open(name, O_RDONLY);
  if (fd<0) { perror("open"); return; }
}

InputDev::~InputDev()
{
  if (fd>=0) close(fd);
  fd = -1;
}

void InputDev::DoRead()
{
  input_event buffer[16];
  int bytes = read( fd, &buffer, sizeof(buffer));

  int idx = 0;
  while (bytes>0)
  {
    Parse(buffer[idx]);
    bytes -= sizeof(buffer[idx]);
    idx++;
  }
}

void InputDev::Parse(input_event & ev)
{
  int c = ev.code;
  int v = ev.value;
  switch (ev.type)
  {
  case EV_SYN: Syn(c,v); break;
  case EV_KEY: Key(c,v); break;
  case EV_REL: Rel(c,v); break;
  case EV_ABS: printf("ABS %d %d\n", c, v); break;
  case EV_MSC: printf("MSC %d %d\n", c, v); break;
  case EV_SW : printf("SW  %d %d\n", c, v); break;
  case EV_LED: printf("LED %d %d\n", c, v); break;
  case EV_SND: printf("SND %d %d\n", c, v); break;
  case EV_REP: printf("REP %d %d\n", c, v); break;
  case EV_FF : printf("FF  %d %d\n", c, v); break;
  case EV_PWR: printf("PWR %d %d\n", c, v); break;
  default:                   
    printf("typ %d %d %d\n", ev.type, c, v);
  }
}

void InputDev::Syn(int,int)
{
}

void InputDev::Key(int,int v)
{
  if (v)
    ChangeState( Event::Press, knob, 0 );
  else
    ChangeState( Event::Release, knob, 0 );
}

void InputDev::Rel(int,int v)
{
  ChangeState( Event::Rotate, knob, v );
}
