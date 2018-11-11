#pragma once

enum class Knob { Unknown, Left, Right };
enum class Event {
    Tick,
    Press,
    Release,
    Rotate,
    Max
  };

extern void ChangeState( Event, Knob, int );

extern void Message( const char * msg );

class Module
{
private:
  friend void Tick();
  static Module * first;
  static Module * last;
  Module * prev { nullptr };
  Module * next { nullptr };

public:
  Module();
  virtual ~Module();
  
  virtual void Tick() = 0;
  virtual void Rotate(Knob,int) = 0;
  virtual void Press(Knob) = 0;
  virtual const char * GetText() = 0;
  virtual void Chosen(int) = 0;

  virtual bool Inactive() { return true; };

  static Module * active;
};
