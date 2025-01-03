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
    "X",
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
    "X",
    "XXXX",
    "   X",
    "XXXX" },

  { "XXXX",
    "X",
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

static std::map<char, std::vector<std::string>> charMap = 
  { { 'W', { "X    X",
             "X    X",
             "X XX X",
             "X XX X",
             "XXXXXX" } },

    { 'o', { "",
             "XXXXXX",
             "X    X",
             "X    X",
             "XXXXXX" } },

    { 'r', { "",
             "XXXXXX",
             "X",
             "X",
             "X" } },

    { 'd', { "     X",
             "     X",
             "XXXXXX",
             "X    X",
             "XXXXXX" } },

    { 'C', { "XXXXXX",
             "X",
             "X",
             "X",
             "XXXXXX" } },

    { 'l', { "X",
             "X",
             "X",
             "X",
             "X" } },

    { 'c', { "",
             "XXXXX",
             "X",
             "X",
             "XXXXX" } },

    { 'k', { "X",
             "X  X",
             "XXX",
             "X  X",
             "X   X" } },

    { 'S', { " XXXX",
             "X",
             " XXXX",
             "     X",
             " XXXX" } },

    { 'E', { "XXXXXX",
             "X",
             "XXXX",
             "X",
             "XXXXXX" } },

    { 'T', { "XXXXXX",
             "  XX",
             "  XX",
             "  XX",
             "  XX" } },

    { 'I', { " XXXX",
             "  XX",
             "  XX",
             "  XX",
             " XXXX" } },

    { 'F', { "XXXXXX",
             "X",
             "XXXX",
             "X",
             "X" } },

    { 'U', { "X    X",
             "X    X",
             "X    X",
             "X    X",
             " XXXX" } },

    { 'P', { "XXXXX",
             "X    X",
             "XXXXX",
             "X",
             "X" } },

    { 'D', { "XXXX",
             "X   X",
             "X    X",
             "X   X",
             "XXXX" } },

    { 'A', { "  XX",
             " X  X",
             "XXXXXX",
             "X    X",
             "X    X" } },

    { ':', { "",
             "X",
             "",
             "X",
             "" } },

    { '?', { " XXXX",
             "     X",
             "   X",
             "",
             "   X" } }
  };

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
// # Actual function, which controls 1/0 of the LED with color value:
// ###########################################################################################################################################
void setLEDcol(int ledNrFrom, int ledNrTo, uint32_t color, const int CharDelay = 0) {
  if (ledNrFrom > ledNrTo) {
    setLEDcol(ledNrTo, ledNrFrom, color, CharDelay);  // Sets LED numbers in correct order
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
      {
        strip.setPixelColor(i, color);

        if (CharDelay > 0)
        {
          strip.show();
          delay(CharDelay);
        }
      }
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
// # Get color at coordinate:
// ###########################################################################################################################################
uint32_t getLEDcolXY(int coordX, int coordY) {
  int firstRow, secondRow;
  coords2Pixel(coordX, coordY, firstRow, secondRow);
  return strip.getPixelColor(firstRow);
}


// ###########################################################################################################################################
// # Actual function, which controls 1/0 of the LED with X/Y position and color value:
// ###########################################################################################################################################
void setLEDcolXY(int coordX, int coordY, int numCols, uint32_t color, const int CharDelay = 0) {
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

  setLEDcol(firstRow, firstRow - numCols + 1, color, CharDelay);  // firstRow ist von rechts nach links numeriert
  setLEDcol(secondRow, secondRow + numCols - 1, color, CharDelay);
}


// ###########################################################################################################################################
// # Update the display / time on it:
// ###########################################################################################################################################
void setLEDcolXY(const position_t& p, uint32_t color, const int CharDelay = 0)
{
  setLEDcolXY(p.x, p.y, p.len, color, CharDelay);
}


// ###########################################################################################################################################
// # Show the IP-address on the display:
// ###########################################################################################################################################
void ShowIPaddress() {
  Serial.println("Show current IP-address on the display: " + IpAddress2String(WiFi.localIP()));
  int ipdelay = 2000;

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

      showNumber(DigitValue, digit, ipColor);
    }

    if (octet < 3)                     // nicht beim letzten Wert
      setLEDcolXY(15, 5, 1, ipColor);  // Punkt hinter dem Wert

    setLEDcolXY(0, 7, (octet + 1) * 4, ipColor);  // Fortschrittsbalken unten

    strip.show();
    delay(ipdelay);
  }
}


// ###########################################################################################################################################
// # Set one char as a matrix on the display at a specific coordinate:
// ###########################################################################################################################################
void showCharMatrix(const std::vector<std::string>& number, int coordX, int coordY, uint32_t color) {
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
void showNumber(int wert, int segment, uint32_t color) {
  showCharMatrix(numberVector[wert], (2 - segment) * 5, 1, color);
}


// ###########################################################################################################################################
// # Character output function:
// ###########################################################################################################################################
void showChar(char letter, int wait, uint32_t c) {
  ClearDisplay();

  const auto Index = charMap.find(letter);
  if (Index != charMap.end())
    showCharMatrix(Index->second, 5, 2, c);

  strip.show();
  delay(wait);
  ClearDisplay();
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

  switch (getSetting(setting_type::langLEDlayout)) {
    case 0:                          // DE:
      setLEDcolXY(5, 4, 2, color);   // RE
      setLEDcolXY(7, 5, 2, color);   // SE
      setLEDcolXY(11, 6, 1, color);  // T
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
      setLEDcolXY(10, 4, 1, color); // E
      setLEDcolXY(0, 5, 1, color);  // S
      setLEDcolXY(5, 5, 1, color);  // E
      setLEDcolXY(15, 5, 1, color); // T
      break;

    default:
      break;
  }

  strip.show();
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
//    usenightmode = 0;
    uint32_t c = 0x000000;
    int TextWait = 500;
    showTextCharByChar("TEST", TextWait, c);
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
  uint32_t c0 = getSetting(setting_type::colorBack);  // Background color
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, c0);
  }
}


// ###########################################################################################################################################
// # Actual function, which controls 1/0 of the LED:
// ###########################################################################################################################################
void switchLED(int ledNrFrom, int ledNrTo, int switchOn) {
  if (ledNrFrom > ledNrTo) {
    switchLED(ledNrTo, ledNrFrom, switchOn);  // Sets LED numbers in correct order
  } else {
    for (int i = ledNrFrom; i <= ledNrTo; i++) {
      if ((i >= 0) && (i < NUMPIXELS))
        strip.setPixelColor(i, getSetting(setting_type::colorTime));
    }
  }
  if (switchOn == 0) {
    for (int i = ledNrFrom; i <= ledNrTo; i++) {
      if ((i >= 0) && (i < NUMPIXELS))
        strip.setPixelColor(i, 0x000000);  // Switch LEDs off
    }
  }
}


// ###########################################################################################################################################
// # Actual function, which controls 1/0 of the LED with x/y coords:
// ###########################################################################################################################################
void switchLEDXY(int coordX, int coordY, int numCols, int switchOn) {
  uint32_t color = getSetting(setting_type::colorTime);
  if (switchOn == 0)
    color = strip.Color(0, 0, 0);

  setLEDcolXY(coordX, coordY, numCols, color);
}


// ###########################################################################################################################################
// # Startup LED test function
// ###########################################################################################################################################
void callStartText() {
  Serial.println("Show 'WordClock' startup text...");
  uint32_t c = getSetting(setting_type::colorTime);
  showScrollingText("WordClock", c);
}


// ###########################################################################################################################################
// # Text output function:
// ###########################################################################################################################################
void showTextCharByChar(String text, int wait, uint32_t c) {
  for (const char ch : text)
    showChar(ch, wait, c);
}


// ###########################################################################################################################################
// # Startup WiFi text function:
// ###########################################################################################################################################
void ShowWLAN(uint32_t color) {
  if (debugtexts == 1) Serial.println("Show text WLAN/WIFI...");
  ClearDisplay();

  int coordX = -1, coordY = -1;

  switch (getSetting(setting_type::langLEDlayout)) {
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


static std::vector<uint32_t> PixelValues;

// ###########################################################################################################################################
// # Save the display internally.
// ###########################################################################################################################################
void SaveDisplay() {
  PixelValues.clear();
  PixelValues.reserve(256);

  for (auto i = 0; i < 256; ++i)
    PixelValues.push_back(strip.getPixelColor(i));
}


// ###########################################################################################################################################
// # Restore the display.
// ###########################################################################################################################################
void RestoreDisplay() {
  if (PixelValues.empty())
    return;

  for (auto i = 0; i < 256; ++i)
    strip.setPixelColor(i, PixelValues[i]);

  strip.show();
}


// ###########################################################################################################################################
// # Get the longest string len of vector:
// ###########################################################################################################################################
int getLongestStringLen(const std::vector<std::string>& stringVec) {
  int result = 0;

  for (int i = 0; i < stringVec.size(); ++i)
    if (stringVec[i].size() > result)
      result = stringVec[i].size();

  return result;
}


// ###########################################################################################################################################
// # Append string with blanks:
// ###########################################################################################################################################
std::string padTo(const std::string &str, const size_t num)
{
  std::string result = str;

  if (num > result.size())
    result += std::string(num - str.size(), ' ');

  return result;
}


// ###########################################################################################################################################
// # Display scrolling text:
// ###########################################################################################################################################
void showScrollingText(const std::string& Text, uint32_t color) {
  int maxHeight = 5;

  std::vector<std::string> fullMatrix;
  for (auto row = 0; row < maxHeight; ++row)
  {
    std::string singleLine;

    for (const auto singleChar : Text)
    {
      std::vector<std::string> charMatrix;

      if (isdigit(singleChar))
        charMatrix = numberVector[singleChar - '0'];
      else
      {
        auto mapIndex = charMap.find(singleChar);
        if (mapIndex == charMap.end()) // char not found?
          mapIndex = charMap.find('?');

        charMatrix = mapIndex->second;
      }

      const auto longestStringLen = getLongestStringLen(charMatrix);
      singleLine += padTo(charMatrix[row], longestStringLen) + " ";
    }

    fullMatrix.push_back(singleLine.substr(0, singleLine.size() - 1)); // strip last blank
  }

  ClearDisplay();

  const auto lineLen = fullMatrix[0].size();
  const int startY = 1;

  for (auto col = 0; col < lineLen + 16; ++col) // +16 to scroll completely out of view
  {
    // copy columns to the left
    for (auto row = 0; row < maxHeight; ++row)
      for (auto X = 1; X < 16; ++X)
        setLEDcolXY(X - 1, startY + row, 1, getLEDcolXY(X, startY + row));

    // show new column
    for (auto row = 0; row < maxHeight; ++row)
    {
      uint32_t c = color_black;

      if ((col < lineLen) && (fullMatrix[row][col] != ' '))
        c = color;

      setLEDcolXY(15, startY + row, 1, c);
    }

    strip.show(); 
    delay(getSetting(setting_type::textScrollDelay));  
  }
}
