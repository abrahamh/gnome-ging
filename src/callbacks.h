/*
 * Copyright (C) 2003-2004 the gnome-ding project
 *
 * This file is part of gnome-ding, a free translator/spellchecker
 *
 * gnome-ding is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * gnome-ding is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
 *
 * callbacks.h
 *
 * begin                : Sat May 31 2003
 * copyright            : (C) 2003 by Heiko Abraham
 */

#include <gnome.h>

#ifndef _CALLBACK_H_
#define _CALLBACK_H_

enum {
  			COLUMN_ASPELL_FIXED,
  			COLUMN_ASPELL_DESCRIPTION,
			COLUMN_ASPELL_NUM,
  			NUM_ASPELL_COLUMNS
		};
		enum {
  			COLUMN_THES_FIXED,
  			COLUMN_THES_DESCRIPTION,
			COLUMN_THES_NUM,
  			NUM_THES_COLUMNS
		};
		enum {
  			COLUMN_DICT_FIXED,
  			COLUMN_DICT_DESCRIPTION,
			COLUMN_DICT_NUM,
  			NUM_DICT_COLUMNS
		};
		
		typedef struct {
  			const gboolean  fixed;
  			const gchar    *description;
			const guint     number;
		} ASPELL_TREE; 
		typedef struct {
  			const gboolean  fixed;
  			const gchar    *description;
			const guint     number;
		} THES_TREE; 
		typedef struct {
  			const gboolean  fixed;
  			const gchar    *description;
			const guint     number;
		} DICT_TREE; 


void on_search_history_wordlist_init();

void  on_search_history_wordlist_delete();


void
on_menu_back_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_menu_forward_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_menu_search_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_menu_preferences_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_menu_quit_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_menu_paste_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_menu_copy_left_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_menu_copy_right_activate         (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_menu_mini_mode_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_menu_window_mode_activate        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_menu_about_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_menu_search_word_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_mainwin_destroy                     (GtkObject       *object,
                                        gpointer         user_data);
										
void
on_submitbutton_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
on_statusbar_print                         (GtkWidget      *parent,
                                         gchar           *msg);


gboolean
on_eventbox1_button_press_event        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

void
on_search_combobox_fill( GtkWidget *mainwin ) ;

void
on_button_back_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_vor_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_pref_dialog_close                  (GtkDialog       *dialog,
                                        gpointer         user_data);

void
on_pref_dialog_destroy                (GtkObject       *object,
                                        gpointer         user_data);

void
on_pref_dialog_ok_clicked                   (GtkButton       *button,
                                        gpointer         user_data);

void
on_pref_grep_word_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_pref_grep_case_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_pref_aspell_suggest_value_changed        (GtkRange        *range,
                                        gpointer         user_data);
										

void
run_translation (GtkWidget *caller, gchar *lang_from, gchar *lang_to, gchar *dict);

void 
on_statusbar_cleanup(GtkWidget      *parent);

void showtable (GtkWidget *widget, GtkListStore *store, 
			gint col_num, gint basewidth, gchar *hdr, gchar *hdr2);

void
on_button_reset_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void on_treeview_set_color1(GtkWidget * widget);
void on_treeview_set_font(GtkWidget *widget);



void
on_pref_font_default_toggled           (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_pref_default_color_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_list_cursor_changed                 (GtkTreeView     *treeview,
                                        gpointer         user_data);



gboolean
on_list_button_press_event             (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);
void on_contex_prepare(gint value);


void
on_contex_copy_l                         (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_contex_copy_r                         (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

										
void on_contex_menu_show(GtkWidget *widget);


#endif // _CALLBACK_H_
