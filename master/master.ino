#include <SoftwareSerial.h>
#include "Pokemon.h"
#include <stdlib.h>

SoftwareSerial xBee(2, 3);

String msg;
bool msgComplete;
const byte newLineChar = 0x0A;

int humidity;
int temperature;
bool inBattle;

//SETUP
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  xBee.begin(9600);
  msgComplete = false;

  //check that the two subunits are running
  allActive();

  //buffer for the String
  msg = "";
  inBattle = false;

}

//MAIN LOOP
void loop() {
  if (inBattle) {
    // run the battle
    getInitialConditions();
    // start the battle!
    bool results = battle();
    // at this point the battle has ended
    inBattle = false;
  } else {
    checkMessageReceived();
    if (msgComplete) {
      if (msg.equals("zubat")) {
        inBattle = true;
      }
    }
  }
}

void getInitialConditions() {
  bool receivedHumidity = false;
  bool receivedTemperature = false;
  bool readyToStart = false;

  while (!receivedHumidity || receivedTemperature) {
    checkMessageReceived();
    if (msgComplete) {
      // t e m p :   n n
      // 0 1 2 3 4 5 6 7
      String msgType = msg.substr(0,4);
      int num = int(msg.substr(6));

      if (msgType.equals("temp") {
        receivedTemperature = true;
        humidity = num;
      } else if (msgType.equals("humi")) {
        receivedHumidity = true;
        temperature = num;
      }
      msgComplete = false;
      msg = "";
    }
  }

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
      } else if (msg.equals("player_unit_ready")) {
        player_unit_exists = true;
      }
      msgComplete = false;
      msg = "";
    }
  }
  //at this point, both units are known to be working
  xBee.println("master_unit_ready");
  //PRINT READY TO DISPLAYS
  Serial.println("READY!");
}

void checkMessageReceived() {
  if (xBee.available()) {
    byte ch = xBee.read();
    if (ch == newLineChar) {
      msgComplete = true;
      msg.trim(); // KMS THIS ONE FUCKING LINE OF CODE
    } else {
      msg += char(ch);
    }
  }
}

int waitForMove() {
  Serial.print("Waiting for move...");
  msg = "";
  int currMove = getMove();
  return currMove;
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

int sendMove(int move) {

  bool confirmSend = false;
  msg = ""; //make sure the buffer is empty first

  while (!confirmSend) { //not confirmed that the receiving unit has received it
    xBee.println(move);
    Serial.println("Sending move...");
    Serial.println("Waiting for move to be received...");
    unsigned long startTime = millis();
    unsigned long timeOut = 5000; //5-second timeout

    while (!msgComplete) {
      checkMessageReceived();
      if (msgComplete) {
        if (msg.equals("move_received")) {
          Serial.println("Move Received by Player Unit...");
          msgComplete = false;
          msg = "";
          confirmSend = true;
        } else {
          // message is complete but it's not correct. retry.
          msg = "";
        }
      } else {
        if ((millis() - startTime) > timeOut) {
          //if it has exceeded the timeout time, perhaps the player unit did not receive it.
          //break the msgComplete while loop to restart the confirmSend while loop
          //to re-send the move
          msg = "";
          break;
        }
      }
    }
  }
  //at this point it has been confirmed that the move has been received by the player unit


  //RETURN POSITION
  //0 INVALID 1 VALID 2 VALID+EFFECT
}

// battle logic
// returns true if still the player wins
// returns false if the cpu has won
bool battle(Pokemon human, Pokemon cpu) {
  int getRandomTurn = rand() % 2;  // roll a die!
  bool humanGoesFirst;
  if (getRandomTurn == 0) {
    humanGoesFirst = true; // human goes first
  } else {
    humanGoesFirst == false; // computer goes first
  }

  //check whether either one is dead first, just to make sure
  if (cpu.ko) {
    return true; // player has won
  } else if (human.ko) {
    return false; // player has lost
  }

  // organise players by humanGoesFirst
  Pokemon p1;
  Pokemon p2;
  if (humanGoesFirst == true) {
    p1 = human;
    p2 = cpu;
  } else {
    p1 = cpu;
    p2 = human;
  }

  int currTurn = 0; // turn counter

  // while the cpu is not dead AND the human is not dead
  while (!p1.ko && !p2.ko) {
    if (currTurn % 2 == 0) { //p1's turn
      int currentMove = waitForMove();
      attack(p1, p2, currentMove);
    } else {
      int currentMove = waitForMove();
      attack(p2, p1, currentMove);
    }
    currTurn++;
  }

  //at this point, one of them is dead

  //return who won
  if (cpu.ko) {
    return true; //player has won
  } else {
    return false; // player has lost
  }
}

void attack(Pokemon attacker, Pokemon target, int move) {
  if (move == 1) {
    target.take(attacker, attacker.move1);
  } else if (move == 2) {
    target.take(attacker, attacker.move2);
  } else if (move == 3) {
    target.take(attacker, attacker.move2);
  } else if (move == 4) {
    target.take(attacker, attacker.move2);
  }
}
