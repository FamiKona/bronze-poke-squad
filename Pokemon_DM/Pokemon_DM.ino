
#include <SoftwareSerial.h>
#include <Button.h>
#include "DHT.h"
#include "Adafruit_LiquidCrystal.h"
#include "SPI.h"
#include "TFT_22_ILI9225.h"

SoftwareSerial xBee(2, 3);
Adafruit_LiquidCrystal lcd(0);
TFT_22_ILI9225 tft = TFT_22_ILI9225(9, 10, 8, 12, 200);
DHT dht(7, DHT22);

String msg;
int matrix[16][16] = {};
int x = 0;
int y = 0;
int type = 0;
int fight = 0;
uint16_t color = COLOR_BLACK;
int start = 1;
boolean msgComplete = false;
int beginningX = 0;
int beginningY = 0;

//Button left (A0, INPUT_PULLUP);
//Button right (A1, INPUT_PULLUP);
//Button up (A2, INPUT_PULLUP);
//Button down (A3, INPUT_PULLUP);
//Button change (5, INPUT_PULLUP);
//Button attack (4, INPUT_PULLUP);

const int dat = A4;
const int clk = A5;

  void checkMessageReceived() {
    if (xBee.available()) {
      byte ch = xBee.read();
      if (ch == 0x0A) {
        msgComplete = true;
        msg.trim();
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
  
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);

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
    int action1 = digitalRead(A0);
    int action2 = digitalRead(A1);
    int action3 = digitalRead(A2);
    int action4 = digitalRead(A3);
    int action5 = digitalRead(4);
    int action6 = digitalRead(5);

    type = matrix[y][x];
  
    for (int i = 0; i < 16; i++) {
      for (int j = 0; j < 16; j++) {
        
        if (matrix[j][i] == 0) {
          color = COLOR_BROWN;
        }
        else if (matrix[j][i] == 1) {
          color = COLOR_BLACK;
        }
        else if (matrix[j][i] == 2) {
          color = COLOR_GREEN;
          beginningX = i;
          beginningY = j;
        }
        else if (matrix[j][i] == 3) {
          color = COLOR_RED;
        }
        
        tft.fillRectangle((i * 11), (j * 11) + 25, (i * 11) + 8, (j * 11) + 33, color);
        tft.drawText(10, 70, "solidRectangle");
      }
    }
    tft.fillRectangle((x * 11), (y * 11) + 25, (x * 11) + 3, (y * 11) + 28,COLOR_WHITE);
    
    
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
      if (action1 == 0) {
        x -= 1;
        //Serial.println("Left");
      }
    
      else if (action2 == 0) {
        x += 1;
        //Serial.println("Right");
      }
    
      else if (action3 == 0) {
        y -= 1;
        //Serial.println("Down");
      }
    
      else if (action4 == 0) {
        y += 1;
        //Serial.println("Up");
      }
  
      if (action5 == 0) {
        matrix[y][x]++;
        //Serial.println("Change");
    
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
      if (action6 == 0) {
        fight = 1;
        //send start and temp data
        int h = dht.readHumidity();
        int t = dht.readTemperature();
        xBee.print("temp: ");
        xBee.println(t);
        xBee.print("humi: ");
        xBee.println(h);
        xBee.println("start");
        
      }
    }
    
    if (fight == 1) {

      x = beginningX;
      y = beginningY;
      
      if (action6 == 0) {
        xBee.println("zoobat");
      }

      checkMessageReceived();
      if (msgComplete) {
        
        if (msg.equals("lf")) {
          x -= 1;
          if (matrix[y][x] == 1) {
            x += 1;
            xBee.println("Can't go there");
          }
          else {
            xBee.println("Safe");
          }
        }

        if (msg.equals("rt")) {
          x += 1;
          if (matrix[y][x] == 1) {
            x -= 1;
            xBee.println("Can't go there");
          }
          else {
            xBee.println("Safe");
          }
        }

        if (msg.equals("up")) {
          y += 1;
          if (matrix[y][x] == 1) {
            y -= 1;
            xBee.println("Can't go there");
          }
          else {
            xBee.println("Safe");
          }
        }

        if (msg.equals("dn")) {
          y -= 1;
          if (matrix[y][x] == 1) {
            y += 1;
            xBee.println("Can't go there");
          }
          else {
            xBee.println("Safe");
          }
        } 
      }     
    }
  }
  
}
