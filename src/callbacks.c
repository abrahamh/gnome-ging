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

static void on_spell_toggle(GtkCellRendererToggle *cell, gchar *path_str, gpointer data);
static void on_thes_toggle(GtkCellRendererToggle *cell, gchar *path_str, gpointer data);
static void on_dict_toggle(GtkCellRendererToggle *cell, gchar *path_str, gpointer data);

	 
GList *wordlist;
GtkWidget *pref_dlg = NULL;


/**
 * init wordlist-history for (prev/next search string  
**/
void 
on_wordlist_init()
{
	wordlist = g_list_append (wordlist, g_strdup( "" ) );
	wordlist = g_list_last(wordlist);
}



/**
 * delete wordlist-history for search-string 
**/
void 
on_wordlist_delete()
{
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




/* history back/forward for search-sting */
void 
on_go_back(GtkWidget *widget)
{
	if (wordlist != NULL ) {
		GtkWidget * entry = NULL;
		GtkWidget *prev_button = NULL;
		GtkWidget *next_button = NULL;
		
		entry = lookup_widget(widget, "suchinput");
		prev_button = lookup_widget( widget, "button1");
		next_button = lookup_widget( widget, "button_vor");
		
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

/* history back/forward for search-sting */
void 
on_go_forward(GtkWidget *widget)
{
	if( wordlist != NULL) {
		GtkWidget * entry = NULL;
		GtkWidget *prev_button = NULL;
		GtkWidget *next_button = NULL;
		entry = lookup_widget(widget, "suchinput");
		prev_button = lookup_widget( widget, "button1");
		next_button = lookup_widget( widget, "button_vor");
	
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
 * get the widgets to restore
 **/
void
unhide (GtkWidget      *widget)
{
	if ( widget == NULL ) return;
		
	GtkWidget *list;
	GtkWidget *menu;
	GtkWidget *status;
	GtkWidget *win;
	
	list = lookup_widget(GTK_WIDGET(widget), "vbox7");
    menu = lookup_widget(GTK_WIDGET(widget), "menubar1");
    status = lookup_widget(GTK_WIDGET(widget), "statusbar");
    win = lookup_widget(GTK_WIDGET(widget), "gnome_ding");
                                                                                                       
	/* restore the win */
	if( list != NULL) gtk_widget_show(list);
	if (menu != NULL) gtk_widget_show(menu);
	if (status != NULL) gtk_widget_show(status);
	if (win != NULL) gtk_window_resize(GTK_WINDOW(win), get_model_int(MAIN_SIZE_X), get_model_int(MAIN_SIZE_Y) );
	if (win != NULL) gtk_window_set_resizable(GTK_WINDOW(win), 1);
} // unhide



/**
 * get the widgets and set vars
**/
void
showtable (GtkWidget *widget, GtkListStore *store, 
			gint col_num, gint basewidth, gchar *hdr, gchar *hdr2)
{
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
 * print message for user information
 **/
void
print_to_status                         (GtkWidget      *parent,
                                         gchar           *msg)
{
	if (parent == NULL) return;
	if (msg == NULL) return;
	GtkWidget *statusbar;
	statusbar = lookup_widget(GTK_WIDGET(parent), "statusbar");	
        
    gtk_statusbar_push(GTK_STATUSBAR(statusbar),
           	gtk_statusbar_get_context_id( 
			GTK_STATUSBAR(statusbar), "err" ) , msg);
	
}


/**
 * cleanup statusbar
**/
void
cleanup_status  (GtkWidget      *parent)
{
	if (parent == NULL) return;
	GtkWidget       *statusbar; 
                                                                                                      
    statusbar = lookup_widget(GTK_WIDGET(parent), "statusbar");                                                                                               
    gtk_statusbar_push(GTK_STATUSBAR(statusbar),
                        gtk_statusbar_get_context_id(
                        GTK_STATUSBAR(statusbar), "err") , " ");
}



/**
 * use a ding dictinory to translate String(lang_from) ---> String(to)
 * 
 * @lang_from - language, form translate = string for tab-header
 * @lang_to   - language, to translate = string for tab-header
 * @dict - the filename of the uncompressed dict
**/
void
run_translation (GtkWidget *caller, gchar *lang_from, gchar *lang_to, gchar *dict)
{
	if( get_debug() ) {
		g_print("call translation '%s' '%s' <--> '%s'\n",
			dict, lang_from, lang_to);
		
	}
	
	if (caller == NULL) return;        
	if (lang_from == NULL) return;
	if (lang_to == NULL) return;
	if (dict == NULL) return;
                                                                                                       
    GtkWidget       *entry = lookup_widget(GTK_WIDGET(caller), "suchinput");
    GtkTreeIter     iter;
    GtkListStore    *store = NULL;
                                                                                                       
    /* initialize the vars */
	GString *cmd = g_string_new("");
	GString *grep_out = g_string_new("");
	char line[200];
	gchar *reste = NULL;
	FILE *ptr;
	gchar *msg = NULL;
    gchar *such = NULL;
	gint flag = 0;
	
    such  = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry) ) );
    store = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_STRING);                                                                                               
    
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
								out0 = g_string_append(out0, "<span foreground=\"darkblue\">");	
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
								out1 = g_string_append(out1, "<span foreground=\"darkblue\">");	
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
    	print_to_status(GTK_WIDGET(caller), msg);
	
	}
	g_free(msg);
	g_free(such);
	reste = g_string_free(grep_out, TRUE);
	reste = g_string_free(cmd, TRUE);		
 
}
		
		
		
		
/**
 * quit main program
 * before, save windows settings to GConf
 */
void
file_quit_activated                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	gint x,y;
	gtk_window_get_size( 
			GTK_WINDOW( lookup_widget( 
			GTK_WIDGET(menuitem), "gnome_ding") ) ,
			&x , &y );
	set_model_int(MAIN_SIZE_X, x);
	set_model_int(MAIN_SIZE_Y, y);
	gtk_main_quit ();
}

/**
 * paste the clipboard data to the input entry
**/
void
file_paste_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	if( menuitem == NULL) return;
	GtkWidget *entry;
	entry = lookup_widget(GTK_WIDGET(menuitem), "suchinput");
        
	gtk_entry_set_text(GTK_ENTRY(entry), " ");
    gtk_editable_paste_clipboard(GTK_EDITABLE(GTK_ENTRY(entry)));
    gtk_editable_select_region(GTK_EDITABLE(GTK_ENTRY(entry)), 0, -1);

}


/**
 *  show up the infodialog
**/
void
on_info1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GtkWidget *about_gnome_ding;
  about_gnome_ding = create_about_gnome_ding ();
  // gtk_window_set_transient_for( GTK_WINDOW(infodialog), GTK_WINDOW( get_mainwindow() ) );
  gtk_widget_show (about_gnome_ding);
}

/**
 * visual wait info
 * follow code is only for test, idee, make search-spider
**/
void *my_wait_thread (void *data)
{
	GtkWidget *button = (GtkWidget *)data;

  	pthread_setcancelstate (PTHREAD_CANCEL_ENABLE, NULL);
  	pthread_setcanceltype  (PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	
	while (1) {
    	g_print (".");
    	usleep(1000 * 500);
		/* for future		
		GdkPixbuf *myspider = NULL;
        if (myflag) {
			myspider = create_pixbuf( "gnome-ding/gnome-ding24-1.png");
		} else {
			myspider = create_pixbuf( "gnome-ding/gnome-ding24-2.png");
		}
		if (myflag) {
			myflag = FALSE;
		} else {
			myflag = TRUE;
		}
        g_object_set ( G_OBJECT(spider_w) , "pixbuf", myspider, NULL);
        gdk_pixbuf_unref( myspider );
		*/
		// gtk_main_iteration();
  	}
  	return NULL;
}


/** 
 * start search
**/
void
on_search(GtkWidget *widget)
{
 	GtkWidget *entry; 
    GtkWidget *select;
	GtkWidget *win;
	entry  = lookup_widget(widget, "suchinput");
	select = lookup_widget(widget, "selection");
	win    = lookup_widget(widget, "gnome_ding");

	/* get the user input */
    gchar *suchart = NULL;
    gchar *match   = NULL;
                          
	suchart = g_strdup(gtk_entry_get_text( GTK_ENTRY (GTK_COMBO (select)->entry)));	
	match   = g_strdup(gtk_entry_get_text( GTK_ENTRY(entry) ));

	cleanup_status(widget);
                                                                                                       
    /* check for valid chars */
	if( (strchr(match, '&')) ||
        (strchr(match, '%'))||
        (strchr(match, '$')) ||
        (strchr(match, '/')) ||
        (strchr(match, '(')) ||
        (strchr(match, ')')) ||
        (strchr(match, '=')) ||
        (strchr(match, '#')) ||
        (strchr(match, '\"'))||
        (strchr(match, '!')) ||
        /* (strchr(match, 'Paragraph')) || */
        (strchr(match, '`')) ||
        (strchr(match, '\'')) ) {
			// show errormessage 
            print_to_status(widget, _(" Please insert only characters !"));
			g_free(suchart);
			g_free(match);
            return;
	}
	
                                                                                                       
    /* call function by user selection if valid input is there */
    if (strlen(gtk_entry_get_text(GTK_ENTRY(entry))) > 1) {
		
    	if (!gtk_window_get_resizable(GTK_WINDOW(win)))      unhide(win);

		wordlist = g_list_append (wordlist, g_strdup( gtk_entry_get_text(GTK_ENTRY(entry)) ) );
		wordlist = g_list_first(wordlist);
		if ( ((gint)g_list_length(wordlist)) > 100 ) {
			wordlist = g_list_first(wordlist);
			gchar *foo = wordlist->data;
			wordlist = g_list_remove(wordlist, foo);
			g_free(foo);
		}
		wordlist = g_list_last(wordlist);
		GtkWidget *prev_button = lookup_widget( widget, "button1");
		GtkWidget *sub_button =  lookup_widget( widget, "submitbutton");
		gtk_widget_set_sensitive (GTK_WIDGET(prev_button), TRUE);
		
		gdk_threads_enter();
		GdkCursor *my_cursor;
		my_cursor = gdk_cursor_new( GDK_WATCH );
		gdk_window_set_cursor ( (lookup_widget(widget, "gnome_ding"))->window, my_cursor);
		gtk_widget_set_sensitive( sub_button, FALSE);
		gtk_main_iteration(); 
		gdk_threads_leave();
		
        if (SPELLIT_MACRO(_("Spellcheck (German)"), "de_DE") 
		} else if (SPELLIT_MACRO( _("Spellcheck (Canadian)"), "en_CA" )
		} else if (SPELLIT_MACRO( _("Spellcheck (American)"), "en_US" )
		} else if (SPELLIT_MACRO( _("Spellcheck (French)"), "fr" )
		} else if (SPELLIT_MACRO( _("Spellcheck (Spanish)"), "es" )
		} else if (SPELLIT_MACRO( _("Spellcheck (British)"), "en_UK")
		/* more aspell languages to be test */
		
		} else if ( THESAURUS_MACRO(THES_DE, _("Thesaurus (German)")) 
		} else if ( THESAURUS_MACRO(THES_ES, _("Thesaurus (Spanish)")) 
		} else if ( THESAURUS_MACRO(THES_FR, _("Thesaurus (French)")) 
		} else if ( THESAURUS_MACRO(THES_EN, _("Thesaurus (English)")) 
		} else if ( THESAURUS_MACRO(THES_PT, _("Thesaurus (Portuguese)")) 
		} else if ( THESAURUS_MACRO(THES_IT, _("Thesaurus (Italian)")) 
		
		} else if ( TRANSLATION_MACRO( _("Latin <--> German") , _("Latin"), _("German") , DICT_LAT_DE ) 
		
		} else if ( TRANSLATION_MACRO( _("German <--> English"), _("German"), _("English"), DICT_DE_EN )
		} else if ( TRANSLATION_MACRO( _("German <--> Spanish"), _("German"), _("Spanish"), DICT_DE_ES )
		} else if ( TRANSLATION_MACRO( _("German <--> French"), _("German"), _("French"), DICT_DE_FR )
		} else if ( TRANSLATION_MACRO( _("German <--> Italian"), _("German"), _("Italian"), DICT_DE_IT )
		} else if ( TRANSLATION_MACRO( _("German <--> Portuguese"), _("German"), _("Portuguese"), DICT_DE_PT )
		
		} else if ( TRANSLATION_MACRO( _("English <--> German"), _("English"), _("German"), DICT_EN_DE )
		} else if ( TRANSLATION_MACRO( _("English <--> Spanish"), _("English"), _("Spanish"), DICT_EN_ES )
		} else if ( TRANSLATION_MACRO( _("English <--> Italian"), _("English"), _("Italian"), DICT_EN_IT )
		} else if ( TRANSLATION_MACRO( _("English <--> French"), _("English"), _("French"), DICT_EN_FR )
		} else if ( TRANSLATION_MACRO( _("English <--> Portuguese"), _("English"), _("Portuguese"), DICT_EN_PT )
		} else if ( TRANSLATION_MACRO( _("English <--> Norwegian"), _("English"), _("Norwegian"), DICT_EN_NO )
		} else if ( TRANSLATION_MACRO( _("Norwegian <--> English"), _("Norwegian"), _("English"), DICT_NO_EN )
		} else if ( get_debug() ) {
			g_print("unknown selection in combo box\n");
		}
		
		my_cursor = gdk_cursor_new( GDK_LAST_CURSOR );
		gdk_window_set_cursor ( (lookup_widget(widget, "gnome_ding"))->window, my_cursor);
		gtk_widget_set_sensitive (GTK_WIDGET(sub_button), TRUE);
		
	} else {
		/* else print error to statusbar */
        print_to_status(widget, _("Your entry is to short!") );
	}
	
	
	
	g_free(suchart);
	g_free(match);
}

/**
 * submit search
**/
void
on_submitbutton_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
	on_search( GTK_WIDGET(button) );
}


void
fill_combo( GtkWidget *mainwin ) 
{
	if (mainwin == NULL) return;
	GtkWidget *entry;
	GList *combo_entry = NULL;
	entry  = lookup_widget(GTK_WIDGET(mainwin), "selection");

	COMBO_MACRO2(DICT_DE_EN, _("German <--> English") )
	COMBO_MACRO2(DICT_DE_ES, _("German <--> Spanish") )
	COMBO_MACRO2(DICT_DE_IT, _("German <--> Italian") )
	COMBO_MACRO2(DICT_DE_FR, _("German <--> French") )
	COMBO_MACRO2(DICT_DE_PT, _("German <--> Portuguese") )
	
	COMBO_MACRO2(DICT_EN_DE, _("English <--> German") )
	COMBO_MACRO2(DICT_EN_ES, _("English <--> Spanish") )
	COMBO_MACRO2(DICT_EN_PT, _("English <--> Portuguese") )
	COMBO_MACRO2(DICT_EN_FR, _("English <--> French") )
	COMBO_MACRO2(DICT_EN_IT, _("English <--> Italian") )
	COMBO_MACRO2(DICT_EN_NO, _("English <--> Norwegian") )
	
	COMBO_MACRO2(DICT_NO_EN, _("Norwegian <--> English") )
	COMBO_MACRO2(DICT_LAT_DE, _("Latin <--> German") )
	
	COMBO_MACRO2(THES_DE, _("Thesaurus (German)") )
	COMBO_MACRO2(THES_ES, _("Thesaurus (Spanish)") ) 
	COMBO_MACRO2(THES_EN, _("Thesaurus (English)") )
	COMBO_MACRO2(THES_IT, _("Thesaurus (Italian)") ) 
	COMBO_MACRO2(THES_FR, _("Thesaurus (French)") )
	COMBO_MACRO2(THES_PT, _("Thesaurus (Portuguese)") )


	
	
#ifdef USE_ASPELL
	AspellConfig *spell_config;
	AspellDictInfoList * dlist;
  	AspellDictInfoEnumeration * dels;
  	const AspellDictInfo * entryd;
	
	spell_config = new_aspell_config();
	dlist = get_aspell_dict_info_list(spell_config);
	
	dels = aspell_dict_info_list_elements(dlist);

		
	/*
	possible languages:
	de, de_CH, de_DE
	en, en_CA, en_CA-w-accents, en_GB, en_GB-w-accents, en_US, en_US-w-accents
	es
	fr, fr_CH, fr_FR
af     Afrikaans
bg     Bulgarian
br     Breton
ca     Catalan/Valencian
cs     Czech
cy     Welsh
da     Danish
el     Greek
eo     Esperanto
fo     Faroese
ga     Irish
gl     Gallegan
hr     Croatian
it     Italian
nl     Dutch
pl     Polish
pt     Portuguese
ro     Romanian
ru     Russian
sk     Slovak
sl     Slovenian
sv     Swedish
uk     Ukrainian
wa     Walloon
zu     Zulu
	
	*/
	
	
  	while ( (entryd = aspell_dict_info_enumeration_next(dels)) != 0) {
		if( get_debug() ) {
			g_print("aspell language available: %s\n",entryd->name); 
		}
    	if ( COMBO_MACRO(ASPELL_DE, "de_DE", _("Spellcheck (German)") )
		} else if (COMBO_MACRO(ASPELL_DECH, "de_CH", _("Spellcheck (Swiss, German)") )
		} else if (COMBO_MACRO(ASPELL_GB, "en_GB", _("Spellcheck (British)") )
		} else if (COMBO_MACRO(ASPELL_UC, "en_CA", _("Spellcheck (Canadian)") )
		} else if (COMBO_MACRO(ASPELL_US, "en_US", _("Spellcheck (American)") )
		} else if (COMBO_MACRO(ASPELL_GA, "ga", _("Spellcheck (Irish)") )
		} else if (COMBO_MACRO(ASPELL_FR, "fr_FR", _("Spellcheck (French)") )
		/* } else if (COMBO_MACRO(ASPELL_FRCH, "fr_CH", _("Spellcheck (Swiss, French)") ) */
		} else if (COMBO_MACRO(ASPELL_ES, "es", _("Spellcheck (Spanish)") )
		} else if (COMBO_MACRO(ASPELL_IT, "it", _("Spellcheck (Italian)") )
		} else if (COMBO_MACRO(ASPELL_DA, "da", _("Spellcheck (Danish)") )
		} else if (COMBO_MACRO(ASPELL_NL, "nl", _("Spellcheck (Dutch)") )
		} else if (COMBO_MACRO(ASPELL_NO, "no", _("Spellcheck (Norwegian)") )
		} else if (COMBO_MACRO(ASPELL_PT, "pt", _("Spellcheck (Portuguese)") ) /*pt versus pt_BR, check this */
		} else if (COMBO_MACRO(ASPELL_SV, "sv", _("Spellcheck (Swedish)") )
		/* follow parts are untested */
		} else if (COMBO_MACRO(ASPELL_BG, "bg", _("Spellcheck (Bulgarian)") )
		} else if (COMBO_MACRO(ASPELL_EL, "el", _("Spellcheck (Greek)") )
		} else if (COMBO_MACRO(ASPELL_HR, "hr", _("Spellcheck (Croatian)") )
		} else if (COMBO_MACRO(ASPELL_IS, "is", _("Spellcheck (Icelandic)") )
		} else if (COMBO_MACRO(ASPELL_PT, "pt", _("Spellcheck (Portuguese)") )
		} else if (COMBO_MACRO(ASPELL_RU, "ru", _("Spellcheck (Russian)") )
		} 
  	}
	
	delete_aspell_config(spell_config);
   	delete_aspell_dict_info_enumeration(dels);
	
#endif // USE_ASPELL

	gtk_combo_set_popdown_strings (GTK_COMBO (entry), combo_entry);
    g_list_free (combo_entry);

	
}




/**
 * hide/unhide the mainwindow if the logo is clicked
**/
gboolean
on_eventbox1_button_press_event        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
	GtkWidget *list;
	GtkWidget *menu;
	GtkWidget *status;
	GtkWidget *win;
                                                                                         
    list = lookup_widget(GTK_WIDGET(widget), "vbox7");
    menu = lookup_widget(GTK_WIDGET(widget), "menubar1");
    status = lookup_widget(GTK_WIDGET(widget), "statusbar");
    win = lookup_widget(GTK_WIDGET(widget), "gnome_ding");
	if (win != NULL) {
		if (gtk_window_get_resizable(GTK_WINDOW(win))) {
			if( list != NULL) gtk_widget_hide(list);
			if( menu != NULL) gtk_widget_hide(menu);
			if (status != NULL) gtk_widget_hide(status);
			gtk_window_resize(GTK_WINDOW(win), get_model_int(MAIN_SIZE_X), 40);
			if( win != NULL) gtk_window_set_resizable(GTK_WINDOW(win), 0);
		} else {
			unhide(widget);
		}
	}
  	return FALSE;
}



/* history back/forward for search-sting */
void
on_button_back_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
	on_go_back(GTK_WIDGET(button)); 
}


/* history back/forward for search-sting */
void
on_button_vor_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
	on_go_forward(GTK_WIDGET(button));
}


/* history back/forward for search-sting */
void
on_back1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	on_go_back(GTK_WIDGET(menuitem));
}

/* history back/forward for search-sting */
void
on_forward1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	on_go_forward(GTK_WIDGET(menuitem));
}


/**
 * start serach form menu
**/
void
on_search1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	on_search( GTK_WIDGET(menuitem) );
}

/**
 * dialog-window preferences for search 
**/
void
on_preferences1_activate               (GtkMenuItem     *menuitem,
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
		struct stat my_stat;
		
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
		
		case_sense = lookup_widget(pref_dlg, "grep_case");
		exact_word = lookup_widget(pref_dlg, "grep_word");
		aspell_sug  = lookup_widget(pref_dlg, "aspell_suggest");
		aspell_char = lookup_widget(pref_dlg, "aspell_charset");
		
		tree_spell = lookup_widget( pref_dlg, "treeview_aspell");
		tree_thes = lookup_widget( pref_dlg, "treeview_thes");
		tree_dict = lookup_widget( pref_dlg, "treeview_dict");
		
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (case_sense), get_model_bool(MAIN_CASE) );
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (exact_word), get_model_bool(MAIN_EXACT) );
	
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
			g_print("check for %s\n", entryd->name);
		
			if( PREF_SPELL_MACRO( ASPELL_DE, "de_DE", _("German (German)") ) 
			/* } else if (PREF_SPELL_MACRO( ASPELL_DECH, "de_CH", _("German (Swiss)") )  */
			} else if (PREF_SPELL_MACRO( ASPELL_GB, "en_GB", _("English (British)") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_CA, "en_CA", _("English (Canadian)") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_US, "en_US", _("English (American)") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_GA, "ga", _("Irish") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_FR, "fr_FR", _("French") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_ES, "es", _("Spanish") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_IT, "it", _("Italian") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_DA, "da", _("Danish") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_NL, "nl", _("Dutch") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_NO, "no", _("Norwegian") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_SV, "sv", _("Swedish") ) 
			} else if (PREF_SPELL_MACRO( ASPELL_BR, "br", _("Portuguese") ) 
			}
		
		}
			
		g_object_set (GTK_TREE_VIEW(tree_spell), "model", store_aspell, NULL);
		gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (tree_spell), TRUE);
      	gtk_tree_view_set_search_column (GTK_TREE_VIEW (tree_spell),
				       COLUMN_ASPELL_DESCRIPTION);
					   
		textrenderer = gtk_cell_renderer_text_new ();
		togglerenderer = gtk_cell_renderer_toggle_new ();
		g_signal_connect (togglerenderer, "toggled", G_CALLBACK (on_spell_toggle), store_aspell);
		
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

		g_object_set (GTK_TREE_VIEW(tree_thes), "model", store_thes, NULL);
		gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (tree_thes), TRUE);
      	gtk_tree_view_set_search_column (GTK_TREE_VIEW (tree_thes),
				       COLUMN_THES_DESCRIPTION);
					   
		textrenderer2 = gtk_cell_renderer_text_new ();
		togglerenderer2 = gtk_cell_renderer_toggle_new ();
		g_signal_connect (togglerenderer2, "toggled", G_CALLBACK (on_thes_toggle), store_thes);
		
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
		
		if (PREF_DICT_MACRO( DICT_EN_DE, _("English <--> German") ) 	}
		if (PREF_DICT_MACRO( DICT_EN_FR, _("English <--> French") ) 	}
		if (PREF_DICT_MACRO( DICT_EN_IT, _("English <--> Italian") ) 	}
		if (PREF_DICT_MACRO( DICT_EN_PT, _("English <--> Portuguese") ) 	}
		if (PREF_DICT_MACRO( DICT_EN_ES, _("English <--> Spanish") ) 	}
		if (PREF_DICT_MACRO( DICT_EN_NO, _("English <--> Norwegian") ) 	}
				
		if (PREF_DICT_MACRO( DICT_NO_EN, _("Norwegian <--> English") ) 	}
		if (PREF_DICT_MACRO( DICT_LAT_DE, _("Latin <--> German") )   } 
		
		g_object_set (GTK_TREE_VIEW(tree_dict), "model", store_dict, NULL);
		gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (tree_dict), TRUE);
      	gtk_tree_view_set_search_column (GTK_TREE_VIEW (tree_dict),
				       COLUMN_DICT_DESCRIPTION);
					   
		textrenderer3 = gtk_cell_renderer_text_new ();
		togglerenderer3 = gtk_cell_renderer_toggle_new ();
		g_signal_connect (togglerenderer3, "toggled", G_CALLBACK (on_dict_toggle), store_dict);
		
		sp_DICT_TOG  = gtk_tree_view_column_new_with_attributes( _("active") , togglerenderer3, "active", COLUMN_DICT_FIXED, NULL);
		sp_DICT_LANG = gtk_tree_view_column_new_with_attributes( _("Language") , textrenderer3, "text", COLUMN_DICT_DESCRIPTION, NULL);
		sp_DICT_INT  = gtk_tree_view_column_new_with_attributes( _(" ") , textrenderer3, "text", COLUMN_DICT_NUM, NULL);
		
		gtk_tree_view_column_set_sort_column_id (sp_DICT_TOG, COLUMN_DICT_FIXED);
        gtk_tree_view_append_column(GTK_TREE_VIEW(tree_dict), sp_DICT_TOG);
		
		gtk_tree_view_column_set_sort_column_id (sp_DICT_LANG, COLUMN_DICT_DESCRIPTION);
        gtk_tree_view_append_column( GTK_TREE_VIEW(tree_dict), sp_DICT_LANG);

		gtk_widget_show(pref_dlg);
	}
}

static void
on_spell_toggle(GtkCellRendererToggle *cell, gchar *path_str, gpointer data) {
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
		
  /* get toggled iter */
  gtk_tree_model_get_iter (model, &iter, path);
  gtk_tree_model_get (model, &iter, 
  	COLUMN_ASPELL_FIXED, &fixed, 
  	COLUMN_ASPELL_NUM, &i,
  -1);

  fixed ^= 1;
  set_model_bool(i, fixed);

  gtk_list_store_set (GTK_LIST_STORE (model), &iter, COLUMN_ASPELL_FIXED, fixed, -1);
  gtk_tree_path_free (path);
}

static void
on_thes_toggle(GtkCellRendererToggle *cell, gchar *path_str, gpointer data) {
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

  fixed ^= 1;
  set_model_bool(i, fixed);

  gtk_list_store_set (GTK_LIST_STORE (model), &iter, COLUMN_THES_FIXED, fixed, -1);
  gtk_tree_path_free (path);
}

static void
on_dict_toggle(GtkCellRendererToggle *cell, gchar *path_str, gpointer data) {
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
		
  /* get toggled iter */
  gtk_tree_model_get_iter (model, &iter, path);
  gtk_tree_model_get (model, &iter, 
  	COLUMN_DICT_FIXED, &fixed, 
  	COLUMN_DICT_NUM, &i,
  -1);

  fixed ^= 1;
  set_model_bool(i, fixed);

  gtk_list_store_set (GTK_LIST_STORE (model), &iter, COLUMN_DICT_FIXED, fixed, -1);
  gtk_tree_path_free (path);
}


/**
 * switch windows to smaller display size
**/
void
on_mini_mode1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget *list;
	GtkWidget *menu;
	GtkWidget *status;
	GtkWidget *win;

    list = lookup_widget(GTK_WIDGET(menuitem), "output");
    menu = lookup_widget(GTK_WIDGET(menuitem), "menubar1");
    status = lookup_widget(GTK_WIDGET(menuitem), "statusbar");
    win = lookup_widget(GTK_WIDGET(menuitem), "gnome_ding");

	if (gtk_window_get_resizable(GTK_WINDOW(win))) {
		gtk_widget_hide(list);
		gtk_widget_hide(menu);
		gtk_widget_hide(status);
		gtk_window_resize(GTK_WINDOW(win), get_model_int(MAIN_SIZE_X), 40);
		gtk_window_set_resizable(GTK_WINDOW(win), 0);
	}
}

/**
 * switch windows to normal display size
**/
void
on_window_mode1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
   unhide(GTK_WIDGET(menuitem) );
}

/**
 * only close the prefereces-dialog, settings are global
**/
void
on_preferences1_close                  (GtkDialog       *dialog,
                                        gpointer         user_data)
{
	gtk_widget_destroy(pref_dlg);
	pref_dlg = NULL;
}

/**
 * only close the prefereces-dialog
**/
void
on_preferences1_destroy                (GtkObject       *object,
                                        gpointer         user_data)
{
	gtk_widget_destroy(pref_dlg);
	pref_dlg = NULL;
}

/**
 * close the prefereces-dialog and save back language settings
**/
void
on_okbutton1_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{
	
	gtk_widget_destroy(pref_dlg);
	pref_dlg = NULL;
	
	fill_combo( get_mainwin() );
} // on_okbutton1_clicked (preferences dialog)


/**
 * preferences-dialog : grep with/without exactly-word-match
**/
void
on_grep_word_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	GtkWidget *exact_word = NULL;
	exact_word = lookup_widget(pref_dlg, "grep_word");
	set_model_bool(MAIN_EXACT, gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (exact_word) ) );
}

/**
 * preferences-dialog : grep with/without case-sensitive
**/
void
on_grep_case_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	GtkWidget *case_sense = NULL;
	case_sense = lookup_widget(pref_dlg, "grep_case");
	set_model_bool(MAIN_CASE , gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (case_sense) ) );
}

/**
 * preferences-dialog, scale for suggestion-mode (fast...bad-speller)
**/
void
on_aspell_suggest_value_changed        (GtkRange        *range,
                                        gpointer         user_data)
{
	GtkAdjustment *adj;
	adj = gtk_range_get_adjustment( range );
	set_model_int(MAIN_SUGEST_MODE,  1 + (gint)( gtk_adjustment_get_value(adj) ));
}


/**
 * count entries in dictionary 
  less de-en.ding | grep -v '^#' | sed 's/:.*$//' | sed 's/|/\n/p' | wc -l
**/


/* delete this later
void dummy() {
	
	char *languages = {
	// "ab", _("Abkhazian"),
	// "ae", _("Avestan"), 
	"af", _("Afrikaans"),
	// "an", -("Aragonese"),
	// "ar", _("Arabic"), 
	// "as", _("Assamese"),
	// "ay",  _("Aymara"),  - unused
	// "az",  _("Azerbaijani"),  - unused
	// "ba",  _("Bashkir"), - unused
	// "be",  _("Belarusian"), - unused
	"bg",  _("Bulgarian"),
	// "bh",  _("Bihari"), - unused
	// "bn",  _("Bengali"), - unused
	// "bo",  _("Tibetan"), - unused
	"br",  _("Breton"),
	// "bs",  _("Bosnian"), - unused
	"ca",  _("Catalan/Valencian"),
	// "ce",  _("Chechen"), - unused
	"dech",  _("Swiss/German"),
	// "co",  _("Corsican"),  - unused
	// "cr",  _("Cree"), - unused
	"cs",  _("Czech"),
	"cy",  _("Welsh"),
	"da",  _("Danish"),
	"de",  _("German"),
	// "dv",  _("Divehi"), - unused
	// "dz",  _("Dzongkha"), - unused
	"el",  _("Greek"),
	"en",  _("English"),
	"eo",  _("Esperanto"),
	"es",  _("Spanish"),
	// "et",  _("Estonian"), - unused
	// "eu",  _("Basque"),  - unused
	// "fa",  _("Persian"), - unused
	// "fi",  _("Finnish"), - unused
	// "fj",  _("Fijian"),  - unused
	"fo",  _("Faroese"),
	"fr",  _("France"),
	// "fy",  _("Frisian"), - unused
	"ga",  _("Irish"),
	"gb",  _("English"), //  great brit.
	// "gd",  _("Scottish Gaelic"),  - unused
	"gl",  _("Gallegan"),
	// "gn",  _("Guarani"), - unused
	// "gu",  _("Gujarati"), - unused
	// "gv",  _("Manx"), - unused
	// "ha",  _("Hausa"),  - unused
	// "he",  _("Hebrew"), - unused
	// "hi",  _("Hindi"), - unused
	"hr",  _("Croatian"),
	// "hu",  _("Hungarian"), - unused
	// "hy",  _("Armenian"), - unused
	// "ia",  _("Interlingua"), - unused
	// "id",  _("Indonesian"), - unused
	// "io",  _("Ido"), - unused
	"is",  _("Icelandic"),
	"it",  _("Italian"),
	// "iu",  _("Inuktitut"), - unused
	// "ja",  -("Japanese"), - unused
	// "ka",  _("Georgian"), - unused
	// "kk",  _("Kazakh"), - unused
	// "kl",  _("Kalaallisut/Greenlandic"), - unused
	// "km",  _("Khmer"), - unused
	// "kn",  _("Kannada"), - unused
	// "ko",  _("Korean"),  - unused
	// "kr",  _("Kanuri"),  - unused
	// "ks",  _("Kashmiri"), - unused
	// "ku",  _("Kurdish"),  - unused
	// "kv",  _("Komi"),  - unused
	// "kw",  _("Cornish"),  - unused
	// "ky",  _("Kirghiz"), - unused
	// "la",  _("Latin"), - unused
	// "lo",  _("Lao"), - unused
	// "lt",  _("Lithuanian"), - unused
	// "lv",  _("Latvian"), - unused
	// "mi",  _("Maori"), - unused
	// "mk",  _("Makasar"), - unused
	// "ml",  _("Malayalam"), - unused
	// "mn",  _("Mongolian"), - unused
	// "mo",  _("Moldavian"), - unused
	// "mr",  _("Marathi"), - unused
	// "ms",  _("Malay"), - unused
	// "mt",  _("Maltese"), - unused
	// "my",  _("Burmese"), - unused
	// "nb",  _("Norwegian Bokmal"), - unused
	// "ne",  _("Nepali"), - unused
	"nl",  _("Dutch"),
	// "nn",  _("Norwegian Nynorsk"), - unused
	"no",  _("Norwegian"),
	// "nv",  _("Navajo"), - unused
	// "oc",  _("Occitan/Provencal"), - unused
	// "oj",  _("Ojibwa"), - unused
	// "or",  _("Oriya"), - unused
	// "os",  _("Ossetic"), - unused
	// "pa",  _("Punjabi"), - unused
	// "pi",  _("Pali"), - unused
	"pl",  _("Polish"),
	// "ps",  _("Pushto"),	- unused
	"pt",  _("Portuguese"),
	// "qu",  _("Quechua"), - unused
	// "rm",  _("Raeto-Romance"), - unused
	// "ro",  _("Romanian"),
	"ru",  _("Russian"),
	// "sa",  _("Sanskrit"), - unused
	// "sd",  _("Sindhi"), - unused
	// "se",  _("Northern Sami"), - unused
	"sk",  _("Slovak"),
	"sl",  _("Slovenian"),
	// "sn",  _("Shona"),  - unused
	// "so",  _("Somali"), - unused
	// "sq",  _("Albanian"), - unused
	// "sr",  _("Serbian"), - unused
	// "su",  _("Sundanese"), - unused
	"sv",  _("Swedish"),
	// "sw",  _("Swahili"),  - unused
	// "ta",  _("Tamil"), -unused
	// "te",  _("Telugu"), - unused
	// "tg",  _("Tajik"), - unused
	// "tk",  _("Turkmen"), - unused
	// "tl",  _("Tagalog"),  - unused
	// "tr",  _("Turkish"), - unused
	// "tt",  _("Tatar"), - unused
	// "ty",  _("Tahitian"), - unused
	"uc",  _("English/Canadian"),
	// "ug",  _("Uighur"), - unused
	"uk",  _("Ukrainian"),
	// "ur",  _("Urdu"), - unused
	"us",  _("English/American"),
	// "uz",  _("Uzbek"), - unused
	// "vi",  _("Vietnamese"), - unused
	// "vo",  _("Volapuk"), - unused
	"wa",  _("Walloon"),
	// "yi",  _("Yiddish"), - unused
	// "yo",  _("Yoruba"), - unused
	"zu",  _("Zulu")
	// "aa", _("Afar"),
	// "ak", _("Akan"),
	// "av", _("Avaric"),
	// "bi", _("Bislama"),
	// "bm", _("Bambara"),
	// "cu", _("Old Slavonic"),
	// "ee", _("Ewe"),
	// "ff", _("Fulah"),
	// "ho", _("Hiri Motu"),
	// "ht", _("Haitian Creole"),
	// "hz", _("Herero"),
	// "ie", _("Interlingue"),
	// "ig", _("Igbo"),
	// "ii", _("Sichuan Yi"),
	// "ik", _("Inupiaq"),
	// "kg", _("Kongo"),
	// "ki", _("Kikuyu/Gikuyu"),
	// "kj", _("Kwanyama"),
	// "lb", _("Luxembourgish"),
	// "lg", _("Ganda"),
	// "li", _("Limburgan"),
	// "ln", _("Lingala"),
	// "lu", _("Luba-Katanga"),
	// "mg", _("Malagasy"),
	// "mh", _("Marshallese"),
	// "na", _("Nauru"),
	// "nd", _("North Ndebele"),
	// "ng", _("Ndonga"),
	// "nr", _("South Ndebele"),
	// "ny", _("Nyanja"),
	// "rn", _("Rundi"),
	// "rw", _("Kinyarwanda"),
	// "sc", _("Sardinian"),
	// "sg", _("Sango"),
	// "si", _("Sinhalese"),
	// "sm", _("Samoan"),
	// "ss", _("Swati"),
	// "st", _("Southern Sotho"),
	// "tn", _("Tswana"),
	// "to", _("Tonga"),
	// "ts", _("Tsonga"),
	// "tw", _("Twi"),
	// "ve", _("Venda"),
	// "wo", _("Wolof"),
	// "xh", _("Xhosa"),
	// "za", _("Zhuang")
	}
	
}
*/

void
on_button_reset_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
	model_default();
	gtk_widget_destroy( lookup_widget( GTK_WIDGET(button), "preferences1") ); 
}
