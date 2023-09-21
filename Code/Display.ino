// ###########################################################################################################################################
// # Clear the display:
// ###########################################################################################################################################
void ClearDisplay() {
  uint32_t c0 = strip.Color(0, 0, 0);
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, c0);
  }
}


// ###########################################################################################################################################
// # Show the IP-address on the display:
// ###########################################################################################################################################
void ShowIPaddress() {
  Serial.println("Show current IP-address on the display: " + IpAddress2String(WiFi.localIP()));
  int ipdelay = 2000;

  // Testing the digits:
  // for (int i = 0; i < 10; i++) {
  //   ClearDisplay();
  //   numbers(i, 3);
  //   numbers(i, 2);
  //   numbers(i, 1);
  //   strip.show();
  //   delay(ipdelay);
  // }

  uint32_t ipColor = strip.Color(255, 0, 0);

  for (int octet = 0; octet < 4; ++octet) {
    ClearDisplay();

    int OctetValue = int(WiFi.localIP()[octet]);
    Serial.print("Octet ");
    Serial.print(octet);
    Serial.print(" = ");
    Serial.println(OctetValue);

    for (int digit = 2; digit >= 0; --digit) {
      int DigitValue = getDigit(OctetValue, digit);
      Serial.print("digit ");
      Serial.print(digit);
      Serial.print(" = ");
      Serial.println(DigitValue);

      numbers(DigitValue, digit, ipColor);
    }

    if (octet < 3)                     // nicht beim letzten Wert
      setLEDcolXY(15, 5, 1, ipColor);  // Punkt hinter dem Wert

    setLEDcolXY(3 + octet, 7, 1, ipColor);  // Ordnungszahl unten (1 bis 4)

    strip.show();
    delay(ipdelay);
  }
}


// ###########################################################################################################################################
// # Set one number on the display at a specific coordinate:
// ###########################################################################################################################################
void showNumber(const std::vector<std::string>& number, int coordX, int coordY, uint32_t color) {
  for (int row = 0; row < number.size(); ++row)
  {
    const std::string line = number[row];

    for (int col = 0; col < line.size(); ++col)
    {
      const char Ch = line[col];

      if (Ch != ' ')
        setLEDcolXY(coordX + col, coordY + row, 1, color);
    }
  }
}


// ###########################################################################################################################################
// # Set the numbers on the display in each single row:
// ###########################################################################################################################################
void numbers(int wert, int segment, uint32_t color) {
  static std::vector<std::vector<std::string>> numberVector = {
    { "XXXX",
      "X  X",
      "X  X",
      "X  X",
      "XXXX" },

    { "   X",
      "   X",
      "   X",
      "   X",
      "   X" },

    { "XXXX",
      "   X",
      "XXXX",
      "X   ",
      "XXXX" },

    { "XXXX",
      "   X",
      "XXXX",
      "   X",
      "XXXX" },

    { "X  X",
      "X  X",
      "XXXX",
      "   X",
      "   X" },

    { "XXXX",
      "X   ",
      "XXXX",
      "   X",
      "XXXX" },

    { "XXXX",
      "X   ",
      "XXXX",
      "X  X",
      "XXXX" },

    { "XXXX",
      "   X",
      "   X",
      "   X",
      "   X" },

    { "XXXX",
      "X  X",
      "XXXX",
      "X  X",
      "XXXX" },

    { "XXXX",
      "X  X",
      "XXXX",
      "   X",
      "XXXX" }
  };

  showNumber(numberVector[wert], (2 - segment) * 5, 1, color);
}


// ###########################################################################################################################################
// # Get a digit from a number at position pos: (Split IP-address octets in single digits)
// ###########################################################################################################################################
int getDigit(int number, int pos) {
  return (pos == 0) ? number % 10 : getDigit(number / 10, --pos);
}


// ###########################################################################################################################################
// # Show a LED output for RESET in the different languages:
// ###########################################################################################################################################
void ShowReset(uint32_t color) {
  updatedevice = false;
  delay(1000);
  ClearDisplay();

  switch (langLEDlayout) {
    case 0:                          // DE:
      setLEDcolXY(5, 4, 2, color);   // RE
      setLEDcolXY(7, 5, 2, color);   // SE
      setLEDcolXY(11, 6, 2, color);  // T
      break;

    case 1:                          // EN:
      setLEDcolXY(10, 3, 2, color);  // RE
      setLEDcolXY(0, 5, 2, color);   // SE
      setLEDcolXY(11, 6, 1, color);  // T
      break;


    case 2:                          // NL:
      setLEDcolXY(14, 1, 1, color);  // R
      setLEDcolXY(14, 3, 2, color);  // ES
      setLEDcolXY(11, 5, 1, color);  // E
      setLEDcolXY(11, 6, 1, color);  // T
      break;

    case 3:                         // SWE:
      setLEDcolXY(8, 2, 5, color);  // RESET
      break;

    case 4:                         // IT:
      setLEDcolXY(4, 0, 1, color);  // R
      setLEDcolXY(6, 0, 1, color);  // E
      setLEDcolXY(0, 1, 3, color);  // SET
      break;

    case 5:                         // FR:
      setLEDcolXY(2, 0, 3, color);  // RES
      setLEDcolXY(10, 0, 1, color); // E
      setLEDcolXY(11, 1, 1, color); // T
      break;

    case 6:                         // GSW:
      setLEDcolXY(0, 0, 5, color);  // RESET
      break;

    case 7:                         // CN:
      setLEDcolXY(8, 1, 2, color);  // RESET 重置
      break;

    case 8:                         // SWABIAN GERMAN:
      setLEDcolXY(6, 1, 2, color);  // RE
      setLEDcolXY(8, 3, 2, color);  // SE
      setLEDcolXY(3, 6, 1, color);  // T
      break;

    case 9:                         // BAVARIAN:
      setLEDcolXY(5, 3, 1, color);  // R
      setLEDcolXY(10, 4, 1, color);  // E
      setLEDcolXY(0, 5, 1, color);  // S
      setLEDcolXY(5, 5, 1, color);  // E
      setLEDcolXY(15, 5, 1, color);  // T
      break;

    default:
      break;
  }

  strip.show();
}


// ###########################################################################################################################################
// # Actual function, which controls 1/0 of the LED with color value:
// ###########################################################################################################################################
void setLEDcol(int ledNrFrom, int ledNrTo, uint32_t color) {
  if (ledNrFrom > ledNrTo) {
    setLEDcol(ledNrTo, ledNrFrom, color);  // Sets LED numbers in correct order
  } else {
    /*
    Serial.print("setLEDcol: from ");
    Serial.print(ledNrFrom);
    Serial.print(" to ");
    Serial.print(ledNrTo);
    Serial.print(" with color ");
    Serial.println(color);
*/

    for (int i = ledNrFrom; i <= ledNrTo; i++) {
      if ((i >= 0) && (i < NUMPIXELS))
        strip.setPixelColor(i, color);
    }
  }
}


// ###########################################################################################################################################
// # Calculate pixel nrs from x/y coordinate:
// ###########################################################################################################################################
void coords2Pixel(int coordX, int coordY, int& firstRow, int& secondRow) {
  firstRow = 15 - coordX + 32 * coordY;
  secondRow = 16 + coordX + 32 * coordY;
}


// ###########################################################################################################################################
// # Actual function, which controls 1/0 of the LED with X/Y position and color value:
// ###########################################################################################################################################
void setLEDcolXY(int coordX, int coordY, int numCols, uint32_t color) {
  /*
  Serial.print("setLEDcolXY: X = ");
  Serial.print(coordX);
  Serial.print(", Y = ");
  Serial.print(coordY);
  Serial.print(", numCols = ");
  Serial.print(numCols);
*/
  int firstRow, secondRow;
  coords2Pixel(coordX, coordY, firstRow, secondRow);

  /*
  Serial.print(" -> firstRow = ");
  Serial.print(firstRow);
  Serial.print(", secondRow = ");
  Serial.println(secondRow);
*/

  setLEDcol(firstRow, firstRow - numCols + 1, color);  // firstRow ist von rechts nach links numeriert
  setLEDcol(secondRow, secondRow + numCols - 1, color);
}


// ###########################################################################################################################################
// # Draw a circle round the clock.
// ###########################################################################################################################################
void circleRound() {
  delay(200);
  Serial.println("Starting circle round the display ...");

  uint32_t color = strip.Color(255, 0, 0);
  uint32_t color_black = strip.Color(0, 0, 0);

  const int max_cols = 16;
  const int max_rows = 8;
  const int delay_val = 50;

  // top from left to right
  for (int x = 0; x < max_cols; ++x) {
    setLEDcolXY(x, 0, 1, color);
    strip.show();
    //    delay(delay_val);
    setLEDcolXY(x, 0, 1, color_black);
    //  strip.show();
  }

  // right from top to bottom (starting in second row, ending in second but last row)
  for (int y = 1; y < max_rows - 1; ++y) {
    setLEDcolXY(max_cols - 1, y, 1, color);
    strip.show();
    //    delay(delay_val);
    setLEDcolXY(max_cols - 1, y, 1, color_black);
    //  strip.show();
  }

  // bottom from right to left
  for (int x = max_cols - 1; x >= 0; --x) {
    setLEDcolXY(x, max_rows - 1, 1, color);
    strip.show();
    //    delay(delay_val);
    setLEDcolXY(x, max_rows - 1, 1, color_black);
    //  strip.show();
  }

  // left from bottom to top (starting in second but last row, ending in second row)
  for (int y = max_rows - 2; y > 0; --y) {
    setLEDcolXY(0, y, 1, color);
    strip.show();
    //    delay(delay_val);
    setLEDcolXY(0, y, 1, color_black);
    //  strip.show();
  }

  ClearDisplay();
  strip.show();

  Serial.println("Ending circle round the display ...");
}


// ###########################################################################################################################################
// # Update the display / time on it:
// ###########################################################################################################################################
void update_display() {
  // if (debugtexts == 1) Serial.println("Time: " + iStartTime);

  // Show the current time or use the time text test function:
  if (testTime == 0) {  // Show the current time:
    show_time(iHour, iMinute);
  } else {  // TEST THE DISPLAY TIME OUTPUT:
    ClearDisplay();
    strip.show();
    Serial.println(" ");
    Serial.println("Show 'TEST' text...");
    strip.setBrightness(25);
    redVal_back = 0;
    greenVal_back = 0;
    blueVal_back = 0;
    usenightmode = 0;
    uint32_t c = strip.Color(redVal_time, greenVal_time, blueVal_time);
    int TextWait = 500;
    showText("TEST", TextWait, c);
    for (int i = 1; i <= 12; i++) {  // 12 hours only:
      show_time(i, 0);
      delay(3000);
    }
    Serial.println(" ");
    Serial.println(" ");
    Serial.println(" ");
    for (int i = 0; i <= 55; i += 5) {  // 5 minutes steps only:
      show_time(9, i);
      delay(3000);
    }
    Serial.println(" ");
    Serial.println(" ");
    Serial.println(" ");
    for (int i = 9; i <= 12; i++) {  // Hours 9 to 12 with all minute texts:
      for (int y = 0; y < 60; y++) {
        show_time(i, y);
        delay(500);
      }
    }
  }
}


// ###########################################################################################################################################
// # Background color function: SET ALL LEDs OFF
// ###########################################################################################################################################
void back_color() {
  uint32_t c0 = strip.Color(redVal_back, greenVal_back, blueVal_back);  // Background color
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, c0);
  }
}


// ###########################################################################################################################################
// # Actual function, which controls 1/0 of the LED:
// ###########################################################################################################################################
void setLED(int ledNrFrom, int ledNrTo, int switchOn) {
  if (ledNrFrom > ledNrTo) {
    setLED(ledNrTo, ledNrFrom, switchOn);  // Sets LED numbers in correct order
  } else {
    for (int i = ledNrFrom; i <= ledNrTo; i++) {
      if ((i >= 0) && (i < NUMPIXELS))
        strip.setPixelColor(i, strip.Color(redVal_time, greenVal_time, blueVal_time));
    }
  }
  if (switchOn == 0) {
    for (int i = ledNrFrom; i <= ledNrTo; i++) {
      if ((i >= 0) && (i < NUMPIXELS))
        strip.setPixelColor(i, strip.Color(0, 0, 0));  // Switch LEDs off
    }
  }
}


// ###########################################################################################################################################
// # Actual function, which controls 1/0 of the LED with x/y coords:
// ###########################################################################################################################################
void setLEDXY(int coordX, int coordY, int numCols, int switchOn) {
  uint32_t color = strip.Color(redVal_time, greenVal_time, blueVal_time);
  if (switchOn == 0)
    color = strip.Color(0, 0, 0);

  setLEDcolXY(coordX, coordY, numCols, color);
}


// ###########################################################################################################################################
// # Startup LED test function
// ###########################################################################################################################################
void callStartText() {
  Serial.println("Show 'WordClock' startup text...");
  uint32_t c = strip.Color(redVal_time, greenVal_time, blueVal_time);
  int TextWait = 500;
  showText("WordClock", TextWait, c);
}


// ###########################################################################################################################################
// # Text output function:
// ###########################################################################################################################################
void showText(String text, int wait, uint32_t c) {
  for (const char ch : text)
    showChar(ch, wait, c);
}


// ###########################################################################################################################################
// # Character output function:
// ###########################################################################################################################################
void showChar(char letter, int wait, uint32_t c) {
  ClearDisplay();

  static std::vector<std::vector<std::string>> numberVector = {
    { "XXXX",
      "X  X",
      "X  X",
      "X  X",
      "XXXX" }
  };

  int myArray[50];
  memset(myArray, 0, sizeof(myArray));

  if (letter == 'W') {
    int myArray2[] = { 42, 53, 74, 85, 106, 117, 138, 149, 170, 181, 169, 182, 183, 168, 151, 136, 135, 152, 167, 184, 166, 185, 186, 165, 154, 133, 122, 101, 90, 69, 58, 37 };
    memcpy(myArray, myArray2, sizeof(myArray2));
  }

  if (letter == 'o') {
    int myArray2[] = { 106, 117, 138, 149, 170, 181, 169, 182, 183, 168, 167, 184, 166, 185, 186, 165, 154, 133, 122, 101, 102, 121, 120, 103, 104, 119, 118, 105 };
    memcpy(myArray, myArray2, sizeof(myArray2));
  }

  if (letter == 'r') {
    int myArray2[] = { 154, 133, 122, 101, 102, 121, 120, 103, 104, 119, 118, 105, 106, 117, 138, 149, 170, 181 };
    memcpy(myArray, myArray2, sizeof(myArray2));
  }

  if (letter == 'd') {
    int myArray2[] = { 102, 121, 120, 103, 104, 119, 118, 105, 106, 117, 138, 149, 170, 181, 169, 182, 183, 168, 167, 184, 166, 185, 186, 165, 154, 133, 122, 101, 90, 69, 58, 37 };
    memcpy(myArray, myArray2, sizeof(myArray2));
  }

  if (letter == 'C') {
    int myArray2[] = { 58, 37, 57, 38, 39, 56, 40, 55, 41, 54, 42, 53, 74, 85, 106, 117, 138, 149, 170, 181, 169, 182, 183, 168, 167, 184, 166, 185, 186, 165 };
    memcpy(myArray, myArray2, sizeof(myArray2));
  }

  if (letter == 'l') {
    int myArray2[] = { 42, 53, 74, 85, 106, 117, 138, 149, 170, 181 };  // , 169, 182, 183, 168
    memcpy(myArray, myArray2, sizeof(myArray2));
  }

  if (letter == 'c') {
    int myArray2[] = { 122, 101, 102, 121, 120, 103, 104, 119, 118, 105, 106, 117, 138, 149, 170, 181, 169, 182, 183, 168, 167, 184, 166, 185, 186, 165 };
    memcpy(myArray, myArray2, sizeof(myArray2));
  }

  if (letter == 'k') {
    int myArray2[] = { 42, 53, 74, 85, 106, 117, 138, 149, 170, 181, 105, 118, 104, 119, 71, 88, 37, 58, 134, 153, 165, 186 };
    memcpy(myArray, myArray2, sizeof(myArray2));
  }

  if (letter == 'S') {
    int myArray2[] = { 37, 38, 39, 40, 41, 42, 53, 54, 55, 56, 57, 58, 74, 85, 101, 102, 103, 104, 105, 106, 117, 118, 119, 120, 121, 122, 133, 154, 165, 166, 167, 168, 169, 170, 181, 182, 183, 184, 185, 186 };
    memcpy(myArray, myArray2, sizeof(myArray2));
  }

  if (letter == 'E') {
    int myArray2[] = { 37, 38, 39, 40, 41, 42, 53, 54, 55, 56, 57, 58, 74, 85, 101, 102, 103, 104, 105, 106, 117, 118, 119, 120, 121, 122, 138, 149, 165, 166, 167, 168, 169, 170, 181, 182, 183, 184, 185, 186 };
    memcpy(myArray, myArray2, sizeof(myArray2));
  }

  if (letter == 'T') {
    int myArray2[] = { 37, 38, 39, 40, 41, 42, 43, 52, 53, 54, 55, 56, 57, 58, 72, 87, 104, 119, 136, 151, 168, 183 };
    memcpy(myArray, myArray2, sizeof(myArray2));
  }

  if (letter == 'I') {
    int myArray2[] = { 40, 55, 72, 87, 104, 119, 136, 151, 168, 183 };
    memcpy(myArray, myArray2, sizeof(myArray2));
  }

  if (letter == 'F') {
    int myArray2[] = { 37, 38, 39, 40, 41, 42, 53, 54, 55, 56, 57, 58, 74, 85, 101, 102, 103, 104, 105, 106, 117, 118, 119, 120, 121, 122, 138, 149, 170, 181 };
    memcpy(myArray, myArray2, sizeof(myArray2));
  }

  if (letter == 'U') {
    int myArray2[] = { 42, 53, 74, 85, 106, 117, 138, 149, 170, 181, 169, 182, 183, 168, 167, 184, 166, 185, 186, 165, 154, 133, 122, 101, 90, 69, 58, 37 };
    memcpy(myArray, myArray2, sizeof(myArray2));
  }

  if (letter == 'P') {
    int myArray2[] = { 37, 38, 39, 40, 41, 42, 53, 54, 55, 56, 57, 58, 74, 85, 69, 90, 101, 102, 103, 104, 105, 106, 117, 118, 119, 120, 121, 122, 138, 149, 170, 181 };
    memcpy(myArray, myArray2, sizeof(myArray2));
  }

  if (letter == 'D') {
    int myArray2[] = { 38, 39, 40, 41, 42, 53, 54, 55, 56, 57, 74, 85, 69, 90, 101, 106, 117, 122, 138, 149, 170, 181, 133, 154, 169, 182, 183, 168, 167, 184, 166, 185 };
    memcpy(myArray, myArray2, sizeof(myArray2));
  }

  if (letter == 'A') {
    int myArray2[] = { 37, 38, 39, 40, 41, 42, 53, 54, 55, 56, 57, 58, 74, 85, 69, 90, 101, 102, 103, 104, 105, 106, 117, 118, 119, 120, 121, 122, 138, 149, 170, 181, 133, 154, 165, 186 };
    memcpy(myArray, myArray2, sizeof(myArray2));
  }

  if (letter == ' ') {
    int myArray2[] = { 255 };
    memcpy(myArray, myArray2, sizeof(myArray2));
    c = strip.Color(0, 0, 0);
  }

  for (int element : myArray) {
    if (element != 0) {
      strip.setPixelColor(element, c);
    }
  }

  strip.show();
  delay(wait);
  ClearDisplay();
}


// ###########################################################################################################################################
// # Startup WiFi text function:
// ###########################################################################################################################################
void ShowWLAN(uint32_t color) {
  if (debugtexts == 1) Serial.println("Show text WLAN/WIFI...");
  ClearDisplay();

  int coordX = -1, coordY = -1;

  switch (langLEDlayout) {
    case 0:  // DE
      coordX = 7;
      coordY = 0;
      break;

    case 1:  // EN:
      coordX = 5;
      coordY = 0;
      break;

    case 2:  // NL:
      coordX = 1;
      coordY = 2;
      break;

    case 3:  // SWE:
      coordX = 12;
      coordY = 0;
      break;

    case 4:                         // IT:
      setLEDcolXY(6, 7, 1, color);  // W
      setLEDcolXY(8, 7, 1, color);  // I
      setLEDcolXY(13, 7, 1, color); // F
      setLEDcolXY(15, 7, 1, color); // I
      break;

    case 5:                         // FR:
      setLEDcolXY(0, 7, 1, color);  // W
      setLEDcolXY(2, 7, 1, color);  // I
      setLEDcolXY(7, 7, 1, color);  // F
      setLEDcolXY(15, 7, 1, color);  // I
      break;

    case 6:  // GSW:
      coordX = 5;
      coordY = 0;
      break;

    case 7:                        // CN:
      setLEDcolXY(4, 1, 2, color); // WIFI
      break;

    case 8:                        // SWABIAN:
      setLEDcolXY(2, 0, 2, color); // WI
      setLEDcolXY(7, 0, 2, color); // FI
      break;

    case 9:  // BAVARIAN:
      coordX = 2;
      coordY = 0;
      break;

    default:
      break;
  }

  if ((coordX >= 0) && (coordY >= 0))
    setLEDcolXY(coordX, coordY, 4, color);

  strip.show();
}
