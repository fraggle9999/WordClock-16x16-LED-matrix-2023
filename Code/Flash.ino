struct setting
{
    int val{};
    int default{};
};

std::map<std::string, setting> all_settings = { 
    { "langLEDlayout", { 0, langLEDlayout_default} } 
};


// ###########################################################################################################################################
// # Read settings from flash:
// ###########################################################################################################################################
void getFlashValues() {
  if (debugtexts == 1) Serial.println("Read settings from flash: START");
  
  for (auto& [setting_name, single_setting] : all_settings)
    single_setting.val = preferences.getUInt(setting_name, single_setting.default);
  
  langLEDlayout = preferences.getUInt("langLEDlayout", langLEDlayout_default);
  redVal_time = preferences.getUInt("redVal_time", redVal_time_default);
  greenVal_time = preferences.getUInt("greenVal_time", greenVal_time_default);
  blueVal_time = preferences.getUInt("blueVal_time", blueVal_time_default);
  redVal_back = preferences.getUInt("redVal_back", redVal_back_default);
  greenVal_back = preferences.getUInt("greenVal_back", greenVal_back_default);
  blueVal_back = preferences.getUInt("blueVal_back", blueVal_back_default);
  intensity_day = preferences.getUInt("intensity_day", intensity_day_default);
  intensity_night = preferences.getUInt("intensity_night", intensity_night_default);
  usenightmode = preferences.getUInt("usenightmode", usenightmode_default);
  day_time_start = preferences.getUInt("day_time_start", day_time_start_default);
  day_time_stop = preferences.getUInt("day_time_stop", day_time_stop_default);
  useshowip = preferences.getUInt("useshowip", useshowip_default);
  useStartupText = preferences.getUInt("useStartupText", useStartupText_default);
  usesinglemin = preferences.getUInt("usesinglemin", usesinglemin_default);
  RandomColor = preferences.getUInt("RandomColor", RandomColor_default);

  UseFixedHourColor = preferences.getUInt("UseFixedHourColor", UseFixedHourColor_default);
  FixedHourColor_redVal = preferences.getUInt("FixedHourColor_redVal", FixedHourColor_redVal_default);
  FixedHourColor_greenVal = preferences.getUInt("FixedHourColor_greenVal", FixedHourColor_greenVal_default);
  FixedHourColor_blueVal = preferences.getUInt("FixedHourColor_blueVal", FixedHourColor_blueVal_default);

  UseFixedMinuteColors = preferences.getUInt("UseFixedMinuteColors", UseFixedMinuteColors_default);
  for (int i = 0; i < 4; ++i)
  {
    std::string Prefix = "FixedMinuteColor" + std::to_string(i);
    FixedMinuteColor_redVal[i] = preferences.getUInt(std::string(Prefix + "_redVal").c_str(), FixedMinuteColor_redVal_default[i]); 
    FixedMinuteColor_greenVal[i] = preferences.getUInt(std::string(Prefix + "_greenVal").c_str(), FixedMinuteColor_greenVal_default[i]); 
    FixedMinuteColor_blueVal[i] = preferences.getUInt(std::string(Prefix + "_blueVal").c_str(), FixedMinuteColor_blueVal_default[i]);  
  }

  AnimationDelay = preferences.getUInt("AnimationDelay", AnimationDelay_default);
  TextScrollDelay = preferences.getUInt("TextScrollDelay", TextScrollDelay_default);

  BlinkingSecond = preferences.getUInt("BlinkingSecond", BlinkingSecond_default);
  MinuteCountdown = preferences.getUInt("MinuteCountdown", MinuteCountdown_default);

  ShowScrollingTimeEveryMinute = preferences.getUInt("ShowScrollingTimeEveryMinute", ShowScrollingTimeEveryMinute_default);
  
  if (debugtexts == 1) Serial.println("Read settings from flash: END");
}


// ###########################################################################################################################################
// # Write settings to flash:
// ###########################################################################################################################################
void setFlashValues() {
  if (debugtexts == 1) Serial.println("Write settings to flash: START");
  changedvalues = false;

  for (const auto& [setting_name, single_setting] : all_settings)
    preferences.putUInt(setting_name, single_setting.val);

  preferences.putUInt("langLEDlayout", langLEDlayout);
  preferences.putUInt("redVal_time", redVal_time);
  preferences.putUInt("greenVal_time", greenVal_time);
  preferences.putUInt("blueVal_time", blueVal_time);
  preferences.putUInt("redVal_back", redVal_back);
  preferences.putUInt("greenVal_back", greenVal_back);
  preferences.putUInt("blueVal_back", blueVal_back);
  preferences.putUInt("intensity_day", intensity_day);
  preferences.putUInt("intensity_night", intensity_night);
  preferences.putUInt("usenightmode", usenightmode);
  preferences.putUInt("day_time_start", day_time_start);
  preferences.putUInt("day_time_stop", day_time_stop);
  preferences.putUInt("useshowip", useshowip);
  preferences.putUInt("useStartupText", useStartupText);
  preferences.putUInt("usesinglemin", usesinglemin);
  preferences.putUInt("RandomColor", RandomColor);

  preferences.putUInt("UseFixedHourColor", UseFixedHourColor);
  preferences.putUInt("FixedHourColor_redVal", FixedHourColor_redVal);
  preferences.putUInt("FixedHourColor_greenVal", FixedHourColor_greenVal);
  preferences.putUInt("FixedHourColor_blueVal", FixedHourColor_blueVal);

  preferences.putUInt("UseFixedMinuteColors", UseFixedMinuteColors);
  for (int i = 0; i < 4; ++i)
  {
    std::string Prefix = "FixedMinuteColor" + std::to_string(i);
    preferences.putUInt(std::string(Prefix + "_redVal").c_str(), FixedMinuteColor_redVal[i]); 
    preferences.putUInt(std::string(Prefix + "_greenVal").c_str(), FixedMinuteColor_greenVal[i]); 
    preferences.putUInt(std::string(Prefix + "_blueVal").c_str(), FixedMinuteColor_blueVal[i]); 
  }

  preferences.putUInt("AnimationDelay", AnimationDelay);
  preferences.putUInt("TextScrollDelay", TextScrollDelay);

  preferences.putUInt("BlinkingSecond", BlinkingSecond);
  preferences.putUInt("MinuteCountdown", MinuteCountdown);

  preferences.putUInt("ShowScrollingTimeEveryMinute", ShowScrollingTimeEveryMinute);

  if (debugtexts == 1) Serial.println("Write settings to flash: END");
  if (usenightmode == 1) {
    if ((iHour >= day_time_start) && (iHour <= day_time_stop)) {
      ESPUI.print(statusNightModeID, "Day time");
      if ((iHour == 0) && (day_time_stop == 23)) ESPUI.print(statusNightModeID, "Night time");  // Special function if day_time_stop set to 23 and time is 24, so 0...
    } else {
      ESPUI.print(statusNightModeID, "Night time");
    }
  } else {
    ESPUI.print(statusNightModeID, "Night mode not used");
  }
  updatenow = true;  // Update display now...
}


