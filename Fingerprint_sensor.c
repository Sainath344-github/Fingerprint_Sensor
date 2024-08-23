#include <Adafruit_Fingerprint.h>
#include <Keypad.h>
#include <LiquidCrystal.h>

SoftwareSerial mySerial(2, 3); // RX, TX
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// Pin configurations for the LCD
LiquidCrystal lcd(8, 9, 10, 11, 12, 13); 

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {5, 4, 3, 2};
byte colPins[COLS] = {9, 8, 7, 6};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const String correctPassword = "1234"; // Define the correct password
String enteredPassword = "";

void setup() {
  lcd.begin(16, 2);
  lcd.print("Initializing...");
  
  pinMode(12, OUTPUT); // Buzzer
  pinMode(6, OUTPUT);  // Green LED
  pinMode(7, OUTPUT);  // Red LED

  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    lcd.clear();
    lcd.print("Fingerprint Ready");
  } else {
    lcd.clear();
    lcd.print("Sensor Failed");
    while (1);
  }
  
  delay(2000);
  lcd.clear();
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Place Finger:");
  lcd.setCursor(0, 1);
  lcd.print("OR Enter Pass:");
  
  char key = keypad.getKey();
  if (key) {
    if (key == '#') {
      if (enteredPassword == correctPassword) {
        grantAccess();
      } else {
        lcd.clear();
        lcd.print("Wrong Password!");
        delay(2000);
        lcd.clear();
      }
      enteredPassword = "";
    } else {
      enteredPassword += key;
      lcd.setCursor(0, 1);
      lcd.print("Entered: ");
      lcd.print(enteredPassword);
    }
  }

  if (finger.getImage() == FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("Scanning...");
    if (finger.image2Tz() == FINGERPRINT_OK) {
      if (finger.fingerFastSearch() == FINGERPRINT_OK) {
        grantAccess();
      } else {
        lcd.clear();
        lcd.print("Unauthorized");
        delay(2000);
        lcd.clear();
      }
    }
  }
}

void grantAccess() {
  lcd.clear();
  lcd.print("Access Granted");
  digitalWrite(6, HIGH); // Green LED ON
  digitalWrite(12, HIGH); // Buzzer ON
  delay(2000);
  digitalWrite(6, LOW);  // Green LED OFF
  digitalWrite(12, LOW); // Buzzer OFF
  lcd.clear();
}
