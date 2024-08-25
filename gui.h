#include <SPI.h>
#include <XPT2046_Touchscreen.h>
#include <TFT_eSPI.h>
#include <lvgl.h>

#include <sstream>

#include "libs/sqlite3.h"

#include "fonts/DontPanicFont.h"

// ----------------------------
// Touch Screen pins
// ----------------------------

#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33

// ----------------------------

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

extern SPIClass tsSPIClass;
extern XPT2046_Touchscreen ts;

extern TFT_eSPI tft;

#define DRAW_BUF_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT / 10 * (LV_COLOR_DEPTH / 8))
extern uint32_t draw_buf[DRAW_BUF_SIZE / 4];

extern bool startMenuClicked;
extern int x, y, z;

extern lv_obj_t *keyboard, *searchBox;

struct CallbackData;

extern void text_dontpanic_background();

extern lv_obj_t* menu_create_page(char* title, lv_obj_t*& menu, lv_obj_t*& content, lv_obj_t*& label);
extern void menu_create_button(char* title, lv_obj_t*& menu, lv_obj_t*& mainPage, lv_obj_t*& page);
extern void menu_callback(lv_event_t* event);

extern void gui_startupscreen();
extern void gui_main_menu();
extern void gui_search_results(const char* query);

extern void backButtonCallback(lv_event_t* event);
extern void searchBoxCallback(lv_event_t* event);

extern void touchscreen_read(lv_indev_t* indev, lv_indev_data_t* data);
