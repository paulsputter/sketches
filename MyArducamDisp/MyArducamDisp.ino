//DFRobot.com
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 

  // Uses I2C controller. Mine connects the four leads as follows
  //   GND GND
  //   VCC +5v
  //   SDA A4
  //   SCL A5    
#include <Arducam_I2C.h> // Works with more IIC boards than LiquidCrystal_I2C.h

static int portNum = 0x3F;
Arducam_I2C lcd(portNum, 16, 2); 

void setup()
{
  int number = 10;
  double pi = 3.14159;
  lcd.init();                      // initialize the lcd 
 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.print("Hello, MyArducam!");
  char lcd_buffer[16];
  delay(1000);
  lcd.clear();
  lcd.home();
  sprintf(lcd_buffer,"%x",portNum);
  lcd.print("Port: 0x");
  lcd.print(lcd_buffer);
  delay(3000);
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
