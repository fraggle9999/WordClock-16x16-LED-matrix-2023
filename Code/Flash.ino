// ###########################################################################################################################################
// # Read settings from flash:
// ###########################################################################################################################################
void getFlashValues() {
  if (debugtexts == 1) Serial.println("Read settings from flash: START");
  
//  for (auto& [setting_name, single_setting] : all_settings)
//    single_setting.val = preferences.getUInt(setting_name, single_setting.default_val);
  for (auto& single_setting : all_settings)
  {
    const auto* key = single_setting.second.name.c_str();
    Serial.println(key);
    const auto val = preferences.getInt(key, single_setting.second.default_val);
    Serial.println(val);
    single_setting.second.val = val;
  }
  
  usenightmode = preferences.getUInt("usenightmode", usenightmode_default);
  day_time_start = preferences.getUInt("day_time_start", day_time_start_default);
  day_time_stop = preferences.getUInt("day_time_stop", day_time_stop_default);

  if (debugtexts == 1) Serial.println("Read settings from flash: END");
}


// ###########################################################################################################################################
// # Write settings to flash:
// ###########################################################################################################################################
void setFlashValues() {
  if (debugtexts == 1) Serial.println("Write settings to flash: START");
  changedvalues = false;

  for (const auto& single_setting : all_settings)
  {
    const auto* key = single_setting.second.name.c_str();
    Serial.println(key);
    const auto val = single_setting.second.val;
    Serial.println(val);
    preferences.remove(key); // in case it was formerly stored as other data type
    if (preferences.putInt(key, val) == 0)
      Serial.println("error");
    else
      Serial.println("OK");
  }

  preferences.putUInt("usenightmode", usenightmode);
  preferences.putUInt("day_time_start", day_time_start);
  preferences.putUInt("day_time_stop", day_time_stop);

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
