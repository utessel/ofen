#pragma once

#include <sys/select.h>

#include "module.h"

class InputDev
{
private:
  int fd { -1 };
  Knob knob;

public:
  InputDev(const char * name, Knob k);
  ~InputDev();

  void DoRead();

  void FdSet(int & max, fd_set & fds);
  void Check( const fd_set & fds );

  void Parse(struct input_event & ev);

  void Syn(int c, int v);
  void Rel(int c, int v);
  void Key(int c, int v);
};
