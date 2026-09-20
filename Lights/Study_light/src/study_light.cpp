#include <Arduino.h>
#include <WiFi.h>
#include "SinricPro.h"
#include "SinricProSwitch.h"
#include "secrets.h"

const int RELAY_PIN = 26;

// The raw relay hardware triggers ON when IN is pulled LOW (confirmed by bench
// testing). These two are flipped on purpose: SinricPro's onPowerState()
// reported the opposite of what actually happened here, so swapping the
// meaning of these constants was the fix, rather than inverting the ternary
// below. See README.md in this folder for the full story.
const bool RELAY_ON  = HIGH;
const bool RELAY_OFF = LOW;

bool onPowerState(const String &deviceId, bool &state) {
  Serial.printf("Study Light turned %s\n", state ? "ON" : "OFF");
  digitalWrite(RELAY_PIN, state ? RELAY_ON : RELAY_OFF);
  return true;
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, RELAY_OFF);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected! IP: ");
  Serial.println(WiFi.localIP());

  SinricProSwitch &mySwitch = SinricPro[SWITCH_ID];
  mySwitch.onPowerState(onPowerState);
  SinricPro.begin(APP_KEY, APP_SECRET);
}

void loop() {
  SinricPro.handle();
}