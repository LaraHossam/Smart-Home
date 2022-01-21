
#include <LiquidCrystal.h>
#include <Servo.h>
#include <Keypad.h>

void haramy();

bool access_granted = false;
int i = 0;
String password = "";
const int LEDS = 12;
int IR1 = 2;
int IRstat = HIGH;
const int securityLED = 13;
const int buzzer = 13;
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {3, 4, 5, 6};
byte colPins[COLS] = {7, 8, 10, 11};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS );

int pos = 0;
Servo door;
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);


void setup() {
  Serial.begin(9600);
  pinMode(LEDS, OUTPUT);
  door.attach(9, 500, 2500);
  door.write(0);
  lcd.begin(16, 2);
  pinMode(IR1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(IR1), haramy, CHANGE);
  pinMode(securityLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
}

void loop() {
  i = 0;
  password = "";
  while(i < 4)
  {
    char key = keypad.getKey();
    if (key){
      Serial.println(key);
      password.concat(key);
      i++;   
    }
  }
  if(password.equals("7150"))
  {
    access_granted = true;
    alaska();
    opendoor();
  }
  else if(password.equals("6886"))
  {
    access_granted = true;
    hamo();
    opendoor();
  }       
  else if(password.equals("6853"))
  {
    lara();
    opendoor();
  }
  else if(password.equals("6850"))
  {
    mekky();
    opendoor();
  }
  else if(password.equals("6825"))
  {
    seif();
    opendoor();
  }
  else if(password.equals("0000"))
  {
    reset();
  } 
  else
  {
    wrongpin();
  }
  lcd.setCursor(0, 1);
  lcd.print(millis() / 1000);
}

void opendoor()
{
  digitalWrite(LEDS, HIGH);
  for (pos = 0; pos <= 90; pos += 1) {
    door.write(pos);
    delay(15);
  }
  delay(5000);
  for (pos = 90; pos >= 0; pos -= 1) {
    door.write(pos);
    delay(15);  
  }
}

void hamo()
{
  lcd.clear();
  lcd.print("Hello Mohamed");
}
void alaska()
{
  lcd.clear();
  lcd.print("Hello Nour");
}
void lara()
{
  lcd.clear();
  lcd.print("Hello Lara");
}
void mekky()
{
  lcd.clear();
  lcd.print("Hello Mekky");
}
void seif()
{
  lcd.clear();
  lcd.print("Hello Seif");
}
void wrongpin()
{
  lcd.clear();
  lcd.print("WRONG PIN");
}
void reset()
{
  digitalWrite(LEDS, LOW);
  lcd.clear();
  lcd.print("Goodbye!");
  access_granted = false;
}


void haramy()
{
  if(access_granted == false)
  {
    lcd.clear();
    lcd.print("CALL 911!");
    IRstat =digitalRead(IR1);
    if(IRstat==LOW){
      digitalWrite(securityLED, HIGH);
      tone(buzzer, 2000);
      delay(500);
    } 
    else{
      digitalWrite(securityLED,LOW);
      noTone(buzzer);
    }
  }
  else{
    digitalWrite(securityLED,LOW);
    digitalWrite(buzzer, LOW);
  } 
}
