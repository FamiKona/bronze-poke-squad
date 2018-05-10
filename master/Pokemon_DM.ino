#include "DHT.h"
#include "Adafruit_LiquidCrystal.h"
#include <Button.h>

Adafruit_LiquidCrystal lcd(0);

int matrix[16][32] = {};
int x = 0;
int y = 0;
int type = 0;
int fight = 0;

Button left (12, INPUT_PULLUP);
Button right (11, INPUT_PULLUP);
Button up (10, INPUT_PULLUP);
Button down (9, INPUT_PULLUP);
Button change (8, INPUT_PULLUP);
Button attack (7, INPUT_PULLUP);

const int dat = A4;
const int clk = A5;
DHT dht(2, DHT22);

void setup() {
  
  Serial.begin(9600);
  Serial.println();
  Serial.println();
  Serial.println("starting set-up");
  
  pinMode(6, INPUT);

  lcd.begin(16, 2);
  dht.begin();
}

void loop() {
  
  int action1 = left.checkButtonAction();
  int action2 = right.checkButtonAction();
  int action3 = up.checkButtonAction();
  int action4 = down.checkButtonAction();
  int action5 = change.checkButtonAction();
  int action6 = attack.checkButtonAction();

  //display
  lcd.setCursor(0, 0);
  lcd.print("Position ");
  lcd.print(x);
  lcd.print(":");
  lcd.println(y);
  lcd.print("Type ");
  
  if (type == 0) {
    lcd.print("ground");
  }

  else if (type == 1) {
    lcd.print("chasm");
  }

  else if (type == 2) {
    lcd.print("start");
  }

  //set-up buttons
  if (fight == 0) {
    if (action1 == Button::CLICKED) {
      x -= 1;
    }
  
    else if (action2 == Button::CLICKED) {
      x += 1;
    }
  
    else if (action3 == Button::CLICKED) {
      y -= 1;
    }
  
    else if (action4 == Button::CLICKED) {
      y += 1;
    }

    if (action5 == Button::CLICKED) {
    
      if(matrix[y][x] == 0) {
        matrix[y][x] = 1;
      }
  
      if(matrix[y][x] == 1) {
        matrix[y][x] = 0;
      }
  
      else {
        matrix[y][x] = 0;
      } 
    }
  
    if (y > 16) {
      y = 16;
    }
  
    if (y < 0) {
      y = 0;
    }
  
    if (x > 32) {
      x = 32;
    }
  
    if (x < 0) {
      x = 0;
    }
  }
  
  //fight button
  if (action6 == Button::CLICKED && fight == 0) {
    fight = 1;
    //send start and temp data
    //h = dht.readHumidity();
    //t = dht.readTemperature();
  }

  if (action6 == Button::CLICKED && fight == 1) {
    //send zoobat
  }
  
}
