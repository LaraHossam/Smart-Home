#include <Servo.h>

const int gas = 2;
const int flame = 3;
const int emergencyLED = 4;
const int buzzer = 5;
int flameState = HIGH;
int gasState = LOW;
int firstLoop = 1;
const int rain = A7;
const int waterPower = 7;
const int waterA = A2;
const int moisturePower = 6;
const int moisture = A1;
const int moistureLED = 22;
const int redLED = 8;
const int greenLED = 10;
const int yellowLED = 9;
const int gardenLEDS = 11;
const int ldrPin = A0;
int rainState;
const int IRgarage = 20;
const int motionSensor = A12;
const int roomLED = 38;
const int roomledPin = 24;
const int fan = 44;
const int variableLED = 49;
int IRstat = HIGH;
int PIRstat = 0;
int roomLEDstat = LOW;
boolean carInside = false;
boolean closed = false;
int pos = 0;
int pos1 = 0;
int val;
char orderRecieved;
Servo window;
Servo garageDoor;

void setup() {
  Serial.begin(9600);
  window.attach(12, 500, 2500);
  window.write(0);
  garageDoor.attach(51, 500, 2500);
  garageDoor.write(0);
  pinMode(gardenLEDS, OUTPUT);
  pinMode(ldrPin, INPUT);
  pinMode(rain, INPUT);
  pinMode(gas, INPUT_PULLUP);
  pinMode(flame, INPUT_PULLUP);
  pinMode(IRgarage, INPUT);
  pinMode(emergencyLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(motionSensor, INPUT);
  attachInterrupt(digitalPinToInterrupt(gas), ISRgas, CHANGE);
  attachInterrupt(digitalPinToInterrupt(flame), ISRflame, CHANGE);
  //  attachInterrupt(digitalPinToInterrupt(motionSensor), ISRmotion, CHANGE);
  pinMode(waterPower, OUTPUT);
  digitalWrite(waterPower, LOW);
  pinMode(moisturePower, OUTPUT);
  digitalWrite(moisturePower, LOW);
  pinMode(waterA, INPUT);
  pinMode(moisture, INPUT);
  pinMode(moistureLED, OUTPUT);
  pinMode(roomLED, OUTPUT);
  //digitalWrite(roomLED, LOW);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(roomledPin, OUTPUT);
  pinMode(fan, OUTPUT);
  digitalWrite(fan, LOW);
  //digitalWrite(roomLED, HIGH);

}

void loop() {
  //  if(millis() > 1000 && millis() < 2000)
  //  {
  //    if(closed==false)
  //    {
  //      closeWindow();
  //    }
  //  }
  if ( firstLoop == 1 && closed == false) {
    closeWindow();
    //window.write(90);
    closed = true;
  }
  firstLoop = 0;
  if (Serial.available())  /* If data is available on serial port */
  {
    orderRecieved = Serial.read();
    Serial.println(orderRecieved); /* Print character received on to the serial monitor */
    switch (orderRecieved)
    {
      case '1':
        garageDoor.write(90);
        break;
      case '2':
        garageDoor.write(0);
        break;
      case '3':
        //openWindow();
        window.write(0);
        closed = false;
        break;
      case '4':
        //closeWindow();
        window.write(90);
        closed = true;
        break;
      case '5':
        digitalWrite(roomledPin, HIGH);
        break;
      case '6':
        digitalWrite(roomledPin, LOW);
        break;

    }
  }
  //ambient light sensor
  int ldrStatus = analogRead(ldrPin);
  if (ldrStatus < 500) {
    digitalWrite(gardenLEDS, HIGH);
    //Serial.print("Its DARK, Turn on the LED : ");
    //Serial.println(ldrStatus);
  }
  else {
    digitalWrite(gardenLEDS, LOW);
    //Serial.print("Its BRIGHT, Turn off the LED : ");
    //Serial.println(ldrStatus);
  }
  delay(1);
  //water level
  int level = readWaterSensor();
  if (level < 400)
  {
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, HIGH);
  }
  else if (level > 400 && level < 520)
  {
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(greenLED, LOW);
  }
  else
  {
    digitalWrite(redLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
  }
  delay(1);
  //rain
  rainState = analogRead(rain);
  Serial.println(rainState);
  if (rainState < 800)
  {
    //Serial.println("El denya betmatar");
    if (closed == false)
    {
      closeWindow();
      closed = true;
    }
  }
  //  else
  //  {
  //     window.write(0);
  ////if(closed==true)
  ////{
  ////  openWindow();
  ////}
  //
  //     //Serial.println("El denya mesh betmatar");
  //  }
  //moisture
  int moistureLevel = readMoistureSensor();
  if (moistureLevel < 500)
  {
    digitalWrite(moistureLED, HIGH);
  }
  else
  {
    digitalWrite(moistureLED, LOW);
  }
  Serial.print("Moisture = ");
  Serial.println(moistureLevel);
  //temp sensor
  //  val = digitalRead(tempPin);
  //  digitalWrite(fan, LOW);
  //  Serial.print("temp = ");
  //  Serial.print(val);
  //  Serial.print(" fan = ");
  //  Serial.println(digitalRead(fan));
  //  if(val == LOW)
  //  {
  //    digitalWrite(fan, LOW);
  //  }
  //  else
  //  {
  //    digitalWrite(fan, HIGH);
  //  }
  delay(1);
  //IRgarage
  pos1 = 0;
  IRstat = digitalRead(IRgarage);
  if (carInside == false && IRstat == LOW)
  {
    Serial.println("HELLO");
    garageDoor.write(90);
    delay(3000);
    garageDoor.write(0);
  }
  delay(1000);
}
void ISRgas() {
  Serial.println("EL GAS ETGHAYAR");
  gasState = digitalRead(gas);// read flamedigital value and assigne it to val variable
  if (gasState == LOW) // When the flame sensor detects a signal relay is on and buzzer sound (void alarm)
  {
    digitalWrite(emergencyLED, HIGH);
    tone(buzzer, 2000);
    digitalWrite(buzzer, HIGH);
  }
  else
  {
    digitalWrite(emergencyLED, LOW);
    noTone(buzzer);
  }
}
void ISRflame() {
  flameState = digitalRead(flame);
  if (flameState == LOW) // When the flame sensor detects a signal relay is on and buzzer sound (void alarm)
  {
    digitalWrite(emergencyLED, HIGH);
    digitalWrite(buzzer, HIGH);
    tone(buzzer, 2000);
  }
  else
  {
    digitalWrite(emergencyLED, LOW);
    noTone(buzzer);
  }
}
//void ISRgarage()
//{
//  Serial.println("hi");
//  pos1 = 0;
//  IRstat = digitalRead(IRgarage);
//  if(carInside == false && IRstat == LOW)
//  {
//    Serial.println("HELLO");
//    for (pos1 = 0; pos1 <= 90; pos1 += 1) {
//      garageDoor.write(pos1);
//      delay(15);
//    }
//    delay(2000);
//    for (pos1 = 90; pos1 >= 0; pos1 -= 1) {
//      garageDoor.write(pos1);
//      delay(15);
//    }
//  }
//}
//void ISRmotion()
//{
//  Serial.println("MOVEMENT");
//  PIRstat = digitalRead(motionSensor);
//  roomLEDstat = digitalRead(roomLED);
//  if(PIRstat == HIGH && roomLEDstat == LOW)
//  {
//    digitalWrite(roomLED, HIGH);
//  }
//}

int readWaterSensor() {
  digitalWrite(waterPower, HIGH);  // Turn the sensor ON
  delay(10);              // wait 10 milliseconds
  int depth = analogRead(waterA);    // Read the analog value form sensor
  digitalWrite(waterPower, LOW);   // Turn the sensor OFF
  return depth;             // send current reading
}

int readMoistureSensor() {
  digitalWrite(moisturePower, HIGH);  // Turn the sensor ON
  delay(10);              // wait 10 milliseconds
  int level = analogRead(moisture);    // Read the analog value form sensor
  digitalWrite(moisturePower, LOW);   // Turn the sensor OFF
  return level;             // send current reading
}


//BLUETOOTH MODULE FUNCTIONS:
void closeWindow()
{
  if (closed == false)
  {
    for (pos = 0; pos <= 90; pos += 1) {
      window.write(pos);
      delay(15);
    }
    closed = true;
  }


}
void openWindow()
{
  if (closed == true)
  {
    for (pos = 90; pos <= 0; pos -= 1) {
      window.write(pos);
      delay(15);
    }
    closed = false;
  }
}

void openGarageDoor()
{
  for (pos = 0; pos <= 90; pos += 1) {
    garageDoor.write(pos);
    delay(15);
  }
}
void closeGarageDoor()
{
  for (pos = 90; pos <= 0; pos -= 1) {
    window.write(pos);
    delay(15);
  }
}
void openRoomLED()
{
  digitalWrite(roomLED, HIGH);
}
void closeRoomLED()
{
  digitalWrite(roomLED, LOW);
}
