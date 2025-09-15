#include <WiFi.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <GxEPD2_BW.h>

// LilyGO T5 (2.13") wiring
GxEPD2_BW<GxEPD2_213_T5D, GxEPD2_213_T5D::HEIGHT> display(
  GxEPD2_213_T5D(/*CS=*/ 5, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)
);

// helper function to show text on screen
void showMessage(const char* msg) {
  display.setRotation(1);  // adjust rotation if text is sideways
  display.setFont(NULL);   // use built-in font
  display.setTextSize(1);  // scale factor
  display.setCursor(0, 20);
  display.fillScreen(GxEPD_WHITE); // clear
  display.setTextColor(GxEPD_BLACK);
  display.println(msg);
  display.display();       // push buffer to e-ink
  Serial.println(msg);     // also print to Serial
}

void setup() {
  Serial.begin(115200);
  display.init(115200); // init with same baud as Serial

  showMessage("Starting WiFi setup...");

  WiFiManager wifiManager;

  // Uncomment to force portal every boot
  // wifiManager.resetSettings();

  showMessage("Opening AP:\nT5-Setup-XXXX");

  if (!wifiManager.autoConnect("T5-Setup-XXXX", "setup_password")) {
    showMessage("WiFi setup failed.\nRestart device.");
    // optional: ESP.restart();
  } else {
    String ip = WiFi.localIP().toString();
    String successMsg = "Connected!\nIP: " + ip;
    showMessage(successMsg.c_str());
  }
}

void loop() {
  // normal device code
}
