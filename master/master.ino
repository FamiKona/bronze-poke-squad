#include <SoftwareSerial.h>
#include <Button.h>

SoftwareSerial xBee(2, 3);

String buff;

//SETUP
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  xBee.begin(9600);

  //check that the two subunits are running
  bool allActive = allActive();

  //buffer for the String
  buff = "";
}

//MAIN LOOP
void loop() {
  // put your main code here, to run repeatedly:
  int move = waitForMove();
  sendMove(move);
}

//check if all the units are nearby
void allActive() {

  Serial.println("Looking for units...")
  bool dm_unit_exists = false;
  bool player_unit_exists = false;

  while (!dm_unit_exists && !player_unit_exists) {
    if (!dm_unit_exists) {
      if (xBee.available() > 0) {
        if (isAlphaNumeric(c) || c == ' ') {
          buff = buff + c;
        } else {
          if (buff.equals("player_available")) {
            player_unit_exists = true;
          }
          if (buff.equals("dm_available")) {
            player_unit_exists = true;
          }
          buff = "";
        }
      }
    }
  }

  //at this poin, both units are known to be working
  xBee.println("master_unit_ready");
  //PRINT READY TO DISPLAYS
  Serial.println("READY!");
}

int waitForMove() {
  //WRITE "WAITING FOR MOVE"
  //LOOP CHECK FOR VALID MOVE code
  //RETURN MOVE
}

int sendMove(int move) {
  //WrITE SENDING
  //SEND AND CONFIRM
  //WAIT FOR RESPONSE
  //RETURN POSITION
  //0 INVALID 1 VALID 2 VALID+EFFECT
}

bool battle(/*POKEMON*/) {
  
}

class Pokemon
{
    public:
        string type;
        string type2;
        string name;
        int maxHP;
        int currentHP;
        string move1;
        string move2;
        string move3;
        string move4;
        Pokemon();
};

Pokemon::Pokemon(string typep, string namep, int maxHPp) {
    type = typep;
    name = namep;
    maxHP = maxHPp;
    move1 = "tackle";
}