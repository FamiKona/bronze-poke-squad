#include <SoftwareSerial.h>
#include "Pokemon.h"
#include "Move.h"
#include <stdlib.h>

SoftwareSerial xBee(2, 3);

String msg;
bool msgComplete;
const byte newLineChar = 0x0A;

int humidity;
int temperature;
bool inBattle;

Pokemon cpu = Pokemon("Water", "mudkip", 100);
Pokemon player = Pokemon("Water", "magikarp", 100);

//SETUP
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  xBee.begin(9600);
  msgComplete = false;


  //check that the two subunits are running
  allActive();

  player.setMove(1, "Flamethrower", "Fire", 30);

  //buffer for the String
  msg = "";
  inBattle = false;

  Serial.println("READY TO BEGIN ADVENTURE");


}

//MAIN LOOP
void loop() {

  if (inBattle) {
    // run the battle
    getInitialConditions();
    Serial.println("GETTING INITIAL CONDITIONS...");
    // start the battle!
    Serial.println("BATTLE START");
    bool results = battle(player, cpu);
    // at this point the battle has ended
    xBee.println("BATTLEEND");
    inBattle = false;
  } else {
    if (millis() % 5000 == 0) {
      Serial.println("You walk through the dungeon...");
    }
    checkMessageReceived();
    if (msgComplete) {
      if (msg.equals("zubat")) {
        Serial.println("A POKEMON WAS ENCOUNTERED!");
        inBattle = true;
        msgComplete = false;
      }
    }
    msg = "";
  }
}

void getInitialConditions() {
  bool receivedHumidity = false;
  bool receivedTemperature = false;
  bool readyToStart = false;

  while (!receivedHumidity || !receivedTemperature) {
    if (millis() % 2000 == 0) {
      if (!receivedHumidity) {
        Serial.println("Waiting for initial condition: humidity...");
      }
      if (!receivedTemperature) {
        Serial.println("Waiting for initial condition: temperature...");
      }
    }
    checkMessageReceived();
    if (msgComplete) {
      // t e m p :  n n
      // 0 1 2 3 4 5 6 7
      String msgType = msg.substring(0,4);
      String numStr = msg.substring(6);
      int num = numStr.toInt();
      if (msgType.equals("temp")) {
        receivedTemperature = true;
        temperature = int(num);
        Serial.print("TEMP ");
        Serial.println(num);
      } else if (msgType.equals("humi")) {
        receivedHumidity = true;
        temperature = int(num);
        Serial.print("HUMI ");
        Serial.println(num);
      }
      msgComplete = false;
      msg = "";
    }
  }


  Serial.println("Waiting for battle to begin...");
  while (!readyToStart) {
    checkMessageReceived();
    if (msgComplete) {
      if (msg.equals("start")) {
        readyToStart = true;
      }
    }
    msgComplete = false;
    msg = "";
  }

  xBee.println("READYTOSTART");
}

//check if all the units are nearby
void allActive() {
  Serial.println("Looking for units...");
  bool dm_unit_exists = false;
  bool player_unit_exists = false;

  while (!dm_unit_exists || !player_unit_exists) {
    checkMessageReceived();
    if (msgComplete) {
      if (msg.equals("dm_unit_ready")) {
        dm_unit_exists = true;
        Serial.println("DM UNIT READY");
      } else if (msg.equals("player_unit_ready")) {
        player_unit_exists = true;
        Serial.println("PLAYER UNIT READY");
      }
      msgComplete = false;
      msg = "";
    }
  }
  //at this point, both units are known to be working
  xBee.println("master_unit_ready");
  //PRINT READY TO DISPLAYS
  Serial.println("ALL UNITS READY!");
}

// void checkMessageReceived() {
//   if (xBee.available()) {
//     byte ch = xBee.read();
//     if (ch == newLineChar) {
//       msgComplete = true;
//       msg.trim(); // KMS THIS ONE FUCKING LINE OF CODE
//     } else {
//       msg += char(ch);
//     }
//   }
// }


// TEMPORARY CHECKMESSAGERECEIVED TO EMULATE RECEIVING A MESSAGE FROM XBEE
void checkMessageReceived() {
  while (Serial.available()) {
    msg = Serial.readString();
    Serial.println(msg);
    msgComplete = true;
    msg.trim();
  }
}

// INTERNAL METHOD //
// this method receives the message from the player module
// and returns the appropriate integer
int getMove(){
  bool isValidMove = false;
  int move = 0;
  msg = ""; //make sure the buffer is empty first

  while (!isValidMove){
    checkMessageReceived(); //get the new message as it comes
    if (msgComplete) {
      if (msg.equals("move:1")) {
        move = 1;
        Serial.println("Received move: move 1");
      } else if (msg.equals("move:2")) {
        move = 2;
        Serial.println("Received move: move 2");
      } else if (msg.equals("move:3")) {
        move = 3;
        Serial.println("Received move: move 3");
      } else if (msg.equals("move:4")) {
        move = 4;
        Serial.println("Received move: move 4");
      }
      msgComplete = false; //end of the msg
      msg = ""; //reset the global msg
      isValidMove = true; //exit while
    }
  }
  return move;
}

// battle logic
// returns true if still the player wins
// returns false if the cpu has won
bool battle(Pokemon human, Pokemon computer) {
  int getRandomTurn = rand() % 2;  // roll a die!
  bool humanGoesFirst;
  if (getRandomTurn == 0) {
    humanGoesFirst = true; // human goes first
    Serial.println("PLAYER GOES FIRST!");
  } else {
    humanGoesFirst = false; // computer goes first
    Serial.println("OPPONENT GOES FIRST!");
  }
  int currTurn = 0; // turn counter

  Serial.println();
  
  // while the cpu is not dead AND the human is not dead
  while (!human.ko && !computer.ko) {
    Serial.print("CPU: ");
    Serial.print(computer.name);
    Serial.print(" HP: ");
    Serial.print(computer.currentHP);
    Serial.print("/");
    Serial.println(computer.maxHP);

    Serial.print("PLAYER: ");
    Serial.print(human.name);
    Serial.print(" HP: ");
    Serial.print(human.currentHP);
    Serial.print("/");
    Serial.println(human.maxHP);

    Serial.println();

    
    if (currTurn % 2 == 0) { //p1's turn
      Serial.println("IT IS THE OPPONENTS'S TURN");
      Serial.println("OPPONENT IS CHOOSING A MOVE...");
      int currentMove = (rand() % 4) + 1;
      delay(1000);
      if (currentMove == 1) {
        Serial.print(computer.name);
        Serial.print(" used ");
        Serial.println(computer.move1.name);
        int moveType = getType(computer.move1.moveType);
        human.take(computer.name, computer.move1, moveType);
      } else if (currentMove == 2) {
        Serial.print(computer.name);
        Serial.print(" used ");
        Serial.println(computer.move2.name);
        int moveType = getType(computer.move2.moveType);
        human.take(computer.name, computer.move2, moveType);
      } else if (currentMove == 3) {
        Serial.print(computer.name);
        Serial.print(" used ");
        Serial.println(computer.move3.name);
        int moveType = getType(computer.move3.moveType);
        human.take(computer.name, computer.move3, moveType);
      } else if (currentMove == 4) {
        Serial.print(computer.name);
        Serial.print(" used ");
        Serial.println(computer.move4.name);
        int moveType = getType(computer.move4.moveType);
        human.take(computer.name, computer.move4, moveType);
      }
      Serial.println();
    } else {
      Serial.println("IT IS THE PLAYER'S TURN");
      Serial.println("Waiting for move...");
      int currentMove = getMove();
      if (currentMove == 1) {
        int moveType = getType(human.move1.moveType);
        computer.take(human.name, human.move1, moveType);
      } else if (currentMove == 2) {
        int moveType = getType(human.move2.moveType);
        computer.take(human.name, human.move2, moveType);
      } else if (currentMove == 3) {
        int moveType = getType(human.move3.moveType);
        computer.take(human.name, human.move3, moveType);
      } else if (currentMove == 4) {
        int moveType = getType(human.move4.moveType);
        computer.take(human.name, human.move4, moveType);
      } else {
        Serial.print(player.name);
        Serial.println(" missed! (improper move name)");
        Serial.println();
      }
    }
    currTurn++;
  }

  //at this point, one of them is dead

  //return who won
  if (computer.ko) {
    return true; //player has won
    Serial.println("PLAYER HAS WON!!");
  } else if (human.ko) {
    return false; // player has lost
    Serial.println("PLAYER HAS LOST!!");
  }
}

int getType(String t) {
  if (t == "Normal") {
    return 1;
  } else if (t == "Fire") {
    return 2;
  } else if (t == "Water") {
    return 3;
  } else if (t == "Grass") {
    return 4;
  } else {
    return 0;
  }
}

