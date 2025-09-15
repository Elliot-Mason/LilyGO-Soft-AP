#include <WiFi.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

void setup() {
  Serial.begin(115200);
  // Optional: init e-ink display here, show "Provisioning" message
  WiFiManager wifiManager;

  // Uncomment to force portal every boot
  // wifiManager.resetSettings();

  // This starts an AP named "T5-Setup-XXXX" (auto-generated) and a captive portal.
  if(!wifiManager.autoConnect("T5-Setup-XXXX","setup_password")) {
    Serial.println("Failed to connect and hit timeout");
    // handle failure (e.g. restart or fallback)
  } else {
    Serial.println("Connected to WiFi!");
    // Proceed to normal operation
  }
}

void loop() {
  // normal device code
}
