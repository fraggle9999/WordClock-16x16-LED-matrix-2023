uint32_t colorRGB;
static int lastHourSet = -1;
static int lastMinutesSet = -1;

// ###########################################################################################################################################
// # Display hours and minutes text function:
// ###########################################################################################################################################
void show_time(int hours, int minutes) {

  if ((lastHourSet == hours && lastMinutesSet == minutes) && updatenow == false) {  // Reduce display updates to new minutes and new config updates
    return;
  }

  updatenow = false;
  lastHourSet = hours;
  lastMinutesSet = minutes;

  // Show current time of display update:
  // if (debugtexts == 1) Serial.println("Update display now: " + String(hours) + ":" + String(minutes) + ":" + String(iSecond));

  // Night/Day mode intensity setting:
  if ((usenightmode == 1) && (set_web_intensity == 0)) {
    if ((iHour >= day_time_start) && (iHour <= day_time_stop)) {
      intensity = intensity_day;
      if ((iHour == 0) && (day_time_stop == 23)) intensity = intensity_night;  // Special function if day_time_stop set to 23 and time is 24, so 0...
    } else {
      intensity = intensity_night;
    }
    if (testDayNightmode == 1) {  // Test day/night times function:
      Serial.println("############################################################################################");
      Serial.println("Current time day/night test: " + String(hours) + ":" + String(minutes) + ":" + String(iSecond));
      Serial.println("Current settings: day_time_start: " + String(day_time_start) + " day_time_stop: " + String(day_time_stop));
      for (int i = 0; i < 24; i++) {
        String daynightvar = "-";
        if ((i >= day_time_start) && (i <= day_time_stop)) {
          daynightvar = "Day time";
          if ((i == 0) && (day_time_stop == 23)) daynightvar = "Night time";
        } else {
          daynightvar = "Night time";
        }
        Serial.println("Current hour: " + String(i) + " --> " + daynightvar);
      }
      testDayNightmode = 0;  // Show the list 1x only
      Serial.println("############################################################################################");
    }
  } else {  // Control intensity by WordClock settings or via HTML command:
    if (set_web_intensity == 0) intensity = intensity_day;
    if (set_web_intensity == 1) intensity = 0;
  }
  strip.setBrightness(intensity);

  // Set background color:
  back_color();

  // Static text color or random color mode:
  if (RandomColor == 0) 
    colorRGB = strip.Color(redVal_time, greenVal_time, blueVal_time);
  else
  {
    // avoid black color
    int redVal = 0;
    int greenVal = 0;
    int blueVal = 0;
    
    while ((redVal == 0) && (greenVal == 0) && (blueVal == 0))
    {
      redVal = random(255);
      greenVal = random(255);
      blueVal = random(255);
    }
    
    colorRGB = strip.Color(redVal, greenVal, blueVal);
  }

  // Display time:
  iHour = hours;
  iMinute = minutes;
  
  // circle round the display to new beginning hour
  if (iMinute == 0)
    randomEffect();

  // Test a special time:
  if (testspecialtime == 1) {
    Serial.println("Special time test active: " + String(test_hour) + ":" + String(test_minute));
    iHour = test_hour;
    iMinute = test_minute;
  }

  if (usesinglemin == 1) showMinutes(iMinute);

  showTime(iHour, iMinute);
}


// ###########################################################################################################################################
// # Set LEDs for time part:
// ###########################################################################################################################################
void setLEDtimePart(const std::map<time_parts, position_t>& timeParts, const time_parts timePart, uint32_t color)
{
  const auto Index = timeParts.find(timePart);
  if (Index == timeParts.end())
    return;

  setLEDcolXY(Index->second, color);
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
            { time_parts::o_clock, { 13, 6, 3 } }, 
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
            { time_parts::o_clock, { 8, 6, 7 } }, 
          }
        },
        { 2, // NL
          { } 
        },
        { 3, // SWE
          { } 
        },
        { 4, // IT
          { } 
        },
        { 5, // FR
          { } 
        },
        { 6, // GSW
          { } 
        },
        { 7, // CN
          { } 
        },
        { 8, // SWABIAN
          { } 
        },
        { 9, // BAVARIAN
          { } 
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
        }  
      };

  const auto Index = timePartsMap.find(langLEDlayout);
  if (Index == timePartsMap.end())
  {
    const std::vector<std::string> questionMark =
        { 
          " XXXX",
          "X    X",
          "    X",
          "   X",
          "",
          "   X"
        };

    showCharMatrix(questionMark, 5, 1, strip.Color(255, 0, 0));
    return;
  }

  const auto& timeParts = Index->second;

  // divide minute by 5 to get value for display control
  int minDiv = iMinute / 5;

  int hours = iHour;
  int minutes = iMinute;

  // ES IST:
  setLEDtimePart(timeParts, time_parts::prefix1, colorRGB);
  setLEDtimePart(timeParts, time_parts::prefix2, colorRGB);
  if (testPrintTimeTexts == 1) {
    Serial.println("");
    Serial.print(hours);
    Serial.print(":");
    Serial.print(minutes);
    Serial.print(" --> ES IST ");
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
  // ZEHN: (Minuten)
  if ((minDiv == 2) || (minDiv == 10)) {
    setLEDtimePart(timeParts, time_parts::ten_min, colorRGB);
    if (testPrintTimeTexts == 1) Serial.print("ZEHN ");
  }
  // ZWANZIG:
  if ((minDiv == 4) || (minDiv == 8)) {
    setLEDtimePart(timeParts, time_parts::twenty, colorRGB);
    if (testPrintTimeTexts == 1) Serial.print("ZWANZIG ");
  }
  // NACH:
  if ((minDiv == 1) || (minDiv == 2) || (minDiv == 3) || (minDiv == 4) || (minDiv == 7)) {
    setLEDtimePart(timeParts, time_parts::past, colorRGB);
    if (testPrintTimeTexts == 1) Serial.print("NACH ");
  }
  // VOR:
  if ((minDiv == 5) || (minDiv == 8) || (minDiv == 9) || (minDiv == 10) || (minDiv == 11)) {
    setLEDtimePart(timeParts, time_parts::to, colorRGB);
    if (testPrintTimeTexts == 1) Serial.print("VOR ");
  }
  // HALB:
  if ((minDiv == 5) || (minDiv == 6) || (minDiv == 7)) {
    setLEDtimePart(timeParts, time_parts::half, colorRGB);
    if (testPrintTimeTexts == 1) Serial.print("HALB ");
  }

  //set hour from 1 to 12 (at noon, or midnight)
  int xHour = (iHour % 12);
  if (xHour == 0)
    xHour = 12;
  // at minute 25 hour needs to be counted up:
  // fuenf vor halb 2 = 13:25
  if (iMinute >= 25) {
    if (xHour == 12)
      xHour = 1;
    else
      xHour++;
  }

  uint32_t colorRGBForHour = colorRGB;
  
  if (RandomColor == 1)
    colorRGBForHour = strip.Color(255, 0, 0); // hour always in red

  if ((xHour == 1) && (iMinute <= 4))
  {
    setLEDtimePart(timeParts, time_parts::one1, colorRGB);
    if (testPrintTimeTexts == 1) Serial.print("EIN ");
  }
  else
  {
    const auto Index = hoursMap.find(langLEDlayout);
    if (Index != hoursMap.end())
    {
      const auto& positionVector = Index->second;

      if (xHour < positionVector.size())
        setLEDcolXY(positionVector[xHour - 1], colorRGB);
    }
  }

  if (iMinute < 5) {
    setLEDtimePart(timeParts, time_parts::o_clock, colorRGB);
    if (testPrintTimeTexts == 1) Serial.print("UHR ");
  }

  // ########################################################### NL:
  if (langLEDlayout == 2) {  // NL:

    // HET IS:
    setLEDcol(13, 15, colorRGB);
    setLEDcol(16, 18, colorRGB);  // 2nd row
    setLEDcol(10, 11, colorRGB);
    setLEDcol(20, 21, colorRGB);  // 2nd row

    // VIJF: (Minuten) x:05, x:25, x:35, x:55
    if ((minDiv == 1) || (minDiv == 5) || (minDiv == 7) || (minDiv == 11)) {
      setLEDcol(0, 3, colorRGB);
      setLEDcol(28, 31, colorRGB);  // 2nd row
    }
    // KWART: x:15, x:45
    if ((minDiv == 3) || (minDiv == 9)) {
      setLEDcol(38, 42, colorRGB);
      setLEDcol(53, 57, colorRGB);  // 2nd row
    }
    // TIEN: (Minuten) x:10, x:50
    if ((minDiv == 2) || (minDiv == 10)) {
      setLEDcol(44, 47, colorRGB);
      setLEDcol(48, 51, colorRGB);  // 2nd row
    }
    // TIEN: (TIEN VOOR HALF, TIEN OVER HALF) x:20, x:40 (on request not set to TWINTIG OVER)
    if ((minDiv == 4) || (minDiv == 8)) {
      setLEDcol(44, 47, colorRGB);
      setLEDcol(48, 51, colorRGB);  // 2nd row
    }
    // OVER: x:05, x:10, x:15, x:35, x:40
    if ((minDiv == 1) || (minDiv == 2) || (minDiv == 3) || (minDiv == 7) || (minDiv == 8)) {
      setLEDcol(33, 36, colorRGB);
      setLEDcol(59, 62, colorRGB);  // 2nd row
    }
    // VOOR: x:20, x:25, x:45, x:50, x:55
    if ((minDiv == 4) || (minDiv == 5) || (minDiv == 9) || (minDiv == 10) || (minDiv == 11)) {
      setLEDcol(64, 67, colorRGB);
      setLEDcol(92, 95, colorRGB);  // 2nd row
    }
    // HALF:
    if ((minDiv == 4) || (minDiv == 5) || (minDiv == 6) || (minDiv == 7) || (minDiv == 8)) {
      setLEDcol(107, 110, colorRGB);
      setLEDcol(113, 116, colorRGB);  // 2nd row
    }


    //set hour from 1 to 12 (at noon, or midnight)
    int xHour = (iHour % 12);
    if (xHour == 0)
      xHour = 12;
    // at minute 20 hour needs to be counted up:
    // tien voor half 2 = 13:20
    if (iMinute >= 20) {
      if (xHour == 12)
        xHour = 1;
      else
        xHour++;
    }


    switch (xHour) {
      case 1:
        {
          setLEDcol(99, 101, colorRGB);   // EEN
          setLEDcol(122, 124, colorRGB);  // 2nd row
          break;
        }
      case 2:
        {
          setLEDcol(203, 206, colorRGB);  // TWEE
          setLEDcol(209, 212, colorRGB);  // 2nd row
          break;
        }
      case 3:
        {
          setLEDcol(164, 167, colorRGB);  // DRIE
          setLEDcol(184, 187, colorRGB);  // 2nd row
          break;
        }
      case 4:
        {
          setLEDcol(198, 201, colorRGB);  // VIER
          setLEDcol(214, 217, colorRGB);  // 2nd row
          break;
        }
      case 5:
        {
          setLEDcol(160, 163, colorRGB);  // VIJF
          setLEDcol(188, 191, colorRGB);  // 2nd row
          break;
        }
      case 6:
        {
          setLEDcol(96, 98, colorRGB);    // ZES
          setLEDcol(125, 127, colorRGB);  // 2nd row
          break;
        }
      case 7:
        {
          setLEDcol(129, 133, colorRGB);  // ZEVEN
          setLEDcol(154, 158, colorRGB);  // 2nd row
          break;
        }
      case 8:
        {
          setLEDcol(102, 105, colorRGB);  // ACHT
          setLEDcol(118, 121, colorRGB);  // 2nd row
          break;
        }
      case 9:
        {
          setLEDcol(171, 175, colorRGB);  // NEGEN
          setLEDcol(176, 180, colorRGB);  // 2nd row
          break;
        }
      case 10:
        {
          setLEDcol(140, 143, colorRGB);  // TIEN (Stunden)
          setLEDcol(144, 147, colorRGB);  // 2nd row
          break;
        }
      case 11:
        {
          setLEDcol(168, 170, colorRGB);  // ELF
          setLEDcol(181, 183, colorRGB);  // 2nd row
          break;
        }
      case 12:
        {
          setLEDcol(134, 139, colorRGB);  // TWAALF
          setLEDcol(148, 153, colorRGB);  // 2nd row
          break;
        }
    }

    if (iMinute < 5) {
      setLEDcol(193, 195, colorRGB);  // UUR
      setLEDcol(220, 222, colorRGB);  // 2nd row
    }
  }

  // ########################################################### SWE:
  if (langLEDlayout == 3) {  // SWE:

    // KLOCKAN ÄR:
    setLEDcol(9, 15, colorRGB);
    setLEDcol(16, 22, colorRGB);  // 2nd row
    setLEDcol(5, 6, colorRGB);
    setLEDcol(25, 26, colorRGB);  // 2nd row

    // FEM: (Minuten)
    if ((minDiv == 1) || (minDiv == 5) || (minDiv == 7) || (minDiv == 11)) {
      setLEDcol(64, 66, colorRGB);
      setLEDcol(93, 95, colorRGB);  // 2nd row
    }
    // KVART:
    if ((minDiv == 3) || (minDiv == 9)) {
      setLEDcol(36, 40, colorRGB);
      setLEDcol(55, 59, colorRGB);  // 2nd row
    }
    // TIO: (Minuten)
    if ((minDiv == 2) || (minDiv == 10)) {
      setLEDcol(72, 74, colorRGB);
      setLEDcol(85, 87, colorRGB);  // 2nd row
    }
    // TJUGO:
    if ((minDiv == 4) || (minDiv == 8)) {
      setLEDcol(75, 79, colorRGB);
      setLEDcol(80, 84, colorRGB);  // 2nd row
    }
    // ÖVER:
    if ((minDiv == 1) || (minDiv == 2) || (minDiv == 3) || (minDiv == 4) || (minDiv == 7)) {
      setLEDcol(108, 111, colorRGB);
      setLEDcol(112, 115, colorRGB);  // 2nd row
    }
    // I:
    if ((minDiv == 5) || (minDiv == 8) || (minDiv == 9) || (minDiv == 10) || (minDiv == 11)) {
      setLEDcol(97, 97, colorRGB);
      setLEDcol(126, 126, colorRGB);  // 2nd row
    }
    // HALV:
    if ((minDiv == 5) || (minDiv == 6) || (minDiv == 7)) {
      setLEDcol(140, 143, colorRGB);
      setLEDcol(144, 147, colorRGB);  // 2nd row
    }


    //set hour from 1 to 12 (at noon, or midnight)
    int xHour = (iHour % 12);
    if (xHour == 0)
      xHour = 12;
    // at minute 25 hour needs to be counted up:
    // fuenf vor halb 2 = 13:25
    if (iMinute >= 25) {
      if (xHour == 12)
        xHour = 1;
      else
        xHour++;
    }


    switch (xHour) {
      case 1:
        {
          if (xHour == 1) {
            setLEDcol(170, 172, colorRGB);  // ETT
            setLEDcol(179, 181, colorRGB);  // 2nd row
          }
          break;
        }
      case 2:
        {
          setLEDcol(160, 162, colorRGB);  // TVÅ
          setLEDcol(189, 191, colorRGB);  // 2nd row
          break;
        }
      case 3:
        {
          setLEDcol(132, 134, colorRGB);  // TRE
          setLEDcol(153, 155, colorRGB);  // 2nd row
          break;
        }
      case 4:
        {
          setLEDcol(197, 200, colorRGB);  // FYRA
          setLEDcol(218, 218, colorRGB);  // 2nd row
          break;
        }
      case 5:
        {
          setLEDcol(194, 196, colorRGB);  // FEM
          setLEDcol(219, 221, colorRGB);  // 2nd row
          break;
        }
      case 6:
        {
          setLEDcol(201, 203, colorRGB);  // SEX
          setLEDcol(212, 214, colorRGB);  // 2nd row
          break;
        }
      case 7:
        {
          setLEDcol(173, 175, colorRGB);  // SJU
          setLEDcol(176, 178, colorRGB);  // 2nd row
          break;
        }
      case 8:
        {
          setLEDcol(128, 131, colorRGB);  // ÅTTA
          setLEDcol(156, 159, colorRGB);  // 2nd row
          break;
        }
      case 9:
        {
          setLEDcol(135, 137, colorRGB);  // NIO
          setLEDcol(150, 152, colorRGB);  // 2nd row
          break;
        }
      case 10:
        {
          setLEDcol(168, 170, colorRGB);  // TIO (Stunden)
          setLEDcol(181, 183, colorRGB);  // 2nd row
          break;
        }
      case 11:
        {
          setLEDcol(204, 207, colorRGB);  // ELVA
          setLEDcol(208, 211, colorRGB);  // 2nd row
          break;
        }
      case 12:
        {
          setLEDcol(163, 166, colorRGB);  // TOLV
          setLEDcol(185, 188, colorRGB);  // 2nd row
          break;
        }
    }
  }

  // ########################################################### IT:
  if (langLEDlayout == 4) {  // IT:

    //set hour from 1 to 12 (at noon, or midnight)
    int xHour = (iHour % 12);
    if (xHour == 0)
      xHour = 12;
    // at minute 40 hour needs to be counted up:
    if (iMinute >= 40) {
      if (xHour == 12)
        xHour = 1;
      else
        xHour++;
    }

    // SONO LE:
    if (xHour > 1) { // NOTE: Displayed only from 2 to 23
      setLEDcol(9, 10, colorRGB);   // LE
      setLEDcol(21, 22, colorRGB);  // 2nd row
      setLEDcol(12, 15, colorRGB);  // SONO
      setLEDcol(16, 19, colorRGB);  // 2nd row
      if (testPrintTimeTexts == 1) {
        Serial.println("");
        Serial.print(hours);
        Serial.print(":");
        Serial.print(minutes);
        Serial.print(" --> SONO LE ");
      }
    }

    switch (xHour) {
      case 1:
        {
          setLEDcol(0, 0, colorRGB);      // È
          setLEDcol(31, 31, colorRGB);    // 2nd row
          setLEDcol(104, 108, colorRGB);  // L’UNA
          setLEDcol(115, 119, colorRGB);  // 2nd row
          if (testPrintTimeTexts == 1) Serial.print("È L’UNA ");
          break;
        }
      case 2:
        {
          setLEDcol(101, 103, colorRGB);  // DUE
          setLEDcol(120, 122, colorRGB);  // 2nd row
          if (testPrintTimeTexts == 1) Serial.print("DUE ");
          break;
        }
      case 3:
        {
          setLEDcol(109, 111, colorRGB);  // TRE
          setLEDcol(112, 114, colorRGB);  // 2nd row
          if (testPrintTimeTexts == 1) Serial.print("TRE ");
          break;
        }
      case 4:
        {
          setLEDcol(73, 79, colorRGB);  // QUATTRO
          setLEDcol(80, 86, colorRGB);  // 2nd row
          if (testPrintTimeTexts == 1) Serial.print("QUATTRO ");
          break;
        }
      case 5:
        {
          setLEDcol(64, 69, colorRGB);  // CINQUE
          setLEDcol(90, 95, colorRGB);  // 2nd row
          if (testPrintTimeTexts == 1) Serial.print("CINQUE ");
          break;
        }
      case 6:
        {
          setLEDcol(40, 42, colorRGB);  // SEI
          setLEDcol(53, 55, colorRGB);  // 2nd row
          if (testPrintTimeTexts == 1) Serial.print("SEI ");
          break;
        }
      case 7:
        {
          setLEDcol(43, 47, colorRGB);  // SETTE
          setLEDcol(48, 52, colorRGB);  // 2nd row
          if (testPrintTimeTexts == 1) Serial.print("SETTE ");
          break;
        }
      case 8:
        {
          setLEDcol(70, 73, colorRGB);  // OTTO
          setLEDcol(86, 89, colorRGB);  // 2nd row
          if (testPrintTimeTexts == 1) Serial.print("OTTO ");
          break;
        }
      case 9:
        {
          setLEDcol(97, 100, colorRGB);   // NOVE
          setLEDcol(123, 126, colorRGB);  // 2nd row
          if (testPrintTimeTexts == 1) Serial.print("NOVE ");
          break;
        }
      case 10:
        {
          setLEDcol(138, 142, colorRGB);  // DIECI
          setLEDcol(145, 149, colorRGB);  // 2nd row
          if (testPrintTimeTexts == 1) Serial.print("DIECI ");
          break;
        }
      case 11:
        {
          setLEDcol(1, 6, colorRGB);    // UNDICI
          setLEDcol(25, 30, colorRGB);  // 2nd row
          if (testPrintTimeTexts == 1) Serial.print("UNDICI ");
          break;
        }
      case 12:
        {
          setLEDcol(34, 39, colorRGB);  // DODICI
          setLEDcol(56, 61, colorRGB);  // 2nd row
          if (testPrintTimeTexts == 1) Serial.print("DODICI ");
          break;
        }
    }

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

    // IL EST:
    setLEDcol(14, 15, colorRGB);  // IL
    setLEDcol(16, 17, colorRGB);  // 2nd row
    setLEDcol(10, 12, colorRGB);  // EST
    setLEDcol(19, 21, colorRGB);  // 2nd row

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


    //set hour from 1 to 12 (at noon, or midnight)
    int xHour = (iHour % 12);
    if (xHour == 0)
      xHour = 12;
    // at minute 35 hour needs to be counted up:
    if (iMinute >= 35) {
      if (xHour == 12)
        xHour = 1;
      else
        xHour++;
    }


    switch (xHour) {
      case 1:
        {
          setLEDcol(141, 143, colorRGB);  // UNE
          setLEDcol(144, 146, colorRGB);  // 2nd row
          setLEDcol(135, 139, colorRGB);  // HEURE
          setLEDcol(148, 152, colorRGB);  // 2nd row
          break;
        }
      case 2:
        {
          setLEDcol(96, 99, colorRGB);    // DEUX
          setLEDcol(124, 127, colorRGB);  // 2nd row
          setLEDcol(134, 139, colorRGB);  // HEURES
          setLEDcol(148, 153, colorRGB);  // 2nd row
          break;
        }
      case 3:
        {
          setLEDcol(107, 111, colorRGB);  // TROIS
          setLEDcol(112, 116, colorRGB);  // 2nd row
          setLEDcol(134, 139, colorRGB);  // HEURES
          setLEDcol(148, 153, colorRGB);  // 2nd row
          break;
        }
      case 4:
        {
          setLEDcol(42, 47, colorRGB);    // QUATRE
          setLEDcol(48, 53, colorRGB);    // 2nd row
          setLEDcol(134, 139, colorRGB);  // HEURES
          setLEDcol(148, 153, colorRGB);  // 2nd row
          break;
        }
      case 5:
        {
          setLEDcol(1, 4, colorRGB);      // CINQ
          setLEDcol(27, 30, colorRGB);    // 2nd row
          setLEDcol(134, 139, colorRGB);  // HEURES
          setLEDcol(148, 153, colorRGB);  // 2nd row
          break;
        }
      case 6:
        {
          setLEDcol(64, 66, colorRGB);    // SIX
          setLEDcol(93, 95, colorRGB);    // 2nd row
          setLEDcol(134, 139, colorRGB);  // HEURES
          setLEDcol(148, 153, colorRGB);  // 2nd row
          break;
        }
      case 7:
        {
          setLEDcol(104, 107, colorRGB);  // SEPT
          setLEDcol(116, 119, colorRGB);  // 2nd row
          setLEDcol(134, 139, colorRGB);  // HEURES
          setLEDcol(148, 153, colorRGB);  // 2nd row
          break;
        }
      case 8:
        {
          setLEDcol(32, 35, colorRGB);    // HUIT
          setLEDcol(60, 63, colorRGB);    // 2nd row
          setLEDcol(134, 139, colorRGB);  // HEURES
          setLEDcol(148, 153, colorRGB);  // 2nd row
          break;
        }
      case 9:
        {
          setLEDcol(100, 103, colorRGB);  // NEUF
          setLEDcol(120, 123, colorRGB);  // 2nd row
          setLEDcol(134, 139, colorRGB);  // HEURES
          setLEDcol(148, 153, colorRGB);  // 2nd row
          break;
        }
      case 10:
        {
          setLEDcol(77, 79, colorRGB);    // DIX
          setLEDcol(80, 82, colorRGB);    // 2nd row
          setLEDcol(134, 139, colorRGB);  // HEURES
          setLEDcol(148, 153, colorRGB);  // 2nd row
          break;
        }
      case 11:
        {
          setLEDcol(5, 8, colorRGB);      // ONZE
          setLEDcol(23, 26, colorRGB);    // 2nd row
          setLEDcol(134, 139, colorRGB);  // HEURES
          setLEDcol(148, 153, colorRGB);  // 2nd row
          break;
        }
      case 12:
        {
          // MINUIT (0) or MIDI (12)
          if (iHour == 0 || (iHour == 23 && iMinute >= 35)) setLEDcol(36, 41, colorRGB);   // MINUIT (0)
          if (iHour == 0 || (iHour == 23 && iMinute >= 35)) setLEDcol(54, 59, colorRGB);   // 2nd row
          if (iHour == 12 || (iHour == 11 && iMinute >= 35)) setLEDcol(73, 76, colorRGB);  // MIDI (12)
          if (iHour == 12 || (iHour == 11 && iMinute >= 35)) setLEDcol(83, 86, colorRGB);  // 2nd row
          break;
        }
    }
  }

  // ########################################################### GSW:
  if (langLEDlayout == 6) {  // GSW:

    // ES ISCH:
    setLEDcol(13, 14, colorRGB);  // ES
    setLEDcol(17, 18, colorRGB);  // 2nd row
    setLEDcol(4, 7, colorRGB);    // ISCH
    setLEDcol(24, 27, colorRGB);  // 2nd row

    // FÜÜF: (Minuten)
    if ((minDiv == 1) || (minDiv == 5) || (minDiv == 7) || (minDiv == 11)) {
      setLEDcol(44, 47, colorRGB);
      setLEDcol(48, 51, colorRGB);  // 2nd row
    }
    // VIERTEL:
    if ((minDiv == 3) || (minDiv == 9)) {
      setLEDcol(72, 78, colorRGB);
      setLEDcol(81, 87, colorRGB);  // 2nd row
    }
    // ZÄH: (Minuten)
    if ((minDiv == 2) || (minDiv == 10)) {
      setLEDcol(34, 36, colorRGB);
      setLEDcol(59, 61, colorRGB);  // 2nd row
    }
    // ZWÄNZG:
    if ((minDiv == 4) || (minDiv == 8)) {
      setLEDcol(65, 70, colorRGB);
      setLEDcol(89, 94, colorRGB);  // 2nd row
    }
    // AB:
    if ((minDiv == 1) || (minDiv == 2) || (minDiv == 3) || (minDiv == 4) || (minDiv == 7)) {
      setLEDcol(110, 111, colorRGB);
      setLEDcol(112, 113, colorRGB);  // 2nd row
    }
    // VOR:
    if ((minDiv == 5) || (minDiv == 8) || (minDiv == 9) || (minDiv == 10) || (minDiv == 11)) {
      setLEDcol(107, 109, colorRGB);
      setLEDcol(114, 116, colorRGB);  // 2nd row
    }
    // HALBI:
    if ((minDiv == 5) || (minDiv == 6) || (minDiv == 7)) {
      setLEDcol(101, 105, colorRGB);
      setLEDcol(118, 122, colorRGB);  // 2nd row
    }


    // set hour from 1 to 12 (at noon, or midnight)
    int xHour = (iHour % 12);
    if (xHour == 0)
      xHour = 12;
    // at minute 25 hour needs to be counted up:
    // fuenf vor halb 2 = 13:25
    if (iMinute >= 25) {
      if (xHour == 12)
        xHour = 1;
      else
        xHour++;
    }


    switch (xHour) {
      case 1:
        {
          setLEDcol(128, 130, colorRGB);  // EIS
          setLEDcol(157, 159, colorRGB);  // 2nd row
          break;
        }
      case 2:
        {
          setLEDcol(136, 139, colorRGB);  // ZWEI
          setLEDcol(148, 151, colorRGB);  // 2nd row
          break;
        }
      case 3:
        {
          setLEDcol(96, 99, colorRGB);    // DRÜÜ
          setLEDcol(124, 127, colorRGB);  // 2nd row
          break;
        }
      case 4:
        {
          setLEDcol(160, 164, colorRGB);  // VIERI
          setLEDcol(187, 191, colorRGB);  // 2nd row
          break;
        }
      case 5:
        {
          setLEDcol(171, 175, colorRGB);  // FÜÜFI
          setLEDcol(176, 180, colorRGB);  // 2nd row
          break;
        }
      case 6:
        {
          setLEDcol(165, 170, colorRGB);  // SÄCHSI
          setLEDcol(181, 186, colorRGB);  // 2nd row
          break;
        }
      case 7:
        {
          setLEDcol(202, 207, colorRGB);  // SIEBNI
          setLEDcol(208, 213, colorRGB);  // 2nd row
          break;
        }
      case 8:
        {
          setLEDcol(192, 196, colorRGB);  // ACHTI
          setLEDcol(219, 223, colorRGB);  // 2nd row
          break;
        }
      case 9:
        {
          setLEDcol(131, 135, colorRGB);  // NÜÜNI
          setLEDcol(152, 156, colorRGB);  // 2nd row
          break;
        }
      case 10:
        {
          setLEDcol(197, 201, colorRGB);  // ZÄHNI (Stunden)
          setLEDcol(214, 218, colorRGB);  // 2nd row
          break;
        }
      case 11:
        {
          setLEDcol(140, 143, colorRGB);  // ELFI
          setLEDcol(144, 147, colorRGB);  // 2nd row
          break;
        }
      case 12:
        {
          setLEDcol(233, 238, colorRGB);  // ZWÖLFI
          setLEDcol(241, 246, colorRGB);  // 2nd row
          break;
        }
    }
  }

  // ########################################################### CN:
  if (langLEDlayout == 7) {  // CN:

    // IT IS: 现在 时间
    setLEDcol(44, 45, colorRGB);
    setLEDcol(50, 51, colorRGB);  // 2nd row
    setLEDcol(40, 41, colorRGB);
    setLEDcol(54, 55, colorRGB);  // 2nd row

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

    //set hour from 1 to 12 (at noon, or midnight)
    int xHour = (iHour % 12);
    if (xHour == 0) {
      xHour = 12;
    }


    switch (xHour) {
      case 1:
        {
          setLEDcol(75, 76, colorRGB);  // 一点
          setLEDcol(83, 84, colorRGB);  // 2nd row
          break;
        }
      case 2:
        {
          setLEDcol(72, 73, colorRGB);  // 二点
          setLEDcol(86, 87, colorRGB);  // 2nd row
          break;
        }
      case 3:
        {
          setLEDcol(36, 37, colorRGB);  // 三点
          setLEDcol(58, 59, colorRGB);  // 2nd row
          break;
        }
      case 4:
        {
          setLEDcol(34, 35, colorRGB);  // 四点
          setLEDcol(60, 61, colorRGB);  // 2nd row
          break;
        }
      case 5:
        {
          setLEDcol(70, 71, colorRGB);  // 五点
          setLEDcol(88, 89, colorRGB);  // 2nd row
          break;
        }
      case 6:
        {
          setLEDcol(68, 69, colorRGB);  // 六点
          setLEDcol(90, 91, colorRGB);  // 2nd row
          break;
        }
      case 7:
        {
          setLEDcol(66, 67, colorRGB);  // 七点
          setLEDcol(92, 93, colorRGB);  // 2nd row
          break;
        }
      case 8:
        {
          setLEDcol(108, 109, colorRGB);  // 八点
          setLEDcol(114, 115, colorRGB);  // 2nd row
          break;
        }
      case 9:
        {
          setLEDcol(106, 107, colorRGB);  // 九点
          setLEDcol(116, 117, colorRGB);  // 2nd row
          break;
        }
      case 10:
        {
          setLEDcol(104, 105, colorRGB);  // 十点
          setLEDcol(118, 119, colorRGB);  // 2nd row
          break;
        }
      case 11:
        {
          setLEDcol(75, 77, colorRGB);  // 十一点
          setLEDcol(82, 84, colorRGB);  // 2nd row
          break;
        }
      case 12:
        {
          setLEDcol(72, 74, colorRGB);  // 十二点
          setLEDcol(85, 87, colorRGB);  // 2nd row
          break;
        }
    }

    if (iMinute < 5) {
      setLEDcol(162, 162, colorRGB);  // 整
      setLEDcol(189, 189, colorRGB);  // 2nd row
    }
  }

  // ########################################################### SWABIAN GERMAN:
  if (langLEDlayout == 8) {  // SWABIAN GERMAN:

    // ES ISCH:
    setLEDcol(14, 15, colorRGB);
    setLEDcol(16, 17, colorRGB);  // 2nd row
    setLEDcol(9, 12, colorRGB);
    setLEDcol(19, 22, colorRGB);  // 2nd row
    if (testPrintTimeTexts == 1) {
      Serial.println("");
      Serial.print(hours);
      Serial.print(":");
      Serial.print(minutes);
      Serial.print(" --> ES ISCH ");
    }

    // FÜNF: (Minuten)
    if ((minDiv == 1) || (minDiv == 5) || (minDiv == 7) || (minDiv == 11)) {
      setLEDcol(76, 79, colorRGB);
      setLEDcol(80, 83, colorRGB);  // 2nd row
      if (testPrintTimeTexts == 1) Serial.print("FÜNF ");
    }

    // VIERTL:
    if (minDiv == 3) {
      setLEDcol(0, 5, colorRGB);
      setLEDcol(26, 31, colorRGB);  // 2nd row
      if (testPrintTimeTexts == 1) Serial.print("VIERTL ");
    }

    // DREIVIERTL:
    if (minDiv == 9) {
      setLEDcol(33, 42, colorRGB);
      setLEDcol(53, 62, colorRGB);  // 2nd row
      if (testPrintTimeTexts == 1) Serial.print("DREIVIERTL ");
    }

    // ZEHN: (Minuten)
    if ((minDiv == 2) || (minDiv == 4) || (minDiv == 8) || (minDiv == 10)) {
      setLEDcol(71, 74, colorRGB);
      setLEDcol(85, 88, colorRGB);  // 2nd row
      if (testPrintTimeTexts == 1) Serial.print("ZEHN ");
    }

    // NACH:
    if ((minDiv == 1) || (minDiv == 2) || (minDiv == 7) || (minDiv == 8)) {
      setLEDcol(65, 68, colorRGB);
      setLEDcol(91, 94, colorRGB);  // 2nd row
      if (testPrintTimeTexts == 1) Serial.print("NACH ");
    }

    // VOR:
    if ((minDiv == 4) || (minDiv == 5) || (minDiv == 10) || (minDiv == 11)) {
      setLEDcol(109, 111, colorRGB);
      setLEDcol(112, 114, colorRGB);  // 2nd row
      if (testPrintTimeTexts == 1) Serial.print("VOR ");
    }

    // HALB:
    if ((minDiv == 4) || (minDiv == 5) || (minDiv == 6) || (minDiv == 7) || (minDiv == 8)) {
      setLEDcol(102, 105, colorRGB);
      setLEDcol(118, 121, colorRGB);  // 2nd row
      if (testPrintTimeTexts == 1) Serial.print("HALB ");
    }

    //set hour from 1 to 12 (at noon, or midnight)
    int xHour = (iHour % 12);
    if (xHour == 0)
      xHour = 12;
    // at minute 25 hour needs to be counted up:
    // fuenf vor halb 2 = 13:15
    if (iMinute >= 15) {
      if (xHour == 12)
        xHour = 1;
      else
        xHour++;
    }

    switch (xHour) {
      case 1:
        {
          if (xHour == 1) {
            setLEDcol(165, 168, colorRGB);  // OISE
            setLEDcol(183, 186, colorRGB);  // 2nd row
            if (testPrintTimeTexts == 1) Serial.print("OISE ");
          }
          break;
        }
      case 2:
        {
          setLEDcol(160, 164, colorRGB);  // ZWOIE
          setLEDcol(187, 191, colorRGB);  // 2nd row
          if (testPrintTimeTexts == 1) Serial.print("ZWOIE ");
          break;
        }
      case 3:
        {
          setLEDcol(235, 239, colorRGB);  // DREIE
          setLEDcol(240, 244, colorRGB);  // 2nd row
          if (testPrintTimeTexts == 1) Serial.print("DREIE ");
          break;
        }
      case 4:
        {
          setLEDcol(128, 132, colorRGB);  // VIERE
          setLEDcol(155, 159, colorRGB);  // 2nd row
          if (testPrintTimeTexts == 1) Serial.print("VIERE ");
          break;
        }
      case 5:
        {
          setLEDcol(139, 143, colorRGB);  // FÜNFE
          setLEDcol(144, 148, colorRGB);  // 2nd row
          if (testPrintTimeTexts == 1) Serial.print("FÜNFE ");
          break;
        }
      case 6:
        {
          setLEDcol(133, 138, colorRGB);  // SECHSE
          setLEDcol(149, 154, colorRGB);  // 2nd row
          if (testPrintTimeTexts == 1) Serial.print("SECHSE ");
          break;
        }
      case 7:
        {
          setLEDcol(169, 175, colorRGB);  // SIEBENE
          setLEDcol(176, 182, colorRGB);  // 2nd row
          if (testPrintTimeTexts == 1) Serial.print("SIEBENE ");
          break;
        }
      case 8:
        {
          setLEDcol(203, 207, colorRGB);  // ACHTE
          setLEDcol(208, 212, colorRGB);  // 2nd row
          if (testPrintTimeTexts == 1) Serial.print("ACHTE ");
          break;
        }
      case 9:
        {
          setLEDcol(192, 196, colorRGB);  // NEUNE
          setLEDcol(219, 223, colorRGB);  // 2nd row
          if (testPrintTimeTexts == 1) Serial.print("NEUNE ");
          break;
        }
      case 10:
        {
          setLEDcol(96, 100, colorRGB);   // ZEHNE (Stunden)
          setLEDcol(123, 127, colorRGB);  // 2nd row
          if (testPrintTimeTexts == 1) Serial.print("ZEHNE ");
          break;
        }
      case 11:
        {
          setLEDcol(232, 235, colorRGB);  // ELFE
          setLEDcol(244, 247, colorRGB);  // 2nd row
          if (testPrintTimeTexts == 1) Serial.print("ELFE ");
          break;
        }
      case 12:
        {
          setLEDcol(197, 202, colorRGB);  // ZWÖLFE
          setLEDcol(213, 218, colorRGB);  // 2nd row
          if (testPrintTimeTexts == 1) Serial.print("ZWÖLFE ");
          break;
        }
    }
  }

  // ########################################################### BAVARIAN:
  if (langLEDlayout == 9) {  // BAVARIAN:

    // ES IS:
    setLEDcol(14, 15, colorRGB);
    setLEDcol(16, 17, colorRGB);  // 2nd row
    setLEDcol(9, 10, colorRGB);
    setLEDcol(21, 22, colorRGB);  // 2nd row
    if (testPrintTimeTexts == 1) {
      Serial.println("");
      Serial.print(hours);
      Serial.print(":");
      Serial.print(minutes);
      Serial.print(" --> ES IS ");
    }

    // FÜNF: (Minuten)
    if ((minDiv == 1) || (minDiv == 5) || (minDiv == 7) || (minDiv == 11)) {
      setLEDcol(33, 36, colorRGB);
      setLEDcol(59, 62, colorRGB);  // 2nd row
      if (testPrintTimeTexts == 1) Serial.print("FÜNF ");
    }

    // VIERTL:
    if ((minDiv == 3) || (minDiv == 9)) {
      setLEDcol(37, 42, colorRGB);
      setLEDcol(53, 58, colorRGB);  // 2nd row
      if (testPrintTimeTexts == 1) Serial.print("VIERTL ");
    }

    // ZWANZIG:
    if ((minDiv == 4) || (minDiv == 8)) {
      setLEDcol(72, 78, colorRGB);
      setLEDcol(81, 87, colorRGB);  // 2nd row
      if (testPrintTimeTexts == 1) Serial.print("ZWANZIG ");
    }

    // ZEHN: (Minuten)
    if ((minDiv == 2) || (minDiv == 10)) {
      setLEDcol(44, 47, colorRGB);
      setLEDcol(48, 51, colorRGB);  // 2nd row
      if (testPrintTimeTexts == 1) Serial.print("ZEHN ");
    }

    // NOCH:
    if ((minDiv == 1) || (minDiv == 2) || (minDiv == 3) || (minDiv == 4) || (minDiv == 7)) {
      setLEDcol(64, 67, colorRGB);
      setLEDcol(92, 95, colorRGB);  // 2nd row
      if (testPrintTimeTexts == 1) Serial.print("NOCH ");
    }

    // VOA:
    if ((minDiv == 5) || (minDiv == 8) || (minDiv == 9) || (minDiv == 10) || (minDiv == 11)) {
      setLEDcol(68, 70, colorRGB);
      setLEDcol(89, 91, colorRGB);  // 2nd row
      if (testPrintTimeTexts == 1) Serial.print("VOA ");
    }

    // HOIBE:
    if ((minDiv == 5) || (minDiv == 6) || (minDiv == 7)) {
      setLEDcol(107, 111, colorRGB);
      setLEDcol(112, 116, colorRGB);  // 2nd row
      if (testPrintTimeTexts == 1) Serial.print("HOIBE ");
    }

    //set hour from 1 to 12 (at noon, or midnight)
    int xHour = (iHour % 12);
    if (xHour == 0)
      xHour = 12;
    // at minute 25 hour needs to be counted up:
    // fuenf vor halb 2 = 13:25
    if (iMinute >= 25) {
      if (xHour == 12)
        xHour = 1;
      else
        xHour++;
    }

    switch (xHour) {
      case 1:
        {
          if (xHour == 1) {
            setLEDcol(102, 105, colorRGB);  // OANS
            setLEDcol(118, 121, colorRGB);  // 2nd row
            if (testPrintTimeTexts == 1) Serial.print("OANS ");
          }
          break;
        }
      case 2:
        {
          setLEDcol(161, 164, colorRGB);  // ZWOA
          setLEDcol(187, 190, colorRGB);  // 2nd row
          if (testPrintTimeTexts == 1) Serial.print("ZWOA ");
          break;
        }
      case 3:
        {
          setLEDcol(165, 168, colorRGB);  // DREI
          setLEDcol(183, 186, colorRGB);  // 2nd row
          if (testPrintTimeTexts == 1) Serial.print("DREI ");
          break;
        }
      case 4:
        {
          if (iMinute < 5) {
            setLEDcol(194, 196, colorRGB);  // VIA
            setLEDcol(219, 221, colorRGB);  // 2nd row
            if (testPrintTimeTexts == 1) Serial.print("VIA ");
          } else {
            setLEDcol(192, 196, colorRGB);  // VIARE
            setLEDcol(219, 223, colorRGB);  // 2nd row
            if (testPrintTimeTexts == 1) Serial.print("VIARE ");
          }
          break;
        }
      case 5:
        {
          if (iMinute < 5) {
            setLEDcol(98, 101, colorRGB);   // FÜNF
            setLEDcol(122, 125, colorRGB);  // 2nd row
            if (testPrintTimeTexts == 1) Serial.print("FÜNF ");
          } else {
            setLEDcol(97, 101, colorRGB);   // FÜNFE
            setLEDcol(122, 126, colorRGB);  // 2nd row
            if (testPrintTimeTexts == 1) Serial.print("FÜNFE ");
          }
          break;
        }
      case 6:
        {
          if (iMinute < 5) {
            setLEDcol(203, 207, colorRGB);  // SECKS
            setLEDcol(208, 212, colorRGB);  // 2nd row
            if (testPrintTimeTexts == 1) Serial.print("SECKS ");
          } else {
            setLEDcol(202, 207, colorRGB);  // SECKSE
            setLEDcol(208, 213, colorRGB);  // 2nd row
            if (testPrintTimeTexts == 1) Serial.print("SECKSE ");
          }
          break;
        }
      case 7:
        {
          if (iMinute < 5) {
            setLEDcol(171, 175, colorRGB);  // SIEBN
            setLEDcol(176, 180, colorRGB);  // 2nd row
            if (testPrintTimeTexts == 1) Serial.print("SIEBN ");
          } else {
            setLEDcol(170, 175, colorRGB);  // SIEBNE
            setLEDcol(176, 181, colorRGB);  // 2nd row
            if (testPrintTimeTexts == 1) Serial.print("SIEBNE ");
          }
          break;
        }
      case 8:
        {
          if (iMinute < 5) {
            setLEDcol(129, 132, colorRGB);  // ACHT
            setLEDcol(155, 158, colorRGB);  // 2nd row
            if (testPrintTimeTexts == 1) Serial.print("ACHT ");
          } else {
            setLEDcol(128, 132, colorRGB);  // ACHTE
            setLEDcol(155, 159, colorRGB);  // 2nd row
            if (testPrintTimeTexts == 1) Serial.print("ACHTE ");
          }
          break;
        }
      case 9:
        {
          if (iMinute < 5) {
            setLEDcol(198, 201, colorRGB);  // NEIN
            setLEDcol(214, 217, colorRGB);  // 2nd row
            if (testPrintTimeTexts == 1) Serial.print("NEIN ");
          } else {
            setLEDcol(197, 201, colorRGB);  // NEINE
            setLEDcol(214, 218, colorRGB);  // 2nd row
            if (testPrintTimeTexts == 1) Serial.print("NEINE ");
          }
          break;
        }
      case 10:
        {
          if (iMinute < 5) {
            setLEDcol(134, 137, colorRGB);  // ZEHN (Stunden)
            setLEDcol(150, 153, colorRGB);  // 2nd row
            if (testPrintTimeTexts == 1) Serial.print("ZEHN ");
          } else {
            setLEDcol(133, 137, colorRGB);  // ZEHNE (Stunden)
            setLEDcol(150, 154, colorRGB);  // 2nd row
            if (testPrintTimeTexts == 1) Serial.print("ZEHNE ");
          }
          break;
        }
      case 11:
        {
          if (iMinute < 5) {
            setLEDcol(237, 239, colorRGB);  // EJF
            setLEDcol(240, 242, colorRGB);  // 2nd row
            if (testPrintTimeTexts == 1) Serial.print("EJF ");
          } else {
            setLEDcol(236, 239, colorRGB);  // EJFE
            setLEDcol(240, 243, colorRGB);  // 2nd row
            if (testPrintTimeTexts == 1) Serial.print("EJFE ");
          }
          break;
        }
      case 12:
        {
          if (iMinute < 5) {
            setLEDcol(139, 143, colorRGB);  // ZWÄIF
            setLEDcol(144, 148, colorRGB);  // 2nd row
            if (testPrintTimeTexts == 1) Serial.print("ZWÄIF ");
          } else {
            setLEDcol(138, 143, colorRGB);  // ZWÄIFE
            setLEDcol(144, 149, colorRGB);  // 2nd row
            if (testPrintTimeTexts == 1) Serial.print("ZWÄIFE ");
          }
          break;
        }
    }

    if (iMinute < 5) {
      setLEDcol(232, 234, colorRGB);  // UAH
      setLEDcol(245, 247, colorRGB);  // 2nd row
      if (testPrintTimeTexts == 1) Serial.print("UAH ");
    }
  }

  strip.show();
}
  
  
// ###########################################################################################################################################
// # Display extra minutes function:
// ###########################################################################################################################################
void showMinutes(int minutes) {
  int minMod = (minutes % 5);
  if (minMod == 0)
    return;
  
  // Serial.println(minMod);
  
  int coordY = 7;

  int plus_coordX = -1;
  int minuteVal_offsetX = -1;
  int minuteText_coordX = -1;
  int minuteText_len = -1;

  switch (langLEDlayout)
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
  
  setLEDcolXY(plus_coordX, coordY, 1, colorRGB);
  setLEDcolXY(minuteVal_offsetX + minMod, coordY, 1, colorRGB);
  setLEDcolXY(minuteText_coordX, coordY, minuteText_len, colorRGB);
}
