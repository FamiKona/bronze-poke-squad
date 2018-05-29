#ifndef Pokemon_h
#define Pokemon_h

#include "Arduino.h"
#include <string.h>
#include "Move.h"

class Pokemon
{
    public:
        String type;
        String type2;
        String name;
        int maxHP;
        int currentHP;
        Move move1;
        Move move2;
        Move move3;
        Move move4;
        bool ko;
        Pokemon(String, String, int);
        void take(String, Move, int);
        void setMove(int, String, String, int);
};

#endif
