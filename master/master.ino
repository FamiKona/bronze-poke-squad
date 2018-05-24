#include <SoftwareSerial.h>
<<<<<<< HEAD
#include <stdlib.h>
=======
#include "Pokemon.h"
>>>>>>> c72ca358442fb9879efe1aedebff1a0a8fc53e2c

SoftwareSerial xBee(2, 3);

String msg;
bool msgComplete;
const byte newLineChar = 0x0A;

int humidity;
int temperature;

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
}

//MAIN LOOP
void loop() {
  // get initial data from dm unit
  getInitialConditions();

  // put your main code here, to run repeatedly:

  bool battleActive = true;
  battleActive = battle();


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
      if (msg.equals("up")) {
        move = 1;
        Serial.println("Received move: UP");
      } else if (msg.equals("dn")) {
        move = 2;
        Serial.println("Received move: DOWN");
      } else if (msg.equals("lf")) {
        move = 3;
        Serial.println("Received move: LEFT");
      } else if (msg.equals("rt")) {
        move = 4;
        Serial.println("Received move: RIGHT");
      } else if (msg.equals("sel")) {
        move = 5;
        Serial.println("Received move: SELECT");
      } else if (msg.equals("cancel")) {
        move = 6;
        Serial.println("Received move: CANCEL");
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
  int getRandomTurn = rand() % 2;
  bool turn;
  if (getRandomTurn == 0) {
    turn = true;
  } else {
    turn == false;
  }
  


  while (!cpu.ko || !human.ko) {
    // check whether either is dead

    if (cpu.ko) {
      return true; // player has won
    } else if (human.ko) {
      return false; // player has lost
    }




  }




  //MOVE CYCLE
  if (cpu.ko) {
    return true;
  }
  else if (hmn.ko) {
    return false;
  }
  else {
  return true; //recursive here? battle(hmn, cpu)
  }
  //true is victory, false is loss
}



