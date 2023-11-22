// ###########################################################################################################################################
// # Read settings from flash:
// ###########################################################################################################################################
void getFlashValues() {
  if (debugtexts == 1) Serial.println("Read settings from flash: START");
  
  for (auto& single_setting : all_settings)
  {
    const auto* key = single_setting.second.name.c_str();
    const auto val = preferences.getInt(key, single_setting.second.default_val);
    single_setting.second.val = val;
  }
  
  if (debugtexts == 1) Serial.println("Read settings from flash: END");
}


// ###########################################################################################################################################
// # Write settings to flash:
// ###########################################################################################################################################
void updateNightModeText()
{
  if (getSetting(setting_type::usenightmode) == 1) {
    if ((iHour >= getSetting(setting_type::day_time_start)) && (iHour <= getSetting(setting_type::day_time_stop))) {
      ESPUI.print(statusNightModeID, "Day time");
      if ((iHour == 0) && (getSetting(setting_type::day_time_stop) == 23)) ESPUI.print(statusNightModeID, "Night time");  // Special function if day_time_stop set to 23 and time is 24, so 0...
    } else {
      ESPUI.print(statusNightModeID, "Night time");
    }
  } else {
    ESPUI.print(statusNightModeID, "Night mode not used");
  }
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
    const auto val = single_setting.second.val;
    preferences.remove(key); // in case it was formerly stored as other data type
  }

  if (debugtexts == 1) Serial.println("Write settings to flash: END");
  updateNightModeText();
  updatenow = true;  // Update display now...
}
