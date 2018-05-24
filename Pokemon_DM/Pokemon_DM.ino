/*For the message complete function, there's a few things wrong.  
  First you need to move it outside of the setup and loop for scope reasons.
  Then you need to set either msg to buff or vice versa so the variable is declared.
  newLineChar  and msgComplete is also not defined as written currently.
  Finally your if statemet for the startup sequence will activate when only one sub arduino starts up because (!True & !False) = False, exiting the while.
  It should be an OR statement.*/ 

#include <SoftwareSerial.h>
#include <Button.h>
#include "DHT.h"
#include "Adafruit_LiquidCrystal.h"
#include "SPI.h"
#include "TFT_22_ILI9225.h"

SoftwareSerial xBee(2, 3);
Adafruit_LiquidCrystal lcd(0);
TFT_22_ILI9225 tft = TFT_22_ILI9225(9, 10, 8, 12, 200);
DHT dht(2, DHT22);

String msg;
int matrix[16][16] = {};
int x = 0;
int y = 0;
int type = 0;
int fight = 0;
uint16_t color = COLOR_BLACK;
int start = 0;
boolean msgComplete = false;

Button left (A0, INPUT_PULLUP);
Button right (A1, INPUT_PULLUP);
Button up (A2, INPUT_PULLUP);
Button down (A3, INPUT_PULLUP);
Button change (12, INPUT_PULLUP);
Button attack (13, INPUT_PULLUP);

const int dat = A4;
const int clk = A5;

  void checkMessageReceived() {
    if (xBee.available()) {
      byte ch = xBee.read();
      if (ch == "/n") {
        msgComplete = true;
      } 
      else {
        msg += char(ch);
      }
    }
  }

void setup() {

  xBee.begin(9600);
  Serial.begin(9600);
  Serial.println();
  Serial.println();
  Serial.println("starting set-up");
  
  pinMode(2, INPUT);

  lcd.begin(16, 2);
  dht.begin();
  tft.begin();

  msg = "";

}

void loop() {

  
  if (start == 0) {
    xBee.println("dm_unit_ready");
    delay(1000);
    checkMessageReceived();
    if (msgComplete) {
      if (msg.equals("master_unit_ready")) {
        start = 1;
      }
      msgComplete = false;
      msg = "";
    }
  }

  else {
    int action1 = left.checkButtonAction();
    int action2 = right.checkButtonAction();
    int action3 = up.checkButtonAction();
    int action4 = down.checkButtonAction();
    int action5 = change.checkButtonAction();
    int action6 = attack.checkButtonAction();

    type = matrix[y][x];
  
    for (int i = 0; i < 17; i++) {
      for (int j = 0; j < 17; j++) {
        
        if (matrix[j][i] == 0) {
          color = COLOR_BROWN;
        }
        else if (matrix[j][i] == 1) {
          color = COLOR_BLACK;
        }
        else if (matrix[j][i] == 2) {
          color = COLOR_GREEN;
        }
        else if (matrix[j][i] == 3) {
          color = COLOR_RED;
        }
        
        tft.fillRectangle((i * 12), (j * 12) + 20, (i * 12) + 8, (j*12) + 28, color);
        tft.drawText(10, 70, "solidRectangle");
      }
    }
    
    //display
    lcd.setCursor(0, 0);
    lcd.print("Position ");
    lcd.print(x);
    lcd.print(":");
    lcd.println(y);
    lcd.print("Type ");
    lcd.print(type);
    
    if (type == 0) {
      lcd.print("ground");
    }
  
    else if (type == 1) {
      lcd.print("chasm");
    }
  
    else if (type == 2) {
      lcd.print("start");
    }
  
    else if (type == 3) {
      lcd.print("end");
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
        matrix[y][x]++;
    
        if(matrix[y][x] > 3) {
          matrix[y][x] = 0;
        }
    
      }
    
      if (y > 16) {
        y = 16;
      }
    
      if (y < 0) {
        y = 0;
      }
    
      if (x > 16) {
        x = 16;
      }
    
      if (x < 0) {
        x = 0;
      }
    
      //fight button
      if (action6 == Button::CLICKED) {
        fight = 1;
        //send start and temp data
        int h = dht.readHumidity();
        int t = dht.readTemperature();
        xBee.println(h);
        xBee.println(t);
      }
    }
    
    if (action6 == Button::CLICKED && fight == 1) {
      //send zoobat
    }
  }
  
}
