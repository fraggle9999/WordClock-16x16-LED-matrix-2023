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

    if (langLEDlayout == 0) {  // DE:
      setLEDcol(1, 4, strip.Color(255, 0, 0));
      setLEDcol(27, 30, strip.Color(255, 0, 0));  // 2nd row
    }

    if (langLEDlayout == 1) {  // EN:
      setLEDcol(33, 36, strip.Color(255, 0, 0));
      setLEDcol(59, 62, strip.Color(255, 0, 0));  // 2nd row
    }

    if (langLEDlayout == 2) {  // NL:
      setLEDcol(69, 72, strip.Color(255, 0, 0));
      setLEDcol(87, 90, strip.Color(255, 0, 0));  // 2nd row
    }

    if (langLEDlayout == 3) {  // SWE:
      setLEDcol(96, 98, strip.Color(255, 0, 0));
      setLEDcol(125, 127, strip.Color(255, 0, 0));  // 2nd row
    }

    if (langLEDlayout == 4) {                       // IT:
      setLEDcol(111, 111, strip.Color(255, 0, 0));  // T
      setLEDcol(112, 112, strip.Color(255, 0, 0));  // 2nd row
      setLEDcol(96, 97, strip.Color(255, 0, 0));    // EM
      setLEDcol(126, 127, strip.Color(255, 0, 0));  // 2nd row
      setLEDcol(131, 131, strip.Color(255, 0, 0));  // P
      setLEDcol(156, 156, strip.Color(255, 0, 0));  // 2nd row
      setLEDcol(234, 234, strip.Color(255, 0, 0));  // O
      setLEDcol(245, 245, strip.Color(255, 0, 0));  // 2nd row
    }

    if (langLEDlayout == 5) {                       // FR:
      setLEDcol(10, 10, strip.Color(255, 0, 0));    // T
      setLEDcol(21, 21, strip.Color(255, 0, 0));    // 2nd row
      setLEDcol(41, 42, strip.Color(255, 0, 0));    // EM
      setLEDcol(53, 54, strip.Color(255, 0, 0));    // 2nd row
      setLEDcol(105, 105, strip.Color(255, 0, 0));  // p
      setLEDcol(118, 118, strip.Color(255, 0, 0));  // 2nd row
      setLEDcol(128, 128, strip.Color(255, 0, 0));  // S
      setLEDcol(159, 159, strip.Color(255, 0, 0));  // 2nd row
    }

    if (langLEDlayout == 6) {                     // GSW:
      setLEDcol(0, 3, strip.Color(255, 0, 0));    // ZIIT
      setLEDcol(28, 31, strip.Color(255, 0, 0));  // 2nd row
    }

    if (langLEDlayout == 7) {  // CN:
      setLEDcol(40, 41, strip.Color(255, 0, 0));
      setLEDcol(54, 55, strip.Color(255, 0, 0));  // 2nd row
    }

    if (langLEDlayout == 8) {                       // SWABIAN:
      setLEDcol(73, 74, strip.Color(255, 0, 0));    // ZE
      setLEDcol(85, 86, strip.Color(255, 0, 0));    // 2nd row
      setLEDcol(131, 131, strip.Color(255, 0, 0));  // I
      setLEDcol(156, 156, strip.Color(255, 0, 0));  // 2nd row
      setLEDcol(204, 204, strip.Color(255, 0, 0));  // T
      setLEDcol(211, 211, strip.Color(255, 0, 0));  // 2nd row
    }

    if (langLEDlayout == 9) {                     // BAVARIAN:
      setLEDcol(5, 8, strip.Color(255, 0, 0));    // ZEID
      setLEDcol(23, 26, strip.Color(255, 0, 0));  // 2nd row
    }

    strip.show();
    delay(250);
    ClearDisplay();
  }

  // Time successfully received:
  ClearDisplay();
  if (langLEDlayout == 0) {  // DE:
    setLEDcol(1, 4, strip.Color(0, 255, 0));
    setLEDcol(27, 30, strip.Color(0, 255, 0));  // 2nd row
  }

  if (langLEDlayout == 1) {  // EN:
    setLEDcol(33, 36, strip.Color(0, 255, 0));
    setLEDcol(59, 62, strip.Color(0, 255, 0));  // 2nd row
  }

  if (langLEDlayout == 2) {  // NL:
    setLEDcol(69, 72, strip.Color(0, 255, 0));
    setLEDcol(87, 90, strip.Color(0, 255, 0));  // 2nd row
  }

  if (langLEDlayout == 3) {  // SWE:
    setLEDcol(96, 98, strip.Color(0, 255, 0));
    setLEDcol(125, 127, strip.Color(0, 255, 0));  // 2nd row
  }

  if (langLEDlayout == 4) {                       // IT:
    setLEDcol(111, 111, strip.Color(0, 255, 0));  // T
    setLEDcol(112, 112, strip.Color(0, 255, 0));  // 2nd row
    setLEDcol(96, 97, strip.Color(0, 255, 0));    // EM
    setLEDcol(126, 127, strip.Color(0, 255, 0));  // 2nd row
    setLEDcol(131, 131, strip.Color(0, 255, 0));  // P
    setLEDcol(156, 156, strip.Color(0, 255, 0));  // 2nd row
    setLEDcol(234, 234, strip.Color(0, 255, 0));  // O
    setLEDcol(245, 245, strip.Color(0, 255, 0));  // 2nd row
  }

  if (langLEDlayout == 5) {                       // FR:
    setLEDcol(10, 10, strip.Color(0, 255, 0));    // T
    setLEDcol(21, 21, strip.Color(0, 255, 0));    // 2nd row
    setLEDcol(41, 42, strip.Color(0, 255, 0));    // EM
    setLEDcol(53, 54, strip.Color(0, 255, 0));    // 2nd row
    setLEDcol(105, 105, strip.Color(0, 255, 0));  // p
    setLEDcol(118, 118, strip.Color(0, 255, 0));  // 2nd row
    setLEDcol(128, 128, strip.Color(0, 255, 0));  // S
    setLEDcol(159, 159, strip.Color(0, 255, 0));  // 2nd row
  }

  if (langLEDlayout == 6) {                     // GSW:
    setLEDcol(0, 3, strip.Color(0, 255, 0));    // ZIIT
    setLEDcol(28, 31, strip.Color(0, 255, 0));  // 2nd row
  }

  if (langLEDlayout == 7) {  // CN:
    setLEDcol(40, 41, strip.Color(0, 255, 0));
    setLEDcol(54, 55, strip.Color(0, 255, 0));  // 2nd row
  }

  if (langLEDlayout == 8) {                       // SWABIAN:
    setLEDcol(73, 74, strip.Color(0, 255, 0));    // ZE
    setLEDcol(85, 86, strip.Color(0, 255, 0));    // 2nd row
    setLEDcol(131, 131, strip.Color(0, 255, 0));  // I
    setLEDcol(156, 156, strip.Color(0, 255, 0));  // 2nd row
    setLEDcol(204, 204, strip.Color(0, 255, 0));  // T
    setLEDcol(211, 211, strip.Color(0, 255, 0));  // 2nd row
  }

  if (langLEDlayout == 9) {                     // BAVARIAN:
    setLEDcol(5, 8, strip.Color(0, 255, 0));    // ZEID
    setLEDcol(23, 26, strip.Color(0, 255, 0));  // 2nd row
  }

  strip.show();
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


