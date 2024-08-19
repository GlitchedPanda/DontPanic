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

  // Initialize XPT2046_Touchscreen to be used by LVGL
  tsSPIClass.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  ts.begin(tsSPIClass);

  // Rotate the touch screen
  ts.setRotation(0);

  // Initialize LVGL and set a logging callback funtion for debugging
  lv_init();
  lv_log_register_print_cb(lvglDebug);

  lv_display_t* disp;
  disp = lv_tft_espi_create(SCREEN_WIDTH, SCREEN_HEIGHT, draw_buf, sizeof(draw_buf));
  tft.setRotation(0);

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



