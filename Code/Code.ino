// ###########################################################################################################################################
// #
// # WordClock code for the 3 printables WordClock 16x16 matrix projects:
// # https://www.printables.com/de/model/350568-wordclock-16x16-led-matrix-2023-v1
// # https://www.printables.com/de/model/361861-wordclock-16x16-led-matrix-2023-v2
// # https://www.printables.com/de/model/450556-wordclock-16x16-led-matrix-2023-v3
// #
// # Code by https://github.com/AWSW-de
// #
// # Released under licenses:
// # GNU General Public License v3.0: https://github.com/AWSW-de/WordClock-16x16-LED-matrix-2023/blob/main/LICENSE and
// # Creative Commons Attribution-NonCommercial 3.0 Unported License http://creativecommons.org/licenses/by-nc/3.0
// # Important: NonCommercial — You may not use the material for commercial purposes !
// #
// ###########################################################################################################################################
/*

      ___           ___           ___           ___           ___           ___       ___           ___           ___     
     /\__\         /\  \         /\  \         /\  \         /\  \         /\__\     /\  \         /\  \         /\__\    
    /:/ _/_       /::\  \       /::\  \       /::\  \       /::\  \       /:/  /    /::\  \       /::\  \       /:/  /    
   /:/ /\__\     /:/\:\  \     /:/\:\  \     /:/\:\  \     /:/\:\  \     /:/  /    /:/\:\  \     /:/\:\  \     /:/__/     
  /:/ /:/ _/_   /:/  \:\  \   /::\~\:\  \   /:/  \:\__\   /:/  \:\  \   /:/  /    /:/  \:\  \   /:/  \:\  \   /::\__\____ 
 /:/_/:/ /\__\ /:/__/ \:\__\ /:/\:\ \:\__\ /:/__/ \:|__| /:/__/ \:\__\ /:/__/    /:/__/ \:\__\ /:/__/ \:\__\ /:/\:::::\__\
 \:\/:/ /:/  / \:\  \ /:/  / \/_|::\/:/  / \:\  \ /:/  / \:\  \  \/__/ \:\  \    \:\  \ /:/  / \:\  \  \/__/ \/_|:|~~|~   
  \::/_/:/  /   \:\  /:/  /     |:|::/  /   \:\  /:/  /   \:\  \        \:\  \    \:\  /:/  /   \:\  \          |:|  |    
   \:\/:/  /     \:\/:/  /      |:|\/__/     \:\/:/  /     \:\  \        \:\  \    \:\/:/  /     \:\  \         |:|  |    
    \::/  /       \::/  /       |:|  |        \::/__/       \:\__\        \:\__\    \::/  /       \:\__\        |:|  |    
     \/__/         \/__/         \|__|         ~~            \/__/         \/__/     \/__/         \/__/         \|__|    

*/


// ###########################################################################################################################################
// # Includes:
// #
// # You will need to add the following libraries to your Arduino IDE to use the project:
// # - Adafruit NeoPixel      // by Adafruit:                     https://github.com/adafruit/Adafruit_NeoPixel
// # - AsyncTCP               // by me-no-dev:                    https://github.com/me-no-dev/AsyncTCP
// # - ESPAsyncWebServer      // by me-no-dev:                    https://github.com/me-no-dev/ESPAsyncWebServer
// # - ESPUI                  // by s00500:                       https://github.com/s00500/ESPUI
// # - ArduinoJson            // by bblanchon:                    https://github.com/bblanchon/ArduinoJson
// # - LITTLEFS               // by lorol:                        https://github.com/lorol/LITTLEFS
// #
// ###########################################################################################################################################
#include <WiFi.h>               // Used to connect the ESP32 to your WiFi
#include <WebServer.h>          // ESP32 OTA update function
#include <Update.h>             // ESP32 OTA update function
#include <Adafruit_NeoPixel.h>  // Used to drive the NeoPixel LEDs
#include "time.h"               // Used for NTP time requests
#include <AsyncTCP.h>           // Used for the internal web server
#include <ESPAsyncWebServer.h>  // Used for the internal web server
#include <DNSServer.h>          // Used for the internal web server
#include <ESPUI.h>              // Used for the internal web server
#include "esp_log.h"            // Disable WiFi debug warnings
#include <Preferences.h>        // Used to save the configuration to the ESP32 flash
#include "settings.h"           // Settings are stored in a seperate file to make to code better readable and to be able to switch to other settings faster


// ###########################################################################################################################################
// # Version number of the code:
// ###########################################################################################################################################
const char* WORD_CLOCK_VERSION = "V2023.11.4";


// ###########################################################################################################################################
// # Internal web server settings:
// ###########################################################################################################################################
AsyncWebServer server(80);       // Web server for config
WebServer otaserver(8080);       // Web OTA ESP32 update server
AsyncWebServer ledserver(2023);  // Web server for HTML commands
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;


// ###########################################################################################################################################
// # Declarations and variables used in the functions:
// ###########################################################################################################################################
Preferences preferences;
int iHour = 0;
int iMinute = 0;
int iSecond = 0;
int iDay = 23;
int iMonth = 11;
int iYear = 2022;
bool updatedevice = true;
bool updatenow = false;
bool updatemode = false;
bool changedvalues = false;
String iStartTime = " ";
int intensity, intensity_web;
int set_web_intensity = 0;
int usenightmode, day_time_start, day_time_stop, statusNightMode;
int statusLabelID, statusNightModeID, statusLanguageID, intensity_web_HintID, DayNightSectionID, LEDsettingsSectionID;
int sliderBrightnessDayID, switchNightModeID, sliderBrightnessNightID, call_day_time_startID, call_day_time_stopID;
char* selectLang;
bool WiFIsetup = false;

int minuteCountDown = 0; // global variable, not for flash!

// fixed values
const int max_cols = 16;
const int max_rows = 8;
const int effects_delay = 50;
const uint32_t color_red = strip.Color(255, 0, 0);
const uint32_t color_green = strip.Color(0, 255, 0);
const uint32_t color_black = strip.Color(0, 0, 0);

struct position_t
{
  int x;
  int y;
  int len;
};
  
enum class time_parts { prefix1, prefix2, five_min, quarter_prefix, quarter, three_quarters, ten_min, twenty, past, to, half, one1, o_clock };


// ###########################################################################################################################################
// # Setup function that runs once at startup of the ESP:
// ###########################################################################################################################################
void setup() {
  Serial.begin(115200);
  delay(250);
  Serial.println("######################################################################");
  Serial.print("# WordClock startup of version: ");
  Serial.println(WORD_CLOCK_VERSION);
  Serial.println("######################################################################");
  preferences.begin("wordclock", false);  // Init ESP32 flash
  getFlashValues();                       // Read settings from flash
  strip.begin();                          // Init the LEDs
  strip.show();                           // Init the LEDs --> Set them to OFF
  intensity = getSetting(setting_type::intensity_day);              // Set the intenity to day mode for startup
  strip.setBrightness(intensity);         // Set LED brightness

  initEffects();
  delay(200);
  allEffects();

  WIFI_SETUP();                           // WiFi login and startup of web services
}


// ###########################################################################################################################################
// # Loop function which runs all the time after the startup was done:
// ###########################################################################################################################################
void loop() {
  if ((WiFIsetup == true) || (testTime == 1)) {
    printLocalTime();                               // Locally get the time (NTP server requests done 1x per hour)
    if (updatedevice == true) {                     // Allow display updates (normal usage)
      if (changedvalues == true) setFlashValues();  // Write settings to flash
      update_display();                             // Update display (1x per minute regulary)
    }
    if (updatemode == true) otaserver.handleClient();  // ESP32 OTA update
  }
  dnsServer.processNextRequest();  // Update the web server
}


// ###########################################################################################################################################
// # EOF - You have successfully reached the end of the code - well done ;-)
// ###########################################################################################################################################