#include <pebble.h>

static Window *s_main_window;
GBitmap *rogue;
static BitmapLayer *p_bg_layer;
static TextLayer *s_time_layer;
char msg[13];

static void update_watch_bg() {
  gbitmap_destroy(rogue);
  msg[0] = '\0';
  switch (rand() % 13) {
    case 0:
      rogue = gbitmap_create_with_resource(RESOURCE_ID_IMG_CREEPER);
      strcat(msg, "  NOOoooOO!!");
      break;
    case 1:
      rogue = gbitmap_create_with_resource(RESOURCE_ID_IMG_ENDER);
      strcat(msg, "  DON'T LOOK");
      break;
    case 2:
      rogue = gbitmap_create_with_resource(RESOURCE_ID_IMG_GHAST);
      strcat(msg, "  Aarrghhh!!");
      break;
    case 3:
      rogue = gbitmap_create_with_resource(RESOURCE_ID_IMG_MAGMA);
      strcat(msg, "  HEEEEEELP!");
      break;
    case 4:
      rogue = gbitmap_create_with_resource(RESOURCE_ID_IMG_SLIME);
      strcat(msg, "  Eugh gross");
      break;
    case 5:
      rogue = gbitmap_create_with_resource(RESOURCE_ID_IMG_WITHER);
      strcat(msg, "  RUUUUUN!!!");
      break;
    case 6:
      rogue = gbitmap_create_with_resource(RESOURCE_ID_IMG_ZOMBIEPIG);
      strcat(msg, "  SHOO!");
      break;
    case 7:
      rogue = gbitmap_create_with_resource(RESOURCE_ID_IMG_DRAGON);
      strcat(msg, "  OH CRUM.");
      break;
    case 8:
      rogue = gbitmap_create_with_resource(RESOURCE_ID_IMG_ZOMBIE);
      strcat(msg, "  BRAAAINNNS");
      break;
    case 9:
      rogue = gbitmap_create_with_resource(RESOURCE_ID_IMG_BLAZE);
      strcat(msg, "  FIRE!");
      break;
    case 10:
      rogue = gbitmap_create_with_resource(RESOURCE_ID_IMG_SKELETON);
      strcat(msg, "  U Hungry?");
      break;
    case 11:
      rogue = gbitmap_create_with_resource(RESOURCE_ID_IMG_STRAY);
      strcat(msg, "  r u lost?");
      break;
    case 12:
      rogue = gbitmap_create_with_resource(RESOURCE_ID_IMG_CAVESPIDER);
      strcat(msg, "  BOO!");
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
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer)); 

}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {

  // Create a long-lived buffer
  static char t_buffer[79] = "";
  strftime(t_buffer, 6, "%l:%M", tick_time);
  text_layer_set_text(s_time_layer, strcat(t_buffer, msg));
}

static void tap_handler(AccelAxisType axis, int32_t direction) {
  update_watch_bg();
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  tick_handler(tick_time, MINUTE_UNIT);
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