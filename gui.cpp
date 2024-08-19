#include "gui.h"

SPIClass tsSPIClass(VSPI);
XPT2046_Touchscreen ts(XPT2046_CS, XPT2046_IRQ);

TFT_eSPI tft = TFT_eSPI();

uint32_t draw_buf[DRAW_BUF_SIZE / 4];

bool startMenuClicked = 0;
int x, y, z;

lv_obj_t *keyboard, *searchBox;

void text_dontpanic_background()
{
  static lv_style_t backgroundText;
  lv_style_init(&backgroundText);
  lv_style_set_text_color(&backgroundText, lv_color_make(85, 85, 85));
  lv_style_set_text_font(&backgroundText, &DontPanicFont);


  lv_obj_t* label = lv_label_create( lv_screen_active() );
  lv_obj_add_style(label, &backgroundText, 0);
  lv_label_set_text(label, "DON'T");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, -16);

  lv_obj_t* label2 = lv_label_create( lv_screen_active() );
  lv_obj_add_style(label2, &backgroundText, 0);
  lv_label_set_text(label2, "PANIC");
  lv_obj_align(label2, LV_ALIGN_CENTER, 0, 16);
}

void gui_startupscreen()
{
  lv_obj_t* label = lv_label_create( lv_screen_active() );
  lv_label_set_text(label, "DON'T");
  lv_obj_set_style_text_font(label, &DontPanicFont, 0);
  lv_obj_align(label, LV_ALIGN_CENTER, 0, -16);

  lv_obj_t* label2 = lv_label_create( lv_screen_active() );
  lv_label_set_text(label2, "PANIC");
  lv_obj_set_style_text_font(label2, &DontPanicFont, 0);
  lv_obj_align(label2, LV_ALIGN_CENTER, 0, 16);
}

void gui_main_menu()
{
  text_dontpanic_background();

  searchBox = lv_textarea_create( lv_screen_active() );
  lv_textarea_set_one_line(searchBox, true);
  lv_textarea_set_text(searchBox, "");
  lv_textarea_set_placeholder_text(searchBox, "The Galaxy");
  lv_obj_set_width(searchBox, lv_pct(100));
  lv_obj_set_pos(searchBox, 0, 0);
  lv_obj_add_event_cb(searchBox, searchBoxCallback, LV_EVENT_ALL, NULL);

  keyboard = lv_keyboard_create( lv_screen_active() );
  lv_obj_set_size(keyboard,  lv_pct(100), lv_pct(35));
  lv_keyboard_set_textarea(keyboard, searchBox);
}

void gui_search_results(const char* query)
{
  // Make a menu to house the results
  lv_obj_t* results = lv_menu_create( lv_screen_active() );
  lv_obj_set_style_radius(results, 7.5, LV_PART_MAIN); // Round the edges of the menu for looks
  lv_obj_set_size(results, lv_pct(100), lv_pct(88));
  lv_obj_align(results, LV_ALIGN_BOTTOM_MID, 0, 0);

  // Add the text Back to the back button so its easier to see it's purpose
  lv_obj_t * backButton = lv_menu_get_main_header_back_button(results);
  lv_obj_t * backButtonLabel = lv_label_create(backButton);
  lv_label_set_text(backButtonLabel, "Back");

  lv_obj_t* content;
  lv_obj_t* label;

  // Create test page
  lv_obj_t* clickmePage = lv_menu_page_create(results, "Click Me!");
  content = lv_menu_cont_create(clickmePage);
  label = lv_label_create(content);
  lv_label_set_text(label, "Placeholder. To be replaced with actual content when the page is clicked.");
  lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP );
  lv_obj_set_width(label, 220);
  lv_obj_align(label, LV_ALIGN_LEFT_MID, 0, 0);

  lv_obj_t* clickmePage2 = lv_menu_page_create(results, "Click Me!");
  content = lv_menu_cont_create(clickmePage2);
  label = lv_label_create(content);
  lv_label_set_text(label, "Placeholder. To be replaced with actual content when the page is clicked.");
  lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP );
  lv_obj_set_width(label, 230);
  lv_obj_align(label, LV_ALIGN_LEFT_MID, 0, 0);

  lv_obj_t* clickmePage3 = lv_menu_page_create(results, "Click Me!");
  content = lv_menu_cont_create(clickmePage3);
  label = lv_label_create(content);
  lv_label_set_text(label, "Placeholder. To be replaced with actual content when the page is clicked.");
  lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP );
  lv_obj_set_width(label, 220);
  lv_obj_align(label, LV_ALIGN_LEFT_MID, 0, 0);

  lv_obj_t* mainPage = lv_menu_page_create(results, nullptr);

  // Add button to go to the clickme page
  content = lv_menu_cont_create(mainPage);
  label = lv_label_create(content);
  lv_label_set_text(label, "Click Me!");
  lv_menu_set_load_page_event(results, content, clickmePage);

  content = lv_menu_cont_create(mainPage);
  label = lv_label_create(content);
  lv_label_set_text(label, "Click Me 2!");
  lv_menu_set_load_page_event(results, content, clickmePage2);

  content = lv_menu_cont_create(mainPage);
  label = lv_label_create(content);
  lv_label_set_text(label, "Click Me 3!");
  lv_menu_set_load_page_event(results, content, clickmePage3);

  // Set the starting page to mainPage
  lv_menu_set_page(results, mainPage);
}

void searchBoxCallback(lv_event_t* event)
{
  lv_event_code_t eventCode = lv_event_get_code(event);
  lv_obj_t* target = lv_event_get_target_obj(event);

  if (eventCode == LV_EVENT_CLICKED || eventCode == LV_EVENT_FOCUSED)
  {
    // Ignore
    //lv_keyboard_set_textarea(keyboard, searchBox);
  }
  else if (eventCode == LV_EVENT_READY)
  {
    // Make sure search query is 2 or more characters
    const char* searchQuery = lv_textarea_get_text(target);
    if (strlen(searchQuery) < 2)
    {
      lv_textarea_set_text(searchBox, "");
      lv_textarea_set_placeholder_text(searchBox, "Query is too short");
      return;
    }

    // Delete old content
    lv_obj_delete( keyboard );

    // Load search page
    gui_search_results(searchQuery);
  }
}

void touchscreen_read(lv_indev_t* indev, lv_indev_data_t* data)
{
  if(ts.touched() && ts.tirqTouched())
  {
    if(!startMenuClicked)
    {
      lv_obj_clean(lv_scr_act());
      gui_main_menu();
      startMenuClicked = 1;
    }

    TS_Point point = ts.getPoint();
    x = map(point.x, 200, 3700, 1, SCREEN_WIDTH);
    y = map(point.y, 240, 3800, 1, SCREEN_HEIGHT);
    z = point.z;

    data->state = LV_INDEV_STATE_PRESSED;

    data->point.x = x;
    data->point.y = y;
  }
  else
  {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}
