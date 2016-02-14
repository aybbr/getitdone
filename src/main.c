#include <pebble.h>

Window *my_window;
TextLayer *text_get;
TextLayer *text_shit;
TextLayer *text_done;
TextLayer *time_layer;
TextLayer *text_emoji;
static GFont s_custom_font_24;
static GFont s_custom_font_bold;
static GPoint p0,p1;

static Layer *line_layer;

// Current minute & hour
int minute;
int hour;

// Called every minute
static void update_time() {
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  
  minute = tick_time->tm_min;
  hour = tick_time->tm_hour;

  // Create a long-lived buffer
  static char buffer[] = "00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }

  // Display this time on the TextLayer
  text_layer_set_text(time_layer, buffer);
 
}

static void draw_line(Layer *line_layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorWhite);
  #if defined(PBL_ROUND)
  graphics_fill_rect(ctx, GRect(29, 40, 3, 110), 0, GCornerNone);
  #elif defined(PBL_RECT)
   graphics_fill_rect(ctx, GRect(18, 40, 3, 110), 0, GCornerNone);
  #endif
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

void handle_init(void) {
  my_window = window_create();
  window_set_background_color(my_window, GColorBlack);
  line_layer = layer_create(GRect(0, 0, 144, 168));
  layer_set_update_proc(line_layer, draw_line);
  time_layer = text_layer_create(GRect(90, 20, 144, 32));
  text_layer_set_background_color(time_layer, GColorBlack);
  
   #if defined(PBL_ROUND)
   text_done = text_layer_create(GRect(33,99,144,48));
  text_get = text_layer_create(GRect(33, 44, 144, 32));
  text_shit = text_layer_create(GRect(33,74,144,32));

  text_emoji = text_layer_create(GRect(15,44,50,50));
   #elif defined(PBL_RECT)
  text_done = text_layer_create(GRect(22,95,144,48));
  text_get = text_layer_create(GRect(22, 40, 144, 32));
  text_shit = text_layer_create(GRect(22,70,144,32));
 
  text_emoji = text_layer_create(GRect(2,40,50,50));
   #endif
  text_layer_set_background_color(text_emoji, GColorBlack);
  text_layer_set_text_color(text_emoji, GColorWhite);
  text_layer_set_text(text_emoji, "\U0001F603");
  s_custom_font_bold = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ROBOTO_BOLD_48));
  text_layer_set_font(text_done, s_custom_font_bold);
  text_layer_set_background_color(text_done, GColorBlack);
  
  #if defined(PBL_COLOR)
  text_layer_set_text_color(text_done, GColorRed);
  text_layer_set_text(text_done, "DONE.");
  #elif defined(PBL_BW)
  text_layer_set_text_color(text_done, GColorWhite);
  text_layer_set_text(text_done, "DONE.");
  #endif
  
  s_custom_font_24 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ROBOTO_32));
  text_layer_set_font(text_get, s_custom_font_24);
  text_layer_set_background_color(text_get, GColorBlack);
  text_layer_set_text_color(text_get, GColorWhite);
  text_layer_set_text(text_get, "Get");
  text_layer_set_font(text_shit, s_custom_font_24);
  text_layer_set_background_color(text_shit, GColorBlack);
  text_layer_set_text_color(text_shit, GColorWhite);
  text_layer_set_text(text_shit, "Shit");
  
  
 
  s_custom_font_bold = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ROBOTO_18));
  text_layer_set_font(time_layer, s_custom_font_bold);
  text_layer_set_text_color(time_layer, GColorWhite);
  text_layer_set_text(time_layer, "00:00");
  
  //draw_line(ctx);
  
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(text_done));
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(text_emoji));
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(text_shit));
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(text_get));
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(time_layer));
  layer_add_child(window_get_root_layer(my_window), line_layer);
  window_stack_push(my_window, true);
  
   tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

void handle_deinit(void) {
  text_layer_destroy(text_get);
  window_destroy(my_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
