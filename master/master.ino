#include <SoftwareSerial.h>

SoftwareSerial xBee(2, 3);

String msg;
bool msgComplete;
const byte newLineChar = 0x0A;



//SETUP
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  xBee.begin(9600);

  //check that the two subunits are running
  allActive();
  msgComplete = false;

  //buffer for the String
  msg = "";
}

//MAIN LOOP
void loop() {
  // put your main code here, to run repeatedly:
  int move = waitForMove();
  sendMove(move);
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
      } else if (msg.equals("m1")) {
        move = 5;
        Serial.println("Received move: MOVE 1");
      } else if (msg.equals("m2")) {
        move = 6;
        Serial.println("Received move: MOVE 2");
      } else if (msg.equals("m3")) {
        move = 7;
        Serial.println("Received move: MOVE 3");
      } else if (msg.equals("m4")) {
        move = 8;
        Serial.println("Received move: MOVE 4");
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

bool battle(Pokemon hmn, Pokemon cpu) {
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

/*
class Move
{
  public:
    String name;
    String type;
    int damage;
    Move();
}

Move:Move(String namep, String typep, int damagep){
  name = namep;
  type = typep;
  damage = damagep;
}
*/

/*
class Pokemon
{
    public:
        String type;
        String type2;
        String name;
        int maxHP;
        int currentHP;
        String move1;
        String move2;
        String move3;
        String move4;
        bool ko;
        Pokemon();
        take();
};

Pokemon::Pokemon(String typep, String namep, int maxHPp, Move m1, Move m2, Move m3, Move m4) {
    type = typep;
    name = namep;
    ko = false;
    maxHP = maxHPp;
    currentHP = maxHP;
    if (m1 == null) {
      move1 = Move("Tackle", "Normal", 40);
    }
    if (m2 != null) {
      move2 = m2;
    }
    if (m3 != null) {
      move3 = m3;
    }
    if (m4 != null) {
      move4 = m4;
    }
}

Pokemon:take(Pokemon pokemon, Pokemon opp, Move move) {
  int dmg = move.damage
  //MODIFY DAMAGE BASED ON TYPING
  if (opp.ko) {
    dmg = 0;
  }
  this.currentHP = this.currentHP - dmg;
  if (this.currentHP <= 0) {
    currentHP = 0;
    ko = true;
  }
}
*/