#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define LED_PIN 13 // Pin for LED

MFRC522 mfrc522(SS_PIN, RST_PIN); 
LiquidCrystal_I2C lcd(0x27, 16, 2); 
#define BISCUIT_UID "5367df11" 
#define LAYS_UID "531ea20d"       

#define BISCUIT_PRICE 50
#define LAYS_PRICE 25

int total = 0; 

void setup() {
  Serial.begin(9600); 
  SPI.begin();        
  mfrc522.PCD_Init();  

  lcd.init();                      
  lcd.backlight();                 
  
  pinMode(LED_PIN, OUTPUT);       
  
  lcd.setCursor(0,0);
  lcd.print("Add Products");    
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Read the card's UID
  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    uid += String(mfrc522.uid.uidByte[i], HEX);
  }

  Serial.println("Card UID: " + uid); // Print UID for debugging

  // Check which card was read
  int price = 0;
  String productName = "";
  if (uid == BISCUIT_UID) {
    price = BISCUIT_PRICE;
    productName = "Biscuit";
  } else if (uid == LAYS_UID) {
    price = LAYS_PRICE;
    productName = "Lays";
  } else {
    lcd.clear();
    lcd.print("Unknown card");
    delay(2000);
    lcd.clear();
    return;
  }

  // Add price to total
  total += price;

  // Display product name and price on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(productName);
  lcd.setCursor(0, 1);
  lcd.print("Price RS");
  lcd.print(price);

  // Wait for a moment before resetting LCD
  delay(3000);

  // Reset LCD to show initial message
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Add Products ");

  // Display total price on LCD
  lcd.setCursor(0, 1);
  lcd.print("Total: RS");
  lcd.print(total);

  // Blink LED
  blinkLED();
}

// Function to blink LED
void blinkLED() {
  digitalWrite(LED_PIN, HIGH); 
  delay(500);                   
  digitalWrite(LED_PIN, LOW);  
  delay(500);                   
}
