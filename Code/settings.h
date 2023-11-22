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
// # Compatible with WordClock version: V2.6.3
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
// # LED language layout default: !!! SET YOUR DEFAULT LANGUAGE in map beyond !!!
// ###########################################################################################################################################
// Available languages:
// ####################
// 0 = DE   - GERMAN
// 1 = EN   - ENGLISH
// 2 = NL   - DUTCH
// 3 = SWE  - SWEDISH
// 4 = IT   - ITALIAN
// 5 = FR   - FRENCH
// 6 = GSW  - SWISS GERMAN
// 7 = CN   - CHINESE
// 8 = SCHWB - SWABIAN GERMAN
// 9 = BAY - BAVARIAN
// NOTE: You may need to use the "Reset WordClock settings"-button to update the value on the device

#pragma once

// ###########################################################################################################################################
// # Hardware settings:
// ###########################################################################################################################################
#define LEDPIN 32      // Arduino-Pin connected to the NeoPixels
#define NUMPIXELS 256  // How many NeoPixels are attached to the Arduino
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);


// ###########################################################################################################################################
// # LED intensity setting:
// ###########################################################################################################################################
#define LEDintensityLIMIT 64  // off 255 max. !!! Do NOT change this if you don't know how much power drain this may cause to avoid damage !!!
// Limit the intensity level to be able to select in the configuration to avoid to much power drain and to avoid hardware failures over time.
// In case you want to use the device over longer times with a higher intensity or even higher, you will need to consider a much more
// powerful power supply, a better cable to connect the device and you have to power the matrix directly to 5V of the power supply.
// Otherwise the ESP32 can fail over time. Therefore this setting is limited and should not be raised. !!! You have been warned !!!
// In case you really need a higher value, you need to make sure to power the matrix directly so that the power is not served over the ESP32
// to the matrix to avoid its damage over time.
// Please think about if you really need such a high intensity value. The WordClocks used from me run at a maximum intensity of 33 which is
// really bright in my eyes and all levels above 64 i could not see really an advantage anymore that the display is better able to view...
// !!! Make sure to use propper components !!!


// ###########################################################################################################################################
// # LED defaults:
// ###########################################################################################################################################
int usenightmode_default = 1;     // Use the night mode to reduce LED intensity during set times
int day_time_start_default = 7;   // Define day mode start --> time before is then night mode if used
int day_time_stop_default = 22;   // Define day mode end --> time after is then night mode if used

// ###########################################################################################################################################
// # Various default settings:
// ###########################################################################################################################################
int maxWiFiconnectiontries = 30;  // Maximum connection tries to logon to the set WiFi. After the amount of tries is reached the WiFi settings will be deleted!


// ###########################################################################################################################################
// # Variables declaration:
// ###########################################################################################################################################
#define DEFAULT_AP_NAME "WordClock"  // WiFi access point name of the ESP32
const char* hostname = "WordClock";  // Hostname to be set in your router


// ###########################################################################################################################################
// # NTP time server settings:
// ###########################################################################################################################################
const char* Timezone = "CET-1CEST,M3.5.0,M10.5.0/3";  // You can check a list of timezone string variables here:  https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
const char* NTPserver = "pool.ntp.org";               // Time server address. Choose the closest one to you here: https://gist.github.com/mutin-sa/eea1c396b1e610a2da1e5550d94b0453


// ###########################################################################################################################################
// # Test functions:
// ###########################################################################################################################################
int debugtexts = 0;          // Show more debug information in serial monitor
int testTime = 0;            // LED time text output test
int testPrintTimeTexts = 0;  // Print the time texts shown on the display in the serial monitor (Currently DE + IT only)
int testDayNightmode = 0;    // Test the day/night mode settings
// Test a special time:
int testspecialtime = 0;  // Use test a special time function
int test_hour = 9;       // Test a special time: hour
int test_minute = 38;     // Test a special time: minute


// ###########################################################################################################################################

enum struct setting_type { langLEDlayout, useshowip, useStartupText, usesinglemin, useFixedHourColor, showScrollingTimeEveryXMinutes, 
    useFixedMinuteColors, animationDelay, textScrollDelay, blinkingSecond, minuteCountdown, intensity_day, intensity_night, RandomColor,
    colorTime, colorBack, colorHour, colorMin1, colorMin2, colorMin3, colorMin4 };

struct setting
{
    std::string name;
    int val;
    int default_val;
    int UI_ID; // for GUI
};

// ATTENTION! Key names must not be longer than 15 characters!
std::map<setting_type, setting> all_settings = { 
    { setting_type::langLEDlayout, { "langLEDlayout", {}, 0, {} } },
    { setting_type::useshowip, { "useshowip", {}, 1, {} } },
    { setting_type::useStartupText, { "useStartupText", {}, 1, {} } },
    { setting_type::usesinglemin, { "usesinglemin", {}, 1, {} } },
    { setting_type::useFixedHourColor, { "fixedHourCol", {}, 0, {} } },
    { setting_type::showScrollingTimeEveryXMinutes, { "scrollTime", {}, 0, {} } }, 
    { setting_type::useFixedMinuteColors, { "fixedMinCols", {}, 0, {} } },
    { setting_type::animationDelay, { "animationDelay", {}, 20, {} } },
    { setting_type::textScrollDelay, { "textScrollDelay", {}, 100, {} } },
    { setting_type::blinkingSecond, { "blinkingSecond", {}, 0, {} } },
    { setting_type::minuteCountdown, { "minuteCountdown", {}, 0, {} } },
    { setting_type::intensity_day, { "intensity_day", {}, 22, {} } },
    { setting_type::intensity_night, { "intensity_night", {}, 5, {} } },
    { setting_type::RandomColor, { "RandomColor", {}, 0, {} } },
    { setting_type::colorTime, { "colorTime", {}, 0x00ffff, {} } },
    { setting_type::colorBack, { "colorBack", {}, 0x000000, {} } },
    { setting_type::colorHour, { "colorHour", {}, 0xff0000, {} } },
    { setting_type::colorMin1, { "colorMin1", {}, 0xff0000, {} } },
    { setting_type::colorMin2, { "colorMin2", {}, 0xffffff, {} } },
    { setting_type::colorMin3, { "colorMin3", {}, 0x00ff00, {} } },
    { setting_type::colorMin4, { "colorMin4", {}, 0xffff00, {} } }
};

std::map<int, setting_type> UI2settingMap;

// ###########################################################################################################################################
// # Get setting from map:
// ###########################################################################################################################################
inline int getSetting(const setting_type type) {
  return all_settings[type].val;
}


// ###########################################################################################################################################
// # Put setting to map:
// ###########################################################################################################################################
inline void putSetting(const setting_type type, const int val) {
  auto& setting = all_settings[type];

  // Serial.print("Setting name: ");
  // Serial.print(setting.name.c_str());
  // Serial.print(", old val: ");
  // Serial.println(setting.val);

  setting.val = val;

  // Serial.print("New val: ");
  // Serial.println(setting.val);
}


// ###########################################################################################################################################
// # EOF - You have successfully reached the end of the code - well done ;-)
// ###########################################################################################################################################