int analogPin = 3;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int data;
//  if (Serial.available() > 0) {
  //  char userInput = Serial.read();
    //if(userInput == 'g') {
      //delay(10);
      data = analogRead(analogPin);
      if(data < 300) {
        Serial.println(1);
      } else {
        Serial.println(0);
      }
      // Serial.println(data);
    //}
  //}
}
