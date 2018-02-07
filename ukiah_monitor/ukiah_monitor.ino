#define LOCAL 0
#define USE_I2C_DISPLAY 1 // I2C version of LED display
#define DEBUG 1
#define FOR_TESTING 0 // Set to 0 when actually monitoring pump

#define USE_RAW_LED 0 // Just the 1602 display with a bunch of wires


#include <SPI.h>

#if (USE_I2C_DISPLAY && USE_RAW_LED)
bugger{}; // force compile time error
#endif

#if USE_I2C_DISPLAY
#include <Wire.h> 
  // Uses I2C controller. Mine connects the four leads as follows
  //   GND GND
  //   VCC +5v
  //   SDA A4
  //   SCL A5    
#include <Arducam_I2C.h>
#else 
#if USE_RAW_LED
#include <LiquidCrystal.h>
#endif
#endif //#if USE_I2C_DISPLAY

#define PUMP_PORT A3
#define PUMP_THRESHOLD 300 // Pump is ON if sensor exceeds this value
#define PUMP_POLL_DELAY 5 // Seconds to wait before checking pump sensor

//#include <SD.h>  //adds 8k bytes of 32K available, may not need to use the SD card
#include <Ethernet.h>
//#include <math.h>
//Last updated 7-30-2013, but this is essentially what has been
//running on the Ukiah arduino since May 10.


#if USE_I2C_DISPLAY
static int lcdPort = 0x27;
Arducam_I2C lcd(lcdPort,16,2); // set the LCD address that found by scanner sketch
#else 
#if USE_RAW_LED
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
#endif
#endif

#ifdef DEBUG
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINT(x) Serial.print(x)
#else
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINT(x)
#endif

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
  pinMode(sensorPin,OUTPUT);
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

byte mac[] = {  0x90, 0xA2, 0xDA, 0x0D, 0xB1, 0x80 }; // Board number 1
//byte mac[] = {  0x90, 0xA2, 0xDA, 0x0D, 0xCC, 0x04 }; // Board number 2

#if LOCAL
char serverName[] = "192.168.0.113"; // IP address of local server (RPi #3)
//char serverName[] = "192.168.0.111"; // IP address of local server
#else
char serverName[] = "paulputter.com";
#endif

EthernetClient myClient;
int pumpSensorPin = PUMP_PORT;
int pump_status = 0; // Is the pump on?
//int pump_cycles = 0; // on/off cycle count

//LED boardLED(13);
LED greenLED(3);

#if (USE_I2C_DISPLAY || USE_RAW_LED)
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
#endif

void
makeGetCall(
  int pump_status,
  int pump_cycles,
  double pump_time, 
  double cum_pump_time
) {

if (myClient.connect(serverName, 80)) {
      DEBUG_PRINT(serverName);
      DEBUG_PRINTLN(" connected");
      myClient.print("GET /handle_arduino_post.php");
      //myClient.print("?pump_cycles=27");
      myClient.print("?pump_status=");
      myClient.print(pump_status);
      myClient.print("&pump_cycles=");
      myClient.print(pump_cycles);
      myClient.print("&pump_time=");
      myClient.print(pump_time);
      myClient.print("&cum_pump_time=");
      myClient.print(cum_pump_time);
      myClient.print("&for_testing=");
      myClient.print(FOR_TESTING);
      myClient.println(" HTTP/1.1"); // Peter's suggestion 9/25/2017
  //    myClient.println(" Http/1.0");     
      myClient.print("Host: ");
      myClient.println(serverName);
      myClient.println();
      DEBUG_PRINTLN("I done hit handle_arduino_post.php");
    } else {
      DEBUG_PRINTLN("BUGGER! ");
    }
    delay(1000);
    DEBUG_PRINTLN("Returned by server after call:");
    while(myClient.available()) {
      char c = myClient.read();
      DEBUG_PRINT(c);
    }
    DEBUG_PRINTLN();
   myClient.stop();
}

static char lcd_buffer[16];  

void setup() { 
    
  lcd.init();
  lcd.backlight();
  sprintf(lcd_buffer, "%x", lcdPort);
  lcd.print("lcd on 0x");
  lcd.print(lcd_buffer);
 
#if DEBUG
  Serial.begin(9600); // Start in case of errors
  while (!Serial) ; // wait
#endif
  lcd.setCursor(0,1);
  lcd.print("await ethernet");
  DEBUG_PRINTLN("Wait for ethernet");
  greenLED.turnOn();
  while(Ethernet.begin(mac) == 0) {
    DEBUG_PRINTLN("Failed to configure Ethernet using DHCP");
    delay(1000); // Just keep trying, it can't hurt.
  }
  greenLED.turnOff();
    // give the Ethernet shield a second to initialize:
  delay(1000);
  lcd.home(); lcd.clear();
  lcd.print("Connected!");
  DEBUG_PRINTLN("Configured ethernet using DHCP");
  delay(5000);
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
    DEBUG_PRINT("pump sensor: ");
    DEBUG_PRINTLN(sensorValue);
    
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
        DEBUG_PRINTLN("Pump is ON");
      } else { // Pump just turned off. Report it.
        cumulative_pump_time += pump_timer;
        pump_cycles++;
        DEBUG_PRINTLN("Pump is OFF");
        greenLED.turnOff();
      }
      makeGetCall(current_pump_status,pump_cycles,pump_timer,cumulative_pump_time);
     // updateLcd(current_pump_status,pump_cycles,pump_timer,cumulative_pump_time);
  
    } 
    if(current_pump_status) {
      updateLcd(current_pump_status,pump_cycles+1,pump_timer,cumulative_pump_time+pump_timer); 
      pump_timer += PUMP_POLL_DELAY;
    } else {
      updateLcd(current_pump_status,pump_cycles,pump_timer,cumulative_pump_time);
    }
 
    delay(1000*PUMP_POLL_DELAY); // This is the only delay in the loop. To do: use the time library instead.

}
