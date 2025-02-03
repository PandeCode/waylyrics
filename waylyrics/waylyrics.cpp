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

  GtkContainer *root = init_info->get_root_widget(init_info->obj);

  inst->container = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 5));
  gtk_container_add(GTK_CONTAINER(root), GTK_WIDGET(inst->container));

  init();

  GtkLabel *label = GTK_LABEL(gtk_label_new(loadingText));
  gtk_container_add(GTK_CONTAINER(inst->container), GTK_WIDGET(label));

  GtkProgressBar *progress = GTK_PROGRESS_BAR(gtk_progress_bar_new());
  gtk_container_add(GTK_CONTAINER(inst->container), GTK_WIDGET(progress));

  std::thread([label, progress]() {
    for (;;) {

      auto d = getCurrentLine();
      if (d.has_value()) {
        auto [line, pos, dur] = d.value();

        gtk_label_set_text(label, line.c_str());

        auto val = double(pos) / dur;
        if (val > 0)
          gtk_progress_bar_set_fraction(progress, val);
      }

      using namespace std::chrono_literals;
      std::this_thread::sleep_for(1s);
    }
  }).detach();

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
