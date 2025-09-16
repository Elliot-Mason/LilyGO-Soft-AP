#include <WiFi.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <GxEPD2_BW.h>

// LilyGO T5 (2.13") wiring
GxEPD2_BW<GxEPD2_213_T5D, GxEPD2_213_T5D::HEIGHT> display(
  GxEPD2_213_T5D(/*CS=*/ 5, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)
);

// Button for Wi-Fi reset (GPIO 39 is input-only)
#define BUTTON_PIN 39

// ---- VERSION TAG ----
// Change this string whenever you upload a new build.
// Forces WiFi credentials to be reset on first boot of that build.
const char* BUILD_TAG = "build_v1.0";

// helper function to show text on screen
void showMessage(const char* msg) {
  display.setRotation(1);  // adjust if text appears sideways
  display.setFont(NULL);   // built-in font
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.println(msg);
  display.display();
  Serial.println(msg);
}

#include <Preferences.h>
Preferences prefs;

void resetCredentials(WiFiManager& wifiManager) {
  wifiManager.resetSettings();   // clear WiFi credentials
  prefs.begin("app", false);
  prefs.putString("build_tag", BUILD_TAG); // mark current build
  prefs.end();
}

void setup() {
  Serial.begin(115200);
  display.init(115200);

  pinMode(BUTTON_PIN, INPUT); // IO39 is input-only

  showMessage("Starting WiFi setup...");

  WiFiManager wifiManager;

  // --- Check if firmware build has changed ---
  prefs.begin("app", false);
  String savedTag = prefs.getString("build_tag", "");
  if (savedTag != BUILD_TAG) {
    showMessage("New firmware build.\nResetting WiFi...");
    resetCredentials(wifiManager);
  }
  prefs.end();

  // --- Check if user pressed button at boot ---
  if (digitalRead(BUTTON_PIN) == LOW) { // assuming pull-up + button to GND
    Serial.println("PRESSED");
    showMessage("Button pressed!\nResetting WiFi...");
    resetCredentials(wifiManager);
  }

  // Try auto-connect, else start portal
  showMessage("Opening AP:\nT5-Setup-XXXX");

  if (!wifiManager.autoConnect("T5-Setup-XXXX", "")) {
    showMessage("WiFi setup failed.\nRestart device.");
    // optional: ESP.restart();
  } else {
    String ip = WiFi.localIP().toString();
    String successMsg = "Connected!\nIP: " + ip;
    showMessage(successMsg.c_str());
  }
}

void loop() {
  // normal device code here
}
