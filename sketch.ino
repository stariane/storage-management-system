#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define TRIG 9
#define ECHO 10
#define BUZZER 8
#define LED 13
#define DHTPIN 7
#define DHTTYPE DHT22

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);

  Serial.begin(9600);

  dht.begin();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("System Ready!");
  tone(BUZZER, 1000); // Startup beep
  delay(500);
  noTone(BUZZER);
  delay(1500);
  lcd.clear();
}

// Function for LED blinking and buzzer sound
void blinkAndBeep(int onTime, int offTime) {
  digitalWrite(LED, HIGH);
  tone(BUZZER, 1000);   // Generate 1000Hz sound
  delay(onTime);

  digitalWrite(LED, LOW);
  noTone(BUZZER);
  delay(offTime);
}

// Measure distance using HC-SR04
int getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH);

  int distance = duration * 0.034 / 2;

  return distance;
}

void loop() {

  int distance = getDistance();

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Display distance
  lcd.setCursor(0, 0);
  lcd.print("Dist:");
  lcd.print(distance);
  lcd.print("cm     ");

  // Display temperature and humidity
  lcd.setCursor(0, 1);
  lcd.print("H:");
  lcd.print(humidity, 0);
  lcd.print("% T:");
  lcd.print(temperature, 0);
  lcd.print("C   ");

  // Serial Monitor output
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm | Temp: ");
  Serial.print(temperature);
  Serial.print(" C | Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  // Food level alert
  if (distance < 5) {
    // Container full - very fast beep
    blinkAndBeep(50, 50);
  }
  else if (distance < 10) {
    // Medium level - moderate beep
    blinkAndBeep(150, 200);
  }
  else if (distance < 20) {
    // Low level - slow warning beep
    blinkAndBeep(300, 500);
  }
  else {
    // Very low or empty
    digitalWrite(LED, HIGH);
    tone(BUZZER, 1500); // Continuous alarm
    delay(500);
    noTone(BUZZER);
    digitalWrite(LED, LOW);
    delay(500);
  }

}