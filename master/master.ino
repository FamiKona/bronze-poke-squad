#include <SoftwareSerial.h>
#include "Pokemon.h"
#include "Move.h"
#include <stdlib.h>
#include <Servo.h>

// Objects
SoftwareSerial xBee(2, 3);
Servo cpuServo;
Servo humServo;

// Global variables for use throughout the program
String msg;
bool msgComplete;
const byte newLineChar = 0x0A;
bool rainy = false;
int humidity;
int temperature;
bool inBattle;

// Declare Pokemon
Pokemon cpu = Pokemon("Water", "Mudkip", 100);
Pokemon cpu2 = Pokemon("Ground", "Geodude", 100);
Pokemon cpu3 = Pokemon("Ground", "Zubat", 80);
Pokemon cpu4 = Pokemon("Ground", "Machop", 80);
Pokemon cpu5 = Pokemon("Grass", "Caterpie", 90);
Pokemon player = Pokemon("Fire", "Eevee", 100);


//SETUP
void setup() {

  // begin serial/hardwareserial components
  Serial.begin(9600);
  xBee.begin(9600);

  msgComplete = false;
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  digitalWrite(11, HIGH);
  cpuServo.attach(7);
  humServo.attach(8);
  humServo.write(0);
  cpuServo.write(90);

  //check that the two subunits are running
  allActive();

  // Set the moves for each Pokemon
  player.setMove(1, "Flamethrower", "Fire", 30);
  player.setMove(2, "Ember", "Fire", 20);

  //buffer for the String
  msg = "";
  inBattle = false;

  Serial.println("READY TO BEGIN ADVENTURE");
}

//MAIN LOOP
void loop() {

  if (inBattle) {
    digitalWrite(11, LOW);
    // run the battle
    // get the initial environmental conditions
    Serial.println("GETTING INITIAL CONDITIONS...");
    getInitialConditions();

    //let all units know that we are ready to start battling
    xBee.println("master_unit_ready");
    xBee.println("READYTOSTART");

    // start the battle!
    Serial.println("BATTLE START");

    //roll a random pokemon!
    int rollDice = (rand() % 5) + 1;
    bool results;

    if (rollDice == 1) {
      results = battle(player, cpu);
    } else if (rollDice == 2) {
      results = battle(player, cpu2);
    } else if (rollDice == 3) {
      results = battle(player, cpu3);
    } else if (rollDice == 4) {
      results = battle(player, cpu4);
    } else {
      results = battle(player, cpu5);
    }

    // at this point the battle has ended
    xBee.println("BATTLEEND");
    inBattle = false;

  } else {

    digitalWrite(11, HIGH); // turn on the yellow light - indicator that the user is not in the

    if (millis() % 5000 == 0) {
      Serial.println("You walk through the dungeon...");
      xBee.print("BATTLEEND");
    }

    checkMessageReceived();

    if (msgComplete) {
      if (msg.equals("zubat")) { // DM unit sent the "zubat" command
        Serial.println("A POKEMON WAS ENCOUNTERED!");
        xBee.println("READYTOSTART");
        inBattle = true;
        msgComplete = false;
      }
    }

    msg = ""; //clear the message buffer
  }
}

void getInitialConditions() {

  bool receivedHumidity = false;
  bool receivedTemperature = false;
  bool readyToStart = false;

  while (!receivedHumidity || !receivedTemperature) {
    delay(2000);
    receivedHumidity = true;
    receivedTemperature = true;

    if (millis() % 5000 == 0) {
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
      String msgType = msg.substring(0,4); // parse the string
      String numStr = msg.substring(6);

      int num = numStr.toInt(); //conver the latter part to an int

      if (msgType.equals("temp")) {

        receivedTemperature = true;
        temperature = int(num);
        Serial.print("Temperature received: ");
        Serial.println(num);

        if (num > 30) { // humidity control
          rainy = true;
        }

      } else if (msgType.equals("humi")) {

        receivedHumidity = true;
        temperature = int(num);
        Serial.print("Humidity received: ");
        Serial.println(num);

        if (num > 45) {
          rainy = true;
        }

      }

      msgComplete = false;
      msg = "";

    }
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

  //PRINT READY TO DISPLAYS
  Serial.println("ALL UNITS READY!");
  for (int i = 0; i < 2; i++) {
    xBee.println("master_unit_ready");
    xBee.println("master_unit_ready");
    xBee.println("master_unit_ready");
  }
}


// THIS IS THE GLOBAL METHOD TO CHECK THAT A MESSAGE HAS BEEN RECEIVED
 void checkMessageReceived() {
    if (xBee.available()) {
      byte ch = xBee.read();
      if (ch == newLineChar) {
        msgComplete = true;
        msg.trim(); // KMS THIS ONE FUCKING LINE OF CODE
        Serial.println(msg);
      } else {
        msg += char(ch);
      }
    }
  }

// TEMPORARY CHECKMESSAGERECEIVED TO EMULATE RECEIVING A MESSAGE FROM XBEE
//void checkMessageReceived() {
// while (Serial.available()) {
//   msg = Serial.readString();
//   msgComplete = true;
//   msg.trim();
// }
//}

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
        Serial.println("Received move: move 0");
        isValidMove = true; //exit while
      } else if (msg.equals("move:2")) {
        move = 2;
        Serial.println("Received move: move 1");
        isValidMove = true; //exit while
      } else if (msg.equals("move:3")) {
        move = 3;
        Serial.println("Received move: move 2");
        isValidMove = true; //exit while
      } else if (msg.equals("move:4")) {
        move = 4;
        Serial.println("Received move: move 3");
        isValidMove = true; //exit while
      }
      msgComplete = false; //end of the msg
      msg = ""; //reset the global msg
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
    if (rainy) {
      Serial.print("It is raining... Fire power dampened");
    }

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
      Serial.println();
      int currentMove = (rand() % 4) + 1;
      delay(1000);
      if (currentMove == 1) {
        Serial.print(computer.name);
        Serial.print(" used ");
        Serial.println(computer.move1.name);
        int moveType = getType(computer.move1.moveType);
        human.take(computer.name, computer.move1, moveType, rainy);
      } else if (currentMove == 2) {
        Serial.print(computer.name);
        Serial.print(" used ");
        Serial.println(computer.move2.name);
        int moveType = getType(computer.move2.moveType);
        human.take(computer.name, computer.move2, moveType, rainy);
      } else if (currentMove == 3) {
        Serial.print(computer.name);
        Serial.print(" used ");
        Serial.println(computer.move3.name);
        int moveType = getType(computer.move3.moveType);
        human.take(computer.name, computer.move3, moveType, rainy);
      } else if (currentMove == 4) {
        Serial.print(computer.name);
        Serial.print(" used ");
        Serial.println(computer.move4.name);
        int moveType = getType(computer.move4.moveType);
        human.take(computer.name, computer.move4, moveType, rainy);
      }
      cpuServo.write(130);
      delay(500);
      cpuServo.write(90);


      delay(4000); // let the player actually see what's going on
      Serial.println();
    } else {
      Serial.println("IT IS THE PLAYER'S TURN");
      Serial.println("Waiting for move...");
      digitalWrite(13, HIGH);
      digitalWrite(12, LOW);
      int currentMove = getMove();
      if (currentMove == 1) {
        int moveType = getType(human.move1.moveType);
        computer.take(human.name, human.move1, moveType, rainy);
      } else if (currentMove == 2) {
        int moveType = getType(human.move2.moveType);
        computer.take(human.name, human.move2, moveType, rainy);
      } else if (currentMove == 3) {
        int moveType = getType(human.move3.moveType);
        computer.take(human.name, human.move3, moveType, rainy);
      } else if (currentMove == 4){
        int moveType = getType(human.move4.moveType);
        computer.take(human.name, human.move4, moveType, rainy);
      }
      digitalWrite(13, LOW);
      digitalWrite(12, HIGH);
      humServo.write(30);
      delay(500);
      humServo.write(0);
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

  xBee.println("BATTLEEND");
  xBee.println("BATTLEEND");

}

// a method to return an interger for a corresponding pokemon type (String)
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
