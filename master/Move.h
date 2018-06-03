#ifndef Move_h
#define Move_h

#include "Arduino.h"
#include <string.h>

class Move {
  public:
    String name;
    String moveType;
    int damage;
    Move(String name, String moveType, int dmg);
    Move();
};

#endif
