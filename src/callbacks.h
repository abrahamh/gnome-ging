#include <gnome.h>

#ifndef _CALLBACK_H_
#define _CALLBACK_H_

void on_wordlist_init();

void  on_wordlist_delete();

void
file_quit_activated                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
file_paste_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_info1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_submitbutton_clicked                (GtkButton       *button,
                                        gpointer         user_data);


gboolean
on_eventbox1_button_press_event        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

void
fill_combo( GtkWidget *mainwin ) ;

void
on_button_back_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_vor_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_back1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_forward1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_search1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_preferences1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_mini_mode1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_window_mode1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_preferences1_close                  (GtkDialog       *dialog,
                                        gpointer         user_data);

void
on_preferences1_destroy                (GtkObject       *object,
                                        gpointer         user_data);

void
on_okbutton1_clicked                   (GtkButton       *button,
                                        gpointer         user_data);

void
on_grep_word_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_grep_case_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_aspell_suggest_value_changed        (GtkRange        *range,
                                        gpointer         user_data);
										

void
run_translation (GtkWidget *caller, gchar *lang_from, gchar *lang_to, gchar *dict);



#endif // _CALLBACK_H_

void
on_button_reset_clicked                (GtkButton       *button,
                                        gpointer         user_data);
