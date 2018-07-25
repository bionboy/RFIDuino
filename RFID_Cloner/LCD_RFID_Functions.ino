/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 2
 * LCD Enable pin to digital pin 3
 * LCD D4 pin to digital pin 4
 * LCD D5 pin to digital pin 5
 * LCD D6 pin to digital pin 6
 * LCD D7 pin to digital pin 7
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V

*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
//const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void LCD_Read_Wait(){
  lcd.clear();
  lcd.print("Click button to");
  lcd.setCursor(0,1);
  lcd.print("read card.");
  delay(250);
}

void LCD_RW_Ready(){
  lcd.clear();
  lcd.print("Place card");
  lcd.setCursor(0,1);
  lcd.print("on sensor.");
  delay(250);
}

void LCD_Busy(){
  lcd.clear();
  lcd.print("Card Detected.");
  lcd.setCursor(0,1);
  lcd.print("Do not remove!");
  delay(500);
}

void LCD_Write_Wait(){
  lcd.clear();
  lcd.print("Copied! Click");
  lcd.setCursor(0,1);
  lcd.print("button to write.");
  delay(250);
}

void LCD_Write_Done(){
  lcd.clear();
  lcd.print("Successfully");
  lcd.setCursor(0,1);
  lcd.print("wrote to card.");
  delay(250);
}

void LCD_Display_Data(byte buff[]){
  lcd.clear();
  for (uint8_t i = 0; i < 16; i++)
  {
    lcd.write(buff[i]);
  }
  lcd.setCursor(0,1);
  lcd.print("Click to write.");
}
