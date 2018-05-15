// LCD setup
#include "Adafruit_LiquidCrystal.h"
#include "Wire.h"
Adafruit_LiquidCrystal lcd(0);

// We'll use SoftwareSerial to communicate with the XBee:
#include <SoftwareSerial.h>


// XBee's DOUT (TX) is connected to pin 2 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 3 (Arduino's Software TX)
SoftwareSerial xbee(2, 3); // RX, TX
//char c = 'A';
// Joystick Setup
int joyPin1 = 0;                 // slider variable connecetd to analog pin 0
 int joyPin2 = 1;                 // slider variable connecetd to analog pin 1
 int value1 = 0;                  // variable to read the value from the analog pin 0
 int value2 = 0;                  // variable to read the value from the analog pin 1

String moveSet[4]={"wind", "fire", "water", "ground"};

void setup() {
  // Set up both ports at 9600 baud. This value is most important
  // for the XBee. Make sure the baud rate matches the config
  // setting of your XBee.
  xbee.begin(9600);
  Serial.begin(9600);
  Serial.println( "Arduino started sending bytes via XBee" );
  //configure pin 7 & 8 as an input and enable the internal pull-up resistor
  pinMode(7, INPUT_PULLUP );
  pinMode(8, INPUT_PULLUP);
  pinMode(13, OUTPUT);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Move Name:");
  lcd.print(moveSet[0]);
  lcd.setCursor(0,1);

}

// Function that calculates the joystick position values
int treatValue(int data) {
  return (data * 9 / 1024) + 48;
}

void loop() {
  
  // BUTTON //
  //read the pushbutton value into a variable
  int buttonA = digitalRead(7);
  int buttonB = digitalRead(8);
  //print out the value of the pushbutton
  Serial.println(buttonA);
  Serial.println(buttonB);

  // Keep in mind the pull-up means the pushbutton's logic is inverted. It goes
  // HIGH when it's open, and LOW when it's pressed. Turn on pin 13 when the
  // button's pressed, and off when it's not:
  if (buttonA == HIGH) {
    digitalWrite(13, LOW);
  } else {
    digitalWrite(13, HIGH);
    lcd.print("Select");
    lcd.setCursor(0,1);
//    Serial.println("Button A/Select");
  }

  if (buttonB == HIGH) {
    digitalWrite(12, LOW);
  } else {
    digitalWrite(12, HIGH);
    lcd.print("Cancel");
    lcd.setCursor(0,1);
//    Serial.println("Button B/Cancel");
  }
   
  // END OF BUTTON //

  // x BEE //
  // send character via XBee to other XBee connected to Mac
  // via USB cable
//  xbee.print( c );
//  xbee.print("lol");
  
  //--- display the character just sent on console ---
//  Serial.println( c );
  
//  //--- get the next letter in the alphabet, and reset to ---
//  //--- 'A' once we have reached 'Z'. 
//  c = c + 1;
//  if ( c>'Z' ) 
//       c = 'A';
//  // END OF X BEE //

  // JOYSTICK //
  // reads the value of the variable resistor 
  value1 = analogRead(joyPin1);   
  // this small pause is needed between reading
  // analog pins, otherwise we get the same value twice
//  delay(100);             
  // reads the value of the variable resistor 
  value2 = analogRead(joyPin2);   

  Serial.print("UP/DOWN 1: ");
  Serial.print(treatValue(value1));
  Serial.print("    L/R 2: ");
  Serial.println(treatValue(value2));

  //END OF JOYSTICK //
  
}
