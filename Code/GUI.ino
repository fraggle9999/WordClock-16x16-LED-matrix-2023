// ###########################################################################################################################################
// # GUI: Reset the WordClock settings:
// ###########################################################################################################################################
int WordClockResetCounter = 0;
void buttonWordClockReset(Control* sender, int type, void* param) {
  updatedevice = false;
  delay(100);
  if (WordClockResetCounter == 0) ShowReset(strip.Color(255, 0, 0));
  if (WordClockResetCounter == 1) ShowReset(strip.Color(0, 255, 0));
  switch (type) {
    case B_DOWN:
      break;
    case B_UP:
      if (WordClockResetCounter == 1) {
        Serial.println("Status: WORDCLOCK SETTINGS RESET REQUEST EXECUTED");
        // Save stored values for WiFi:
        String tempDelWiFiSSID = preferences.getString("WIFIssid");
        String tempDelWiFiPASS = preferences.getString("WIFIpass");
        preferences.clear();
        delay(100);

        preferences.putString("WIFIssid", tempDelWiFiSSID);  // Restore entered WiFi SSID
        preferences.putString("WIFIpass", tempDelWiFiPASS);  // Restore entered WiFi password

        for (const auto& single_setting : all_settings)
          preferences.putInt(single_setting.second.name.c_str(), single_setting.second.default_val);

        preferences.putUInt("redVal_time", redVal_time_default);
        preferences.putUInt("greenVal_time", greenVal_time_default);
        preferences.putUInt("blueVal_time", blueVal_time_default);
        preferences.putUInt("redVal_back", redVal_back_default);
        preferences.putUInt("greenVal_back", greenVal_back_default);
        preferences.putUInt("blueVal_back", blueVal_back_default);
        preferences.putUInt("usenightmode", usenightmode_default);
        preferences.putUInt("day_time_stop", day_time_stop_default);
        preferences.putUInt("day_time_stop", day_time_stop_default);

        preferences.putUInt("FixedHourColor_redVal", FixedHourColor_redVal_default);
        preferences.putUInt("FixedHourColor_greenVal", FixedHourColor_greenVal_default);
        preferences.putUInt("FixedHourColor_blueVal", FixedHourColor_blueVal_default);

        for (int i = 0; i < 4; ++i)
        {
          std::string Prefix = "FixedMinuteColor" + std::to_string(i);
          preferences.putUInt(std::string(Prefix + "_redVal").c_str(), FixedMinuteColor_redVal_default[i]); 
          preferences.putUInt(std::string(Prefix + "_greenVal").c_str(), FixedMinuteColor_greenVal_default[i]); 
          preferences.putUInt(std::string(Prefix + "_blueVal").c_str(), FixedMinuteColor_blueVal_default[i]); 
        }

        delay(100);
        preferences.end();
        Serial.println("####################################################################################################");
        Serial.println("# WORDCLOCK SETTING WERE SET TO DEFAULT... WORDCLOCK WILL NOW RESTART... PLEASE CONFIGURE AGAIN... #");
        Serial.println("####################################################################################################");
        ClearDisplay();
        strip.show();
        delay(250);
        ESP.restart();
      } else {
        Serial.println("Status: WORDCLOCK SETTINGS RESET REQUEST");
        ESPUI.print(statusLabelID, "WORDCLOCK SETTINGS RESET REQUESTED");
        ESPUI.updateButton(sender->id, "! Press button once more to apply settings reset !");
        WordClockResetCounter = WordClockResetCounter + 1;
      }
      break;
  }
}


// ###########################################################################################################################################
// # GUI: Language selection
// ###########################################################################################################################################
void call_language_select(Control* sender, int type) {
  updatedevice = false;
  delay(1000);
  int langLEDlayout = sender->value.toInt();
  putSetting(setting_type::langLEDlayout, langLEDlayout);

  // Set layout language text in gui:
  if (langLEDlayout == 0) selectLang = "German";
  if (langLEDlayout == 1) selectLang = "English";
  if (langLEDlayout == 2) selectLang = "Dutch";
  if (langLEDlayout == 3) selectLang = "Swedish";
  if (langLEDlayout == 4) selectLang = "Italian";
  if (langLEDlayout == 5) selectLang = "French";
  if (langLEDlayout == 6) selectLang = "Swiss German";
  if (langLEDlayout == 7) selectLang = "Chinese";
  if (langLEDlayout == 8) selectLang = "Swabian German";
  if (langLEDlayout == 9) selectLang = "Bavarian";
  ESPUI.print(statusLanguageID, selectLang);
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Reset the WiFi settings of the WordClock:
// ###########################################################################################################################################
void buttonWiFiReset(Control* sender, int type, void* param) {
  updatedevice = false;
  Serial.println("Status: WIFI SETTINGS RESET REQUEST");
  ShowReset(strip.Color(0, 255, 0));
  WiFi.disconnect();  // DISCONNECT FROM WIFI
  delay(1000);
  preferences.putString("WIFIssid", "");                // Reset WiFi SSID
  preferences.putString("WIFIpass", "");                // Reste WiFi password
  preferences.putUInt(all_settings[setting_type::useshowip].name.c_str(), all_settings[setting_type::useshowip].default_val);  // Show IP-address again
  preferences.end();
  Serial.println("Status: WIFI SETTINGS RESET REQUEST EXECUTED");
  Serial.println("####################################################################################################");
  Serial.println("# WIFI SETTING WERE SET TO DEFAULT... WORDCLOCK WILL NOW RESTART... PLEASE CONFIGURE WIFI AGAIN... #");
  Serial.println("####################################################################################################");
  delay(500);
  ESP.restart();
}


// ###########################################################################################################################################
// # GUI: Update the WordClock:
// ###########################################################################################################################################
void buttonUpdate(Control* sender, int type, void* param) {
  preferences.end();
  updatedevice = false;
  ESPUI.print(statusLabelID, "Update requested");
  ESPUI.updateButton(sender->id, "Update mode active now - Use the update url: >>>");
  if (updatemode == false) {
    updatemode = true;
    int32_t c = strip.Color(0, 0, 255);
    showTextCharByChar("UPDATE", 500, c);
    switchLEDXY(0, 0, 1, 1); // top left
    switchLEDXY(15, 0, 1, 1); // top right
    switchLEDXY(0, 7, 1, 1); // bottom left
    switchLEDXY(15, 7, 1, 1); // bottom right
    
    showCharMatrix(charMap['U'], 5, 2, c);
    strip.show();

    Serial.println("Status: Update request");
  }
}


// ###########################################################################################################################################
// # GUI: Night mode switch:
// ###########################################################################################################################################
void switchNightMode(Control* sender, int value) {
  updatedevice = false;
  delay(1000);
  switch (value) {
    case S_ACTIVE:
      usenightmode = 1;
      if ((iHour >= day_time_start) && (iHour <= day_time_stop)) {
        intensity = getSetting(setting_type::intensity_day);
        if ((iHour == 0) && (day_time_stop == 23)) intensity = getSetting(setting_type::intensity_night);  // Special function if day_time_stop set to 23 and time is 24, so 0...
      } else {
        intensity = getSetting(setting_type::intensity_night);
      }
      break;
    case S_INACTIVE:
      intensity = getSetting(setting_type::intensity_day);
      usenightmode = 0;
      break;
  }
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Init variables:
// ###########################################################################################################################################
void __initVars() {
  minuteCountDown = 0;
  
  ESPUI.updateVisibility(text_colour_background, getSetting(setting_type::RandomColor) == 0);
  ESPUI.updateVisibility(text_colour_time, getSetting(setting_type::RandomColor) == 0);
  ESPUI.jsonReload();

  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Generic switcher:
// ###########################################################################################################################################
void call_generic_switcher(Control* sender, int value) {
  updatedevice = false;
  delay(1000);
  const auto setting = UI2settingMap[sender->id];
  switch (value) {
    case S_ACTIVE:
      putSetting(setting, 1);
      break;
    case S_INACTIVE:
      putSetting(setting, 0);
      break;
  }
  __initVars();
}


// ###########################################################################################################################################
// # GUI: Generic number:
// ###########################################################################################################################################
void call_generic_number(Control* sender, int value) {
  updatedevice = false;
  delay(1000);
  const auto setting = UI2settingMap[sender->id];
  putSetting(setting, sender->value.toInt());
  __initVars();
}


// ###########################################################################################################################################
// # GUI: Generic color:
// ###########################################################################################################################################
void call_generic_color(Control* sender, int type) {
  updatedevice = false;
  delay(1000);
  const auto setting = UI2settingMap[sender->id];
  putSetting(setting, sender->value.toInt());
  __initVars();
}


// ###########################################################################################################################################
// # GUI: Convert hex color value to RGB int values:
// ###########################################################################################################################################
void getRGB(const std::string& hexValue, int& red, int& green, int& blue) {
  updatedevice = false;
  delay(1000);
  red = hexcolorToInt(hexValue.substr(0, 2));
  green = hexcolorToInt(hexValue.substr(2, 2));
  blue = hexcolorToInt(hexValue.substr(4, 2));
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Convert hex color value to RGB int values - TIME:
// ###########################################################################################################################################
void getRGBTIME(const std::string& hexValue) {
  getRGB(hexValue, redVal_time, greenVal_time, blueVal_time);
}


// ###########################################################################################################################################
// # GUI: Convert hex color value to RGB int values - BACKGROUND:
// ###########################################################################################################################################
void getRGBBACK(const std::string& hexValue) {
  getRGB(hexValue, redVal_back, greenVal_back, blueVal_back);
}


// ###########################################################################################################################################
// # GUI: Convert hex color value to RGB int values - helper function:
// ###########################################################################################################################################
int hexcolorToInt(const std::string& hexValue) {
  if (hexValue.size() != 2)
    return 0;

  int uVal = hexValue[0];
  int lVal = hexValue[1];
  uVal = uVal > 64 ? uVal - 55 : uVal - 48;
  uVal = uVal << 4;
  lVal = lVal > 64 ? lVal - 55 : lVal - 48;
  return uVal + lVal;
}


// ###########################################################################################################################################
// # GUI: Color change for time color:
// ###########################################################################################################################################
void colCallTIME(Control* sender, int type) {
  getRGBTIME(sender->value.c_str());
}


// ###########################################################################################################################################
// # GUI: Color change for background color:
// ###########################################################################################################################################
void colCallBACK(Control* sender, int type) {
  getRGBBACK(sender->value.c_str());
}


// ###########################################################################################################################################
// # GUI: Time Day Mode Start
// ###########################################################################################################################################
void call_day_time_start(Control* sender, int type) {
  updatedevice = false;
  delay(1000);
  day_time_start = sender->value.toInt();
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Time Day Mode Stop
// ###########################################################################################################################################
void call_day_time_stop(Control* sender, int type) {
  updatedevice = false;
  delay(1000);
  day_time_stop = sender->value.toInt();
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Convert IP-address value to string:
// ###########################################################################################################################################
String IpAddress2String(const IPAddress& ipAddress) {
  return String(ipAddress[0]) + String(".") + String(ipAddress[1]) + String(".") + String(ipAddress[2]) + String(".") + String(ipAddress[3]);
}


// ###########################################################################################################################################
// # GUI: Restart the WordClock:
// ###########################################################################################################################################
void buttonRestart(Control* sender, int type, void* param) {
  updatedevice = false;
  ShowReset(strip.Color(0, 255, 0));
  if (changedvalues == true) setFlashValues();  // Write settings to flash
  preferences.end();
  delay(250);
  ESP.restart();
}


