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
 * callbacks.c
 *
 * all gtk+-callbacks for gui interaction
 *
 * begin                : Sat May 31 2003
 * copyright            : (C) 2003 by Heiko Abraham
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gnome.h>


#ifdef USE_ASPELL
#  include <aspell.h>
#endif
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "main.h"
#include "thesaurus.h"
#include "aspell.h"

#define TIMESTAMP 0

// global list for Search-history
GList *wordlist;

// global pointer for preferences dialog
GtkWidget *pref_dlg = NULL;


#ifdef TIMESTAMP
void sub_time(struct timeval *e, struct timeval *a, struct timeval *b) {
        e->tv_sec = a->tv_sec - b->tv_sec;
        e->tv_usec = a->tv_usec - b->tv_usec;
        if(e->tv_usec < 0) {
                e->tv_usec += 1000000;
                e->tv_sec--;
        } else if(e->tv_usec >= 1000000) {
                e->tv_usec -= 1000000;
                e->tv_sec ++;
        }
}
void add_time(struct timeval *e, struct timeval *a, struct timeval *b) {
        e->tv_sec = a->tv_sec + b->tv_sec;
        e->tv_usec = a->tv_usec + b->tv_usec;
        if(e->tv_usec < 0) {
                e->tv_usec += 1000000;
                e->tv_sec--;
        } else if(e->tv_usec >= 1000000) {
                e->tv_usec -= 1000000;
                e->tv_sec ++;
        }
}
int sub_abs_time(struct timeval *e, struct timeval *a, struct timeval *b) {
        sub_time(e, a, b);
        if(e->tv_sec>=0) return 0;
        sub_time(e, b, a);
        return 1;
}
/*
   struct timeval difftime, endtime, starttime;
   gettimeofday( &starttime, NULL);
	
	sub_time( &difftime, &endtime, &starttime);
	g_print(" diff time %i.%05i \n", difftime.tv_sec, difftime.tv_usec );
*/
#endif // TIMESTAMP





// usage:  THESAURUS_MACRO( THES_DE, _("Thesaurus German") )
#define THESAURUS_MACRO(x, y) \
	strcmp ((char*)suchart, (char*)y ) == 0 ) { \
			run_thesaurus(widget, get_model_char(x), y);

// usage: SPELLIT_MACRO( _("Spellchecker German"), "de_DE" )
#define SPELLIT_MACRO(x,y) \
	strcmp ((char*)suchart, (char*)x ) == 0 ) { \
			run_spellchecker(widget, y);
		
// usage: TRANSLATION_MACRO( _("Latin <--> German") , _("Latin"), _("German") , USE_DICT_LAT_DE ) 
#define TRANSLATION_MACRO(x,y,z, a ) \
	strcmp ((char*)suchart, (char*)x ) == 0 ) { \
		run_translation(widget, y, z, get_model_char(a) );

		// usage: COMBO_MACRO( ASPELL_DE, "de_DE", _("Spellcheck (German)") 		
#define COMBO_MACRO(x,y,z) \
		( get_model_bool(x) == TRUE) && (strcmp(entryd->name, (char*)y) == 0) ) { \
			combo_entry = g_list_append (combo_entry, (gpointer)z);
// usage: COMBO_MACRO2( DICT_DE_EN, _("German <--> English") )
#define COMBO_MACRO2(x, y) \
	if( get_model_bool( x ) ) { \
	combo_entry = g_list_append (combo_entry, (gpointer)y ); \
	}
			
// usage: PREF_SPELL_MACRO( ASPELL_GB, "en_GB", _("English (GB)") )
#define PREF_SPELL_MACRO(x,y,z) \
 /* if */ (strcmp(entryd->name, (char*)y) == 0) ) { \
		gtk_list_store_append (store_aspell, &iter_aspell); \
		gtk_list_store_set (store_aspell, &iter_aspell, \
			  COLUMN_ASPELL_FIXED, get_model_bool(x), \
			  COLUMN_ASPELL_DESCRIPTION, z, \
			  COLUMN_ASPELL_NUM, x, \
			  -1);

// usage: PREF_THES_MACRO( THES_DE, _("German") )	 
#define PREF_THES_MACRO(x,y) \
	 /* if ( */ stat( get_model_char(x), &my_stat) == 0 ) { \
	 	gtk_list_store_append (store_thes, &iter_thes);  \
        gtk_list_store_set (store_thes, &iter_thes,  \
                          COLUMN_THES_FIXED, get_model_bool(x),  \
                          COLUMN_THES_DESCRIPTION, y,  \
                          COLUMN_THES_NUM, x, \
                          -1);

// usage: PREF_DICT_MACRO( DICT_DE_EN, _("German <--> English") )	 
#define PREF_DICT_MACRO(x,y) \
	 /* if ( */ stat( get_model_char(x), &my_stat) == 0 ) { \
	 	gtk_list_store_append (store_dict, &iter_dict);  \
        gtk_list_store_set (store_dict, &iter_dict,  \
                          COLUMN_DICT_FIXED, get_model_bool(x),  \
                          COLUMN_DICT_DESCRIPTION, y,  \
                          COLUMN_DICT_NUM, x, \
                          -1);		 

static void on_pref_spell_toggle(GtkCellRendererToggle *cell, gchar *path_str, gpointer data);
static void on_pref_thes_toggle(GtkCellRendererToggle *cell, gchar *path_str, gpointer data);
static void on_pref_dict_toggle(GtkCellRendererToggle *cell, gchar *path_str, gpointer data);

void on_window_unhide (GtkWidget *widget);
void on_search_history_go_back(GtkWidget *widget);
void on_search_history_go_forward(GtkWidget *widget);
void on_search(GtkWidget *widget);
		 

/**
 * @doc this function initialize the global stored Glist to
 *     store a history of the search words.
 *     By using this GList we provide a access to the 
 *     prev/next search string in the history
 *
 * @parameter wordlist, a pointer to a global stored GList
 *
 * @return wordlist - global stored pointer
 *
 * @author Heiko Abraham, <abrahamh@web.de>
 *
 * @see on_search_history_wordlist_delete()
 *
**/
void on_search_history_wordlist_init() {
	wordlist = g_list_append (wordlist, g_strdup( "" ) );
	wordlist = g_list_last(wordlist);
}



/**
 * @doc delete wordlist-history for search-string 
 *
 * @author Heiko Abraham, <abrahamh@web.de>
 *
**/
void on_search_history_wordlist_delete() {
	if (wordlist != NULL) {
		gchar *foo;
		while( g_list_last(wordlist) != NULL ) {
			wordlist = g_list_last(wordlist);
			g_free(wordlist->data);
			wordlist = g_list_remove(wordlist, (gpointer)wordlist->data);
		}
		g_list_free(wordlist);
	}
}



/**
 *
 * @doc history back/forward for search-sting
 *
 * @author Heiko Abraham, <abrahamh@web.de>
 *
**/
void on_search_history_go_back(GtkWidget *widget) {
	if (wordlist != NULL ) {
		GtkWidget * entry = NULL;
		GtkWidget *prev_button = NULL;
		GtkWidget *next_button = NULL;
		
		entry = lookup_widget(widget, "search_input");
		prev_button = lookup_widget( widget, "button_back");
		next_button = lookup_widget( widget, "button_forward");
		
		if (wordlist->prev != NULL ) {
			wordlist = g_list_previous(wordlist);
			gtk_entry_set_text(GTK_ENTRY(entry), (gchar *)wordlist->data);
		}

		if (g_list_previous(wordlist) == NULL ) {
			gtk_widget_set_sensitive (GTK_WIDGET(prev_button), FALSE);
		} else {
			gtk_widget_set_sensitive (GTK_WIDGET(prev_button), TRUE);
		}
		if (g_list_next(wordlist) == NULL ) {
			gtk_widget_set_sensitive (GTK_WIDGET(next_button), FALSE);
		} else {
			gtk_widget_set_sensitive (GTK_WIDGET(next_button), TRUE);
		}
	}
}


/**
 * 
 * @doc history back/forward for search-sting
 *
 * @author Heiko Abraham, <abrahamh@web.de>
 * 
**/
void on_search_history_go_forward(GtkWidget *widget) {
	if( wordlist != NULL) {
		GtkWidget * entry = NULL;
		GtkWidget *prev_button = NULL;
		GtkWidget *next_button = NULL;
		entry = lookup_widget(widget, "search_input");
		prev_button = lookup_widget( widget, "button_back");
		next_button = lookup_widget( widget, "button_forward");
	
		if( wordlist->next != NULL) {
			wordlist = g_list_next(wordlist);
			gtk_entry_set_text(GTK_ENTRY(entry), (gchar *)wordlist->data);
		}

		if (g_list_previous(wordlist) == NULL ) {
			gtk_widget_set_sensitive (GTK_WIDGET(prev_button), FALSE);
		} else {
			gtk_widget_set_sensitive (GTK_WIDGET(prev_button), TRUE);
		}
		if (g_list_next(wordlist) == NULL ) {
			gtk_widget_set_sensitive (GTK_WIDGET(next_button), FALSE);
		} else {
			gtk_widget_set_sensitive (GTK_WIDGET(next_button), TRUE);
		}
	}
}

/**
 *
 * @doc get the widgets to restore
 *
 * @author Heiko Abraham, <abrahamh@web.de>
 *
 **/
void on_window_unhide (GtkWidget      *widget) {
	if ( widget == NULL ) return;
		
	GtkWidget *list = NULL;
	GtkWidget *status = NULL;
	GtkWidget *win = NULL;
	gboolean value = FALSE;
	
	win = get_mainwin(); 
	if ( win != NULL ) {
		GtkWidget *toggle = NULL;
		toggle = lookup_widget(win, "on_menu_window_mode");
		value = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(toggle) );
		
		if (value != TRUE) {
			list = lookup_widget( win, "scrolledwindow1");
    		status = lookup_widget(win, "appbar1");

			gtk_widget_show(list);
			gtk_widget_show_all( GNOME_APP(win)->menubar );
			gtk_widget_show(status);
			gtk_window_resize(GTK_WINDOW(win), get_model_int(MAIN_SIZE_X), get_model_int(MAIN_SIZE_Y) );
			gtk_window_set_resizable(GTK_WINDOW(win), 1);
			// set menuitem 
			gtk_toggle_button_set_active ( GTK_TOGGLE_BUTTON ( toggle), TRUE);
		}
	}
} // unhide



/**
 *
 * @doc gtk-callback function, typical called from main-win help menu.
 *     This function create and show a Gnome-About-Box.
 *
 * @author Heiko Abraham, <abrahamh@web.de>
 *
**/
void
on_menu_about_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GtkWidget *about_gnome_ding;
  about_gnome_ding = create_about_gnome_ding ();
  // gtk_window_set_transient_for( GTK_WINDOW(infodialog), GTK_WINDOW( get_mainwindow() ) );
  gtk_widget_show (about_gnome_ding);
}


/**
 *
 * @doc gtk-callback function, called, if the application will 
 *      be destroyed.
 *      <BR>Store lates main window size that we can save it
 *      as personal configuration.
 *
**/
void
on_mainwin_destroy                     (GtkObject       *object,
                                        gpointer         user_data)
{
	gint x,y;
	gtk_window_get_size(GTK_WINDOW( get_mainwin()) , &x , &y );
	set_model_int(MAIN_SIZE_X, x);
	set_model_int(MAIN_SIZE_Y, y);
	gtk_main_quit();
}

/**
 * @doc gtk-callback to swich main window into the mini-mode.
 *      The Mini-Mode resize the main window and show only
 *      the search toolbar.
 *
**/
void
on_menu_mini_mode_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget *list = NULL;
	GtkWidget *status = NULL;
	GtkWidget *win = NULL;

	win = get_mainwin();
	if ( win != NULL ) {
    	list = lookup_widget( win, "scrolledwindow1");
	    status = lookup_widget( win, "appbar1");

		if (gtk_window_get_resizable(GTK_WINDOW(win))) {
			gtk_widget_hide(list);
			gtk_widget_hide( GNOME_APP(win)->menubar  );
			gtk_widget_hide(status);
			gtk_window_resize(GTK_WINDOW(win), get_model_int(MAIN_SIZE_X), 40);
			gtk_window_set_resizable(GTK_WINDOW(win), 0);
		}
	}
}


/**
 * @doc gtk-callback - switch main window in the normal 
 *      display mode (versus mini-mode). 
 *
**/
void
on_menu_window_mode_activate        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	on_window_unhide(GTK_WIDGET(menuitem) );
}


/**
 *
 * @doc helper function to create and show the listview with the 
 *      search result 
 *
 * @parameter widget - a pointer to main window
 *
 * @parameter store - a pointer to the list store model 
 *
 * @parameter col_num - integer to define how many columns 
 *           to be shown (typical One or Two)
 *
 * @parameter basewidth - width of the column in pixel
 *
 * @parameter hdr - string to define the list header (left)
 *
 * @parameter hdr2 - string to define the list header (right)
 *
 * @return void
 *
**/
void showtable (GtkWidget *widget, GtkListStore *store, 
			gint col_num, gint basewidth, gchar *hdr, gchar *hdr2) {
	if (widget == NULL) return;
	if( store == NULL) return;
	gint i;
    GtkWidget *treeview;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;

	treeview = lookup_widget(GTK_WIDGET(widget), "list");
    gtk_tree_view_set_model (GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(treeview), TRUE);
                                                                                                       
	/* clean up the view */
                                                                                                       
    if (gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview)))  {
		GList *col = gtk_tree_view_get_columns( GTK_TREE_VIEW(treeview));
		for (i=0; i < g_list_length(col); i++)
			gtk_tree_view_remove_column(GTK_TREE_VIEW(treeview), g_list_nth_data(col, i));
	}
                                                                                                       
	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes ( hdr, renderer, "markup", 0, NULL);
	gtk_tree_view_column_set_sort_column_id(column, 0);
	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview), column, 0);		
	g_object_set (G_OBJECT (column), "min-width", basewidth, "resizable", TRUE, NULL);
		
	if ( col_num > 1 ) {
		column = gtk_tree_view_column_new_with_attributes ( hdr2, renderer, "markup", 1, NULL);
		gtk_tree_view_column_set_sort_column_id(column, 1);
		gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview), column, 1);
		g_object_set (G_OBJECT (column), "min-width", basewidth, "resizable", TRUE, NULL);
	} 
}
        


/** 
 *
 * @doc print message for user information
 *
 * @parameter parent - a pointer to the main window
 *
 * @parameter msg - the message to be show in the statusbar
 *
 * @return void 
 *
**/
void on_statusbar_print(GtkWidget *parent, gchar *msg) {
	if (parent == NULL) return;
	GtkWidget *statusbar = NULL;
	statusbar = lookup_widget(GTK_WIDGET(parent), "appbar1");	
	
	if ( statusbar == NULL) return;
		
    if ( msg == NULL ) {
		gnome_appbar_clear_stack (GNOME_APPBAR(statusbar));
		gnome_appbar_set_default( GNOME_APPBAR(statusbar) , ""  );
	} else {
		gnome_appbar_set_default( GNOME_APPBAR(statusbar) , msg  );
	}
}


/**
 *
 * @doc clean up the status bar of the main window
 *
 * @parameter parent - a pointer to the main window
 *
**/
void on_statusbar_cleanup(GtkWidget *parent) {
	if (parent == NULL) return;
	GtkWidget       *statusbar = NULL; 
	statusbar = lookup_widget(GTK_WIDGET(parent), "appbar1");	
	gnome_appbar_clear_stack (GNOME_APPBAR(statusbar));
	gnome_appbar_set_default( GNOME_APPBAR(statusbar) , ""  );

}


/**
 *
 * @doc gtk-callback - quit the main application and store the
 *      last main window size to the personal configuration 
 *      (GConf)
 * 
 * @parameter menuitem - pointer to the called menu entry 
 *
 * @parameter user_data - unused
 *
 * @return void - destory application main loop
 *
**/
void
on_menu_quit_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	gint x,y;
	gtk_window_get_size( GTK_WINDOW( get_mainwin() ), 
			&x , &y );
	set_model_int(MAIN_SIZE_X, x);
	set_model_int(MAIN_SIZE_Y, y);
	gtk_main_quit ();
}


/**
 *
 * @doc gtk-callback - go one step back in the search history
 *     and get the last search word from the global list and
 *     fill it into the search entry.
 *
 * @parameter menuitem - pointer to the called menu entry 
 *
 * @parameter user_data - unused
 *
 * return void - fill last word into the search entry
 *
**/
void
on_menu_back_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	on_search_history_go_back( GTK_WIDGET(menuitem) );
}


/**
 *
 * @doc gtk-callback - go one step forward in the search history
 *     and get the next search word from the global list and
 *     fill it into the search entry.
 *
 * @parameter menuitem - pointer to the called menu entry 
 *
 * @parameter user_data - unused
 *
 * return void - fill next word into the search entry
 *
**/
void
on_menu_forward_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	on_search_history_go_forward( GTK_WIDGET(menuitem) );
}


/**
 *
 * @doc gtk-callback - start search for the enter word in
 *      the search entry and the selected dictionary.
 *
 * @parameter menuitem - pointer to the called menu entry 
 *
 * @parameter user_data - unused
 *
 * @return void - search result in listview if possible
 *
**/

void
on_menu_search_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	on_search( GTK_WIDGET(menuitem) );
}

/**
 *
 * @doc gtk-callback - function is called if the user wish to
 *      insert the content of the clipboard to the entry for
 *      the search word 
 *
 * @parameter menuitem - pointer to the called menu entry 
 *
 * @parameter user_data - unused
 *
 * @return void 
 *
**/
void
on_menu_paste_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	if( menuitem == NULL) return;
	GtkWidget *entry;
	entry = lookup_widget(GTK_WIDGET(menuitem), "search_input");
        
	gtk_entry_set_text(GTK_ENTRY(entry), "");
    gtk_editable_paste_clipboard(GTK_EDITABLE(GTK_ENTRY(entry)));
    gtk_editable_select_region(GTK_EDITABLE(GTK_ENTRY(entry)), 0, -1);
}


/**
 *
 * @doc gtk-callback - called, if the user wish to copy the 
 *      content of the left cell of the column to the clipboard
 *
 * @parameter menuitem - pointer to the called menu entry 
 *
 * @parameter user_data - unused
 *
 * @return void 
 * 
**/
void
on_menu_copy_left_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	on_contex_copy_l(menuitem, NULL);
}


/**
 *
 * @doc gtk-callback - called, if the user wish to copy the 
 *      content of the right cell of the column to the clipboard
 *
 * @parameter menuitem - pointer to the called menu entry 
 *
 * @parameter user_data - unused
 *
 * @return void 
 * 
**/
void
on_menu_copy_right_activate         (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	on_contex_copy_r(menuitem, NULL);
}



/**
 *
 *  @dic use a ding dictinory to translate String(lang_from) ---> String(to)
 * 
 * @parameter caller - a pointer to the main window
 *
 * @parameter lang_from - description string of the language (from translation),
 *           display in the list header 
 *
 * @parameter lang_to - description string of the language (translate to .. ),
 *            displayed in the list header
 *
 * @paramter dict - the filename of the uncompressed dict
 *
 * @return void
 *
**/
void run_translation (GtkWidget *caller, gchar *lang_from, gchar *lang_to, gchar *dict) {
	if( get_debug() ) {
		g_print("call translation '%s' '%s' <--> '%s'\n",
			dict, lang_from, lang_to);
	}
	
	if (caller == NULL) return;        
	if (lang_from == NULL) return;
	if (lang_to == NULL) return;
	if (dict == NULL) return;
    
	set_model_int(MAIN_COLUMN_NUM, 2);
	on_contex_menu_show(caller);
	
    GtkWidget       *entry = lookup_widget(GTK_WIDGET(caller), "search_input");
    GtkTreeIter     iter;
    GtkListStore    *store = NULL;
                                                                                                       
    /* initialize the vars */
	GString *cmd = g_string_new("");
	GString *grep_out = g_string_new("");
	char line[2048];
	gchar *reste = NULL;
	FILE *ptr;
	gchar *msg = NULL;
    gchar *such = NULL;
	gint flag = 0;
	
	gint use_egrep = TRUE;  // new algorithm is to slow
	
		
    such  = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry) ) );
    store = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_STRING);                                                                                               
    
	
	if ( use_egrep == TRUE ) {
    	g_string_printf(cmd, "egrep -h %s %s '%s' '%s' | grep -v '^#'", 
	    	(get_model_bool(MAIN_CASE) == FALSE) ? "-i" : " ",
			(get_model_bool(MAIN_EXACT) == TRUE) ? "-w" : " ",
			such, dict );
		if ( get_debug() == TRUE) {
     	   g_print("command: '%s'\n", cmd->str);
		}
		ptr = popen(cmd->str,"r");
		if (ptr != NULL ) {
			while (  fgets( line, sizeof(line), ptr)  ) {
    			g_string_append_printf(grep_out, "%s", line );
			}
			pclose(ptr);
		}	
	} else {
		// use split-algorithm
		
		// make it with pointer to gchar, not with gstring
		GString *helper = g_string_new("");
		FILE *fd;
		fd = fopen(/* cmd->str */ dict ,"r");
		if (fd != NULL ) {
			while (  fgets( line, sizeof(line), fd)  ) {
    			g_string_append_printf(helper, "%s", line );
			}
			fclose(fd);
		}
		gchar *mypointer = NULL;
		gint i;
		while (helper->len > 5) {
			gchar **field_line;
			gchar **field;
			field_line = g_strsplit ( helper->str, "\n", 2);
			field = g_strsplit( field_line[0], " :: ", 2);
			helper = g_string_erase(helper, 0, g_utf8_strlen(field_line[0], -1)  + 1 ); 			
			if ( field[0] != NULL ) {
				// search 
				// i = g_utf8_strlen( field[0], -1 ); 
				// mypointer = g_strrstr_len( field[0], i , such );
				mypointer = g_strrstr( field[0], such );
				if ( mypointer != NULL ) {
					g_string_append_printf(grep_out, "%s\n", field_line[0] );
				}
			}
			g_strfreev (field);
			g_strfreev (field_line);
		} // while
		g_string_free(helper, TRUE);
	}
       
	
	
                                                                                                       
	if ( grep_out->len > 2 ) {
		if ( get_debug() == TRUE ) {
			g_print("grep-result:\n%s\n", grep_out->str);
		}
		if ( strlen(grep_out->str) < 4 ) {
			/* no match found */
			gtk_list_store_append (GTK_LIST_STORE(store), &iter);
			gtk_list_store_set (GTK_LIST_STORE(store), &iter,
                        0, _("No match found"),
                        1, _("No match"), -1 );
                                
		} else {
			/* list found matches */
			gint i=0;
			while ( strlen(grep_out->str) > 5 ) {
				gchar **result_line;
				gchar **result;
				result_line = g_strsplit ( grep_out->str, "\n", 2);
				result = g_strsplit ( result_line[0], " :: ", 2);
				flag++;
                   	
 					GString *out0 = g_string_sized_new( strlen(result[0] ) ) ;
					GString *out1 = g_string_sized_new( strlen(result[1] ) ) ;
        			gint i=0;
					gchar *reste = NULL;
					
					gint st_len = strlen(result[0]);
					gint such_len = strlen(such);
					for(i=0; i < st_len; i++ ) {
                		if ( result[0][i] == '[' ) {
                        	out0 = g_string_append(out0, "[<i>");
                		} else if (result[0][i] == ']') {
							out0 = g_string_append(out0, "</i>]");
						} else if (result[0][i] == '{') {
							out0 = g_string_append(out0, "<i>");
						} else if (result[0][i] == '}') {
							out0 = g_string_append(out0, ".</i>");
						} else if (result[0][i] == '|') {
							out0 = g_string_append(out0, "\r ");
						} else if ( ( (i + such_len -1 ) <  st_len ) && ( result[0][i] == such[0] ) ) { 
							gboolean mismatch = FALSE;
							gint ii=0;
							for (ii=0; ii<such_len; ii++) {
								if ( (mismatch == FALSE) && (result[0][i + ii] == such[ii]) ) {
									mismatch = FALSE;
								} else {
									mismatch = TRUE;
								}
							}
							if (mismatch == FALSE) {
								// out0 = g_string_append(out0, "<span foreground=\"darkblue\">");	
								out0 = g_string_append(out0, "<span foreground=\"");	
								out0 = g_string_append(out0, get_model_char(MAIN_COLOR2) );	
								out0 = g_string_append(out0, "\">");	
								out0 = g_string_append(out0, such);	
								out0 = g_string_append(out0, "</span>");	
								i = i + such_len - 1;
							} else {
								out0 = g_string_append_c(out0, result[0][i] );
							}
						} else {
							out0 = g_string_append_c(out0, result[0][i] );
						}
					}
					st_len = strlen(result[1]);
					for(i=0; i<strlen(result[1] ); i++) {
                		if ( result[1][i] == '[' ) {
                        	out1 = g_string_append(out1, "[<i>");
                		} else if (result[1][i] == ']') {
							out1 = g_string_append(out1, "</i>]");
						} else if (result[1][i] == '{') {
							out1 = g_string_append(out1, "<i>");
						} else if (result[1][i] == '}') {
							out1 = g_string_append(out1, ".</i>");
						} else if (result[1][i] == '|') {
							out1 = g_string_append(out1, "\r ");
						} else if ( ( (i + such_len -1 ) <  st_len ) && ( result[1][i] == such[0] ) ) { 
							/* todo: syntax-highlight also with one error char difference */
							gboolean mismatch = FALSE;
							gint ii=0;
							for (ii=0; ii<such_len; ii++) {
								if ( (mismatch == FALSE) && (result[1][i + ii] == such[ii]) ) {
									mismatch = FALSE;
								} else {
									mismatch = TRUE;
								}
							}
							if (mismatch == FALSE) {
								out1 = g_string_append(out1, "<span foreground=\"");	
								out1 = g_string_append(out1, get_model_char(MAIN_COLOR2) );	
								out1 = g_string_append(out1, "\">");	
								out1 = g_string_append(out1, such);	
								out1 = g_string_append(out1, "</span>");	
								i = i + such_len - 1;
							} else {
								out1 = g_string_append_c(out1, result[1][i] );
							}
						} else {
							out1 = g_string_append_c(out1, result[1][i] );
						}
					}
					gtk_list_store_append (GTK_LIST_STORE(store), &iter);
					gtk_list_store_set (GTK_LIST_STORE(store), &iter,
                        0, out0->str,  
						1, out1->str,  -1);
					
					reste = g_string_free(out0, TRUE);
					reste = g_string_free(out1, TRUE);
				grep_out = g_string_erase(grep_out, 0, strlen(result_line[0])+1 );
                g_strfreev (result);
                g_strfreev (result_line);
			}
		}
	
    	/* output */
    	showtable(GTK_WIDGET(caller), store, 2, 250, lang_from, lang_to );
    	msg = g_strdup_printf( _("%i matches found to '%s'"), flag, such);
    	on_statusbar_print(GTK_WIDGET(caller), msg);
	
	}
	g_free(msg);
	g_free(such);
	reste = g_string_free(grep_out, TRUE);
	reste = g_string_free(cmd, TRUE);		
}




/**
 * visual wait info
 * follow code is only for test, idee, make search-spider
**/
/*
void *my_wait_thread (void *data)
{
	GtkWidget *button = (GtkWidget *)data;
  	pthread_setcancelstate (PTHREAD_CANCEL_ENABLE, NULL);
  	pthread_setcanceltype  (PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	
	while (1) {
    	g_print (".");
    	usleep(1000 * 500);
		// GdkPixbuf *myspider = NULL;
       // if (myflag) {
		//	myspider = create_pixbuf( "gnome-ding/gnome-ding24-1.png");
		// } else {
		//	myspider = create_pixbuf( "gnome-ding/gnome-ding24-2.png");
		// }
		// if (myflag) {
		// 	myflag = FALSE;
		// } else {
		//	myflag = TRUE;
		// }
       //  g_object_set ( G_OBJECT(spider_w) , "pixbuf", myspider, NULL);
       //  gdk_pixbuf_unref( myspider );
		// gtk_main_iteration();
  	}
  	return NULL;
}
*/


/**
 *
 * @doc get-callback - function called, it the user press the 'search'
 *      button to start the search
 *
 * @parameter menuitem - pointer to the called menu entry 
 *
 * @parameter user_data - unused
 *
 * @return void 
 *
**/
void on_submitbutton_clicked(GtkButton *button, gpointer user_data) {
	on_search( GTK_WIDGET(button) );
}



/**
 * @doc gtk-callback - function called if user toggled the 
 *       hide/unhide area in the toolbar to minimize/resize the
 *       main window.
 *      
 * @parameter widget - pointer this event area widget
 *
 * @parameter event - information about actually event
 *
 * @parameter user_data - unused
 *
 * @return FALSE every times
 *
**/
gboolean
on_eventbox1_button_press_event        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
	GtkWidget *list = NULL;
	GtkWidget *status = NULL;
	GtkWidget *win = NULL;
	
	win = get_mainwin();
	if (win != NULL ) {
    	list = lookup_widget( win , "scrolledwindow1");
    	status = lookup_widget(win, "appbar1");
    
		if (gtk_window_get_resizable(GTK_WINDOW(win))) {
			gtk_widget_hide(list);
			gtk_widget_hide( GNOME_APP(win)->menubar );
			gtk_widget_hide(status);
			gtk_window_resize(GTK_WINDOW(win), get_model_int(MAIN_SIZE_X), 40);
			gtk_window_set_resizable(GTK_WINDOW(win), 0);
		} else {
			on_window_unhide(widget);
		}
	}
  	return FALSE;
}


/**
 *
 * @doc gtk-callback - function is called if user press the back
 *      button on toolbar to go back one word into the history
 *      list of searched words.
 *      As result the previews word of the history list will
 *      inserted into the search entry.
 * 
 * @parameter button - pointer to this button
 * 
 * @parameter user_data - unused
 * 
 * @return void
 *
**/
void
on_button_back_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
	on_search_history_go_back(GTK_WIDGET(button)); 
}



/**
 *
 * @doc gtk-callback - function is called if user press the forward
 *      button on toolbar to become next word into the history
 *      list of searched words.
 *      As result the previews word of the history list will
 *      inserted into the search entry.
 * 
 * @parameter button - pointer to this button
 * 
 * @parameter user_data - unused
 * 
 * @return void
 *
**/
void
on_button_vor_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
	on_search_history_go_forward(GTK_WIDGET(button));
}




/**
 *
 * @doc gtk-callback - called inside the preferences dialog.
 *      Toggle flag for use or not use a selected language for
 *      spelling. This means that only languages, that was selected 
 *      will also show inside the combo-box for dictionay selction
 *      into the main win toolbar.
 *
 * @parameter cell - pointer to the widget, that was toggled
 * 
 * @parameter path_str - treeview path to selected cell
 *
 * @parameter data - pointer to the listview data model 
 *
 * @return void 
 *
**/
static void
on_pref_spell_toggle(GtkCellRendererToggle *cell, gchar *path_str, gpointer data) {
  GtkTreeModel *model = (GtkTreeModel *)data;
  GtkTreeIter  iter;
  GtkTreePath *path = gtk_tree_path_new_from_string (path_str);
  gboolean fixed;
  gint i;
  enum {
	COLUMN_ASPELL_FIXED,
	COLUMN_ASPELL_DESCRIPTION,
	COLUMN_ASPELL_NUM,
	NUM_ASPELL_COLUMNS
  };
		
  // get toggled iter 
  gtk_tree_model_get_iter (model, &iter, path);
  gtk_tree_model_get (model, &iter, 
  	COLUMN_ASPELL_FIXED, &fixed, 
  	COLUMN_ASPELL_NUM, &i,
  -1);

  // toggle flag
  fixed ^= 1;
  // store it back to the  internal data model 
  set_model_bool(i, fixed);

  gtk_list_store_set (GTK_LIST_STORE (model), &iter, COLUMN_ASPELL_FIXED, fixed, -1);
  gtk_tree_path_free (path);
}


/**
 *
 * @doc gtk-callback - called inside the preferences dialog.
 *      Toggle flag for use or not use a selected language for
 *      thesaurus. This means that only thesaurus, that was selected,
 *      will also show inside the combo-box for dictionay selction
 *      into the main win toolbar.
 *
 * @parameter cell - pointer to the widget, that was toggled
 * 
 * @parameter path_str - treeview path to selected cell
 *
 * @parameter data - pointer to the listview data model 
 *
 * @return void 
 *
**/
static void
on_pref_thes_toggle(GtkCellRendererToggle *cell, gchar *path_str, gpointer data) {
  GtkTreeModel *model = (GtkTreeModel *)data;
  GtkTreeIter  iter;
  GtkTreePath *path = gtk_tree_path_new_from_string (path_str);
  gboolean fixed;
  gint i;
  enum {
	COLUMN_THES_FIXED,
	COLUMN_THES_DESCRIPTION,
	COLUMN_THES_NUM,
	NUM_THES_COLUMNS
  };
		
  /* get toggled iter */
  gtk_tree_model_get_iter (model, &iter, path);
  gtk_tree_model_get (model, &iter, 
  	COLUMN_THES_FIXED, &fixed, 
  	COLUMN_THES_NUM, &i,
  -1);
  // toggle flag and store back
  fixed ^= 1;
  set_model_bool(i, fixed);

  gtk_list_store_set (GTK_LIST_STORE (model), &iter, COLUMN_THES_FIXED, fixed, -1);
  gtk_tree_path_free (path);
}

/**
 *
 * @doc gtk-callback - called inside the preferences dialog.
 *      Toggle flag for use or not use a selected language for
 *      translation. This means that only languages, that was selected 
 *      will also show inside the combo-box for dictionay selction
 *      into the main win toolbar.
 *
 * @parameter cell - pointer to the widget, that was toggled
 * 
 * @parameter path_str - treeview path to selected cell
 *
 * @parameter data - pointer to the listview data model 
 *
 * @return void 
 *
**/
static void
on_pref_dict_toggle(GtkCellRendererToggle *cell, gchar *path_str, gpointer data) {
  GtkTreeModel *model = (GtkTreeModel *)data;
  GtkTreeIter  iter;
  GtkTreePath *path = gtk_tree_path_new_from_string (path_str);
  gboolean fixed;
  gint i;
  enum {
	COLUMN_DICT_FIXED,
	COLUMN_DICT_DESCRIPTION,
	COLUMN_DICT_NUM,
	NUM_DICT_COLUMNS
  };
		
  //  get toggled iter 
  gtk_tree_model_get_iter (model, &iter, path);
  gtk_tree_model_get (model, &iter, 
  	COLUMN_DICT_FIXED, &fixed, 
  	COLUMN_DICT_NUM, &i,
  -1);
  // toggle flag and store back
  fixed ^= 1;
  set_model_bool(i, fixed);

  gtk_list_store_set (GTK_LIST_STORE (model), &iter, COLUMN_DICT_FIXED, fixed, -1);
  gtk_tree_path_free (path);
}


/**
 *
 * @doc gtk-callback - close the prefereces-dialog, store
 *      back all unsaved settings and refresh main window.
 *
 * @parameter dialog - pointer to the preferences dialog
 *
 * @parameter user_data -  unused
 *
 * @return void 
 *
 * @see on_pref_dialog_destroy
 *
**/
void on_pref_dialog_close(GtkDialog *dialog, gpointer user_data) {
	gtk_widget_destroy(pref_dlg);
	pref_dlg = NULL;
}


/**
 *
 * @doc gtk-callback - close the prefereces-dialog, store
 *      back all unsaved settings and refresh main window.
 *
 * @parameter onject - pointer to the preferences dialog
 *
 * @parameter user_data -  unused
 *
 * @return void 
 *
 * @see on_pref_dialog_close
**/
void on_pref_dialog_destroy(GtkObject *object, gpointer user_data) {
	gtk_widget_destroy(pref_dlg);
	pref_dlg = NULL;
}

/**
 *
 * @doc gtk-callback - called if user press 'ok' button and 
 *      close the preferences dialog.
 *      Store back all unsaved data to internal data model
 *      like {color, font} and refresh the main window
 *
 * @parameter button - pointer to the button widget of the dialog
 *
 * @parameter user_data - unused
 *
 * @return void 
 *
**/
void on_pref_dialog_ok_clicked(GtkButton *button, gpointer user_data) {
	
	GtkWidget *text_font = NULL;
	GtkWidget *text_color1 = NULL; // text color
	GtkWidget *text_color2 = NULL; // highlight color
	GdkColor color;
	guint16 color_r = 0;
	guint16 color_g = 0;
	guint16 color_b = 0;
	guint16 color_a = 0;
	text_font = lookup_widget( GTK_WIDGET(button), "fontpicker1");
	text_color1 = lookup_widget( GTK_WIDGET(button), "colorpicker1");
	text_color2 = lookup_widget( GTK_WIDGET(button), "colorpicker2");
	// get text color
	gnome_color_picker_get_i16 (GNOME_COLOR_PICKER (text_color1),
				    &color_r, &color_g, &color_b, &color_a);
	color.red = color_r;
	color.green = color_g;
	color.blue = color_b;
	set_model_char(MAIN_COLOR1, gdk_color_to_string (color) );
    // get highlight color
	gnome_color_picker_get_i16 (GNOME_COLOR_PICKER (text_color2),
				    &color_r, &color_g, &color_b, &color_a);
	color.red = color_r;
	color.green = color_g;
	color.blue = color_b;
	set_model_char(MAIN_COLOR2, gdk_color_to_string (color) );

	// get text font
	set_model_char(MAIN_FONT, 
		(gchar*)gnome_font_picker_get_font_name(GNOME_FONT_PICKER(text_font)));

	// close dialog
	gtk_widget_destroy(pref_dlg);
	pref_dlg = NULL;
	// reset search combo 
	on_search_combobox_fill( get_mainwin() );
	on_treeview_set_font( get_mainwin() );
	on_treeview_set_color1( get_mainwin() );
} // on_okbutton1_clicked (preferences dialog)


/**
 *
 * @doc gtk-callback - called if user toggled 'exact search' inside
 *     the preferences dialog
 *
 * @parameter togglebutton - pointer to widget 
 * 
 * @parameter user_data - unused
 *
 * @return void 
 *
**/
void on_pref_grep_word_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
	GtkWidget *exact_word = NULL;
	exact_word = lookup_widget(pref_dlg, "grep_word");
	set_model_bool(MAIN_EXACT, gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (exact_word) ) );
}

/**
 *
 * @doc gtk-callback - called if user toggled 'case-sensitive' inside
 *     the preferences dialog
 *
 * @parameter togglebutton - pointer to widget 
 * 
 * @parameter user_data - unused
 *
 * @return void 
 *
**/
void on_pref_grep_case_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
	GtkWidget *case_sense = NULL;
	case_sense = lookup_widget(pref_dlg, "grep_case");
	set_model_bool(MAIN_CASE , gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (case_sense) ) );
}

/**
 *
 * @doc gtk-callback - called if user change suggestion mode by 
 *       changing the slider inside the preferences dialog
 *
 * @parameter range - the adjustment (value) of the slider
 * 
 * @parameter user_data - unused
 *
 * @return void 
 *
**/
void
on_pref_aspell_suggest_value_changed        (GtkRange        *range,
                                        gpointer         user_data)
{
	GtkAdjustment *adj;
	adj = gtk_range_get_adjustment( range );
	set_model_int(MAIN_SUGEST_MODE,  1 + (gint)( gtk_adjustment_get_value(adj) ));
}


/**
 * 
 * @doc gtk-callback - this function is called, if the user press
 *      the reset button inside the preferences dialog to reset 
 *      al settings of the internal data model 
 *
 * @parameter button - pointer to the button-widget of the dialog
 *
 * @parameter user_data - unused
 *
 * @return void 
 *
**/
void
on_button_reset_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
	model_default();
	gtk_widget_destroy( lookup_widget( GTK_WIDGET(button), "preferences1") ); 
}



/** 
 * start search
**/
void
on_search(GtkWidget *widget) {
 	GtkWidget *entry; 
    GtkWidget *select;
	GtkWidget *win;
	entry  = lookup_widget(widget, "search_input");
	select = lookup_widget(widget, "selection");
	win    = lookup_widget(widget, "ding_mainwin");

	/* get the user input */
    gchar *suchart = NULL;  // the dictionary that are selected 
    gchar *match   = NULL;  // the word to be search 
                          
	suchart = g_strdup(gtk_entry_get_text( GTK_ENTRY (GTK_COMBO (select)->entry)));	
	match   = g_strdup(gtk_entry_get_text( GTK_ENTRY(entry) ));

	on_statusbar_cleanup(widget);
                                                                                                       
    /* check for valid chars */
	if( (strchr(match, '&')) // ||
        // (strchr(match, '%'))||
        // (strchr(match, '$')) ||
        // (strchr(match, '/')) ||
        // (strchr(match, '(')) ||
        // (strchr(match, ')')) ||
        // (strchr(match, '=')) ||
        // (strchr(match, '#')) ||
        // (strchr(match, '\"'))||
        // (strchr(match, '!')) ||
        /* (strchr(match, 'Paragraph')) || */
        /* (strchr(match, 'quote')) || */
        // (strchr(match, '\'')) 
		) {
			// show errormessage 
            on_statusbar_print(widget, _(" Please insert only characters !"));
			g_free(suchart);
			g_free(match);
            return;
	}
	
                                                                                                       
    /* call function by user selection if valid input is there */
    if (strlen(gtk_entry_get_text(GTK_ENTRY(entry))) > 1) {
		
		set_model_char(MAIN_LATEST_DICT, suchart);
	
		
    	if (!gtk_window_get_resizable(GTK_WINDOW(win)))      on_window_unhide(win);

		wordlist = g_list_append (wordlist, g_strdup( gtk_entry_get_text(GTK_ENTRY(entry)) ) );
		wordlist = g_list_first(wordlist);
		if ( ((gint)g_list_length(wordlist)) > 100 ) {
			wordlist = g_list_first(wordlist);
			gchar *foo = wordlist->data;
			wordlist = g_list_remove(wordlist, foo);
			g_free(foo);
		}
		wordlist = g_list_last(wordlist);
		GtkWidget *prev_button = lookup_widget( widget, "button_back");
		GtkWidget *sub_button =  lookup_widget( widget, "button_submit");
		gtk_widget_set_sensitive (GTK_WIDGET(prev_button), TRUE);
		
		/*
		gdk_threads_enter();
		GdkCursor *my_cursor;
		my_cursor = gdk_cursor_new( GDK_WATCH );
		gdk_window_set_cursor ( (lookup_widget(widget, "ding_mainwin"))->window, my_cursor);
		gtk_widget_set_sensitive( sub_button, FALSE);
		gtk_main_iteration(); 
		gdk_threads_leave();
		*/ 
		gtk_widget_set_sensitive( sub_button, FALSE);
		while (gtk_events_pending () ) gtk_main_iteration ();

        if (SPELLIT_MACRO(_("Spellcheck (German)"), "de_DE") 
		} else if (SPELLIT_MACRO( _("Spellcheck (Afrikaans)"), "af" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Bulgarian)"), "bg" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Breton)"), "br" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Catalan)"), "ca" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Czech)"), "cs" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Welsh)"), "cy" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Danish)"), "da" )
		} else if (SPELLIT_MACRO( _("Spellcheck (German, Swiss)"), "de_CH" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Esperanto)"), "eo" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Greek)"), "el" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Canadian)"), "en_CA" )
		} else if (SPELLIT_MACRO( _("Spellcheck (American)"), "en_US" )
		} else if (SPELLIT_MACRO( _("Spellcheck (British)"), "en_UK")
		} else if (SPELLIT_MACRO( _("Spellcheck (Spanish)"), "es" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Faroese)"), "fo" )
		} else if (SPELLIT_MACRO( _("Spellcheck (French, France)"), "fr_FR" )
		} else if (SPELLIT_MACRO( _("Spellcheck (French, Swiss)"), "fr_CH" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Irish, Gaelic)"), "ga" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Gallegan)"), "gl" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Croatian)"), "hr" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Indonesian)"), "id" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Icelandic)"), "is" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Italian)"), "it" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Maori)"), "mi" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Makasar)"), "mk" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Malay)"), "ms" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Maltese)"), "mt" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Dutch)"), "nl" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Norwegian)"), "no" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Polish)"), "pl" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Portuguese)"), "pt" )
		// } else if (SPELLIT_MACRO( _("Spellcheck (Portuguese, BR)"), "pt_BR" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Russian)"), "ru" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Romanian)"), "ro" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Slovak)"), "sk" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Slovenian)"), "sl" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Swedish)"), "sv" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Ukrainian)"), "uk" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Walloon)"), "wa" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Zulu)"), "zu" )
		
		/* more aspell languages to be test */
		
		} else if ( THESAURUS_MACRO(THES_DE, _("Thesaurus (German)")) 
		} else if ( THESAURUS_MACRO(THES_ES, _("Thesaurus (Spanish)")) 
		} else if ( THESAURUS_MACRO(THES_FR, _("Thesaurus (French)")) 
		} else if ( THESAURUS_MACRO(THES_EN, _("Thesaurus (English)")) 
		} else if ( THESAURUS_MACRO(THES_PT, _("Thesaurus (Portuguese)")) 
		} else if ( THESAURUS_MACRO(THES_IT, _("Thesaurus (Italian)")) 
		} else if ( THESAURUS_MACRO(THES_PL, _("Thesaurus (Polish)")) 
		
		} else if ( TRANSLATION_MACRO( _("Latin <--> German") , _("Latin"), _("German") , DICT_LAT_DE ) 
		} else if ( TRANSLATION_MACRO( _("English <--> Latin") , _("English"), _("Latin") , DICT_EN_LAT ) 
		
		} else if ( TRANSLATION_MACRO( _("German <--> English"), _("German"), _("English"), DICT_DE_EN )
		} else if ( TRANSLATION_MACRO( _("German <--> Spanish"), _("German"), _("Spanish"), DICT_DE_ES )
		} else if ( TRANSLATION_MACRO( _("German <--> French"), _("German"), _("French"), DICT_DE_FR )
		} else if ( TRANSLATION_MACRO( _("German <--> Italian"), _("German"), _("Italian"), DICT_DE_IT )
		} else if ( TRANSLATION_MACRO( _("German <--> Portuguese"), _("German"), _("Portuguese"), DICT_DE_PT )
		} else if ( TRANSLATION_MACRO( _("German <--> Dutch"), _("German"), _("Dutch"), DICT_DE_NL )
		
		} else if ( TRANSLATION_MACRO( _("English <--> German"), _("English"), _("German"), DICT_EN_DE )
		} else if ( TRANSLATION_MACRO( _("English <--> Spanish"), _("English"), _("Spanish"), DICT_EN_ES )
		} else if ( TRANSLATION_MACRO( _("English <--> Italian"), _("English"), _("Italian"), DICT_EN_IT )
		} else if ( TRANSLATION_MACRO( _("English <--> French"), _("English"), _("French"), DICT_EN_FR )
		} else if ( TRANSLATION_MACRO( _("English <--> Portuguese"), _("English"), _("Portuguese"), DICT_EN_PT )
		} else if ( TRANSLATION_MACRO( _("English <--> Norwegian"), _("English"), _("Norwegian"), DICT_EN_NO )
		} else if ( TRANSLATION_MACRO( _("English <--> Arabic"), _("English"), _("Arabic"), DICT_EN_AR )
		} else if ( TRANSLATION_MACRO( _("English <--> Swedish"), _("English"), _("Swedish"), DICT_EN_SV )
		
		} else if ( TRANSLATION_MACRO( _("French <--> German"), _("French"), _("German"), DICT_FR_DE )
		} else if ( TRANSLATION_MACRO( _("French <--> English"), _("French"), _("English"), DICT_FR_EN )
		} else if ( TRANSLATION_MACRO( _("French <--> Spanish"), _("French"), _("Spanish"), DICT_FR_ES )
		} else if ( TRANSLATION_MACRO( _("French <--> Italian"), _("French"), _("Italian"), DICT_FR_IT )
		} else if ( TRANSLATION_MACRO( _("French <--> Portuguese"), _("French"), _("Portuguese"), DICT_FR_PT )
		
		} else if ( TRANSLATION_MACRO( _("Spanish <--> German"), _("Spanish"), _("German"), DICT_ES_DE )
		} else if ( TRANSLATION_MACRO( _("Spanish <--> English"), _("Spanish"), _("English"), DICT_ES_EN )
		} else if ( TRANSLATION_MACRO( _("Spanish <--> French"), _("Spanish"), _("French"), DICT_ES_FR )
		} else if ( TRANSLATION_MACRO( _("Spanish <--> Italian"), _("Spanish"), _("Italian"), DICT_ES_IT )
		} else if ( TRANSLATION_MACRO( _("Spanish <--> Portuguese"), _("Spanish"), _("Portuguese"), DICT_ES_PT )
		
		} else if ( TRANSLATION_MACRO( _("Italian <--> German"), _("Italian"), _("German"), DICT_IT_DE )
		} else if ( TRANSLATION_MACRO( _("Italian <--> English"), _("Italian"), _("English"), DICT_IT_EN )
		} else if ( TRANSLATION_MACRO( _("Italian <--> Spanish"), _("Italian"), _("Spanish"), DICT_IT_ES )
		} else if ( TRANSLATION_MACRO( _("Italian <--> Italian"), _("Italian"), _("French"), DICT_IT_FR )
		} else if ( TRANSLATION_MACRO( _("Italian <--> Portuguese"), _("Italian"), _("Portuguese"), DICT_IT_PT )
		
		} else if ( TRANSLATION_MACRO( _("Norwegian <--> English"), _("Norwegian"), _("English"), DICT_NO_EN )
		} else if ( get_debug() ) {
			g_print("unknown selection in combo box\n");
		}
		/*
		my_cursor = gdk_cursor_new( GDK_LAST_CURSOR );
		gdk_window_set_cursor ( (lookup_widget(widget, "ding_mainwin"))->window, my_cursor);
		*/
		gtk_widget_set_sensitive (GTK_WIDGET(sub_button), TRUE);
		
	} else {
		/* else print error to statusbar */
        on_statusbar_print(widget, _("Your entry is to short!") );
	}
	
	
	
	g_free(suchart);
	g_free(match);
}


/**
 *
 * @doc a helper function to fill the combo box of the main window
 *     with the dictionaries that the user prefere to use.
 *     This function is called on startup and on close a preferences
 *     dialog.
 *
 * @parameter mainwin - a pointer to the main window
 *
 * @return void 
 *
**/
void on_search_combobox_fill( GtkWidget *mainwin ) {
	if (mainwin == NULL) return;
	GtkWidget *entry;
	GList *combo_entry = NULL;
	entry  = lookup_widget(GTK_WIDGET(mainwin), "selection");

	COMBO_MACRO2(DICT_DE_EN, _("German <--> English") )
	COMBO_MACRO2(DICT_DE_ES, _("German <--> Spanish") )
	COMBO_MACRO2(DICT_DE_IT, _("German <--> Italian") )
	COMBO_MACRO2(DICT_DE_FR, _("German <--> French") )
	COMBO_MACRO2(DICT_DE_PT, _("German <--> Portuguese") )
	COMBO_MACRO2(DICT_DE_NL, _("German <--> Dutch") )
	
	COMBO_MACRO2(DICT_EN_DE, _("English <--> German") )
	COMBO_MACRO2(DICT_EN_ES, _("English <--> Spanish") )
	COMBO_MACRO2(DICT_EN_PT, _("English <--> Portuguese") )
	COMBO_MACRO2(DICT_EN_FR, _("English <--> French") )
	COMBO_MACRO2(DICT_EN_IT, _("English <--> Italian") )
	COMBO_MACRO2(DICT_EN_NO, _("English <--> Norwegian") )
	COMBO_MACRO2(DICT_EN_AR, _("English <--> Arabic") )
	COMBO_MACRO2(DICT_EN_SV, _("English <--> Swedish") )
	
	COMBO_MACRO2(DICT_FR_EN, _("French <--> English") )
	COMBO_MACRO2(DICT_FR_ES, _("French <--> Spanish") )
	COMBO_MACRO2(DICT_FR_IT, _("French <--> Italian") )
	COMBO_MACRO2(DICT_FR_DE, _("French <--> German") )
	COMBO_MACRO2(DICT_FR_PT, _("French <--> Portuguese") )
	
	COMBO_MACRO2(DICT_ES_EN, _("Spanish <--> English") )
	COMBO_MACRO2(DICT_ES_FR, _("Spanish <--> French") )
	COMBO_MACRO2(DICT_ES_IT, _("Spanish <--> Italian") )
	COMBO_MACRO2(DICT_ES_DE, _("Spanish <--> German") )
	COMBO_MACRO2(DICT_ES_PT, _("Spanish <--> Portuguese") )
	
	COMBO_MACRO2(DICT_IT_EN, _("Italian <--> English") )
	COMBO_MACRO2(DICT_IT_ES, _("Italian <--> Spanish") )
	COMBO_MACRO2(DICT_IT_FR, _("Italian <--> French") )
	COMBO_MACRO2(DICT_IT_DE, _("Italian <--> German") )
	COMBO_MACRO2(DICT_IT_PT, _("Italian <--> Portuguese") )
	
	COMBO_MACRO2(DICT_NO_EN, _("Norwegian <--> English") )
	COMBO_MACRO2(DICT_LAT_DE, _("Latin <--> German") )
	COMBO_MACRO2(DICT_EN_LAT, _("English <--> Latin") )
	
	COMBO_MACRO2(THES_DE, _("Thesaurus (German)") )
	COMBO_MACRO2(THES_ES, _("Thesaurus (Spanish)") ) 
	COMBO_MACRO2(THES_EN, _("Thesaurus (English)") )
	COMBO_MACRO2(THES_IT, _("Thesaurus (Italian)") ) 
	COMBO_MACRO2(THES_FR, _("Thesaurus (French)") )
	COMBO_MACRO2(THES_PT, _("Thesaurus (Portuguese)") )
	COMBO_MACRO2(THES_PT, _("Thesaurus (Polish)") )
	
#ifdef USE_ASPELL
	AspellConfig *spell_config;
	AspellDictInfoList * dlist;
  	AspellDictInfoEnumeration * dels;
  	const AspellDictInfo * entryd;
	
	spell_config = new_aspell_config();
	dlist = get_aspell_dict_info_list(spell_config);
	
	dels = aspell_dict_info_list_elements(dlist);

  	while ( (entryd = aspell_dict_info_enumeration_next(dels)) != 0) {
		if( get_debug() ) {
			g_print("aspell language available: '%s'\n",entryd->name); 
		}
    	if ( COMBO_MACRO(ASPELL_DE, "de_DE", _("Spellcheck (German)") )
		} else if (COMBO_MACRO(ASPELL_AF, "af", _("Spellcheck (Afrikaans)") )
		} else if (COMBO_MACRO(ASPELL_BG, "bg", _("Spellcheck (Bulgarian)") )
		} else if (COMBO_MACRO(ASPELL_BR, "br", _("Spellcheck (Breton)") )
		} else if (COMBO_MACRO(ASPELL_CA, "ca", _("Spellcheck (Catalan)") )
		} else if (COMBO_MACRO(ASPELL_CS, "cs", _("Spellcheck (Czech)") )
		} else if (COMBO_MACRO(ASPELL_CY, "cy", _("Spellcheck (Welsh)") )
		} else if (COMBO_MACRO(ASPELL_DA, "da", _("Spellcheck (Danish)") )
		} else if (COMBO_MACRO(ASPELL_CH, "de_CH", _("Spellcheck (German, Swiss)") )
		} else if (COMBO_MACRO(ASPELL_GB, "en_GB", _("Spellcheck (British)") )
		} else if (COMBO_MACRO(ASPELL_UC, "en_CA", _("Spellcheck (Canadian)") )
		} else if (COMBO_MACRO(ASPELL_US, "en_US", _("Spellcheck (American)") )
		} else if (COMBO_MACRO(ASPELL_EO, "eo", _("Spellcheck (Esperanto)") )
		} else if (COMBO_MACRO(ASPELL_EL, "el", _("Spellcheck (Greek)") )
		} else if (COMBO_MACRO(ASPELL_ES, "es", _("Spellcheck (Spanish)") )
		} else if (COMBO_MACRO(ASPELL_FO, "fo", _("Spellcheck (Faroese)") )
		} else if (COMBO_MACRO(ASPELL_FR, "fr_FR", _("Spellcheck (French, France)") )
		} else if (COMBO_MACRO(ASPELL_FRCH, "fr_CH", _("Spellcheck (French, Swiss)") ) 
		} else if (COMBO_MACRO(ASPELL_GA, "ga", _("Spellcheck (Irish, Gaelic)") )
		} else if (COMBO_MACRO(ASPELL_GL, "gl", _("Spellcheck (Gallegan)") )
		} else if (COMBO_MACRO(ASPELL_HR, "hr", _("Spellcheck (Croatian)") )
		} else if (COMBO_MACRO(ASPELL_ID, "id", _("Spellcheck (Indonesian)") )
		} else if (COMBO_MACRO(ASPELL_IT, "it", _("Spellcheck (Italian)") )
		} else if (COMBO_MACRO(ASPELL_IS, "is", _("Spellcheck (Icelandic)") )
		} else if (COMBO_MACRO(ASPELL_MI, "mi", _("Spellcheck (Maori)") )
		} else if (COMBO_MACRO(ASPELL_MK, "mk", _("Spellcheck (Makasar)") )
		} else if (COMBO_MACRO(ASPELL_MS, "ms", _("Spellcheck (Malay)") )
		} else if (COMBO_MACRO(ASPELL_MT, "mt", _("Spellcheck (Maltese)") )
		} else if (COMBO_MACRO(ASPELL_NL, "nl", _("Spellcheck (Dutch)") )
		} else if (COMBO_MACRO(ASPELL_NO, "no", _("Spellcheck (Norwegian)") )
		} else if (COMBO_MACRO(ASPELL_PL, "pl", _("Spellcheck (Polish)") )
		} else if (COMBO_MACRO(ASPELL_PT, "pt", _("Spellcheck (Portuguese)") )
		// } else if (COMBO_MACRO(ASPELL_PT_BR, "pt_BR", _("Spellcheck (Portuguese, BR)") ) 
		} else if (COMBO_MACRO(ASPELL_RU, "ru", _("Spellcheck (Russian)") )
		} else if (COMBO_MACRO(ASPELL_RO, "ro", _("Spellcheck (Romanian)") )
		} else if (COMBO_MACRO(ASPELL_SV, "sv", _("Spellcheck (Swedish)") )
		} else if (COMBO_MACRO(ASPELL_SL, "sl", _("Spellcheck (Slovenian)") )
		} else if (COMBO_MACRO(ASPELL_SK, "sk", _("Spellcheck (Slovak)") )
		} else if (COMBO_MACRO(ASPELL_UK, "uk", _("Spellcheck (Ukrainian)") )
		} else if (COMBO_MACRO(ASPELL_WA, "wa", _("Spellcheck (Walloon)") )
		} else if (COMBO_MACRO(ASPELL_ZU, "zu", _("Spellcheck (Zulu)") )
		} 
  	}
	delete_aspell_config(spell_config);
   	delete_aspell_dict_info_enumeration(dels);
	
#endif // USE_ASPELL
	gtk_combo_set_popdown_strings (GTK_COMBO (entry), combo_entry);
	
	// restore predefine
	// if failed, then (let it) set to the first
	GList *helper = combo_entry;
	gchar *latest_dict = get_model_char(MAIN_LATEST_DICT);
	while ( helper != NULL ) {
		if ( !strcmp (latest_dict, (gchar*)helper->data) ) {
			g_object_set( GTK_COMBO(entry)->entry, "text", latest_dict, NULL);	
		}
		helper = helper->next;
	}
	g_list_free (combo_entry);
}

/**
 * 
 * @doc helper function - set the user defined font for the list view
 *
 * @parameter widget - a pointer to the main window
 * 
 * @return void 
 *
**/
void on_treeview_set_font(GtkWidget *widget) {
	GtkWidget *view = lookup_widget(widget, "list");
	if( get_model_bool( MAIN_FONT_DEFAULT) == TRUE ) {
		// use theme font
		GtkRcStyle *rc_style;
		rc_style = gtk_widget_get_modifier_style (view);
		if (rc_style->font_desc) {
			pango_font_description_free (rc_style->font_desc);
		}
		rc_style->font_desc = NULL;
		gtk_widget_modify_style (view, rc_style);
	} else {
		//	 use own font
		PangoFontDescription *font_desc = NULL;
		gchar *fontname = NULL;
		fontname = get_model_char(MAIN_FONT);
		if ( fontname != NULL) {
			font_desc = pango_font_description_from_string (get_model_char(MAIN_FONT));
			gtk_widget_modify_font (view, font_desc);
			pango_font_description_free (font_desc);
		} // if 
	}
}


/**
 *
 * @doc a helper function to set the text color of the list view 
 *
 * @parameter widget - a pointer to the main window
 *
 * @return void 
 *
**/
void on_treeview_set_color1(GtkWidget * widget)
{
	GtkWidget *view = lookup_widget (widget, "list");
	if( get_model_bool(MAIN_COLOR_DEFAULT) == TRUE ) {
		GtkRcStyle *rc_style;
		GdkColor color;
		rc_style = gtk_widget_get_modifier_style (view);
		rc_style->color_flags[GTK_STATE_NORMAL] = 0;
		rc_style->color_flags[GTK_STATE_SELECTED] = 0;
		rc_style->color_flags[GTK_STATE_ACTIVE] = 0;
		gtk_widget_modify_style (view, rc_style);
		
		set_model_char(MAIN_COLOR2, "#00000000ffff");
	} else {
		GdkColor color;
		gdk_color_parse (get_model_char(MAIN_COLOR1), &color);
		gtk_widget_modify_text (view, GTK_STATE_NORMAL, &color);
	}
} // on_treeview_set_color1


/**
 *
 * @doc gtk-callback - called, if inside the preferences dialog 
 *     the user hav toggled the button 'use default font'
 *     If enable, then disable the fontpicker.
 *
 * @parameter togglebutton - a pointer to the widget 
 *
 * @parameter user_data  - unused 
 *
 * @return void 
 *
**/
void
on_pref_font_default_toggled           (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	GtkWidget *font_sel = NULL;
	gboolean value = FALSE;
	font_sel = lookup_widget(GTK_WIDGET(togglebutton), "fontpicker1" );
	
	value = gtk_toggle_button_get_active ( togglebutton );
	set_model_bool(MAIN_FONT_DEFAULT , value );
	
	if( value == TRUE ) {
		// disable fontpicker
		gtk_widget_set_sensitive (font_sel, FALSE);
	} else {
		// enable fontpicker
		gtk_widget_set_sensitive (font_sel, TRUE);
	}
} // on_pref_font_default_toggled


/**
 *
 * @doc gtk-callback - called, if inside the preferences dialog 
 *     the user have toggled the button 'use theme color'
 *     If enable, then disable the colorpicker1 and colorpicker2.
 *
 * @parameter togglebutton - a pointer to the widget 
 *
 * @parameter user_data  - unused 
 *
 * @return void 
 *
**/
void
on_pref_default_color_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	GtkWidget *color1_sel = NULL;
	GtkWidget *color2_sel = NULL;
	gboolean value = FALSE;
	color1_sel = lookup_widget( GTK_WIDGET(togglebutton) , "colorpicker1" );
	color2_sel = lookup_widget( GTK_WIDGET(togglebutton) , "colorpicker2" );
	
	value = gtk_toggle_button_get_active ( togglebutton );
	set_model_bool(MAIN_COLOR_DEFAULT , value );
	
	if( value == TRUE ) {
		// disable colorpicker
		gtk_widget_set_sensitive (color1_sel, FALSE);
		gtk_widget_set_sensitive (color2_sel, FALSE);
	} else {
		// enable colorpicker
		gtk_widget_set_sensitive (color1_sel, TRUE);
		gtk_widget_set_sensitive (color2_sel, TRUE);
	}
} // on_pref_default_color_toggled



/**
 * 
 * @doc gtk-callback - called from the list view if the cursor change
 *      then we store back a current setting to prepare contex copy.
 *
 * @parameter treeview - a pointer to the listview 
 *
 * @parameter user_data - unused 
 * 
 * @return void - inside the internal data model, the strings
 *        column1 and column2 are filled
 *
**/
void on_list_cursor_changed(GtkTreeView *treeview, gpointer user_data) {
	GtkTreeIter iter;
	GtkTreeModel *model = gtk_tree_view_get_model (treeview);
	GtkTreeSelection *selection = gtk_tree_view_get_selection (treeview);
	if (gtk_tree_selection_get_selected (selection, NULL, &iter)) {
        gint i;
 		GtkTreePath *path;
		GString colum1;
		GString colum2;
		path = gtk_tree_model_get_path (model, &iter);
        i = gtk_tree_path_get_indices (path)[0];
 
		if ( get_model_int(MAIN_COLUMN_NUM) == 1 ) {
			gtk_tree_model_get( model, &iter,
                        0, &colum1,
                        -1 );
			set_model_char(MAIN_COLUMN1, colum1.str);
			set_model_char(MAIN_COLUMN2, "");
		} else {
			gtk_tree_model_get( model, &iter,
                        0, &colum1,
		  				1, &colum2,
                        -1 );
			set_model_char(MAIN_COLUMN1, colum1.str);
			set_model_char(MAIN_COLUMN2, colum2.str);
		}
		
		gtk_tree_path_free(path);
	} // if
}

// kill all format-tags from  content
// we have global vars 'clumn1' and 'column2'
/**
 * @doc helper function - called to prepare the copy from to 
 *      listview content to the clipboard. To do this, we strip
 *      out from the string all pango-descriptions
 *
 * @parameter value - define, if we use column1 (left) or 
 *          column2 (right)
 *
 * @return void 
 *
**/
void on_contex_prepare(gint value) {
	gchar *help = NULL;
	gchar *myout = NULL;
	gint i = 0;
	gint j = 0;
	gint mynext = 0;
	gsize mylen = 0;
	if ( value == 1) {
		help = g_strdup( get_model_char(MAIN_COLUMN1) );
	} else {
		help = g_strdup( get_model_char(MAIN_COLUMN2) );
	}
	mylen = strlen(help);
	myout = g_strnfill(mylen, 0x0);
	for( i=0; i<(gint)mylen; i++) {
		if ( ( mynext == 0 ) && (help[i] == '<')  ) {
			// begin tag  (i, span)
			mynext = ( (help[i+1] == 's') || (help[i+2] == 's') ) ? 2 : 1 ;  // <span>-tag
		} else if ( ( mynext == 2 ) && (help[i] == '>') )  {
			// last tag (I hope)
			mynext = 0;
		} else if ( ( mynext == 1 ) && (help[i] == '>') )  {
			// first tag
			mynext = 2;
		} else if ( ( mynext == 0 ) && (
			(help[i] == '|') || (help[i] == '[' ) || (help[i] == ']' )
			) ) {
			// do nothing
		}  else if ( ( mynext == 0 ) && (help[i] == '\r') ) {
			myout[j] = '*';
			j++;
		}  else if ( ( ( i == 0 ) || ( i == ((gint)mylen - 1)) ) && (help[i] == ' ') ) {
			// kill it
		} else if (mynext == 0) {
			myout[j] = help[i];
			j++;
		} else {
			// nothing
		}
	}
	// copy back
	if ( value == 1) {
		set_model_char(MAIN_COLUMN1, myout);
	} else {
		set_model_char(MAIN_COLUMN2, myout);
	}
	if( get_model_bool(MAIN_DEBUG) ) {
		g_print("copy prepare result '%s'\n", myout);
	}
	g_free(help);
	g_free(myout);
}


/**
 *
 * @doc gtk-callback - called form main menu or from list-view-contex-menu
 *     to copy the right cell part from the listview column to the clipboard
 *
 * @parameter menuitem - pointer to the menu widget 
 *
 * @parameter user_data - unused
 *
 * @return void 
 *
**/
void on_contex_copy_r(GtkMenuItem *menuitem, gpointer user_data) {
	GtkClipboard *clipboard = NULL;
	on_contex_prepare(2);
	clipboard = gtk_clipboard_get (GDK_NONE);
 	gtk_clipboard_set_text(clipboard, get_model_char(MAIN_COLUMN2), strlen(get_model_char(MAIN_COLUMN2)) );
} 


/**
 *
 * @doc gtk-callback - called form main menu or from list-view-contex-menu
 *     to copy the left cell part from the listview column to the clipboard
 *
 * @parameter menuitem - pointer to the menu widget 
 *
 * @parameter user_data - unused
 *
 * @return void 
 *
**/
void on_contex_copy_l(GtkMenuItem *menuitem, gpointer user_data) {
	GtkClipboard *clipboard = NULL;
	on_contex_prepare(1);
	clipboard = gtk_clipboard_get (GDK_NONE);
 	gtk_clipboard_set_text(clipboard, get_model_char(MAIN_COLUMN1), strlen(get_model_char(MAIN_COLUMN1)) );
} 


// build contex-menu
/**
 * 
 * @doc gtk-callback - called if a button event is catch from the
 *      list view.
 *      If the left button is pressed, then create a contex menu
 *      for copy the cell entry 
 *      This function create the contex menu, if we have two 
 *      columns then we create a second menu entry for it.
 *      Every contex menuitem become a callback connection.
 *
 * @parameter widget - pointer to the listview widget 
 *
 * @parameter event - contain the button event contex 
 * 
 * @parameter user_data - unused 
 *
 * @return void 
 *
**/
gboolean
on_list_button_press_event             (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
	if ( event->button == 3) {
		// right mouse click
		GtkWidget *menu = NULL;
		GnomeUIInfo contex2[] = {
			/* GNOMEUIINFO_ITEM_DATA will not work proper*/
			GNOMEUIINFO_ITEM( N_("Copy left column"), NULL, on_contex_copy_l, NULL ), 
			GNOMEUIINFO_ITEM( N_("Copy right column"), NULL, on_contex_copy_r, NULL ),
			GNOMEUIINFO_ITEM( N_("As new search word"), NULL, on_menu_search_word_activate, NULL ),
			GNOMEUIINFO_END
		};
		GnomeUIInfo contex1[] = {
			/* GNOMEUIINFO_ITEM_DATA will not work proper*/
			GNOMEUIINFO_ITEM( N_("Copy column"), NULL, on_contex_copy_l, NULL ), 
			GNOMEUIINFO_ITEM( N_("As new search word"), NULL, on_menu_search_word_activate, NULL ),
			GNOMEUIINFO_END
		};
		if ( get_model_int(MAIN_COLUMN_NUM) == 1) {
			menu = gnome_popup_menu_new(contex1);
		} else {
			menu = gnome_popup_menu_new(contex2);
		}
		gnome_popup_menu_do_popup(menu, NULL, NULL, NULL, NULL, widget);
	}
  return FALSE;
}


/**
 *
 * @doc helper function - enabel/disable sensitive
 * 
 * @parameter widget - a pointer to the main window
 * 
 * @return void 
 *
**/
void on_contex_menu_show(GtkWidget *widget) {
	GtkWidget *menu1 = NULL;
	GtkWidget *menu2 = NULL;	
	menu1 = lookup_widget(widget, "on_menu_copy_left");
	menu2 = lookup_widget(widget, "on_menu_copy_right");
	
	if ( get_model_int(MAIN_COLUMN_NUM) > 0 ) {
		if ( get_model_int(MAIN_COLUMN_NUM) == 1 ) {
			gtk_widget_set_sensitive(menu1, TRUE);
			gtk_widget_set_sensitive(menu2, FALSE);
		} else {
			gtk_widget_set_sensitive(menu1, TRUE);
			gtk_widget_set_sensitive(menu2, TRUE);
		}
	} else {
		gtk_widget_set_sensitive(menu1, FALSE);
		gtk_widget_set_sensitive(menu2, FALSE);
	}
}


/**
 *
 * @doc gtk-callback - called from main menu, open preferences 
 *      dialog
 *
 * @parameter menuitem - pointer to the menu widget 
 *
 * @parameter user_data - unused
 *
 * @return void
 *
**/
void
on_menu_preferences_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	if ( pref_dlg == NULL ) {
		pref_dlg = create_preferences1();
		GtkWidget *case_sense = NULL;
		GtkWidget *exact_word = NULL;
		
		GtkWidget *aspell_sug = NULL;
		GtkWidget *aspell_char = NULL;
		
		GtkWidget *tree_spell = NULL;
		GtkWidget *tree_thes = NULL;
		GtkWidget *tree_dict = NULL;
		GtkListStore *store_aspell;
		GtkListStore *store_thes;
		GtkListStore *store_dict;
  		GtkTreeIter iter_aspell;
		GtkTreeIter iter_thes;
		GtkTreeIter iter_dict;
 		GtkCellRenderer *textrenderer = NULL;
		GtkCellRenderer *textrenderer2 = NULL;
		GtkCellRenderer *textrenderer3 = NULL;
		GtkCellRenderer *togglerenderer = NULL;
		GtkCellRenderer *togglerenderer2 = NULL;
		GtkCellRenderer *togglerenderer3 = NULL;
		GtkTreeViewColumn *sp_ASPELL_TOG, *sp_ASPELL_LANG, *sp_ASPELL_INT;
		GtkTreeViewColumn *sp_THES_TOG, *sp_THES_LANG, *sp_THES_INT;
		GtkTreeViewColumn *sp_DICT_TOG, *sp_DICT_LANG, *sp_DICT_INT;
		gint i = 0;
		GdkColor color;
		GtkWidget *text_font = NULL;
		GtkWidget *text_color1 = NULL;
		GtkWidget *text_color2 = NULL;
		GtkWidget *text_font_def = NULL;
		GtkWidget *text_color_def = NULL;
		struct stat my_stat;

		case_sense = lookup_widget(pref_dlg, "grep_case");
		exact_word = lookup_widget(pref_dlg, "grep_word");
		aspell_sug  = lookup_widget(pref_dlg, "aspell_suggest");
		aspell_char = lookup_widget(pref_dlg, "aspell_charset");
		
		tree_spell = lookup_widget( pref_dlg, "treeview_aspell");
		tree_thes = lookup_widget( pref_dlg, "treeview_thes");
		tree_dict = lookup_widget( pref_dlg, "treeview_dict");
		
		text_font_def = lookup_widget( pref_dlg, "pref_font_default");
		text_color_def = lookup_widget( pref_dlg, "pref_default_color");
		
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (case_sense), get_model_bool(MAIN_CASE) );
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (exact_word), get_model_bool(MAIN_EXACT) );
	
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (text_font_def), get_model_bool(MAIN_FONT_DEFAULT) );
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (text_color_def), get_model_bool(MAIN_COLOR_DEFAULT) );
		
		gtk_adjustment_set_value(gtk_range_get_adjustment( GTK_RANGE(aspell_sug )),
			(gdouble)(get_model_int(MAIN_SUGEST_MODE) - 1 ) );
		
#if !USE_ASPELL		
	// disable view 
	g_object_set( GTK_LABEL(lookup_widget(tree_spell, "label16")) , "visible", FALSE, NULL);
	g_object_set( lookup_widget(tree_spell, "scrolledwindow_aspell") , "visible", FALSE, NULL);
#else
	// fill aspell treeview
		store_aspell = gtk_list_store_new (NUM_ASPELL_COLUMNS,
			      G_TYPE_BOOLEAN,
			      G_TYPE_STRING,
				  G_TYPE_UINT);
		gtk_list_store_clear( store_aspell );
		
		AspellConfig *spell_config;
		AspellDictInfoList * dlist;
  		AspellDictInfoEnumeration * dels;
  		const AspellDictInfo * entryd;
	
		spell_config = new_aspell_config();
		dlist = get_aspell_dict_info_list(spell_config);
	
		dels = aspell_dict_info_list_elements(dlist);
		
		while ( (entryd = aspell_dict_info_enumeration_next(dels)) != 0) {
			if( get_debug() ) {
				g_print("check for %s\n", entryd->name);
			}
		
			if( PREF_SPELL_MACRO( ASPELL_DE, "de_DE", _("German (German)") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_CH, "de_CH", _("German (Swiss)") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_AF, "af", _("Afrikaans") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_BG, "bg", _("Bulgarian") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_BR, "br", _("Breton") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_CA, "ca", _("Catalan") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_CS, "cs", _("Czech") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_CY, "cy", _("Welsh") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_DA, "da", _("Danish") )
			} else if (PREF_SPELL_MACRO( ASPELL_EO, "eo", _("Esperanto") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_DA, "el", _("Greek") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_GB, "en_GB", _("English (British)") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_CA, "en_CA", _("English (Canadian)") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_US, "en_US", _("English (American)") )
			} else if (PREF_SPELL_MACRO( ASPELL_ES, "es", _("Spanish") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_FR, "fr_FR", _("French, France") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_FRCH, "fr_CH", _("French, Swiss") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_FO, "fo", _("Faroese") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_GA, "ga", _("Irish, Gaelic") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_GL, "gl", _("Gallegan") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_HR, "hr", _("Croatian") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_ID, "id", _("Indonesian") )
			} else if (PREF_SPELL_MACRO( ASPELL_IT, "it", _("Italian") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_IS, "is", _("Icelandic") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_MI, "mi", _("Maori") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_MK, "mk", _("Makasar") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_MS, "ms", _("Malay") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_MT, "mt", _("Maltese") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_NL, "nl", _("Dutch") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_NO, "no", _("Norwegian") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_PT, "pt", _("Portuguese") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_PL, "pl", _("Polish") ) 
			/* } else if (PREF_SPELL_MACRO( ASPELL_GA, "pt_BR", _("Portuguese") )  */
			} else if (PREF_SPELL_MACRO( ASPELL_RU, "ru", _("Russian") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_RO, "ro", _("Romanian") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_SL, "sl", _("Slovenian") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_SK, "sk", _("Slovak") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_SV, "sv", _("Swedish") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_SV, "uk", _("Ukrainian") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_WA, "wa", _("Walloon") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_ZU, "zu", _("Zulu") ) 
			}
		
		}
			
		g_object_set (GTK_TREE_VIEW(tree_spell), "model", store_aspell, NULL);
		gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (tree_spell), TRUE);
      	gtk_tree_view_set_search_column (GTK_TREE_VIEW (tree_spell),
				       COLUMN_ASPELL_DESCRIPTION);
					   
		textrenderer = gtk_cell_renderer_text_new ();
		togglerenderer = gtk_cell_renderer_toggle_new ();
		g_signal_connect (togglerenderer, "toggled", G_CALLBACK (on_pref_spell_toggle), store_aspell);
		
		sp_ASPELL_TOG  = gtk_tree_view_column_new_with_attributes( _("active") , togglerenderer, "active", COLUMN_ASPELL_FIXED, NULL);
		sp_ASPELL_LANG = gtk_tree_view_column_new_with_attributes( _("Language") , textrenderer, "text", COLUMN_ASPELL_DESCRIPTION, NULL);
		sp_ASPELL_INT  = gtk_tree_view_column_new_with_attributes( _(" ") , textrenderer, "text", COLUMN_ASPELL_NUM, NULL);
		
		gtk_tree_view_column_set_sort_column_id (sp_ASPELL_TOG, COLUMN_ASPELL_FIXED);
        gtk_tree_view_append_column(GTK_TREE_VIEW(tree_spell), sp_ASPELL_TOG);
		
		gtk_tree_view_column_set_sort_column_id (sp_ASPELL_LANG, COLUMN_ASPELL_DESCRIPTION);
        gtk_tree_view_append_column( GTK_TREE_VIEW(tree_spell), sp_ASPELL_LANG);
		
		delete_aspell_config(spell_config);
   		delete_aspell_dict_info_enumeration(dels);
#endif // USE_ASPELL


		// now settings for thesaurus
		store_thes = gtk_list_store_new (NUM_THES_COLUMNS,
			      G_TYPE_BOOLEAN,
			      G_TYPE_STRING,
				  G_TYPE_UINT);
		gtk_list_store_clear( store_thes );
		
		
		if( PREF_THES_MACRO( THES_DE, _("German") ) 	} 
		if (PREF_THES_MACRO( THES_ES, _("Spanish") ) 	} 
		if (PREF_THES_MACRO( THES_EN, _("English") ) 	} 
		if (PREF_THES_MACRO( THES_FR, _("French") )  	} 
		if (PREF_THES_MACRO( THES_IT, _("Italian") )  	}
		if (PREF_THES_MACRO( THES_PT, _("Portuguese") )  	}
		if (PREF_THES_MACRO( THES_PL, _("Polish") )  	}

		g_object_set (GTK_TREE_VIEW(tree_thes), "model", store_thes, NULL);
		gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (tree_thes), TRUE);
      	gtk_tree_view_set_search_column (GTK_TREE_VIEW (tree_thes),
				       COLUMN_THES_DESCRIPTION);
					   
		textrenderer2 = gtk_cell_renderer_text_new ();
		togglerenderer2 = gtk_cell_renderer_toggle_new ();
		g_signal_connect (togglerenderer2, "toggled", G_CALLBACK (on_pref_thes_toggle), store_thes);
		
		sp_THES_TOG  = gtk_tree_view_column_new_with_attributes( _("active") , togglerenderer2, "active", COLUMN_THES_FIXED, NULL);
		sp_THES_LANG = gtk_tree_view_column_new_with_attributes( _("Language") , textrenderer2, "text", COLUMN_THES_DESCRIPTION, NULL);
		sp_THES_INT  = gtk_tree_view_column_new_with_attributes( _(" ") , textrenderer2, "text", COLUMN_THES_NUM, NULL);
		
		gtk_tree_view_column_set_sort_column_id (sp_THES_TOG, COLUMN_THES_FIXED);
        gtk_tree_view_append_column(GTK_TREE_VIEW(tree_thes), sp_THES_TOG);
		
		gtk_tree_view_column_set_sort_column_id (sp_THES_LANG, COLUMN_THES_DESCRIPTION);
        gtk_tree_view_append_column( GTK_TREE_VIEW(tree_thes), sp_THES_LANG);

		// now settings for dict
		store_dict = gtk_list_store_new (NUM_DICT_COLUMNS,
			      G_TYPE_BOOLEAN,
			      G_TYPE_STRING,
				  G_TYPE_UINT);
		gtk_list_store_clear( store_dict );

		if( PREF_DICT_MACRO( DICT_DE_EN, _("German <--> English") )  } 
		if (PREF_DICT_MACRO( DICT_DE_ES, _("German <--> Spanish") ) 	} 
		if (PREF_DICT_MACRO( DICT_DE_FR, _("German <--> French") ) 	}
		if (PREF_DICT_MACRO( DICT_DE_PT, _("German <--> Portuguese") ) 	}
		if (PREF_DICT_MACRO( DICT_DE_IT, _("German <--> Italian") ) 	}
		if (PREF_DICT_MACRO( DICT_DE_NL, _("German <--> Dutch") ) 	}
		
		if (PREF_DICT_MACRO( DICT_EN_DE, _("English <--> German") ) 	}
		if (PREF_DICT_MACRO( DICT_EN_FR, _("English <--> French") ) 	}
		if (PREF_DICT_MACRO( DICT_EN_IT, _("English <--> Italian") ) 	}
		if (PREF_DICT_MACRO( DICT_EN_PT, _("English <--> Portuguese") ) 	}
		if (PREF_DICT_MACRO( DICT_EN_ES, _("English <--> Spanish") ) 	}
		if (PREF_DICT_MACRO( DICT_EN_NO, _("English <--> Norwegian") ) 	}
		if (PREF_DICT_MACRO( DICT_EN_AR, _("English <--> Arabic") ) 	}
		if (PREF_DICT_MACRO( DICT_EN_SV, _("English <--> Swedish") ) 	}
		
		if( PREF_DICT_MACRO( DICT_FR_EN, _("French <--> English") )  } 
		if (PREF_DICT_MACRO( DICT_FR_ES, _("French <--> Spanish") ) 	} 
		if (PREF_DICT_MACRO( DICT_FR_DE, _("French <--> German") ) 	}
		if (PREF_DICT_MACRO( DICT_FR_PT, _("French <--> Portuguese") ) 	}
		if (PREF_DICT_MACRO( DICT_FR_IT, _("French <--> Italian") ) 	}
		
		if( PREF_DICT_MACRO( DICT_ES_EN, _("Spanish <--> English") )  } 
		if (PREF_DICT_MACRO( DICT_ES_FR, _("Spanish <--> French") ) 	} 
		if (PREF_DICT_MACRO( DICT_ES_DE, _("Spanish <--> German") ) 	}
		if (PREF_DICT_MACRO( DICT_ES_PT, _("Spanish <--> Portuguese") ) 	}
		if (PREF_DICT_MACRO( DICT_ES_IT, _("Spanish <--> Italian") ) 	}
		
		if( PREF_DICT_MACRO( DICT_IT_EN, _("Italian <--> English") )  } 
		if (PREF_DICT_MACRO( DICT_IT_ES, _("Italian <--> Spanish") ) 	} 
		if (PREF_DICT_MACRO( DICT_IT_DE, _("Italian <--> German") ) 	}
		if (PREF_DICT_MACRO( DICT_IT_PT, _("Italian <--> Portuguese") ) 	}
		if (PREF_DICT_MACRO( DICT_IT_FR, _("Italian <--> French") ) 	}
		
		if (PREF_DICT_MACRO( DICT_NO_EN, _("Norwegian <--> English") ) 	}
		if (PREF_DICT_MACRO( DICT_LAT_DE, _("Latin <--> German") )   } 
		if (PREF_DICT_MACRO( DICT_EN_LAT, _("English <--> Latin") )   } 
		
		g_object_set (GTK_TREE_VIEW(tree_dict), "model", store_dict, NULL);
		gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (tree_dict), TRUE);
      	gtk_tree_view_set_search_column (GTK_TREE_VIEW (tree_dict),
				       COLUMN_DICT_DESCRIPTION);
					   
		textrenderer3 = gtk_cell_renderer_text_new ();
		togglerenderer3 = gtk_cell_renderer_toggle_new ();
		g_signal_connect (togglerenderer3, "toggled", G_CALLBACK (on_pref_dict_toggle), store_dict);
		
		sp_DICT_TOG  = gtk_tree_view_column_new_with_attributes( _("active") , togglerenderer3, "active", COLUMN_DICT_FIXED, NULL);
		sp_DICT_LANG = gtk_tree_view_column_new_with_attributes( _("Language") , textrenderer3, "text", COLUMN_DICT_DESCRIPTION, NULL);
		sp_DICT_INT  = gtk_tree_view_column_new_with_attributes( _(" ") , textrenderer3, "text", COLUMN_DICT_NUM, NULL);
		
		gtk_tree_view_column_set_sort_column_id (sp_DICT_TOG, COLUMN_DICT_FIXED);
        gtk_tree_view_append_column(GTK_TREE_VIEW(tree_dict), sp_DICT_TOG);
		
		gtk_tree_view_column_set_sort_column_id (sp_DICT_LANG, COLUMN_DICT_DESCRIPTION);
        gtk_tree_view_append_column( GTK_TREE_VIEW(tree_dict), sp_DICT_LANG);

		// set color and font
		text_color1 = lookup_widget(pref_dlg, "colorpicker1");
		text_color2 = lookup_widget(pref_dlg, "colorpicker2");
		text_font   = lookup_widget(pref_dlg, "fontpicker1");
		
		gdk_color_parse (get_model_char(MAIN_COLOR1), &color);
		gnome_color_picker_set_i16 (GNOME_COLOR_PICKER (text_color1),
				    color.red, color.green, color.blue, 0);
		gdk_color_parse (get_model_char(MAIN_COLOR2), &color);
		gnome_color_picker_set_i16 (GNOME_COLOR_PICKER (text_color2),
				    color.red, color.green, color.blue, 0);
				
		if ( get_model_char(MAIN_FONT) != NULL) {
			gnome_font_picker_set_font_name (
				GNOME_FONT_PICKER(text_font),
				get_model_char(MAIN_FONT) );
		}
	
		gtk_widget_show(pref_dlg);
	}
}


/**
 *
 * @doc gtk-callback - called if the user start the search,
 *      prepare new menusettings (centex copy entry)
 *      and insert word into the history list 
 *
 * @parameter menuitem - pointer to the menu widget 
 *
 * @parameter user_data - unused 
 *
**/
void
on_menu_search_word_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	if( menuitem == NULL) {
		return;
	}
	GtkWidget *entry = NULL;
	GtkWidget *mainwin = NULL;
	mainwin = get_mainwin();
	if ( mainwin != NULL) { 
		entry = lookup_widget( mainwin, "search_input");
        
		on_contex_prepare(1);
	
		if ( strlen(get_model_char(MAIN_COLUMN1)) > 1 ) {
			gtk_entry_set_text(GTK_ENTRY(entry), "");	
			gtk_entry_set_text(GTK_ENTRY(entry), get_model_char(MAIN_COLUMN1));	
		}
	}
}
