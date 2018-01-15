#include <LiquidCrystal.h>
#define PUMP_PORT A3
#define PUMP_THRESHOLD 300 // Pump is ON if sensor exceeds this value
#define PUMP_POLL_DELAY 5 // Seconds to wait before checking pump sensor

//#if USE_I2C_DISPLAY
//LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x3F for a 16x2 display
//#else 
//#if USE_RAW_LED
// The board wiring is as follows:
//
// 1 Ground
// 2 +5v (power for logic)
// 3 Middle connector of 10K variable resistor (for backlight)
// 4 RS to Arduino D7
// 5 RW Ground
// 6 E (en) to Arduino D8
// 7 D0 not used
// 8 D1 not used
// 9 D2 not used
// 10 D3 not used
// 11 D4 Arduino D9
// 12 D5 Arduino D4
// 13 D6 Arduino D5
// 14 D7 Arduino D6
// 15 A +5v (power for backlight)
// 16 K Ground
//
// Note that we can use any available digital pins provided they are wired accordingly.
//
// Here's a good explanation of the LCD setup:
//   http://www.instructables.com/id/Connect-A-16x2-LCD-Display-To-An-Arduino/
//

const int rs = 7, en = 8, d4 = 9, d5 = 4, d6 = 5, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//#endif
//#endif

class LED
{
  public:
    LED(int pin);
    void turnOn(void);
    void turnOff(void);
    void blink(int count);
    void blink();
  private:
    int sensorPin;
};

LED::LED(int pin):sensorPin(pin)
{
  pinMode(sensorPin,1);
}

void
LED::turnOn(void)
{
  digitalWrite(sensorPin, HIGH);
}
  
void
LED::turnOff(void)
{
  digitalWrite(sensorPin, LOW);
}

void
LED::blink(int count)
{
  int i;
  for(i = 1; i <= count; i++) {
    turnOn();
    delay(1000);
    turnOff();
    delay(1000);
  }
}

void
LED::blink(void)
{
  blink(sensorPin);
}

int pumpSensorPin = PUMP_PORT;
int pump_status = 0; // Is the pump on?
//int pump_cycles = 0; // on/off cycle count

//LED boardLED(13);
LED greenLED(3);

void
updateLcd(
  int pump_status,
  int pump_cycles,
  double pump_time, 
  double cum_pump_time
)
{
    lcd.home();
    lcd.print("stat:");
    lcd.print(pump_status);
    lcd.print(" cycle:");
    lcd.print(pump_cycles);
    lcd.setCursor(0,1);
    lcd.print(pump_time/60.0,1);
    lcd.print(" ");
    lcd.print(cum_pump_time/60.0,1);

}

void setup() { 
  int i;  
  lcd.begin(16,2);
  lcd.print("hello youse");

  Serial.begin(9600); // Start in case of errors
  greenLED.turnOn();
  while (!Serial) ; // wait
  delay(1000);
  greenLED.turnOff();
}


  
void loop()
{
    static int call_count = 0;
    static int last_pump_status = 0; // pump off
    static int pump_cycles = 0;
    int current_pump_status; // indicates on/off status of pump
    static double cumulative_pump_time = 0.0; // Total pump time since start
    static double pump_timer = 0.0; // Set timer to zero. Should timer be a class?
    
    int sensorValue = analogRead(pumpSensorPin);
    
    if(sensorValue < PUMP_THRESHOLD) {
      current_pump_status = 0;
    } else {
      current_pump_status = 1;
    }
    
    if (current_pump_status != last_pump_status) {
      last_pump_status = current_pump_status;
      if(current_pump_status) {
          // Pump is on, start the timer.
        pump_timer = 0.0;
        greenLED.turnOn();
      } else { // Pump just turned off. Report it.
        cumulative_pump_time += pump_timer;
        pump_cycles++;
        greenLED.turnOff();
      }
     // updateLcd(current_pump_status,pump_cycles,pump_timer,cumulative_pump_time);
    } 
    Serial.println(current_pump_status);
    if(current_pump_status) {
      updateLcd(current_pump_status,pump_cycles+1,pump_timer,cumulative_pump_time+pump_timer); 
      pump_timer += PUMP_POLL_DELAY;
    } else {
      updateLcd(current_pump_status,pump_cycles,pump_timer,cumulative_pump_time);
    }
 
    delay(1000*PUMP_POLL_DELAY); // This is the only delay in the loop. To do: use the time library instead.

}
