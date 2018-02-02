//DFRobot.com
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include <Arducam_I2C.h> // Works with more IIC boards than LiquidCrystal_I2C.h

Arducam_I2C lcd(0x27,16,2); // set the LCD address to 0x3F for a 16 chars and 2 line display,If not work, 
                            // you can set it to 0x27 to try. The I2c address is defined by your I2C controller.
                             


void setup()
{
  int number = 10;
  double pi = 3.14159;
  lcd.init();                      // initialize the lcd 
 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.print("Hello, MyArducam!");
  
  lcd.print("Hey!");
  lcd.print(number);
  delay(1000);
  lcd.home();
  lcd.print("Hello again");
  delay(1000);
  lcd.setCursor(0,0);
  lcd.noBacklight();
  lcd.home();
  lcd.clear();
  lcd.print("After backlight off");
  delay(1000);
  lcd.backlight();
  delay(1000);
  lcd.home();
  lcd.print(pi,4); // this shows that print takes a precision specification
delay(1000);

}

void loop()
{
  static int i = 0;
  lcd.clear();
  lcd.print(i);
  delay(1000);
  i++;
  if(i == 12) {
    i = 0;
  }
}
