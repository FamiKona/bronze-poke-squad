#include "Arduino.h"
#include "Move.h"

using namespace std;

Move::Move(String namep, String typep, int damagep){
  name = namep;
  type = typep;
  damage = damagep;
}

Move::Move() {
  name = "Tackle";
  type = "Normal";
  damage = 10;
}

