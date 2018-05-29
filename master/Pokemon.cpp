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
    move1 = Move("Tackle", "Normal", 20);
    move2 = Move("Tackle", "Normal", 20);
    move3 = Move("Tackle", "Normal", 20);
    move4 = Move("Tackle", "Normal", 20);
}

void Pokemon::take(String oppName, Move m, int t) {
  int dmg = m.damage;
  Serial.print("OPP MOVE TYPE:     ");
  Serial.println(t);
  
  if (t == 1) { //attacked pokemon is normal type
    double tempdmg = 1.0 * dmg;
    dmg = (int) dmg; // 0% decrease in effectiveness
  } else if (this->type.equals("Fire") && (t == 2)) {
    double tempdmg = 1.4 * dmg;
    dmg = (int) dmg; // 40% increase in effectiveness
  } else if (this->type.equals("Water") && (t == 3)) {
    double tempdmg = 0.6 * dmg;
    dmg = (int) dmg; // 40% decrease in effectiveness
  } else if (this->type.equals("Grass") && (t == 4)) {
    double tempdmg = 1.4 * dmg;
    dmg = (int) dmg; // 40% increase in effectiveness
  }
  
  //MODIFY DAMAGE BASED ON TYPING
  this->currentHP = this->currentHP - dmg;
  
  Serial.println();
  Serial.print("Dealt ");
  Serial.print(dmg);
  Serial.println(" damage!");
  Serial.println();
  
  if (this->currentHP <= 0) {
    Serial.print(this->name);
    Serial.println(" fainted!");
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

