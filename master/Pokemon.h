#ifndef Pokemon_h
#define Pokemon_h

#include "Arduino.h"
#include <string.h>

class Move
{
  public:
    String name;
    String type;
    int damage;
    Move(String namep, String typep, int damagep);
    Move();
};

class Pokemon
{
    public:
        String type;
        String type2;
        String name;
        int maxHP;
        int currentHP;
        Move *move1;
        Move *move2;
        Move *move3;
        Move *move4;
        bool ko;
        Pokemon(String typep, String namep, int maxHPp);
        take(Pokemon pokemon, Pokemon opp, Move move);
};

#endif
