// ###########################################################################################################################################
// # NTP time functions:
// ###########################################################################################################################################
void configNTPTime() {
  initTime(Timezone);
  printLocalTime();
}


// ###########################################################################################################################################
void setTimezone(String timezone) {
  Serial.printf("Setting timezone to %s\n", timezone.c_str());
  setenv("TZ", timezone.c_str(), 1);  //  Now adjust the TZ.  Clock settings are adjusted to show the new local time
  tzset();
}


// ###########################################################################################################################################
void showTimeText(uint32_t color) {
  switch (getSetting(setting_type::langLEDlayout)) {
    case 0:  // DE:
      setLEDcolXY(11, 0, 4, color);
      break;

    case 1:  // EN:
      setLEDcolXY(11, 1, 4, color);
      break;

    case 2:  // NL:
      setLEDcolXY(7, 2, 4, color);
      break;

    case 3:  // SWE:
      setLEDcolXY(13, 3, 3, color);
      break;

    case 4:                                         // IT:
      setLEDcolXY(0, 3, 1, color);  // T
      setLEDcolXY(14, 3, 2, color); // EM
      setLEDcolXY(12, 4, 1, color); // P
      setLEDcolXY(5, 7, 1, color);  // O
      break;

    case 5:                                         // FR:
      setLEDcolXY(5, 0, 1, color);  // T
      setLEDcolXY(5, 1, 2, color);  // EM
      setLEDcolXY(6, 3, 1, color);  // p
      setLEDcolXY(15, 4, 1, color); // S
      break;

    case 6:                                       // GSW:
      setLEDcolXY(12, 0, 4, color);    // ZIIT
      break;

    case 7:  // CN:
      setLEDcolXY(6, 1, 2, color);
      break;

    case 8:                                         // SWABIAN:
      setLEDcolXY(5, 2, 2, color);    // ZE
      setLEDcolXY(12, 4, 1, color);  // I
      setLEDcolXY(3, 6, 1, color);  // T
      break;

    case 9:                                       // BAVARIAN:
      setLEDcolXY(7, 0, 4, color);    // ZEID
      break;

    default:
      break;
  }

  strip.show();
}

// ###########################################################################################################################################
int TimeResetCounter = 1;
void initTime(String timezone) {
  struct tm timeinfo;
  Serial.println("Setting up time");
  configTime(0, 0, NTPserver);
  while (!getLocalTime(&timeinfo)) {
    ClearDisplay();
    strip.show();
    delay(250);
    Serial.println("! Failed to obtain time - Time server could not be reached ! --> Try: " + String(TimeResetCounter) + " of 3...");
    TimeResetCounter = TimeResetCounter + 1;
    if (TimeResetCounter == 4) {
      Serial.println("! Failed to obtain time - Time server could not be reached ! --> RESTART THE DEVICE NOW...");
      ESP.restart();
    }

    showTimeText(strip.Color(255, 0, 0));
    delay(250);
    ClearDisplay();
  }

  // Time successfully received:
  ClearDisplay();
  showTimeText(strip.Color(0, 255, 0));

  delay(1000);
  Serial.println("Got the time from NTP");
  setTimezone(timezone);
}


// ###########################################################################################################################################
void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return;
  }
  // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S zone %Z %z ");
  char timeStringBuff[50];
  strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);
  iStartTime = String(timeStringBuff);
  iHour = timeinfo.tm_hour;
  iMinute = timeinfo.tm_min;
  iSecond = timeinfo.tm_sec;
  // Serial.print("Time: ");
  // Serial.print(iHour);
  // Serial.print(":");
  // Serial.print(iMinute);
  // Serial.print(":");
  // Serial.println(iSecond);
  delay(1000);
}

// ###########################################################################################################################################
void setTime(int yr, int month, int mday, int hr, int minute, int sec, int isDst) {
  struct tm tm;
  tm.tm_year = yr - 1900;  // Set date
  tm.tm_mon = month - 1;
  tm.tm_mday = mday;
  tm.tm_hour = hr;  // Set time
  tm.tm_min = minute;
  tm.tm_sec = sec;
  tm.tm_isdst = isDst;  // 1 or 0
  time_t t = mktime(&tm);
  Serial.printf("Setting time: %s", asctime(&tm));
  struct timeval now = { .tv_sec = t };
  settimeofday(&now, NULL);
}


// ###########################################################################################################################################
