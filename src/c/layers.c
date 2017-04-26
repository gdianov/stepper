#include <pebble.h>
#include "layers.h"

Window *s_main_window;

TextLayer *s_time_layer;
GFont s_time_font;

TextLayer *s_date_layer;
GFont s_date_font;

TextLayer *getTimeLayer(const GRect *bounds) {
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CAPTURE_40));
  s_time_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(20,20), 
            bounds->size.w, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  return s_time_layer;
}

TextLayer *getDateLayer(const GRect *bounds) {
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CAPTURE_15));
  s_date_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(100,150), bounds->size.w, 50));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorBlack);
  text_layer_set_font(s_date_layer, s_date_font);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  return s_date_layer;
}


