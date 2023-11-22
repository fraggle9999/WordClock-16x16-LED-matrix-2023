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

        preferences.putUInt("usenightmode", usenightmode_default);
        preferences.putUInt("day_time_stop", day_time_stop_default);
        preferences.putUInt("day_time_stop", day_time_stop_default);

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
  
  ESPUI.updateVisibility(all_settings[setting_type::colorBack].UI_ID, getSetting(setting_type::RandomColor) == 0);
  ESPUI.updateVisibility(all_settings[setting_type::colorTime].UI_ID, getSetting(setting_type::RandomColor) == 0);

  ESPUI.updateVisibility(all_settings[setting_type::colorHour].UI_ID, getSetting(setting_type::useFixedHourColor) == 1);
  ESPUI.updateVisibility(all_settings[setting_type::colorMin1].UI_ID, getSetting(setting_type::useFixedMinuteColors) == 1);
  ESPUI.updateVisibility(all_settings[setting_type::colorMin2].UI_ID, getSetting(setting_type::useFixedMinuteColors) == 1);
  ESPUI.updateVisibility(all_settings[setting_type::colorMin3].UI_ID, getSetting(setting_type::useFixedMinuteColors) == 1);
  ESPUI.updateVisibility(all_settings[setting_type::colorMin4].UI_ID, getSetting(setting_type::useFixedMinuteColors) == 1);

//  ESPUI.jsonReload();

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

  std::string color_str = sender->value.c_str();
  Serial.print("Selected color: ");
  Serial.println(color_str.c_str());
  int colorVal = std::stoi(color_str.substr(1), nullptr, 16); // cut off '#' prefix
  Serial.print("Color value: ");
  Serial.println(colorVal);

  Serial.print("Sender id: ");
  Serial.println(sender->id);

  const auto setting = UI2settingMap[sender->id];
  putSetting(setting, colorVal);
  __initVars();
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


