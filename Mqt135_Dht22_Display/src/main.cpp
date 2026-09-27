#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// MQ-135 (ADC1 pin — avoids WiFi conflict later)
const int MQ135_PIN = 34;

// DHT22
const int DHT_PIN = 4;
#define DHT_TYPE DHT22
DHT dht(DHT_PIN, DHT_TYPE);

// OLED (I2C — SDA=GPIO21, SCL=GPIO22 by default on ESP32)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  Wire.begin();
  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found — check wiring, or try address 0x3D instead of 0x3C");
    while (true) delay(1000);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Warming up MQ-135...");
  display.display();

  Serial.println("Warming up MQ-135 (20s)...");
  delay(20000);
}

void loop() {
  int smokeRaw = analogRead(MQ135_PIN);
  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();

  Serial.printf("MQ-135: %d | Temp: %.1fC | Humidity: %.1f%%\n", smokeRaw, temp, hum);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Room Status");
  display.print("Smoke: "); display.println(smokeRaw);

  if (isnan(temp) || isnan(hum)) {
    display.println("DHT22 read error");
  } else {
    display.print("Temp: "); display.print(temp, 1); display.println(" C");
    display.print("Hum : "); display.print(hum, 1); display.println(" %");
  }

  display.display();
  delay(2000); // DHT22 needs ~2s between reads to stay reliable
}