void setupNumber(const setting_type type, const char* caption, void (*callback)(Control*, int), const int min, const int max)
{
  all_settings[type].UI_ID = ESPUI.number(caption, callback, ControlColor::Dark, all_settings[type].val, min, max);
}


// ###########################################################################################################################################
// # Setup the internal web server configuration page:
// ###########################################################################################################################################
void setupWebInterface() {
  dnsServer.start(DNS_PORT, "*", apIP);


  // Section General:
  // ################
  ESPUI.separator("General:");

  // Status label:
  statusLabelID = ESPUI.label("Status:", ControlColor::Dark, "Operational");

  // WordClock version:
  ESPUI.label("Version", ControlColor::None, WORD_CLOCK_VERSION);



  // Section LED settings:
  // #####################
  LEDsettingsSectionID = ESPUI.separator("LED settings:");

  // Time color selector:
  char hex_time[7] = { 0 };
  sprintf(hex_time, "#%02X%02X%02X", redVal_time, greenVal_time, blueVal_time);
  text_colour_time = ESPUI.text("Time", colCallTIME, ControlColor::Dark, hex_time);
  ESPUI.setInputType(text_colour_time, "color");

  // Background color selector:
  char hex_back[7] = { 0 };
  sprintf(hex_back, "#%02X%02X%02X", redVal_back, greenVal_back, blueVal_back);
  text_colour_background = ESPUI.text("Background", colCallBACK, ControlColor::Dark, hex_back);
  ESPUI.setInputType(text_colour_background, "color");

  // Use random color mode:
  switchRandomColorID = ESPUI.switcher("Use random text color every new minute", &switchRandomColor, ControlColor::Dark, RandomColor);
  if (RandomColor == 1) {
    ESPUI.updateVisibility(text_colour_time, false);
    ESPUI.updateVisibility(text_colour_background, false);
  }

  // Show single minutes to display the minute exact time:
  switchSingleMinutesID = ESPUI.switcher("Show single minutes to display the minute exact time", &switchSingleMinutes, ControlColor::Dark, usesinglemin);

  // Show note when intensity is currently controlled via web-url usage and these internal settings get disabled:
  intensity_web_HintID = ESPUI.label("Manual settings disabled due to web URL usage:", ControlColor::Alizarin, "Restart WordClock or deactivate web control usage via http://" + IpAddress2String(WiFi.localIP()) + ":2023/config?LEDs=1");
  ESPUI.updateVisibility(intensity_web_HintID, false);



  // Section LED night mode settings:
  // ################################
  DayNightSectionID = ESPUI.separator("Day/Night LED brightness mode settings:");

  // Use night mode function:
  switchNightModeID = ESPUI.switcher("Use night mode to reduce brightness", &switchNightMode, ControlColor::Dark, usenightmode);

  // Intensity DAY slider selector: !!! DEFAULT LIMITED TO 64 of 255 !!!
  sliderBrightnessDayID = ESPUI.slider("Brightness during the day", &sliderBrightnessDay, ControlColor::Dark, intensity_day, 0, LEDintensityLIMIT);

  // Intensity NIGHT slider selector: !!! DEFAULT LIMITED TO 64 of 255 !!!
  sliderBrightnessNightID = ESPUI.slider("Brightness at night", &sliderBrightnessNight, ControlColor::Dark, intensity_night, 0, LEDintensityLIMIT);

  // Night mode status:
  statusNightModeID = ESPUI.label("Night mode status", ControlColor::Dark, "Night mode not used");

  // Day mode start time:
  call_day_time_startID = ESPUI.number("Day time starts at", call_day_time_start, ControlColor::Dark, day_time_start, 0, 11);

  // Day mode stop time:
  call_day_time_stopID = ESPUI.number("Day time ends at", call_day_time_stop, ControlColor::Dark, day_time_stop, 12, 23);



  // Section Startup:
  // ################
  ESPUI.separator("Startup:");

  // Startup WordClock text function:
  ESPUI.switcher("Show the 'WordClock' text on startup", &switchStartupText, ControlColor::Dark, useStartupText);

  // Show IP-address on startup:
  ESPUI.switcher("Show IP-address on startup", &switchShowIP, ControlColor::Dark, useshowip);



  // Special functions:
  // ################
  ESPUI.separator("Special functions:");

  setupNumber(setting_type::showScrollingTimeEveryXMinutes, "Show scrolling time every ... minutes (-1 = random, 0 = never)", call_scroll_time, -1, 7);

  // Section WiFi:
  // #############
  ESPUI.separator("WiFi:");

  // WiFi SSID:
  ESPUI.label("SSID", ControlColor::Dark, WiFi.SSID());

  // WiFi signal strength:
  ESPUI.label("Signal", ControlColor::Dark, String(WiFi.RSSI()) + "dBm");

  // Hostname:
  ESPUI.label("Hostname", ControlColor::Dark, hostname);

  // WiFi ip-address:
  ESPUI.label("IP-address", ControlColor::Dark, IpAddress2String(WiFi.localIP()));

  // WiFi MAC-address:
  ESPUI.label("MAC address", ControlColor::Dark, WiFi.macAddress());



  // Section smart home control via web URLs:
  // ########################################
  ESPUI.separator("Smart home control via web URLs:");

  // About note:
  ESPUI.label("About note", ControlColor::Dark, "Control WordClock from your smart home environment via web URLs.");

  // Functions note:
  ESPUI.label("Functions", ControlColor::Dark, "You can turn the LEDs off or on via http commands to reduce energy consumption.");

  // Usage note:
  ESPUI.label("Usage hints and examples", ControlColor::Dark, "http://" + IpAddress2String(WiFi.localIP()) + ":2023");



  // Section Time settings:
  // ######################
  ESPUI.separator("Time settings:");

  // NTP server:
  ESPUI.label("NTP server", ControlColor::Dark, NTPserver);

  // Time zone:
  ESPUI.label("Time zone", ControlColor::Dark, Timezone);

  // Time:
  ESPUI.label("Startup time", ControlColor::Dark, iStartTime);



  // Section Update:
  // ###############
  ESPUI.separator("Update:");

  // Update WordClock:
  ESPUI.button("Activate update mode", &buttonUpdate, ControlColor::Dark, "Activate update mode", (void*)1);

  // Update URL
  ESPUI.label("Update URL", ControlColor::Dark, "http://" + IpAddress2String(WiFi.localIP()) + ":8080");

  // AWSW software GitHub repository:
  ESPUI.label("Download newer software updates here", ControlColor::Dark, "https://github.com/AWSW-de/WordClock-16x16-LED-matrix-2023");



  // Section Language:
  // #################
  ESPUI.separator("Language:");

  int langLEDlayout = getSetting(setting_type::langLEDlayout);

  // Set layout language:
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

  // Language overview:
  ESPUI.addControl(ControlType::Label, "Available languages", "<center><table border='3' class='center' width='100%'><tr><th>Value:</th><th>Language:</th><th>Value:</th><th>Language:</th></tr><tr align='center'><td>0</td><td>German</td><td>1</td><td>English</td></tr><tr align='center'><td>2</td><td>Dutch</td><td>3</td><td>Swedish</td></tr><tr align='center'><td>4</td><td>Italian</td><td>5</td><td>French</td></tr><tr align='center'><td>6</td><td>Swiss German</td><td>7</td><td>Chinese</td></tr><tr align='center'><td>8</td><td>Swabian German</td><td>9</td><td>Bavarian</td></tr></table>", ControlColor::Dark, Control::noParent, 0);

  // Change language:
  ESPUI.number("Select your language", call_language_select, ControlColor::Dark, all_settings[setting_type::langLEDlayout].val, 0, 9);

  // Current language:
  statusLanguageID = ESPUI.label("Current layout language", ControlColor::Dark, selectLang);



  // Section Maintenance:
  // ####################
  ESPUI.separator("Maintenance:");

  // Restart WordClock:
  ESPUI.button("Restart", &buttonRestart, ControlColor::Dark, "Restart", (void*)1);

  // Reset WiFi settings:
  ESPUI.button("Reset WiFi settings", &buttonWiFiReset, ControlColor::Dark, "Reset WiFi settings", (void*)2);

  // Reset WordClock settings:
  ESPUI.button("Reset WordClock settings", &buttonWordClockReset, ControlColor::Dark, "Reset WordClock settings", (void*)3);



  // Section License:
  // ####################
  ESPUI.separator("License information:");

  // License information:
  ESPUI.label("License information", ControlColor::Dark, "NonCommercial — You may not use the project for commercial purposes!");



  // Update night mode status text on startup:
  if (usenightmode == 1) {
    if ((iHour >= day_time_start) && (iHour <= day_time_stop)) {
      ESPUI.print(statusNightModeID, "Day time");
      if ((iHour == 0) && (day_time_stop == 23)) ESPUI.print(statusNightModeID, "Night time");  // Special function if day_time_stop set to 23 and time is 24, so 0...
    } else {
      ESPUI.print(statusNightModeID, "Night time");
    }
  }

  // Deploy the page:
  ESPUI.begin("WordClock");
}


// ###########################################################################################################################################
// # HTML command web server:
// ###########################################################################################################################################
int ew = 0;  // Current extra word
String ledstatus = "ON";
void handleLEDupdate() {  // LED server pages urls:

  ledserver.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {  // Show a manual how to use these links:
    String message = "WordClock web configuration and query options examples:\n\n";
    message = message + "General:\n";
    message = message + "http://" + IpAddress2String(WiFi.localIP()) + ":2023 --> Shows this text\n\n";
    message = message + "Get the status of the WordClock LEDs:\n";
    message = message + "http://" + IpAddress2String(WiFi.localIP()) + ":2023/status --> Show the status of the LEDs (0 = OFF and 1 = ON).\n\n";
    message = message + "Turn the LEDs OFF or ON:\n";
    message = message + "http://" + IpAddress2String(WiFi.localIP()) + ":2023/config?LEDs=0 --> LED intensity is set to OFF which will turn the display off.\n";
    message = message + "http://" + IpAddress2String(WiFi.localIP()) + ":2023/config?LEDs=1 --> LED intensity is set to ON which will turn the display on again...\n";
    request->send(200, "text/plain", message);
  });

  ledserver.on("/config", HTTP_GET, [](AsyncWebServerRequest* request) {  // Configure background and time texts color and intensity:
    int paramsNr = request->params();
    // Serial.println(paramsNr);
    for (int i = 0; i < paramsNr; i++) {
      AsyncWebParameter* p = request->getParam(i);
      // Serial.print("Param name: ");
      // Serial.println(p->name());
      // Serial.print("Param value: ");
      // Serial.println(p->value());
      // Serial.println("------------------");
      if ((p->value().toInt() >= 0) && (p->value().toInt() <= 1)) {
        if ((String(p->name()) == "LEDs") && (p->value().toInt() == 0)) {
          set_web_intensity = 1;
          ledstatus = "OFF";
          ESPUI.updateVisibility(intensity_web_HintID, true);

          for (const auto& single_setting : all_settings)
          {
            const auto ID = single_setting.second.UI_ID;

            if (ID != 0)
              ESPUI.updateVisibility(ID, false);
          }

          ESPUI.updateVisibility(statusNightModeID, false);
          ESPUI.updateVisibility(sliderBrightnessDayID, false);
          ESPUI.updateVisibility(switchNightModeID, false);
          ESPUI.updateVisibility(sliderBrightnessNightID, false);
          ESPUI.updateVisibility(call_day_time_startID, false);
          ESPUI.updateVisibility(call_day_time_stopID, false);
          ESPUI.updateVisibility(text_colour_time, false);
          ESPUI.updateVisibility(text_colour_background, false);
          ESPUI.updateVisibility(switchRandomColorID, false);
          ESPUI.updateVisibility(DayNightSectionID, false);
          ESPUI.updateVisibility(switchSingleMinutesID, false);
          ESPUI.jsonReload();
        }
        if ((String(p->name()) == "LEDs") && (p->value().toInt() == 1)) {
          set_web_intensity = 0;
          ledstatus = "ON";
          ESPUI.updateVisibility(intensity_web_HintID, false);

          for (const auto& single_setting : all_settings)
          {
            const auto ID = single_setting.second.UI_ID;

            if (ID != 0)
              ESPUI.updateVisibility(ID, true);
          }

          ESPUI.updateVisibility(statusNightModeID, true);
          ESPUI.updateVisibility(sliderBrightnessDayID, true);
          ESPUI.updateVisibility(switchNightModeID, true);
          ESPUI.updateVisibility(sliderBrightnessNightID, true);
          ESPUI.updateVisibility(call_day_time_startID, true);
          ESPUI.updateVisibility(call_day_time_stopID, true);
          ESPUI.updateVisibility(text_colour_time, true);
          ESPUI.updateVisibility(text_colour_background, true);
          ESPUI.updateVisibility(switchRandomColorID, true);
          ESPUI.updateVisibility(DayNightSectionID, true);
          ESPUI.updateVisibility(switchSingleMinutesID, true);
        }
        changedvalues = true;
        updatenow = true;
      } else {
        request->send(200, "text/plain", "INVALID VALUES - MUST BE BETWEEN 0 and 1");
      }
    }
    request->send(200, "text/plain", "WordClock LEDs set to: " + ledstatus);
  });

  ledserver.on("/status", HTTP_GET, [](AsyncWebServerRequest* request) {  // Show the status of all extra words and the color for the background and time texts:
    String message = ledstatus;
    request->send(200, "text/plain", message);
  });

  ledserver.begin();
}


