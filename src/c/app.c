#include <pebble.h>

static Window *s_main_window;

static TextLayer *s_time_layer;
static GFont s_time_font;

static TextLayer *s_date_layer;
static GFont s_date_font;

tm *getLocalDateTimeInfo() {
  time_t temp = time(NULL);
  return localtime(&temp);
}

void update_time() {
  struct tm *tick_time = getLocalDateTimeInfo();
  static char s_time_buffer[8];
  strftime(s_time_buffer, sizeof(s_time_buffer), clock_is_24h_style() ?"%H:%M" : "%I:%M", tick_time);
  text_layer_set_text(s_time_layer, s_time_buffer);
}

TextLayer *getTimeLayer(GRect *bounds) {
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CAPTURE_40));
  s_time_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(20,20), bounds->size.w, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  return s_time_layer;
}

void update_date() {
  struct tm *tick_time = getLocalDateTimeInfo();
  static char s_date_buffer[30];
  strftime(s_date_buffer, sizeof(s_date_buffer), "%A %D", tick_time);
  text_layer_set_text(s_date_layer, s_date_buffer);
}

TextLayer *getDateLayer(GRect *bounds) {
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CAPTURE_15));
  s_date_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(150,150), bounds->size.w, 50));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorBlack);
  text_layer_set_font(s_date_layer, s_date_font);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  return s_date_layer;
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    if( (units_changed & MINUTE_UNIT) != 0 ) {
        update_time();
    }

    if( (units_changed & DAY_UNIT) != 0 ) {
        update_date();
    }
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window); //Main Layer
  GRect bounds = layer_get_bounds(window_layer);
  GRect *const ptrBounds = &bounds; 
  layer_add_child(window_layer, text_layer_get_layer(getTimeLayer(ptrBounds)));
  layer_add_child(window_layer, text_layer_get_layer(getDateLayer(ptrBounds)));
}

static void main_window_unload(Window *window) {
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_date_font);
}

static void init(void) {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_stack_push(s_main_window, true);
  update_time(); //update time init
  update_date(); //update date init
  tick_timer_service_subscribe(MINUTE_UNIT | DAY_UNIT, tick_handler);
}

static void deinit(void) {
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
	window_destroy(s_main_window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}
