#include "gui.h"

void lvglDebug(lv_log_level_t level, const char* buffer)
{
  LV_UNUSED(level);
  Serial.println(buffer);
  Serial.flush();
}

void setup() 
{
  // Initialize serial for debugging
  Serial.begin(115200);
  Serial.println("Don't Panic");

  // Print info about LVGL for debugging
  String LVGL_Arduino = "Using LVGL ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
  Serial.println(LVGL_Arduino);

  // Initialize XPT2046_Bitbang to be used by LVGL
  ts.begin();

  // Initialize LVGL and set a logging callback funtion for debugging
  lv_init();
  lv_log_register_print_cb(lvglDebug);

  lv_display_t* disp;
  disp = lv_tft_espi_create(SCREEN_WIDTH, SCREEN_HEIGHT, draw_buf, sizeof(draw_buf));

  tft.setRotation(0); // Rotate screen so it will be vertical
  tft.fillScreen(TFT_BLACK); // Nicer to look at if nothing gets drawn to the screen

  if(!MicroSDCardReader::initializeMicroSDReader())
  {
    // Print error and return if the MicroSD Card Reader doesn't initalize correctly, so there wont by any further errors
    Serial.println( MicroSDCardReader::getLastError() ); // Debug
    tft.println( MicroSDCardReader::getLastError() ); // Let the user know the error without making them connect to serial monitor
    while (true); // Wait forever
  }

  sqlite3_initialize(); // Initialize SQLite3 so it can be used later
  sqlite3_soft_heap_limit(164000); // Make sure SQLite3 doesn't try to use more RAM that available 

  // Make device driver for touch
  lv_indev_t* indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, touchscreen_read);

  // Load the startup screen 
  gui_startupscreen();
}

// https://github.com/lvgl/lvgl/blob/master/examples/arduino/LVGL_Arduino/LVGL_Arduino.ino
void loop() 
{
  lv_timer_handler();
  lv_tick_inc(5);
  delay(5);
}



