#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "wecker.h"
#include "timer.h"
#include "ofen.h"
#include "kochen.h"
#include "leds.h"

extern Timer timer;
extern Wecker wecker;
extern Ofen ofen;
extern Kochen kochen;
extern LEDs leds;

extern int inactive;

const char * commands[] = 
{
 "ogg123 -q magic.ogg &",
 "ogg123 -q day.ogg &",
 "mpc play &",
 "mpc pause &",
 "mpc next &",
 "mpc prev &",
};

#define ARRAY_LENGTH(p) sizeof(p)/sizeof(p[0])

void ExecuteCommand(unsigned int id)
{
  if (id < 1) return;
  if (id > ARRAY_LENGTH(commands)) return;
  system(commands[id-1]);
}

struct MenuItem
{
  const char * title;
  const struct MenuData * sub;
  Module * module;
  int info;
};

struct MenuData
{
  int count;
  const MenuItem * items;
};
// -------------------------------------------------------------

#define MENU( name, list... ) \
const MenuItem name##Items[] = list; \
const MenuData name = { \
  sizeof(name##Items)/sizeof(name##Items[0]), \
  name##Items \
};
// -------------------------------------------------------------

MENU( Sauce, {
  { "TOMATEN ", nullptr, &kochen, 10100 },
  { " SAHNE  ", nullptr, &kochen, 10080 },
})

MENU( KochMenu, {
  { " WASSER ", nullptr, &kochen,  5100 },
  { "KARTOFFELN", nullptr, &kochen, 20100 },
  { " NUDELN ", nullptr, &kochen, 15100 },
  { " SUPPE  ", nullptr, &kochen,  5090 },
  { " SAUCE  ", &Sauce, nullptr, 0 },
})

MENU( Kochfelder, {
  { " FELD 1 ", &KochMenu, &kochen, 1 },
  { " FELD 2 ", &KochMenu, &kochen, 2 },
  { " FELD 3 ", &KochMenu, &kochen, 3 },
  { " FELD 4 ", &KochMenu, &kochen, 4 },
})

MENU( Rezepte, {
  { "  BROT  ", nullptr, &ofen, 10200 },
  { " KEKSE  ", nullptr, &ofen,  5160 },
  { " KUCHEN ", nullptr, &ofen, 10160 },
  { " PIZZA  ", nullptr, &ofen, 10180 },
  { " BRATEN ", nullptr, &ofen, 30150 },
  { "HAHNCHEN", nullptr, &ofen, 30180 },
  { "LASAGNE ", nullptr, &ofen, 20150 },
  { "CUPCAKES", nullptr, &ofen, 10160 },
})

MENU( Backen, {
  { " REZEPTE", &Rezepte, nullptr, 0 },
  { "   OBER-HITZE ", nullptr , &ofen, 160 },
  { "  UNTER-HITZE ", nullptr , &ofen, 160 },
  { "  HEISS-LUFT ", nullptr , &ofen, 160 },
  { " GRILLEN", nullptr, &ofen, 200 },
})

MENU( Musik, {
  { " MAGIC  ", nullptr, nullptr, 1 },
  { "  DAY   ", nullptr, nullptr, 2 },
#if 0
  { "PLAY", nullptr, nullptr, 3 },
  { "PAUSE", nullptr, nullptr, 4 },
  { "WEITER", nullptr, nullptr, 5 }, 
  { "VORHER", nullptr, nullptr, 6 }
#endif
#if 0
  { " LED 1  ", nullptr, &leds, 1 },
  { " LED 2  ", nullptr, &leds, 2 },
  { " LED 3  ", nullptr, &leds, 3 },
  { " LED 4  ", nullptr, &leds, 4 },
  { " LED 5  ", nullptr, &leds, 5 },
  { " LED 6  ", nullptr, &leds, 6 },
  { " LED 7  ", nullptr, &leds, 7 },
  { " LED 8  ", nullptr, &leds, 8 },
#endif
})

MENU( WeckerMenu, {
  { "WECKER 1", nullptr, &wecker, 0 },
  { "WECKER 2", nullptr, &wecker, 1 },
  { "WECKER 3", nullptr, &wecker, 2 },
  { "WECKER 4", nullptr, &wecker, 3 },
})

MENU( Main, {
  { " BACKEN ", &Backen,     &ofen, 0 },
  { " KOCHEN ", &Kochfelder, nullptr, 0 },
  { " TIMER  ", nullptr,     &timer,  0 },
  { " WECKER ", &WeckerMenu, nullptr, 0 },
  { " MUSIK  ", &Musik,      nullptr, 0 },
})
// -------------------------------------------------------------

Menu menu;
// -------------------------------------------------------------

Menu::Menu()
{
}
// -------------------------------------------------------------

void Menu::Reset()
{
  for (auto & item: menuStack) 
  { 
    item.menu = nullptr; 
    item.current = 0; 
  }
  menuStack[0].menu = &Main;

  stack = 0;
}
// -------------------------------------------------------------

void Menu::Rotate(Knob k, int v)
{
  if (stack<0) Reset();

  StackItem & top = menuStack[stack];

  if (top.menu == nullptr) return;

  top.current = top.current + v;

  int max = top.menu->count;

  if (top.current < 0) top.current = 0;
  if (top.current > max) top.current = max;

}
// -------------------------------------------------------------

void Menu::Press(Knob k)
{

  if (stack<0) Reset();

  StackItem & top = menuStack[stack];

  if (top.menu == nullptr) return;

  int max = top.menu->count;

  if (top.current<0) return;
  if (top.current>=max) 
  { 
    stack--; 
    top.current = 0; 
    top.menu = nullptr;
    if (stack<0) active=nullptr; 
    return; 
  }

  const MenuItem & item = top.menu->items[top.current];

  if (item.sub == nullptr)
  {
    Reset();
    active = item.module;
    if (active==nullptr)
    {
      ExecuteCommand(item.info);
    }
  }

  if (item.module)
  {
    item.module->Chosen(item.info);
  }

  if (active != this) return;

  stack ++;
  menuStack[stack].menu = item.sub;
  menuStack[stack].current = 0;
}
// -------------------------------------------------------------

void Menu::Chosen(int)
{
  Reset();
}
// -------------------------------------------------------------

void Menu::Tick()
{
}
// -------------------------------------------------------------

const char * Menu::GetText()
{
  if (stack<0) return nullptr;

  StackItem & top = menuStack[stack];

  if (top.menu == nullptr) return "WIESO";

  int & current = top.current;

  int max = top.menu->count;
  if (current<0) return nullptr;
  if (current>=max) return " ZURUCK ";

  const char * text = top.menu->items[current].title;

  if (inactive<30) return text;

  int len = strlen(text);
  if (len<=8) return text;

  len -= 8;
  int p = inactive - 30;
 
  p/=4;

  p %= (2*len);

  if (p>=len) p=2*len - p;

  return &text[p];
}
// -------------------------------------------------------------

