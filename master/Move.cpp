#include "Arduino.h"
#include "Move.h"

using namespace std;

Move::Move(String namep, String typep, int damagep){
  name = namep;
  moveType = typep;
  damage = damagep;
}

Move::Move() {
  name = "Tackle";
  moveType = "Normal";
  damage = 10;
}

