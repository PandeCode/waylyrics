#include "graphics.hpp"
#include "gtk/gtk.h"
#include "lib.hpp"
#include "waybar_cffi_module.h"
#include <thread>

typedef struct {
  wbcffi_module *waybar_module;
  GtkBox *container;
} Mod;

static int instance_count = 0;
const size_t wbcffi_version = 1;

void *wbcffi_init(const wbcffi_init_info *init_info,
                  const wbcffi_config_entry *config_entries,
                  size_t config_entries_len) {
  fprintf(stderr, "waylyrics: init config:\n");
  for (size_t i = 0; i < config_entries_len; i++) {
    fprintf(stderr, "  %s = %s\n", config_entries[i].key,
            config_entries[i].value);
  }

  Mod *inst = (Mod *)malloc(sizeof(Mod));
  inst->waybar_module = init_info->obj;

  addArea(init_info->get_root_widget(init_info->obj));

  fprintf(stderr, "waylyrics inst=%p: init success ! (%d total instances)\n",
          inst, ++instance_count);
  return inst;
}

void wbcffi_deinit(void *instance) {
  fprintf(stderr, "waylyrics inst=%p: free memory\n", instance);
  free(instance);
}

// void wbcffi_update(void *instance) {
//   fprintf(stderr, "waylyrics inst=%p: Update request\n", instance);
// }
//
// void wbcffi_refresh(void *instance, int signal) {
//   fprintf(stderr, "waylyrics inst=%p: Received refresh signal %d\n",
//   instance,
//           signal);
// }
//
// void wbcffi_doaction(void *instance, const char *name) {
//   fprintf(stderr, "waylyrics inst=%p: doAction(%s)\n", instance, name);
// }
