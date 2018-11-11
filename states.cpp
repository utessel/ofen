#include "input.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <fcntl.h>
#include <linux/input.h>
#include "display.h"

extern Display display;

const int InactiveTimeout = 300;

const char * knobName[3] = {
  "Unkn", "Left", "Right"
};

enum class State 
{
    Nothing,
    Rotate,
    Pressed,
    Released,
    PressHold,
    ReleaseHold,
    Max
};

const unsigned int StateCount { static_cast<unsigned int>(State::Max) };

const char * stateName[StateCount] = {
  "Nothing",
  "Rotate",
  "Pressed",
  "Released",
  "PressHold",
  "ReleaseHold"
};


struct Action {
  enum Value {
    Nop,
    CountInactive = 0x01,
    Press = 0x02,
    Rotate = 0x04,
    ForgetKnob = 0x08,
    EndInactive = 0x10
  };
};

struct Transition
{
  State newState;
  int action;
};

const unsigned int EventCount { static_cast<unsigned int>(Event::Max) };

const Transition transitions[StateCount][EventCount] = {
/* Nothing */ {
/*  Tick */     { State::Nothing, Action::CountInactive },
/*  Press */    { State::Pressed, Action::EndInactive },
/*  Release */  { State::Nothing, Action::ForgetKnob },
/*  Rotate */   { State::Rotate, Action::Rotate | Action::EndInactive },
},
/* Rotate */ {
/*  Tick */     { State::Nothing, Action::ForgetKnob },
/*  Press */    { State::Rotate, Action::Nop },
/*  Release */  { State::Rotate, Action::Nop },
/*  Rotate */   { State::Rotate, Action::Rotate },
},
/* Pressed */ {
/*  Tick */     { State::PressHold, Action::Nop },
/*  Press */    { State::Pressed, Action::Nop },
/*  Release */  { State::Released, Action::Nop },
/*  Rotate */   { State::Pressed, Action::Nop },
},
/* Released */ {
/*  Tick */     { State::ReleaseHold, Action::Press },
/*  Press */    { State::Pressed, Action::Nop },
/*  Release */  { State::Released, Action::Nop },
/*  Rotate */   { State::Released, Action::Nop },
},
/* PressHold */ {
/*  Tick */     { State::PressHold, Action::Nop },
/*  Press */    { State::PressHold, Action::Nop },
/*  Release */  { State::ReleaseHold, Action::Press },
/*  Rotate */   { State::PressHold, Action::Nop },
},
/* ReleaseHold */ {
/*  Tick */     { State::Nothing, Action::ForgetKnob },
/*  Press */    { State::ReleaseHold, Action::Nop },
/*  Release */  { State::ReleaseHold, Action::Nop },
/*  Rotate */   { State::ReleaseHold, Action::Nop },
}};

static State state { State::Nothing };
static Knob lastKnob { Knob::Unknown };
int inactive { 501 };

void EndInactive()
{
  inactive = 0;
  display.SetBrightness(8);
}

void CountInactive()
{
  inactive++;
  if (inactive < InactiveTimeout) return;

  if ((inactive == InactiveTimeout) && 
      (Module::active!=nullptr) &&
      (Module::active->Inactive()))
    Module::active = nullptr;

  int dim = InactiveTimeout - inactive + 10;

  if (dim < 0) 
  {
     inactive = InactiveTimeout + 10;
     return;
  }

  if (dim > 8) return;

  display.SetBrightness(dim);
}

void ChangeState( Event ev, Knob k, int v)
{
  if (lastKnob == Knob::Unknown) lastKnob = k;

  if ((lastKnob != k) &&
      (k != Knob::Unknown)) return;

  unsigned int evIdx { static_cast<unsigned int>(ev) };
  unsigned int stateIdx { static_cast<unsigned int>(state) };

  const Transition & t = transitions[stateIdx][evIdx];

  state = t.newState;

  printf("%3d %6s %s     \r", inactive, 
     knobName[static_cast<unsigned int>(lastKnob)],
     stateName[static_cast<unsigned int>(state)]
     ); 
  fflush(stdout);

  if (Module::active) 
  {
    if (t.action & Action::Press)
       Module::active->Press(lastKnob);

    if (t.action & Action::Rotate)
       Module::active->Rotate(lastKnob, v);
  }
  
  if (t.action & Action::EndInactive)
    EndInactive();

  if (t.action & Action::CountInactive)
    CountInactive();


  if (t.action & Action::ForgetKnob)
    lastKnob = Knob::Unknown;
}
