#include <stdint.h>
#include <Wire.h>
#include "LiquidCrystal_I2C.h"

#define HILETGO_2004A_ADDR 0x27
#define BUFSIZE 120
static uint8_t buf[BUFSIZE] = {0,};
static uint8_t index = 0;

LiquidCrystal_I2C lcd(HILETGO_2004A_ADDR, 20, 4);

void setup()
{
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();
  lcd.print("BLE LCD PROGRAM");
  delay(1000);

  lcd.clear();
}

void exec_command(uint8_t buf[], uint8_t size)
{
  if(buf[0] == '$'){
    switch(buf[1]){
    case 0:
      if((size == 3) && (buf[size-1] == '\n')){
        lcd.clear();
        Serial.println("clear");
        return;
      }
      break;
    case 1:
      if((size==5) &&(buf[size-1] == '\n')){
        uint8_t row = buf[2];
        uint8_t col = buf[3];
        lcd.setCursor(col, row);
        Serial.println("move cursor");
        return;
      }
      break;
    case 2:
      if((size==4) && (buf[size-1] == '\n')){
        uint8_t data = buf[2];
        lcd.write(data);
        Serial.println("show data");
        return;
      }
      break;
    default:
      break;
    }
  }

  Serial.print("Error");
  return;
}

void loop()
{
  if(Serial.available() > 0){
    uint8_t d = Serial.read();
    buf[index] = d;
    index++;
    if((buf[index-1] == '\n') || (index > BUFSIZE)){
      exec_command(buf, index);
      index = 0;
    }
  }
}