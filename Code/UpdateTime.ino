uint32_t colorRGB;
static int lastHourSet = -1;
static int lastMinutesSet = -1;
static int lastSecondSet = -1;

// ###########################################################################################################################################
// # Get a random color, avoid black and very light colors
// ###########################################################################################################################################
uint32_t getRandomColor() {
  int redVal = 0;
  int greenVal = 0;
  int blueVal = 0;
  
  while (((redVal == 0) && (greenVal == 0) && (blueVal == 0)) || // avoid black color
         (redVal + greenVal + blueVal < 64)) // avoid very light colors
  {
    redVal = random(255);
    greenVal = random(255);
    blueVal = random(255);
  }
  
  return strip.Color(redVal, greenVal, blueVal);
}


// ###########################################################################################################################################
// # Do something every second:
// ###########################################################################################################################################
void handleWithSeconds(const int hours, const int minutes) {
  int currentSecond = iSecond;

  if (currentSecond == lastSecondSet)
    return;

  lastSecondSet = currentSecond;

  int coordY = -1;
  int plus_coordX = -1;
  int minuteVal_offsetX = -1;
  int minuteText_coordX = -1;
  int minuteText_len = -1;

  getMinuteCoordsAndOffsets(0, plus_coordX, coordY, minuteVal_offsetX, minuteText_coordX, minuteText_len);
  
  if (getSetting(setting_type::blinkingSecond) == 1)
  {
    uint32_t c = color_green;
    if (currentSecond % 2 == 0)
      c = color_black;

    // show blinking plus sign every second
    setLEDcolXY(plus_coordX, coordY, 1, c);
  }

  if (getSetting(setting_type::minuteCountdown) == 1)
  {
    // show countdown to next minute
    if ((currentSecond >= 56) && (currentSecond <= 59))
    {
      // shut off all minute values
      switchLEDXY(minuteVal_offsetX + 1, coordY, 4, 0);

      // shut off "minutes" text
      switchLEDXY(minuteText_coordX, coordY, minuteText_len, 0);

      // set remaining seconds as actual minute value
      setLEDcolXY(minuteVal_offsetX + (60 - currentSecond), coordY, 1, color_green);
    }
  }

  strip.show();
}


// ###########################################################################################################################################
// # Display hours and minutes text function:
// ###########################################################################################################################################
void show_time(int hours, int minutes) {
  handleWithSeconds(hours, minutes);

  if ((lastHourSet == hours && lastMinutesSet == minutes) && updatenow == false) {  // Reduce display updates to new minutes and new config updates
    return;
  }

  updatenow = false;
  lastHourSet = hours;
  lastMinutesSet = minutes;

  // Show current time of display update:
  // if (debugtexts == 1) Serial.println("Update display now: " + String(hours) + ":" + String(minutes) + ":" + String(iSecond));

  // Night/Day mode intensity setting:
  if ((getSetting(setting_type::usenightmode) == 1) && (set_web_intensity == 0)) {
    if ((iHour >= getSetting(setting_type::day_time_start)) && (iHour < getSetting(setting_type::day_time_stop))) {
      intensity = getSetting(setting_type::intensity_day);
      if ((iHour == 0) && (getSetting(setting_type::day_time_stop) == 23)) intensity = getSetting(setting_type::intensity_night);  // Special function if day_time_stop set to 23 and time is 24, so 0...
    } else {
      intensity = getSetting(setting_type::intensity_night);
    }
    if (testDayNightmode == 1) {  // Test day/night times function:
      Serial.println("############################################################################################");
      Serial.println("Current time day/night test: " + String(hours) + ":" + String(minutes) + ":" + String(iSecond));
      Serial.println("Current settings: day_time_start: " + String(getSetting(setting_type::day_time_start)) + " day_time_stop: " + String(getSetting(setting_type::day_time_stop)));
      for (int i = 0; i < 24; i++) {
        String daynightvar = "-";
        if ((i >= getSetting(setting_type::day_time_start)) && (i < getSetting(setting_type::day_time_stop))) {
          daynightvar = "Day time";
          if ((i == 0) && (getSetting(setting_type::day_time_stop) == 23)) daynightvar = "Night time";
        } else {
          daynightvar = "Night time";
        }
        Serial.println("Current hour: " + String(i) + " --> " + daynightvar);
      }
      testDayNightmode = 0;  // Show the list 1x only
      Serial.println("############################################################################################");
    }
  } else {  // Control intensity by WordClock settings or via HTML command:
    if (set_web_intensity == 0) intensity = getSetting(setting_type::intensity_day);
    if (set_web_intensity == 1) intensity = 0;
  }
  strip.setBrightness(intensity);

  // Set background color:
  back_color();

  // Static text color or random color mode:
  if (getSetting(setting_type::RandomColor) == 0) 
    colorRGB = getSetting(setting_type::colorTime);
  else
    colorRGB = getRandomColor();

  // Display time:
  iHour = hours;
  iMinute = minutes;
  
  // show random effect to new beginning hour
  if (iMinute == 0)
    randomEffect();

  // Test a special time:
  if (testspecialtime == 1) {
    Serial.println("Special time test active: " + String(test_hour) + ":" + String(test_minute));
    iHour = test_hour;
    iMinute = test_minute;
  }

  // show current time as text
  if (getSetting(setting_type::showScrollingTimeEveryXMinutes) != 0)
  {
    if (minuteCountDown > 0)
      --minuteCountDown;

    if (minuteCountDown == 0)
    {
      showCurrentTimeAsScrollingText(iHour, iMinute);

      minuteCountDown = getSetting(setting_type::showScrollingTimeEveryXMinutes); 
      if (minuteCountDown < 0)
        minuteCountDown = random(1, 7);
    }
  }

  showTime(iHour, iMinute);

  if (getSetting(setting_type::usesinglemin) == 1) 
    showMinutes(iMinute);

  strip.show();
}


// ###########################################################################################################################################
// # Set LEDs for time part:
// ###########################################################################################################################################
void setLEDtimePart(const std::map<time_parts, position_t>& timeParts, const time_parts timePart, uint32_t color)
{
  const auto Index = timeParts.find(timePart);
  if (Index == timeParts.end())
    return;

  setLEDcolXY(Index->second, color, getSetting(setting_type::animationDelay));
}


// ###########################################################################################################################################
// # Check if the map contains a timepart:
// ###########################################################################################################################################
bool containsTimePart(const std::map<time_parts, position_t>& timePartsMap, const time_parts tp)
{
  const auto Index = timePartsMap.find(tp);
  return Index != timePartsMap.end();
}


// ###########################################################################################################################################
// # Display time function:
// ###########################################################################################################################################
void showTime(const int iHour, const int iMinute)
{
  static std::map<int, std::map<time_parts, position_t>> timePartsMap =
      { { 0, // DE
          { 
            { time_parts::prefix1, { 0, 0, 2 } }, // ES
            { time_parts::prefix2, { 3, 0, 3 } }, // IST
            { time_parts::five_min, { 0, 2, 4 } }, 
            { time_parts::quarter, { 4, 2, 7 } }, 
            { time_parts::ten_min, { 12, 1, 4 } }, 
            { time_parts::twenty, { 0, 1, 7 } }, 
            { time_parts::past, { 12, 2, 4 } }, 
            { time_parts::to, { 0, 3, 3 } },
            { time_parts::half, { 4, 3, 4 } }, 
            { time_parts::one1, { 4, 5, 3 } }, 
            { time_parts::o_clock, { 13, 6, 3 } } 
          }
        },
        { 1, // EN
          { 
            { time_parts::prefix1, { 0, 0, 2 } },
            { time_parts::prefix2, { 3, 0, 2 } },
            { time_parts::five_min, { 6, 1, 4 } }, 
            { time_parts::quarter_prefix, { 10, 0, 1} },
            { time_parts::quarter, { 1, 2, 7 } }, 
            { time_parts::ten_min, { 13, 0, 3 } }, 
            { time_parts::twenty, { 0, 1, 6 } }, 
            { time_parts::past, { 10, 2, 4 } }, 
            { time_parts::to, { 13, 2, 2 } },
            { time_parts::half, { 9, 0, 4 } }, 
            { time_parts::one1, { 4, 6, 3 } }, 
            { time_parts::o_clock, { 8, 6, 7 } }
          }
        },
        { 2, // NL
          { 
            { time_parts::prefix1, { 0, 0, 3 } },
            { time_parts::prefix2, { 4, 0, 2 } },
            { time_parts::five_min, { 12, 0, 4 } }, 
            { time_parts::quarter, { 5, 1, 5 } }, 
            { time_parts::ten_min, { 0, 1, 4 } }, 
            { time_parts::past, { 11, 1, 4 } }, 
            { time_parts::to, { 12, 2, 4 } },
            { time_parts::half, { 1, 3, 4 } }, 
            { time_parts::one1, { 10, 3, 3 } }, 
            { time_parts::o_clock, { 12, 6, 3 } } 
          }
        },
        { 3, // SWE
          { 
            { time_parts::prefix1, { 0, 0, 7 } },
            { time_parts::prefix2, { 9, 0, 2 } },
            { time_parts::five_min, { 13, 2, 3 } }, 
            { time_parts::quarter, { 1, 1, 5 } }, 
            { time_parts::ten_min, { 5, 2, 3 } }, 
            { time_parts::twenty, { 0, 2, 5 } }, 
            { time_parts::past, { 0, 3, 4 } }, 
            { time_parts::to, { 14, 3, 1 } },
            { time_parts::half, { 0, 4, 4 } }, 
            { time_parts::one1, { 3, 5, 3 } } 
          }
        },
        { 4, // IT
          { 
            { time_parts::prefix1, { 5, 0, 2 } },
            { time_parts::prefix2, { 0, 0, 4 } },
            { time_parts::one1, { 15, 0, 1 } } 
          }
        },
        { 5, // FR
          {
            { time_parts::prefix1, { 0, 0, 2 } },
            { time_parts::prefix2, { 3, 0, 3 } }
          }
        },
        { 6, // GSW
          { 
            { time_parts::prefix1, { 1, 0, 2 } },
            { time_parts::prefix2, { 8, 0, 4 } },
            { time_parts::five_min, { 0, 1, 4 } }, 
            { time_parts::quarter, { 1, 2, 7 } }, 
            { time_parts::ten_min, { 11, 1, 3 } }, 
            { time_parts::twenty, { 9, 2, 6 } }, 
            { time_parts::past, { 0, 3, 2 } }, 
            { time_parts::to, { 2, 3, 3 } },
            { time_parts::half, { 6, 3, 5 } }
          } 
        },
        { 7, // CN
          {
            { time_parts::prefix1, { 2, 1, 2 } },
            { time_parts::prefix2, { 6, 1, 2 } }
          }
        },
        { 8, // SWABIAN
          { 
            { time_parts::prefix1, { 0, 0, 2 } },
            { time_parts::prefix2, { 3, 0, 4 } },
            { time_parts::five_min, { 0, 2, 4 } }, 
            { time_parts::quarter, { 10, 0, 6 } }, 
            { time_parts::three_quarters, { 5, 1, 10 } },
            { time_parts::ten_min, { 5, 2, 4 } }, 
            { time_parts::past, { 11, 2, 4 } }, 
            { time_parts::to, { 0, 3, 3 } },
            { time_parts::half, { 6, 3, 4 } }
          } 
        },
        { 9, // BAVARIAN
          { 
            { time_parts::prefix1, { 0, 0, 2 } },
            { time_parts::prefix2, { 5, 0, 2 } },
            { time_parts::five_min, { 11, 1, 4 } }, 
            { time_parts::quarter, { 5, 1, 6 } }, 
            { time_parts::twenty, { 1, 2, 7 } },
            { time_parts::ten_min, { 0, 1, 4 } }, 
            { time_parts::past, { 12, 2, 4 } }, 
            { time_parts::to, { 9, 2, 3 } },
            { time_parts::half, { 0, 3, 5 } },
            { time_parts::o_clock, { 5, 7, 3 } } 
           } 
        }
      };
  
  static std::map<int, std::vector<position_t>> hoursMap =
      { { 0, // DE
          { 
            { 4, 5, 4 },  // EINS
            { 0, 4, 4 },  // ZWEI
            { 4, 4, 4 },  // DREI
            { 12, 4, 4 }, // VIER
            { 12, 5, 4 }, // FÜNF
            { 7, 5, 5 },  // SECHS
            { 0, 6, 6 },  // SIEBEN
            { 0, 5, 4 },  // ACHT
            { 8, 4, 4 },  // NEUN
            { 9, 3, 4 },  // ZEHN
            { 13, 3, 3 }, // ELF
            { 6, 6, 5 }   // ZWÖLF
          } 
        },  
        { 1, // EN
          { 
            { 4, 6, 3 },
            { 4, 3, 3 },
            { 8, 3, 5 },
            { 12, 4, 4 },
            { 0, 3, 4 },
            { 10, 5, 3 },
            { 0, 5, 5 },
            { 5, 5, 5 },
            { 0, 6, 4 },
            { 13, 3, 3 },
            { 0, 4, 6 },
            { 6, 4, 6 }
          } 
        },
        { 2, // NL
          { 
            { 10, 3, 3 },
            { 1, 6, 4 },
            { 8, 5, 4 },
            { 6, 6, 4 },
            { 12, 5, 4 },
            { 13, 3, 3 },
            { 10, 4, 5 },
            { 6, 3, 4 },
            { 0, 5, 5 },
            { 0, 4, 4 },
            { 5, 5, 3 },
            { 4, 4, 6 }
          } 
        },
        { 3, // SWE
          { 
            { 3, 5, 3 },
            { 13, 5, 3 },
            { 9, 4, 3 },
            { 7, 6, 4 },
            { 11, 6, 3 },
            { 4, 6, 3 },
            { 0, 5, 3 },
            { 12, 4, 4 },
            { 6, 4, 3 },
            { 5, 5, 3 },
            { 0, 6, 4 },
            { 9, 5, 4 }
          } 
        },  
        { 4, // IT
          { 
            { 3, 3, 5 },
            { 8, 3, 3 },
            { 0, 3, 3 },
            { 0, 2, 7 },
            { 10, 2, 6 },
            { 5, 1, 3 },
            { 0, 1, 5 },
            { 6, 2, 4 },
            { 11, 3, 4 },
            { 1, 4, 5 },
            { 9, 0, 6 },
            { 8, 1, 6 }
          } 
        },  
        { 5, // FR
          { 
            { 0, 4, 3 },
            { 12, 3, 4 },
            { 0, 3, 5 },
            { 0, 1, 6 },
            { 11, 0, 4 },
            { 13, 2, 3 },
            { 4, 3, 4 },
            { 12, 1, 4 },
            { 8, 3, 4 },
            { 0, 2, 3 },
            { 7, 0, 4 },
            { -1, -1, -1 }
          } 
        },  
        { 6, // GSW
          { 
            { 13, 4, 3 },
            { 4, 4, 4 },
            { 12, 3, 4 },
            { 11, 5, 5 },
            { 0, 5, 5 },
            { 5, 5, 6 },
            { 0, 6, 6 },
            { 11, 6, 5 },
            { 8, 4, 5 },
            { 6, 6, 5 },
            { 0, 4, 4 },
            { 1, 7, 6 }
          } 
        },  
        { 7, // CN
          { 
            { 3, 2, 2 },
            { 6, 2, 2 },
            { 10, 1, 2 },
            { 12, 1, 2 },
            { 8, 2, 2 },
            { 10, 2, 2 },
            { 12, 2, 2 },
            { 2, 3, 2 },
            { 4, 3, 2 },
            { 6, 3, 2 },
            { 2, 2, 3 },
            { 5, 2, 3 }
          } 
        },  
        { 8, // SWABIAN
          { 
            { 7, 5, 4 },
            { 11, 5, 5 },
            { 0, 7, 5 },
            { 11, 4, 5 },
            { 0, 4, 5 },
            { 5, 4, 6 },
            { 0, 5, 7 },
            { 0, 6, 5 },
            { 11, 6, 5 },
            { 11, 3, 5 },
            { 4, 7, 4 },
            { 5, 6, 6 }
          } 
        },  
        { 9, // BAVARIAN
          { 
            { 6, 3, 4 },
            { 11, 5, 4 },
            { 7, 5, 4 },
            { 11, 6, 3 },
            { 10, 3, 4 },
            { 0, 6, 5 },
            { 0, 5, 5 },
            { 11, 4, 4 },
            { 6, 6, 4 },
            { 6, 4, 4 },
            { 0, 7, 3 },
            { 0, 4, 5 }
          } 
        }  
      };

  static std::vector<int> hourThresholds =
      { 25, 35, 20, 25, 40, 35, 25, 0, 15, 25 }; 

  int langLEDlayout = getSetting(setting_type::langLEDlayout);

  const auto Index = timePartsMap.find(langLEDlayout);
  if (Index == timePartsMap.end())
  {
    const std::vector<std::string> questionMark = charMap['?'];

    showCharMatrix(questionMark, 5, 1, strip.Color(255, 0, 0));
    return;
  }

  const auto& timeParts = Index->second;

  // divide minute by 5 to get value for display control
  int minDiv = iMinute / 5;

  int hours = iHour;
  int minutes = iMinute;

  const auto twenty_defined = containsTimePart(timeParts, time_parts::twenty);
  const auto three_quarters_defined = containsTimePart(timeParts, time_parts::three_quarters);
  const auto one1_defined = containsTimePart(timeParts, time_parts::one1);

  // set hour from 1 to 12 (at noon, or midnight)
  int xHour = (iHour % 12);
  if (xHour == 0)
    xHour = 12;

  static bool randomize_twenty = true;
  static bool use_twenty = false;

  // ES IST:
  if ((langLEDlayout != 4) || (xHour > 1)) {  // IT:
    setLEDtimePart(timeParts, time_parts::prefix1, colorRGB);
    setLEDtimePart(timeParts, time_parts::prefix2, colorRGB);
  }

  // FÜNF: (Minuten)
  if ((minDiv == 1) || (minDiv == 5) || (minDiv == 7) || (minDiv == 11)) {
    setLEDtimePart(timeParts, time_parts::five_min, colorRGB);
    if (testPrintTimeTexts == 1) Serial.print("FÜNF ");
  }

  // VIERTEL:
  if ((minDiv == 3) || (minDiv == 9)) {
    setLEDtimePart(timeParts, time_parts::quarter_prefix, colorRGB);
    setLEDtimePart(timeParts, time_parts::quarter, colorRGB);
    if (testPrintTimeTexts == 1) Serial.print("VIERTEL ");
  }

  // ZWANZIG:
  if ((minDiv == 4) || (minDiv == 8)) {
    if (twenty_defined && randomize_twenty) // use same randomization for next 5 minutes
    {
      const auto random_value = random(10);
      use_twenty = (random_value % 2 == 0);
      randomize_twenty = false;
    }

    // test, if twenty is defined, otherwise use ten to/ten after
    if (use_twenty && twenty_defined)
      setLEDtimePart(timeParts, time_parts::twenty, colorRGB);

    if (testPrintTimeTexts == 1) Serial.print("ZWANZIG ");
  }
  else
    randomize_twenty = true;

  // ZEHN: (Minuten)
  if ((minDiv == 2) || (minDiv == 10) ||
      (((minDiv == 4) || (minDiv == 8)) && !use_twenty)) {
    setLEDtimePart(timeParts, time_parts::ten_min, colorRGB);
    if (testPrintTimeTexts == 1) Serial.print("ZEHN ");
  }

  // NACH:
  if ((minDiv == 1) || (minDiv == 2) || (minDiv == 3) || (minDiv == 4 && use_twenty) || (minDiv == 7) || (minDiv == 8 && !use_twenty)) {
    setLEDtimePart(timeParts, time_parts::past, colorRGB);
    if (testPrintTimeTexts == 1) Serial.print("NACH ");
  }

  // VOR:
  if ((minDiv == 4 && !use_twenty) || (minDiv == 5) || (minDiv == 8 && use_twenty) || (minDiv == 9) || (minDiv == 10) || (minDiv == 11)) {
    if ((minDiv != 9) || !three_quarters_defined)
      setLEDtimePart(timeParts, time_parts::to, colorRGB);
    if (testPrintTimeTexts == 1) Serial.print("VOR ");
  }

  // HALB:
  if ((minDiv == 5) || (minDiv == 6) || (minDiv == 7) ||
       (((minDiv == 4) || (minDiv == 8)) && !use_twenty)) {
     setLEDtimePart(timeParts, time_parts::half, colorRGB);
    if (testPrintTimeTexts == 1) Serial.print("HALB ");
  }

  // DREIVIERTEL:
  if (minDiv == 9) {
    if (three_quarters_defined)
      setLEDtimePart(timeParts, time_parts::three_quarters, colorRGB);
  }

  // at minute 25 hour needs to be counted up:
  // fuenf vor halb 2 = 13:25
  auto hourThreshold = hourThresholds[langLEDlayout];

  if (twenty_defined && !use_twenty)
    hourThreshold = 20;

  if (hourThreshold > 0)
  {
    if (iMinute >= hourThreshold) {
      if (xHour == 12)
        xHour = 1;
      else
        xHour++;
    }
  }

  uint32_t colorRGBForHour = colorRGB;
  
  if (getSetting(setting_type::useFixedHourColor) == 1)
    colorRGBForHour = getSetting(setting_type::colorHour);

  if (one1_defined && (xHour == 1) && (iMinute < 5))
  {
    setLEDtimePart(timeParts, time_parts::one1, colorRGBForHour);
    if (testPrintTimeTexts == 1) Serial.print("EIN ");
  }
  else
  {
    const auto Index = hoursMap.find(langLEDlayout);
    if (Index != hoursMap.end())
    {
      const auto& positionVector = Index->second;

      if (xHour <= positionVector.size())
        setLEDcolXY(positionVector[xHour - 1], colorRGBForHour, getSetting(setting_type::animationDelay));
    }
  }

  if (iMinute < 5) {
    setLEDtimePart(timeParts, time_parts::o_clock, colorRGB);
    if (testPrintTimeTexts == 1) Serial.print("UHR ");
  }

  // ########################################################### IT:
  if (langLEDlayout == 4) {  // IT:
    // E:
    if ((minDiv == 1) || (minDiv == 2) || (minDiv == 3) || (minDiv == 4) || (minDiv == 5) || (minDiv == 6) || (minDiv == 7)) {
      setLEDcol(134, 134, colorRGB);
      setLEDcol(153, 153, colorRGB);  // 2nd row
      if (testPrintTimeTexts == 1) Serial.print("E ");
    }
    // MENO:
    if ((minDiv == 8) || (minDiv == 9) || (minDiv == 10) || (minDiv == 11)) {
      setLEDcol(132, 135, colorRGB);
      setLEDcol(152, 155, colorRGB);  // 2nd row
      if (testPrintTimeTexts == 1) Serial.print("MENO ");
    }
    // 5/55: CINQUE
    if ((minDiv == 1) || (minDiv == 11)) {
      setLEDcol(162, 167, colorRGB);
      setLEDcol(184, 189, colorRGB);  // 2nd row
      if (testPrintTimeTexts == 1) Serial.print("CINQUE ");
    }
    // 15/45: UN QUARTO
    if ((minDiv == 3) || (minDiv == 9)) {
      setLEDcol(128, 129, colorRGB);  // UN
      setLEDcol(158, 159, colorRGB);  // 2nd row
      setLEDcol(234, 239, colorRGB);  // QUARTO
      setLEDcol(240, 245, colorRGB);  // 2nd row
      if (testPrintTimeTexts == 1) Serial.print("UN QUARTO ");
    }
    // 10/50: DIECI
    if ((minDiv == 2) || (minDiv == 10)) {
      setLEDcol(192, 196, colorRGB);
      setLEDcol(219, 223, colorRGB);  // 2nd row
      if (testPrintTimeTexts == 1) Serial.print("DIECI ");
    }
    // 20/40: VENTI
    if ((minDiv == 4) || (minDiv == 8)) {
      setLEDcol(203, 207, colorRGB);
      setLEDcol(208, 212, colorRGB);  // 2nd row
      if (testPrintTimeTexts == 1) Serial.print("VENTI ");
    }
    // 25: VENTICINQUE
    if (minDiv == 5) {
      setLEDcol(197, 207, colorRGB);
      setLEDcol(208, 218, colorRGB);  // 2nd row
      if (testPrintTimeTexts == 1) Serial.print("VENTICINQUE ");
    }
    // 30: TRENTA
    if (minDiv == 6) {
      setLEDcol(168, 173, colorRGB);
      setLEDcol(178, 183, colorRGB);  // 2nd row
      if (testPrintTimeTexts == 1) Serial.print("TRENTA ");
    }
    // 35: TRENTACINQUE
    if (minDiv == 7) {
      setLEDcol(162, 173, colorRGB);
      setLEDcol(178, 189, colorRGB);  // 2nd row
      if (testPrintTimeTexts == 1) Serial.print("TRENTACINQUE ");
    }
  }

  // ########################################################### FR:
  if (langLEDlayout == 5) {  // FR:
    // CINQ: (Minutes) x:05, x:25, x:35, x:55
    if ((minDiv == 1) || (minDiv == 5) || (minDiv == 7) || (minDiv == 11)) {
      setLEDcol(197, 200, colorRGB);
      setLEDcol(215, 218, colorRGB);  // 2nd row
    }
    // ET QUART: x:15,
    if ((minDiv == 3)) {
      setLEDcol(171, 172, colorRGB);  // ET
      setLEDcol(179, 180, colorRGB);  // 2nd row
      setLEDcol(193, 197, colorRGB);  // QUART
      setLEDcol(218, 222, colorRGB);  // 2nd row
    }
    // LE QUART: x:45
    if ((minDiv == 9)) {
      setLEDcol(172, 173, colorRGB);  // LE
      setLEDcol(178, 179, colorRGB);  // 2nd row
      setLEDcol(193, 197, colorRGB);  // QUART
      setLEDcol(218, 222, colorRGB);  // 2nd row
    }
    // DIX: (Minutes) x:10, x:50
    if ((minDiv == 2) || (minDiv == 10)) {
      setLEDcol(167, 169, colorRGB);
      setLEDcol(182, 184, colorRGB);  // 2nd row
    }
    // VINGT: x:20, x:40
    if ((minDiv == 4) || (minDiv == 8)) {
      setLEDcol(202, 206, colorRGB);
      setLEDcol(209, 213, colorRGB);  // 2nd row
    }
    // VINGT-: x:25, x:35
    if ((minDiv == 5) || (minDiv == 7)) {
      setLEDcol(201, 206, colorRGB);
      setLEDcol(209, 214, colorRGB);  // 2nd row
    }
    // MOINS: x:35, x:40 x:45, x:50, x:55
    if ((minDiv == 7) || (minDiv == 8) || (minDiv == 9) || (minDiv == 10) || (minDiv == 11)) {
      setLEDcol(128, 132, colorRGB);
      setLEDcol(155, 159, colorRGB);  // 2nd row
    }
    // ET DEMIE: x:30
    if ((minDiv == 6)) {
      setLEDcol(171, 172, colorRGB);  // ET
      setLEDcol(179, 180, colorRGB);  // 2nd row
      setLEDcol(161, 165, colorRGB);  // DEMIE
      setLEDcol(186, 190, colorRGB);  // 2nd row
    }

    if (xHour == 12)
    { 
        // MINUIT (0) or MIDI (12)
      if (iHour == 0 || (iHour == 23 && iMinute >= 35)) setLEDcol(36, 41, colorRGB);   // MINUIT (0)
      if (iHour == 0 || (iHour == 23 && iMinute >= 35)) setLEDcol(54, 59, colorRGB);   // 2nd row
      if (iHour == 12 || (iHour == 11 && iMinute >= 35)) setLEDcol(73, 76, colorRGB);  // MIDI (12)
      if (iHour == 12 || (iHour == 11 && iMinute >= 35)) setLEDcol(83, 86, colorRGB);  // 2nd row
    }
 }

  // ########################################################### CN:
  if (langLEDlayout == 7) {  // CN:
    // 零五分                         // x:05
    if ((minDiv == 1)) {
      setLEDcol(101, 103, colorRGB);
      setLEDcol(120, 122, colorRGB);  // 2nd row
    }
    // 十分                         // x:10
    if ((minDiv == 2)) {
      setLEDcol(98, 99, colorRGB);
      setLEDcol(124, 125, colorRGB);  // 2nd row
    }
    // 十五分                         // x:15
    if ((minDiv == 3)) {
      setLEDcol(138, 140, colorRGB);
      setLEDcol(147, 149, colorRGB);  // 2nd row
    }
    // 二十分                         // x:20
    if ((minDiv == 4)) {
      setLEDcol(98, 100, colorRGB);
      setLEDcol(123, 125, colorRGB);  // 2nd row
    }
    // 二十五分                         // x:25
    if ((minDiv == 5)) {
      setLEDcol(138, 141, colorRGB);
      setLEDcol(146, 149, colorRGB);  // 2nd row
    }
    // 三十分                         // x:30
    if ((minDiv == 6)) {
      setLEDcol(135, 137, colorRGB);
      setLEDcol(150, 152, colorRGB);  // 2nd row
    }
    // 三十五分                         // x:35
    if ((minDiv == 7)) {
      setLEDcol(170, 173, colorRGB);
      setLEDcol(178, 181, colorRGB);  // 2nd row
    }
    // 四十分                         // x:40
    if ((minDiv == 8)) {
      setLEDcol(132, 134, colorRGB);
      setLEDcol(153, 155, colorRGB);  // 2nd row
    }
    // 四十五分                         // x:45
    if ((minDiv == 9)) {
      setLEDcol(166, 169, colorRGB);
      setLEDcol(182, 185, colorRGB);  // 2nd row
    }
    // 五十分                         // x:50
    if ((minDiv == 10)) {
      setLEDcol(163, 165, colorRGB);
      setLEDcol(186, 188, colorRGB);  // 2nd row
    }
    // 五十五分                         // x:55
    if ((minDiv == 11)) {
      setLEDcol(202, 205, colorRGB);
      setLEDcol(210, 213, colorRGB);  // 2nd row
    }

    if (iMinute < 5) {
      setLEDcol(162, 162, colorRGB);  // 整
      setLEDcol(189, 189, colorRGB);  // 2nd row
    }
  }

  // ########################################################### BAVARIAN:
  if (langLEDlayout == 9) {  // BAVARIAN:
    switch (xHour) {
      case 4:
        {
          if (iMinute >= 5) {
            setLEDcol(192, 196, colorRGB);  // VIARE
            setLEDcol(219, 223, colorRGB);  // 2nd row
            if (testPrintTimeTexts == 1) Serial.print("VIARE ");
          }
          break;
        }
      case 5:
        {
          if (iMinute >= 5) {
            setLEDcol(97, 101, colorRGB);   // FÜNFE
            setLEDcol(122, 126, colorRGB);  // 2nd row
            if (testPrintTimeTexts == 1) Serial.print("FÜNFE ");
          }
          break;
        }
      case 6:
        {
          if (iMinute >= 5) {
            setLEDcol(202, 207, colorRGB);  // SECKSE
            setLEDcol(208, 213, colorRGB);  // 2nd row
            if (testPrintTimeTexts == 1) Serial.print("SECKSE ");
          }
          break;
        }
      case 7:
        {
          if (iMinute >= 5) {
            setLEDcol(170, 175, colorRGB);  // SIEBNE
            setLEDcol(176, 181, colorRGB);  // 2nd row
            if (testPrintTimeTexts == 1) Serial.print("SIEBNE ");
          }
          break;
        }
      case 8:
        {
          if (iMinute >= 5) {
            setLEDcol(128, 132, colorRGB);  // ACHTE
            setLEDcol(155, 159, colorRGB);  // 2nd row
            if (testPrintTimeTexts == 1) Serial.print("ACHTE ");
          }
          break;
        }
      case 9:
        {
          if (iMinute >= 5) {
            setLEDcol(197, 201, colorRGB);  // NEINE
            setLEDcol(214, 218, colorRGB);  // 2nd row
            if (testPrintTimeTexts == 1) Serial.print("NEINE ");
          }
          break;
        }
      case 10:
        {
          if (iMinute >= 5) {
            setLEDcol(133, 137, colorRGB);  // ZEHNE (Stunden)
            setLEDcol(150, 154, colorRGB);  // 2nd row
            if (testPrintTimeTexts == 1) Serial.print("ZEHNE ");
          }
          break;
        }
      case 11:
        {
          if (iMinute >= 5) {
            setLEDcol(236, 239, colorRGB);  // EJFE
            setLEDcol(240, 243, colorRGB);  // 2nd row
            if (testPrintTimeTexts == 1) Serial.print("EJFE ");
          }
          break;
        }
      case 12:
        {
          if (iMinute >= 5) {
            setLEDcol(138, 143, colorRGB);  // ZWÄIFE
            setLEDcol(144, 149, colorRGB);  // 2nd row
            if (testPrintTimeTexts == 1) Serial.print("ZWÄIFE ");
          }
          break;
        }
    }
  }
}
  
  
// ###########################################################################################################################################
// # Get coords and offsets for showing minutes:
// ###########################################################################################################################################
void getMinuteCoordsAndOffsets(const int minMod, int& plus_coordX, int& coordY, int& minuteVal_offsetX, int& minuteText_coordX, int& minuteText_len) {
  coordY = 7;

  switch (getSetting(setting_type::langLEDlayout))
  {
    case 0:  // DE:
    case 1:  // EN:
    case 2:  // NL:
    case 3:  // SWE:
    case 5:  // FR:
      plus_coordX = 1;
      minuteVal_offsetX = 2;
      minuteText_coordX = 8;
      minuteText_len = minMod == 1 ? 6 : 7;
      break;
  
    case 4:  // IT:
      plus_coordX = 7;
      minuteVal_offsetX = 8;
      minuteText_coordX = 14;
      minuteText_len = 1;
      break;

    case 6:  // GSW:
      plus_coordX = 8;
      minuteVal_offsetX = 9;
      minuteText_coordX = 15;
      minuteText_len = 1;
      break;
      
    case 7:  // CN:
      plus_coordX = 7;
      coordY = 6;
      minuteVal_offsetX = 5;
      minuteText_coordX = 12;
      minuteText_len = 2;
      break;
    
    case 8:  // SWABIAN:
    case 9:  // BAVARIAN:
      plus_coordX = 9;
      minuteVal_offsetX = 9;
      minuteText_coordX = 15;
      minuteText_len = 1;
      break;
  }
}


// ###########################################################################################################################################
// # Display extra minutes function:
// ###########################################################################################################################################
void showMinutes(int minutes) {
  int minMod = (minutes % 5);
  if (minMod == 0)
    return;
  
  // Serial.println(minMod);
  
  int plus_coordX = -1;
  int coordY = -1;
  int minuteVal_offsetX = -1;
  int minuteText_coordX = -1;
  int minuteText_len = -1;

  getMinuteCoordsAndOffsets(minMod, plus_coordX, coordY, minuteVal_offsetX, minuteText_coordX, minuteText_len);
  
  uint32_t colorRGBforMinute = colorRGB;
  
  if (getSetting(setting_type::useFixedMinuteColors) == 1)
  {
    // fixed colors for minutes
    switch (minMod)
    {
      case 1: 
        colorRGBforMinute = getSetting(setting_type::colorMin1);
        break;

      case 2: 
        colorRGBforMinute = getSetting(setting_type::colorMin2);
        break;

      case 3: 
        colorRGBforMinute = getSetting(setting_type::colorMin3);
        break;

      case 4: 
        colorRGBforMinute = getSetting(setting_type::colorMin4);
        break;

      default:
        break;
    }
  }

  setLEDcolXY(plus_coordX, coordY, 1, colorRGB); // +
  setLEDcolXY(minuteVal_offsetX + minMod, coordY, 1, colorRGBforMinute); // num minutes
  setLEDcolXY(minuteText_coordX, coordY, minuteText_len, colorRGB, getSetting(setting_type::animationDelay)); // "MINUTEN"
}


// ###########################################################################################################################################
// # Display current time as scrolling text:
// ###########################################################################################################################################
void showCurrentTimeAsScrollingText(const int hours, const int minutes) {
  const int hours10 = hours / 10;
  const int hours1 = hours % 10;
  const int minutes10 = minutes / 10;
  const int minutes1 = minutes % 10;

  const auto TimeString = std::to_string(hours10) + std::to_string(hours1) + ":" + std::to_string(minutes10) + std::to_string(minutes1);
  showScrollingText(TimeString, color_red); 
}