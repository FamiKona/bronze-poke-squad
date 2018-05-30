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

void Pokemon::take(String oppName, Move m, int t, bool rainy) {
  int dmg = m.damage;

  // debugging code
//  Serial.println(m.moveType);
//  Serial.println(type);
//  Serial.println(t);

  int effective = 0; // 0 is neutral, 1 is effective, -1 is not effective

  if (t == 1) { //attacked pokemon is normal type
    float tempdmg = 1.0 * dmg;
    dmg = (int) tempdmg; // 0% decrease in effectiveness
  } else if (type.equals("Fire") && (t == 2)) {
    float tempdmg = 1.4 * dmg;
    dmg = (int) tempdmg; // 40% increase in effectiveness
    effective = 1;
  } else if (type.equals("Water") && (t == 2)) {
    float tempdmg = 0.6 * dmg;
    dmg = (int) tempdmg; // 40% decrease in effectiveness
    effective = -1;
  } else if (type.equals("Grass") && (t == 2)) {
    float tempdmg = 1.4 * dmg;
    dmg = (int) tempdmg; // 40% increase in effectiveness
    effective = 1;
  }

  if (rainy) {
    // reduce the effect of fire and increase the effect of water
    if (t == 2) {
      dmg = (int) (0.9 * dmg); //reduce the damage by 10%
    } else if (t == 2) {
      dmg = (int) (1.1 * dmg); //increase damage by 10%
    }
  }

  //MODIFY DAMAGE BASED ON TYPING
  this->currentHP = this->currentHP - dmg;

  Serial.println();
  Serial.print("Dealt ");
  Serial.print(dmg);
  Serial.println(" damage!");

  if (effective == 1) {
    Serial.println("It was very effective!");
  } else if (effective == -1) {
    Serial.println("It was not very effective...");
  }

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
