// ###########################################################################################################################################
// # Wifi scan function to help you to setup your WiFi connection
// ###########################################################################################################################################
void ScanWiFi() {
  Serial.println("Scan WiFi networks - START");
  int n = WiFi.scanNetworks();
  Serial.println("WiFi scan done");
  Serial.println(" ");
  if (n == 0) {
    Serial.println("No WiFi networks found");
  } else {
    Serial.print(n);
    Serial.println(" WiFi networks found:");
    Serial.println(" ");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("Scan WiFi networks - END");
}


// ###########################################################################################################################################
// # Wifi setup and reconnect function that runs once at startup and during the loop function of the ESP:
// ###########################################################################################################################################
void WIFI_SETUP() {
  Serial.println(" ");
  esp_log_level_set("wifi", ESP_LOG_WARN);  // Disable WiFi debug warnings
  if (testTime == 0) {                      // If time text test mode is NOT used:
    String WIFIssid = preferences.getString("WIFIssid");
    bool WiFiConfigEmpty = false;
    if (WIFIssid == "") {
      // Serial.println("WIFIssid empty");
      WiFiConfigEmpty = true;
    } else {
      // Serial.print("WIFIssid = ");
      // Serial.println(WIFIssid);
    }
    String WIFIpass = preferences.getString("WIFIpass");
    if (WIFIpass == "") {
      // Serial.println("WIFIpass empty");
      WiFiConfigEmpty = true;
    } else {
      // Serial.print("WIFIpass = ");
      // Serial.println(WIFIpass);
    }
    if (WiFiConfigEmpty == true) {
      Serial.println("Show SET WIFI...");
      uint32_t c = strip.Color(0, 255, 255);
      int TextWait = 500;
      showText("SET WIFI ", TextWait, c);
      ShowWLAN(strip.Color(0, 255, 255));
      CaptivePortalSetup();
    } else {
      Serial.println("Try to connect to found WiFi configuration: ");
      WiFi.disconnect();
      int tryCount = 0;
      WiFi.mode(WIFI_STA);
      WiFi.begin((const char*)WIFIssid.c_str(), (const char*)WIFIpass.c_str());
      Serial.println("Connecting to WiFi " + String(WIFIssid));
      while (WiFi.status() != WL_CONNECTED) {
        ShowWLAN(strip.Color(0, 0, 255));
        tryCount = tryCount + 1;
        Serial.print("Connection try #: ");
        Serial.println(tryCount);
        if (tryCount >= maxWiFiconnctiontries - 10) {
          ShowWLAN(strip.Color(255, 0, 0));
        }
        if (tryCount == maxWiFiconnctiontries) {
          Serial.println("\n\nWIFI CONNECTION ERROR: If the connection still can not be established please check the WiFi settings or location of the device.\n\n");
          preferences.putString("WIFIssid", "");  // Reset entered WiFi ssid
          preferences.putString("WIFIpass", "");  // Reset entered WiFi password
          preferences.end();
          delay(250);
          Serial.println("WiFi settings deleted because in " + String(maxWiFiconnctiontries) + " tries the WiFi connection could not be established. Temporary WordClock access point will be started to reconfigure WiFi again.");
          ESP.restart();
        }
        delay(500);
        ShowWLAN(strip.Color(0, 0, 0));
        delay(500);
      }
      Serial.println(" ");
      WiFIsetup = true;
      Serial.print("Successfully connected now to WiFi SSID: ");
      Serial.println(WiFi.SSID());
      Serial.println("IP: " + WiFi.localIP().toString());
      Serial.println("DNS: " + WiFi.dnsIP().toString());
      ShowWLAN(strip.Color(0, 255, 0));
      delay(1000);

      if (useStartupText == 1) callStartText();  // Show "WordClock" startup text
      if (useshowip == 1) ShowIPaddress();       // Display the current IP-address
      configNTPTime();                           // NTP time setup
      setupWebInterface();                       // Generate the configuration page
      updatenow = true;                          // Update the display 1x after startup
      update_display();                          // Update LED display
      handleLEDupdate();                         // LED update via web
      setupOTAupate();                           // ESP32 OTA update
      Serial.println("######################################################################");
      Serial.println("# Web interface online at: http://" + IpAddress2String(WiFi.localIP()));
      Serial.println("# HTTP controls online at: http://" + IpAddress2String(WiFi.localIP()) + ":2023");
      Serial.println("######################################################################");
      Serial.println("# WordClock startup finished...");
      Serial.println("######################################################################");
      Serial.println(" ");
    }
  }
}

