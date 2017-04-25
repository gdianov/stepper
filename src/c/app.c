#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static GFont s_time_font;


static void update_time() {
  time_t temp = time(NULL); //Duplicate code
  struct tm *tick_time = localtime(&temp);
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?"%H:%M" : "%I:%M", tick_time);
  text_layer_set_text(s_time_layer, s_buffer);
}

TextLayer *getTimeLayer(GRect *bounds) {
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_48));
  s_time_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds->size.w, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  return s_time_layer;
}


static void tick_minute_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}


static void main_window_load(Window *window) {
  TextLayer *getTimeLayer(GRect *bounds);
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  GRect *const ptrBounds = &bounds; 
  
  layer_add_child(window_layer, text_layer_get_layer(getTimeLayer(ptrBounds)));
}

static void main_window_unload(Window *window) {
  fonts_unload_custom_font(s_time_font);
}

static void init(void) {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_stack_push(s_main_window, true);
  update_time(); //update time init
  tick_timer_service_subscribe(MINUTE_UNIT, tick_minute_handler);
}

static void deinit(void) {
  text_layer_destroy(s_time_layer);
	window_destroy(s_main_window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}
