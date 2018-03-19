#include <pebble.h>

static Window *s_main_window;
GBitmap *rogue;
static BitmapLayer *p_bg_layer;
static TextLayer *s_time_layer;

static void update_watch_bg() {
  gbitmap_destroy(rogue);
  switch (rand() % 6) {
    case 0:
      rogue = gbitmap_create_with_resource(RESOURCE_ID_IMG_CREEPER);
      break;
    case 1:
      rogue = gbitmap_create_with_resource(RESOURCE_ID_IMG_ENDER);
      break;
    case 2:
      rogue = gbitmap_create_with_resource(RESOURCE_ID_IMG_GHAST);
      break;
    case 3:
      rogue = gbitmap_create_with_resource(RESOURCE_ID_IMG_MAGMA);
      break;
    case 4:
      rogue = gbitmap_create_with_resource(RESOURCE_ID_IMG_SLIME);
      break;
    case 5:
      rogue = gbitmap_create_with_resource(RESOURCE_ID_IMG_WITHER);
      break;
  }
  bitmap_layer_set_bitmap(p_bg_layer, rogue);
}      

static void main_window_load(Window *window) {
  // Create BitmapLayers to show GBitmaps and add to Window
  p_bg_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_background_color(p_bg_layer, GColorBlack); 
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(p_bg_layer));  
  update_watch_bg();
  
  // Create TextLayers for time
  s_time_layer = text_layer_create(GRect(0, 144, 144, 24));
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  text_layer_set_text(s_time_layer, "00:00");
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer)); 

}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {

  // Create a long-lived buffer
  static char t_buffer[] = "00:00";
  strftime(t_buffer, sizeof(t_buffer), "%l:%M", tick_time);
  
  static char msg[79] = "";
  strcat(msg, t_buffer);
  switch (rand() % 4){
    case 0:
      strcat(msg, "   Arrrgh!!!");
      break;
    case 1:
      strcat(msg, "   RUUUUUUN!");
      break;
    case 2:
      strcat(msg, "   Heeeeelp!");
      break;
    case 3:
      strcat(msg, "   NOOooOO!!");
      break;
  }
  text_layer_set_text(s_time_layer, msg);
}

static void tap_handler(AccelAxisType axis, int32_t direction) {
  update_watch_bg();
}

static void main_window_unload(Window *window) {
  gbitmap_destroy(rogue);
  bitmap_layer_destroy(p_bg_layer);
  text_layer_destroy(s_time_layer);
}

static void init () {
  // Create main Window element and assign to pointer
  s_main_window = window_create();  
  window_set_background_color(s_main_window, GColorBlack); 

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  // Register with  AccelTimeService
  accel_tap_service_subscribe(tap_handler);
  
}

static void deinit () {
  // Destroy Window
  window_destroy(s_main_window);
  // tick_timer_service_unsubscribe();
  accel_tap_service_unsubscribe();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}