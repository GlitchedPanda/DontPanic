#include <SPI.h>
#include <XPT2046_Bitbang.h> // Required as we have 3 spi devices and only 3 spi busses
#include <TFT_eSPI.h>
#include <lvgl.h>

#include <sstream>
#include <vector>
#include <string.h>

#include "sdcard.h"

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

extern XPT2046_Bitbang ts;
extern TFT_eSPI tft;

#define DRAW_BUF_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT / 10 * (LV_COLOR_DEPTH / 8))
extern uint32_t draw_buf[DRAW_BUF_SIZE / 4];

extern bool startMenuClicked;
extern int x, y, z;

extern lv_obj_t *keyboard, *searchBox;

struct CallbackData;

extern std::vector<std::string> titles;
extern std::vector<std::string> content;

extern void text_dontpanic_background();

extern lv_obj_t* menu_create_page(char* title, lv_obj_t*& menu, lv_obj_t*& content, lv_obj_t*& label);
extern void menu_create_button(char* title, lv_obj_t*& menu, lv_obj_t*& mainPage, lv_obj_t*& page);
extern void menu_callback(lv_event_t* event);

extern void gui_startupscreen();
extern void gui_main_menu();
extern void gui_search_results(const char* query);

static int sqliteCallback(void *unused, int count, char** data, char** columns);

extern void backButtonCallback(lv_event_t* event);
extern void searchBoxCallback(lv_event_t* event);

extern void touchscreen_read(lv_indev_t* indev, lv_indev_data_t* data);
