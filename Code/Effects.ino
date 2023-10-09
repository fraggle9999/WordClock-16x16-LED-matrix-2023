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

  int start_x = 7;
  int end_x = 8;
  int start_y = 3;
  int end_y = 4;

  // proportional zum Abstand bis zum Rand die Rechtecke vergrößern
  float factor = max_rows / max_cols; // < 1
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
// # Create a list of all effects
// ###########################################################################################################################################
void initEffects() {
  effectList.push_back(circleEffect);
  effectList.push_back(horizontalLineEffect);
  effectList.push_back(explosionEffect);

  randomSeed(analogRead(0));
}