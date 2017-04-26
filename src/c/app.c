#include <pebble.h>
#include "settings.h"
#include "layers.h"

extern Window *s_main_window;

extern TextLayer *s_time_layer;
extern GFont s_time_font;

extern TextLayer *s_date_layer;
extern GFont s_date_font;

tm* get_system_time(){
	static time_t now;
	now = time(NULL);
	return localtime(&now);
}

void update_time() {
  static char s_time_buffer[8];
  strftime(s_time_buffer, sizeof(s_time_buffer), WATCH_FORMAT_VIEW, get_system_time());
  text_layer_set_text(s_time_layer, s_time_buffer);
}

void update_date() {
  static char s_date_buffer[22];
  strftime(s_date_buffer, sizeof(s_date_buffer), DATE_FORMAT_VIEW, get_system_time());
  text_layer_set_text(s_date_layer, s_date_buffer);
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
  Layer *window_layer = window_get_root_layer(window);
  
  GRect bounds = layer_get_bounds(window_layer);
  
  layer_add_child(window_layer, text_layer_get_layer(getTimeLayer(&bounds)));//Init Time Layer
  layer_add_child(window_layer, text_layer_get_layer(getDateLayer(&bounds)));//Init Date Layer
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
  update_time();
  update_date();
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
