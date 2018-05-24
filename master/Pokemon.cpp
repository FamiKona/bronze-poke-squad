#include "Arduino.h"
#include "Pokemon.h"

using namespace std;

Move::Move(String namep, String typep, int damagep){
  name = namep;
  type = typep;
  damage = damagep;
}

Move::Move(){

}


Pokemon::Pokemon(String typep, String namep, int maxHPp) {
    type = typep;
    name = namep;
    ko = false;
    maxHP = maxHPp;
    currentHP = maxHP;
    this->move1 = new Move("Tackle", "Normal", 40);
    this->move2 = new Move();
    this->move3 = new Move();
    this->move4 = new Move();
}

Pokemon::take(Pokemon opp, Move move) {
  int dmg = move.damage;
  //MODIFY DAMAGE BASED ON TYPING
  if (opp.ko) {
    dmg = 0;
  }
  this->currentHP = this->currentHP - dmg;
  if (this->currentHP <= 0) {
    currentHP = 0;
    ko = true;
  }
}
