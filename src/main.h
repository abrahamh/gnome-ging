#include <gnome.h>

gint get_x_size();
gint get_y_size();


void set_x_size( gint val );
void set_y_size( gint val );

/* filename of dictionaries */
gchar *get_trans_de_en();
gchar *get_trans_lat_de();
gchar *get_thes_de();

/* config-flags */
gboolean  get_debug();
gboolean  get_hidden();
gboolean  get_search_case_sense();
gboolean  get_search_exact_words();

void set_search_case_sense(gboolean value);
void set_search_exact_words(gboolean value);
