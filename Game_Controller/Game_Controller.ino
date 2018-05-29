// LCD setup
#include "Adafruit_LiquidCrystal.h"
#include "Wire.h"
Adafruit_LiquidCrystal lcd(0);

// SoftwareSerial to communicate with the XBee:
#include <SoftwareSerial.h>

// XBee's DOUT (TX) is connected to pin 2 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 3 (Arduino's Software TX)
SoftwareSerial xbee(2, 3); // RX, TX

// Joystick Setup
int joyPin1 = 0;                 // slider variable connected to analog pin 0
int joyPin2 = 1;                 // slider variable connected to analog pin 1
int value1 = 0;                  // variable to read the value from the analog pin 0
int value2 = 0;                  // variable to read the value from the analog pin 1

String msg;
bool msgComplete;
const byte newLineChar = 0x0A;
int start = 0;
int x = 0;
int y = 0;

// pull in the move set TEMPORARY
String moveSet[4] = {"wind", "fire", "water", "ground"};
//int move]Curr = 0;

// RFID setup
// RFID Libraries
#include <MFRC522.h> // Include of the RC522 Library
#include <SPI.h> // Used for communication via SPI with the Module
#define SDAPIN 10 // RFID Module SDA Pin is connected to the UNO 10 Pin
#define RESETPIN 4 // RFID Module RST Pin is connected to the UNO 8 Pin

byte FoundTag; // Variable used to check if Tag was found
byte ReadTag; // Variable used to store anti-collision value to read Tag information
byte TagData[MAX_LEN]; // Variable used to store Full Tag Data
byte TagSerialNumber[5]; // Variable used to store only Tag Serial Number
byte GoodTagSerialNumber[5] = {0x95, 0xEB, 0x17, 0x53}; // The Tag Serial number we are looking for

MFRC522 nfc(SDAPIN, RESETPIN); // Init of the library using the UNO pins declared above

bool inBattle = false;

void setup() {
  // Set up both ports at 9600 baud. This value is most important
  // for the XBee. Make sure the baud rate matches the config
  // setting of your XBee.
  xbee.begin(9600);
  Serial.begin(9600);
  Serial.println( "Arduino started sending bytes via XBee" );
  // configure pin 7 & 8 as an input and enable the internal pull-up resistor
  pinMode(7, INPUT_PULLUP );
  pinMode(8, INPUT_PULLUP);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.setCursor(0, 0);
  lcd.print("   Welcome to");
  lcd.setCursor(0, 1);
  lcd.print("Pokemon Dungeon!");
  
  //RFID setup//
  SPI.begin();
  rfidCheck();
}

// Function that calculates the joystick position values
int treatValue(int data) {
  return (data * 9 / 1024) + 48;
}

/* Message check
   Function is called to determine contents of the xBee message
*/
void checkMessageReceived() {
  if (xbee.available()) {
    byte ch = xbee.read();
    if (ch == newLineChar) {
      msgComplete = true;
      msg.trim();
    } else {
      msg += char(ch);
    }
  }
}

/* RFID CHECK FUNCTION
   Function is called at the beginning to determine if RFID is there/functioning.
*/
void rfidCheck() {
  // Start to find an RFID Module
  Serial.println("Looking for RFID Reader");
  nfc.begin(); // Init MFRC522 
  byte version = nfc.getFirmwareVersion(); // Variable to store Firmware version of the Module
  
  // If can't find an RFID Module 
  if (! version) { 
    Serial.print("Didn't find RC522 board.");
    while(1); //Wait until a RFID Module is found
  }
  
  // If found, print the information about the RFID Module
  Serial.print("Found chip RC522 ");
  Serial.print("Firmware version: 0x");
  Serial.println(version, HEX);
  Serial.println();
}

void loop() {

  // RFID READ IN //
  rfidRead();

  // CHECKING XBEE CONNECTIVITY //
  if (start == 0) {
    xbee.println("player_unit_ready");
    delay(1000);
    checkMessageReceived();
    if (msgComplete) {
      if (msg.equals("master_unit_ready")) {
        start = 1;
        Serial.println("MASTER UNIT READY");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Ready to begin!");
      }
      msgComplete = false;
      msg = "";
    }
  }

  /************* BUTTONS *************/
  //read the pushbutton value into a variable
  int buttonA = digitalRead(7);
  int buttonB = digitalRead(8);

  // Keep in mind the pull-up means the pushbutton's logic is inverted. It goes
  // HIGH when it's open, and LOW when it's pressed. Turn on pin 13 when the
  // button's pressed, and off when it's not:

  if (buttonA == LOW) {
    lcd.clear();
    xbee.println("selectA");
    lcd.setCursor(0, 1);
    lcd.print("Select");
    Serial.println("Button A/Select");
  }

  if (buttonB == LOW) {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Cancel");
    Serial.println("Button B/Cancel");
  }
  /************* END OF BUTTON *************/

  /************* JOYSTICK *************/
  // UP/DOWN // value1: 48 down, 52 neutral, 56 up
  // LEFT/RIGHT // value2: 48 left, 52 neutral, 56 right

  // reads the value of the variable resistor
  value1 = analogRead(joyPin1);
  // this small pause is needed between reading
  // analog pins, otherwise we get the same value twice
  //  delay(100);
  // reads the value of the variable resistor
  value2 = analogRead(joyPin2);

  int vertiMove = treatValue(value1);
  int horiMove = treatValue(value2);

  Serial.print("UP/DOWN 1: ");
  Serial.print(horiMove);
  Serial.print("    L/R 2: ");
  Serial.println(vertiMove);

//  xbee.println("player_unit_ready");
  
  
  /************* BATTLE *************/
  // this should activate the battle loop. to be replaced w/ an activation from
  // the other remote team

  checkMessageReceived();
  if (msgComplete) {
    if (msg.equals("READYTOSTART")) {
      inBattle = true;
    }
    while (inBattle == true) {
      inBattle = battle(inBattle, joyPin1, joyPin2);
    }
    Serial.println("OUT OF BATTLE");
    // lcd.setcursor
    // you won/lost
    // delay 2 seconds
  }
  /************* OUT OF BATTLE *************/

  // MOVING AROUND -- exiting prescribed "battle" state
  if (horiMove == 48) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Left");
    Serial.println("lf");
    xbee.println("lf");
    delay(100);

  } else if (horiMove == 56) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Right");
    Serial.println("rt");
    xbee.println("rt");
    delay(100);
  }

  // tracking vertical movement
  if (vertiMove == 48) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Down");
    Serial.println("dn");
    xbee.println("dn");
    delay(100);
  } else if (vertiMove == 56) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Up");
    Serial.println("up");
    xbee.println("up");
    delay(100);
  }
  /************* END OF JOYSTICK *************/
}

/* BATTLE FUNCTION
   Enters a loop in which a player can choose moves against their opponent.
   Variables: bool inBattle imports the state of battle
              int joyPin1 & 2: reads in state of joystick from analog pin
   Returns: boolean true/false to stay in or leave battle state
*/
bool battle(bool inBattle, int joyPin1, int joyPin2) {
  Serial.println("<<<<<<<<<<<<<<IN BATTLE>>>>>>>>>>>>>>>");
  int moveCurr = 0;
  int vertiRead;
  int horiRead;
  int vertiMove;
  int horiMove;

  while (inBattle = true) {
    // reads the up/down value of the variable resistor
    vertiRead = analogRead(joyPin1);
    // reads the L/R value of the variable resistor
    horiRead = analogRead(joyPin2);

    vertiMove = treatValue(vertiRead);
    horiMove = treatValue(horiRead);

    lcd.setCursor(0, 0);
    
    lcd.print("Move Name");
    if (horiMove == 48) {
      lcd.setCursor(0, 1);
      moveCurr -= 1;
      // delay to allow for easy array traversing
      delay(300);
      if (moveCurr == -1) {
        moveCurr = 0;
      }
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Move Name");
      lcd.setCursor(0, 1);
      lcd.print(moveSet[moveCurr]);

      lcd.setCursor(0, 1);
    } else if (horiMove == 56) {
      lcd.setCursor(0, 1);
      moveCurr += 1;
      delay(300);
      if (moveCurr == 4) {
        moveCurr = 3;
      }
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Move Name");
      lcd.setCursor(0, 1);
      lcd.print(moveSet[moveCurr]);
    }

    int buttonA = digitalRead(7);
    int buttonB = digitalRead(8);
  
    // selecting the move
    if (buttonA == LOW) {
      lcd.setCursor(0, 1);
      xbee.print("move: ");
      xbee.println(String(moveCurr));
      delay(300);                        
      Serial.print("move: ");
      Serial.println(String(moveCurr));
    }

    // check if the battle is over
    checkMessageReceived();
    if (msgComplete) {
      if (msg.equals("BATTLEEND")) {
        inBattle = false;
        return false;
      }
    }
    msgComplete = false;
    msg = "";
  }
}

/* RFID READ FUNCTION
   Function is called when a tag is detected.
   Prints welcome message to new players
*/
void rfidRead() {
  String GoodTag="False"; // Variable used to confirm good Tag Detected

  // Check to see if a Tag was detected
  // If yes, then the variable FoundTag will contain "MI_OK"
  FoundTag = nfc.requestTag(MF1_REQIDL, TagData);
  
  if (FoundTag == MI_OK) {
    // Get anti-collision value to properly read information from the Tag
    ReadTag = nfc.antiCollision(TagData);
    memcpy(TagSerialNumber, TagData, 4); // Write the Tag information in the TagSerialNumber variable
    
    Serial.println("Tag detected.");
    Serial.print("Serial Number: ");
    for (int i = 0; i < 4; i++) { // Loop to print serial number to serial monitor
      Serial.print(TagSerialNumber[i], HEX);
      Serial.print(", ");
    }
    Serial.println("");
    Serial.println();
    
    
    // Check if detected Tag has the right Serial number we are looking for 
    for(int i=0; i < 4; i++){
      if (GoodTagSerialNumber[i] != TagSerialNumber[i]) {
        break; // if not equal, then break out of the "for" loop
      }
      if (i == 3) { // if we made it to 4 loops then the Tag Serial numbers are matching
        GoodTag="TRUE";
      } 
    }
    if (GoodTag == "TRUE"){
//      Serial.println("Success!!!!!!!");
      Serial.println("TAG NOT ACCEPTED...... :(");
    }
    else {
//      Serial.println("TAG NOT ACCEPTED...... :(");
      Serial.println("Success!!!!!!!");
      lcd.setCursor(0,0);
      lcd.print("New player found");
      lcd.setCursor(0,1);
      lcd.print("Welcome!");
      delay(2000);
      lcd.clear();
    }
  }
}
