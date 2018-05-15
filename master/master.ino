void setup() {
  // put your setup code here, to run once:
  allActive();
}

void loop() {
  // put your main code here, to run repeatedly:
  int move = waitForMove();
  sendMove(move);
}

bool allActive() {
  //WRITE "LOOKING FOR UNITS"
  //CHECK PLAYER UNIT
  //CHECK DM UNIT
  //WAIT IF FAIL THEN LOOP
  return true;
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