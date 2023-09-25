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
  delay(200);
  Serial.println("Starting circle effect ...");

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

  Serial.println("Ending circle effect ...");
}


// ###########################################################################################################################################
// # Create a list of all effects
// ###########################################################################################################################################
void initEffects() {
  effectList.push_back(circleEffect);
  randomSeed(analogRead(0));
}