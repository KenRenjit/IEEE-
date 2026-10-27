#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Pin Definitions
#define DHTPIN 2          // Data pin connected to DHT22
#define DHTTYPE DHT22     // DHT 22 (AM2302)
#define LED_PIN 8         // LED alert pin

// Threshold temperature in Celsius
const float TEMP_THRESHOLD = 25.0;

// Initialize DHT sensor and I2C LCD (Address 0x27, 16 columns, 2 rows)
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
  
  // Pin setup
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // LED initially OFF

  // Start DHT sensor
  dht.begin();

  // Start LCD
  lcd.init();
  lcd.backlight();

  // Welcome Message
  lcd.setCursor(0, 0);
  lcd.print("Temp Monitor");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Reading temperature (DHT22 takes ~250ms to read)
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if reading failed
  if (isnan(temp) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!   ");
    return;
  }

  // Display temperature on LCD Line 1
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp, 1);
  lcd.print((char)223); // Degree symbol
  lcd.print("C   ");

  // Display status on LCD Line 2 & control LED
  if (temp > TEMP_THRESHOLD) {
    digitalWrite(LED_PIN, HIGH); // Turn ON LED
    lcd.setCursor(0, 1);
    lcd.print("Status: HIGH!   ");
  } else {
    digitalWrite(LED_PIN, LOW);  // Turn OFF LED
    lcd.setCursor(0, 1);
    lcd.print("Status: NORMAL  ");
  }

  delay(2000); // Refresh every 2 seconds
}
