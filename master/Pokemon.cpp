#include "Arduino.h"
#include "Pokemon.h"
#include "Move.h"

using namespace std;

Pokemon::Pokemon(String typep, String namep, int maxHPp) {
    type = typep;
    name = namep;
    ko = false;
    maxHP = maxHPp;
    currentHP = maxHP;
    move1 = Move("Tackle", "Normal", 40);
    move2 = Move("Tackle", "Normal", 40);
    move3 = Move("Tackle", "Normal", 40);
    move4 = Move("Tackle", "Normal", 40);
}

void Pokemon::take(String oppName, Move move) {
  int dmg = move.damage;
  //MODIFY DAMAGE BASED ON TYPING
  this->currentHP = this->currentHP - dmg;
  Serial.println();
  Serial.print(oppName);
  Serial.print(" dealt ");
  Serial.print(dmg);
  Serial.println(" damage!");
  Serial.println();
  
  if (this->currentHP <= 0) {
    Serial.print(this->name);
    Serial.println(" perished!");
    this->currentHP = 0;
    this->ko = true;
  }
}

void Pokemon::setMove(int moveNo, String name, String type, int dmg) {
  if (moveNo == 1) {
    this->move1 = Move(name, type, dmg);
  } else if (moveNo == 2) {
    this->move2 = Move(name, type, dmg);
  } else if (moveNo == 3) {
    this->move3 = Move(name, type, dmg);
  } else if (moveNo == 4) {
    this->move4 = Move(name, type, dmg);
  }
}

