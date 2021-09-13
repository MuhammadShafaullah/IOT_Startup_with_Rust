//lcd display value
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
//rfid/////////////////
#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
///////////////////////////
#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  // led green
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT); //buzzer
  //ECG module
  pinMode(7, INPUT); // Setup for leads off detection LO +
  pinMode(6, INPUT); // Setup for leads off detection LO -
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  // lcd display
  Serial.begin(9600);
  lcd.init();                      // initialize the lcd
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("System");
  lcd.setCursor(2, 1);
  lcd.print("Initialsing");
  delay(20000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("IoT based smart");
  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print("System for Heart");
  delay(2000);
  lcd.setCursor(-4, 2);
  lcd.print("Attack  Monitor");
  delay(2000);
  lcd.setCursor(-4, 3);
  lcd.print(" and Prediction");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Place your RFID");
  lcd.setCursor(0, 1);
  lcd.print("card to the ");
  lcd.setCursor(0, 2);
  lcd.print("reader...");
  delay(8000);
  lcd.clear();
  lcd.setCursor(0, 0); //Set LCD cursor to upper left corner, column 0, row 0
  lcd.print("Health Care Sys:");
  lcd.setCursor(0, 1);
  lcd.print("Access:");
  lcd.setCursor(-4, 2);
  lcd.print("BTem=");
  lcd.setCursor(5, 2);
  lcd.print(",");
  lcd.setCursor(6, 2);
  lcd.print("R=");
  lcd.setCursor(-4, 3);
  lcd.print("ECG Value= ");
  //body Tem
  // Serial.println("Adafruit MLX90614 test");
  if (!mlx.begin()) {
    // Serial.println("Error connecting to MLX sensor. Check wiring.");
    while (1);
  };
  //rfid
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  // Serial.println("Place your RFID card to the reader...");
  digitalWrite(5, HIGH);//buzzer
  delay(1000);
  digitalWrite(5, LOW);//buzzer
  digitalWrite(4, HIGH);
  //Serial.println();
}

void loop() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Show UID on serial monitor
  //Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    //Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  // Serial.println();
  // Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "59 68 42 8C") //Replace with your UID of the card/cards that you need to give access
  {
    lcd.setCursor(7, 1);
    lcd.print("Authorizd");
    lcd.setCursor(1, 2);
    lcd.print(round(mlx.readObjectTempC()));
    lcd.print("*C");
    lcd.setCursor(8, 2);
    lcd.print(round(mlx.readAmbientTempC()));
    lcd.print("*C");
    lcd.setCursor(7, 3);
    lcd.print(analogRead(A0));
    lcd.print("mm");
    
    Serial.println(mlx.readAmbientTempC());
    Serial.println(mlx.readObjectTempC());
    Serial.println(analogRead(A0));
    delay(5000);
    
  }

  if (content.substring(1) != "87 8C 63 B2" || content.substring(1) != "59 68 42 8C")
  {
    lcd.setCursor(7, 1);
    lcd.print(" Denied  ");
    lcd.setCursor(1, 2);
    lcd.print("___");
    lcd.setCursor(8, 2);
    lcd.print("___");
    lcd.setCursor(7, 3);
    lcd.print("___");

  };
  if (mlx.readObjectTempC() > 39)
  {
    digitalWrite(5, HIGH);//buzzer
    digitalWrite(3, HIGH);//red led
    delay(500);
    digitalWrite(5, LOW);
    digitalWrite(3, HIGH);//red led
  }
}
