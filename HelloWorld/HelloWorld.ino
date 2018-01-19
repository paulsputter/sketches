#include <LiquidCrystal_I2C.h>

#include <Wire.h> 
  // Uses I2C controller. Mine connects the four leads as follows
  //   GND GND
  //   VCC +5v
  //   SDA A4
  //   SCL A5    
//#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x35 or 0x27 (use i2c_scanner.ino to find) for a 20 chars and 4 line display

void setup()
{
  int number = 10;
  double pi = 3.14159;
  lcd.init();                      // initialize the lcd 
 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.print("Hello, world!");
  delay(1000);
  //lcd.noBacklight();
#if 1 
  lcd.print("Hey!");
  lcd.print(number);
  delay(1000);
  lcd.home();
  lcd.print("Hello again");
  lcd.setCursor(0,0);
  lcd.noBacklight();
  lcd.home();
  lcd.clear();
  lcd.print("After backlight off");
//delay(9000);
  lcd.backlight();
  delay(9000);
  lcd.home();
  lcd.print(pi,4); // this shows that print takes a precision specification
delay(4000);
#endif
}

void loop()
{
  #if 1
  static int i = 0;
  lcd.clear();
  lcd.print(i);
  delay(1000);
  i++;
  if(i == 12) {
    i = 0;
  }

#endif
}
