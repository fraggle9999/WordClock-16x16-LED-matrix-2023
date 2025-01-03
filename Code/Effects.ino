std::vector<std::function<void()>> effectList;

// ###########################################################################################################################################
// # Show all effects
// ###########################################################################################################################################
void allEffects() {
  for (auto f: effectList)
    f();
}


// ###########################################################################################################################################
// # Show a random effect
// ###########################################################################################################################################
void randomEffect() {
  long randNumber = random(effectList.size());
  effectList[randNumber]();
}


// ###########################################################################################################################################
// # Draw a circle round the clock.
// ###########################################################################################################################################
void circleEffect() {
  // top from left to right
  for (int x = 0; x < max_cols; ++x) {
    setLEDcolXY(x, 0, 1, getRandomColor());
    strip.show();
    delay(effects_delay / 2);
    setLEDcolXY(x, 0, 1, color_black);
    //  strip.show();
  }

  // right from top to bottom (starting in second row, ending in second but last row)
  for (int y = 1; y < max_rows - 1; ++y) {
    setLEDcolXY(max_cols - 1, y, 1, getRandomColor());
    strip.show();
    delay(effects_delay / 2);
    setLEDcolXY(max_cols - 1, y, 1, color_black);
    //  strip.show();
  }

  // bottom from right to left
  for (int x = max_cols - 1; x >= 0; --x) {
    setLEDcolXY(x, max_rows - 1, 1, getRandomColor());
    strip.show();
    delay(effects_delay / 2);
    setLEDcolXY(x, max_rows - 1, 1, color_black);
    //  strip.show();
  }

  // left from bottom to top (starting in second but last row, ending in second row)
  for (int y = max_rows - 2; y > 0; --y) {
    setLEDcolXY(0, y, 1, getRandomColor());
    strip.show();
    delay(effects_delay / 2);
    setLEDcolXY(0, y, 1, color_black);
  }

  ClearDisplay();
  strip.show();
}


// ###########################################################################################################################################
// # Draw a line top to bottom.
// ###########################################################################################################################################
void horizontalLineEffect() {
  for (int y = 0; y < max_rows; ++y) {
    setLEDcolXY(0, y, max_cols, getRandomColor());
    strip.show();
    delay(effects_delay);
    setLEDcolXY(0, y, max_cols, color_black);
    strip.show();
    delay(effects_delay);
  }

  ClearDisplay();
  strip.show();
}


// ###########################################################################################################################################
// # Draw a rectangle
// ###########################################################################################################################################
void drawRectangle(int x1, int y1, int x2, int y2, uint32_t color) {
  setLEDcolXY(x1, y1, x2 - x1 + 1, color);
  for (int y = y1 + 1; y < y2; ++y)
  {
    setLEDcolXY(x1, y, 1, color);
    setLEDcolXY(x2, y, 1, color);
  }
  setLEDcolXY(x1, y2, x2 - x1 + 1, color);

  strip.show();
}


// ###########################################################################################################################################
// # Show an explosion from the middle to the outer rim
// ###########################################################################################################################################
void explosionEffect() {
  int start_x = 6;
  int end_x = 9;
  int start_y = 3;
  int end_y = 4;

  while ((start_x >= 0) && (end_x < max_cols) && (start_y >= 0) && (end_y < max_rows))
  {
    drawRectangle(start_x, start_y, end_x, end_y, getRandomColor());
    delay(effects_delay);
    drawRectangle(start_x, start_y, end_x, end_y, color_black);
    delay(effects_delay);

    start_x -= 2;
    ++end_x += 2;

    --start_y;
    ++end_y;
  }

  ClearDisplay();
  strip.show();
}


// ###########################################################################################################################################
// # Show the actual hour blinking on screen
// ###########################################################################################################################################
void blinkingHour() {
  for (int i = 0; i < 5; ++i)
  {
    int segment = 1;
    uint32_t color = getRandomColor();
    if (iHour > 9)
    {
      showNumber(iHour / 10, segment, color);
      --segment;
    }
    showNumber(iHour % 10, segment, color);
    strip.show();
    delay(500);
    ClearDisplay();
    strip.show();
    delay(500);
  }

  ClearDisplay();
  strip.show();
}


// ###########################################################################################################################################
// # Show a snake running from top right down to bottom right
// ###########################################################################################################################################
void snake() {
  for (int y = 0; y < 8; ++y)
  {
    int x_start = 0;
    int x_end = 16;
    int step = 1;

    if (y % 2 == 0)
    {
      x_start = 15;
      x_end = -1;
      step = -1;      
    }

    while (x_start != x_end)
    {
      setLEDcolXY(x_start, y, 1, getRandomColor());
      strip.show();
      delay(10);
      setLEDcolXY(x_start, y, 1, color_black);
      strip.show();
      delay(10);

      x_start += step;
    }
  }

  ClearDisplay();
  strip.show();
}


// ###########################################################################################################################################
// # Show lines from left to right and vice versa
// ###########################################################################################################################################
void linesleftright() {
  std::vector<uint32_t> c;
  for (int y = 0; y < 8; ++y)
    c.push_back(getRandomColor());

  for (int round = 0; round < 2; ++round)
  {
    for (int x = 0; x < 16; ++x)
    {
      for (int y = 0; y < 8; ++y)
      {
        auto x_pos = (y % 2 == 0) ? x : 15 - x;
        auto color = (round == 0) ? c[y] : 0;

        setLEDcolXY(x_pos, y, 1, color);
      }

      strip.show();
      delay(10);
    }
  }

  ClearDisplay();
  strip.show();
}


// ###########################################################################################################################################
// # Show lines from top to bottom and vice versa
// ###########################################################################################################################################
void linestopbottom() {
  std::vector<uint32_t> c;
  for (int x = 0; x < 16; ++x)
    c.push_back(getRandomColor());

  for (int round = 0; round < 2; ++round)
  {
    for (int y = 0; y < 8; ++y)
    {
      for (int x = 0; x < 16; ++x)
      {
        auto y_pos = (x % 2 == 0) ? y : 7 - y;
        auto color = (round == 0) ? c[x] : 0;

        setLEDcolXY(x, y_pos, 1, color);
      }

      strip.show();
      delay(10);
    }
  }

  ClearDisplay();
  strip.show();
}


// ###########################################################################################################################################
// # Create a list of all effects
// ###########################################################################################################################################
void initEffects() {
  effectList.push_back(circleEffect);
  effectList.push_back(horizontalLineEffect);
  effectList.push_back(explosionEffect);
  effectList.push_back(blinkingHour);
  effectList.push_back(snake);
  effectList.push_back(linesleftright);
  effectList.push_back(linestopbottom);

  randomSeed(analogRead(0));
}