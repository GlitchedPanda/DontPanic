#include "gui.h"

SPIClass tsSPIClass(VSPI);
XPT2046_Touchscreen ts(XPT2046_CS, XPT2046_IRQ);

TFT_eSPI tft = TFT_eSPI();

uint32_t draw_buf[DRAW_BUF_SIZE / 4];

bool startMenuClicked = 0;
int x, y, z;

lv_obj_t *keyboard, *searchBox;

struct CallbackData
{
  char* title;
  lv_obj_t* page;
};

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

lv_obj_t* menu_create_page(char* title, lv_obj_t*& menu, lv_obj_t*& content, lv_obj_t*& label)
{
  lv_obj_t* page = lv_menu_page_create(menu, title);
  content = lv_menu_cont_create(page);
  label = lv_label_create(content);
  lv_label_set_text(label, ":D"); // Placeholder. Will be replaced by actual content when clicked.
  lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP );
  lv_obj_set_width(label, 220);
  lv_obj_align(label, LV_ALIGN_LEFT_MID, 0, 0);

  return page;
}

void menu_create_button(char* title, lv_obj_t*& menu, lv_obj_t*& mainPage, lv_obj_t*& page)
{
  lv_obj_t* label;
  lv_obj_t* content;

  content = lv_menu_cont_create(mainPage);
  label = lv_label_create(content);
  lv_label_set_text(label, title);
  lv_menu_set_load_page_event(menu, content, page);

  CallbackData* data = new CallbackData;
  data->title = title;
  data->page = page;

  lv_obj_add_event_cb(content, menu_callback, LV_EVENT_CLICKED, data);
}

void menu_callback(lv_event_t* event)
{
  lv_obj_t* page = lv_event_get_target_obj(event);
  CallbackData* data = static_cast<CallbackData*>( lv_event_get_user_data(event) );

  // Get content object as it is the parent of the label object
  lv_obj_t* content = lv_obj_get_child(data->page, 0);
  if (!content)
  {
    Serial.println("Could not find content");
    return;
  }

  // Get label object so we can chnage it's text
  lv_obj_t* label = lv_obj_get_child_by_type(content, 0, &lv_label_class);
  if (!label)
  {
    Serial.println("Could not find label");
    return;
  }

  std::stringstream ss;
  ss << "Actual Content for ";
  ss << (char*)data->title;

  lv_label_set_text(label, ss.str().c_str());
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
  lv_obj_set_size(results, lv_pct(100), lv_pct(75));
  lv_obj_align(results, LV_ALIGN_CENTER, 0, 0);

  // Add back button to main page
  lv_obj_t* mainBackButton = lv_button_create( lv_screen_active() );
  lv_obj_set_size(mainBackButton, lv_pct(100), 36);
  lv_obj_align(mainBackButton, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_obj_t* mainBackButtonLabel = lv_label_create( mainBackButton );
  lv_label_set_text(mainBackButtonLabel, "Back to Main Menu");
  lv_obj_set_style_bg_color(mainBackButton, lv_obj_get_style_bg_color(searchBox, LV_PART_MAIN), LV_PART_MAIN);
  lv_obj_center(mainBackButtonLabel);
  // Add a callback for when the button is pressed
  lv_obj_add_event_cb(mainBackButton, backButtonCallback, LV_EVENT_PRESSED, nullptr);

  // Add the text Back to the back button on child pages so its easier to see it's purpose
  lv_obj_t * backButton = lv_menu_get_main_header_back_button(results);
  lv_obj_t * backButtonLabel = lv_label_create(backButton);
  lv_label_set_text(backButtonLabel, "Back");

  lv_obj_t* content;
  lv_obj_t* label;

  // Create test pages
  lv_obj_t* clickmePage = menu_create_page("Click me!", results, content, label);
  lv_obj_t* clickmePage2 = menu_create_page("Click me!", results, content, label);
  lv_obj_t* clickmePage3 = menu_create_page("Click me!", results, content, label);

  lv_obj_t* mainPage = lv_menu_page_create(results, nullptr);

  // Add buttons to go to the clickme pages
  menu_create_button("Click me!", results, mainPage, clickmePage);
  menu_create_button("Click me 2!", results, mainPage, clickmePage2);
  menu_create_button("Click me 3!", results, mainPage, clickmePage3);

  // Set the starting page to mainPage
  lv_menu_set_page(results, mainPage);
}

void backButtonCallback(lv_event_t* event)
{
  // Clean screen and load main menu
  lv_obj_clean( lv_screen_active() );
  gui_main_menu();
}

void searchBoxCallback(lv_event_t* event)
{
  lv_event_code_t eventCode = lv_event_get_code(event);
  lv_obj_t* target = lv_event_get_target_obj(event);

  if (eventCode == LV_EVENT_CLICKED || eventCode == LV_EVENT_FOCUSED)
  {
    // Ignore
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
      startMenuClicked = 1;
      lv_obj_clean( lv_screen_active() );
      gui_main_menu();
    }

    TS_Point point = ts.getPoint();
    x = map(point.x, 200, 3700, 1, SCREEN_WIDTH);
    y = map(point.y, 240, 3800, 1, SCREEN_HEIGHT);
    z = point.z;

    data->point.x = x;
    data->point.y = y;

    data->state = LV_INDEV_STATE_PRESSED;
  }
  else
  {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}
