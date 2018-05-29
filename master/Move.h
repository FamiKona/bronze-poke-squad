#ifndef Move_h
#define Move_h

#include "Arduino.h"
#include <string.h>

class Move
{
  public:
    String name;
    String type;
    int damage;
    Move(String name, String type, int dmg);
    Move();
};

#endif
