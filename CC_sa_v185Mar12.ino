#include <LiquidCrystal.h>

// Latest v1.8.6 as of March 24. Confirmed L298 Works (without pwn connected) change delay to 1k at least
// Added Github 7.28.2200000000000000000000000000000000000000000000000000000000000000
// motor goes oneway only

LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2); // LCD pin rs rw en d4 d5 d6 d7.

int StartPin                = 9;      // switch input

int motor1Pin               = 8;      // H-bridge leg 1 (pin 2, 1A)

int motor2Pin               = 7;      // H-bridge leg 2 (pin 7, 2A)

int enablePin               = 6;      // H-bridge enable pin

int DirPin                  = 13;     // Motor direction select

//int DirSwCounter            = 0;

//int LastDirState            = 0;

bool Dir                    = true;

byte runningCan             = 0;

int cansCrushed;                       // Initial number of cans crushed set to 0

void writeLCD(int cans) {

  if (cans == 0) {

    lcd.begin(16, 2);

    lcd.print("Adams CC 7.28.22.v1");

    delay(3000);

    lcd.clear();

    lcd.setCursor(0, 0);

    lcd.print("Crushed:");

    lcd.setCursor(10, 0);

    lcd.print((int) cansCrushed);

    lcd.setCursor(0, 1);

    lcd.print("Weight:");

    lcd.setCursor(9, 1);

    lcd.print((int) cansCrushed * .034375);

  } else {

    lcd.begin(16, 2);

    lcd.print("Adams CanCrshr1");

    delay(3000);

    lcd.clear();

    lcd.setCursor(0, 0);

    lcd.print("Crushed:");

    lcd.setCursor(10, 0);

    lcd.print((int) cans);

    lcd.setCursor(0, 1);

    lcd.print("Weight:");

    lcd.setCursor(9, 1);

    lcd.print((int) cans * .034375);

  }

}

 

void setup()

{

  Serial.begin(9600);

  // INITIALIZE

  pinMode(StartPin, INPUT);

  pinMode(DirPin, INPUT);

  pinMode(motor1Pin, OUTPUT);

  pinMode(motor2Pin, OUTPUT);

  pinMode(enablePin, OUTPUT);

  digitalWrite(enablePin, HIGH);

  lcd.begin(16, 2);
  digitalWrite(motor1Pin, LOW);
  digitalWrite(motor2Pin, LOW);

  //cansCrushed = 0;

}

/////////////This is where it is done at///////////////////////////

byte canCycler() {
  // Start if statment.
   
  if (digitalRead(StartPin) == HIGH && runningCan == 0) {
    Serial.print("StartPin: Pressed ");
    Serial.println(StartPin);
    Serial.print("runningCan: ");
    Serial.println(runningCan);
    // Check if crusher is ready for operation.
    if (digitalRead(DirPin) == HIGH) {
      //digitalWrite(enablePin, 200);
      digitalWrite(motor1Pin, HIGH);
      digitalWrite(motor2Pin, LOW); // forward motors.
      runningCan = 1; // Passes function to stage 2.
      Serial.println("Crushing started. M1GH");
      delay(500);
    }
  }
  //phase 2 return action called.
  if (digitalRead(DirPin) == HIGH && runningCan == 1) {
    //digitalWrite(enablePin, 200);    
    Serial.println("StartPin Enabled and Dir True:M1LOW");
    delay(500);
    runningCan = 2; // Pass the function to stage 3.
    Serial.print("Cans Crushed: ");
    Serial.println(cansCrushed);  
  //return cansCrushed++; // Add to the total amount of cans crushed.
  }
  //if (digitalRead(DirPin) == HIGH && runningCan == 2) {
  if (digitalRead(DirPin) ==HIGH && runningCan ==2 && (digitalRead (StartPin)== LOW)) {    
    //digitalWrite(enablePin, 0);
    digitalWrite(motor1Pin, LOW);
    digitalWrite(motor2Pin, LOW);
    Serial.println("Motor stopped  M1&M2 LOW");
    delay(500);
  }
  else 
    digitalWrite(motor1Pin, LOW);
    digitalWrite(motor2Pin, LOW);
    Serial.println("Motor stopped Catch ALL");
    //delay(1000);
  runningCan = 0;
  //Serial.print("Cans Crushed: ");
  //Serial.println(cansCrushed);  
  return cansCrushed++; // Add to the total amount of cans crushed.
}

void loop() {
  writeLCD(canCycler());

}