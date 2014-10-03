#include <gtk/gtk.h>
#include "diagram.h"

typedef enum {
  CONFIRM_PAGES = (1<<0),
  CONFIRM_MEMORY = (1<<1),
  CONFIRM_PRINT = (1<<2)
} ConfirmationKind;

gboolean confirm_export_size (Diagram *dia, GtkWindow *parent, guint flags);
