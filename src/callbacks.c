#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gnome.h>


#ifdef USE_ASPELL
#  include <aspell.h>
#endif



#include <pthread.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "main.h"

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
	
	list = lookup_widget(GTK_WIDGET(widget), "output");
    menu = lookup_widget(GTK_WIDGET(widget), "menubar1");
    status = lookup_widget(GTK_WIDGET(widget), "statusbar");
    win = lookup_widget(GTK_WIDGET(widget), "gnome_ding");
                                                                                                       
	/* restore the win */
	gtk_widget_show(list);
	gtk_widget_show(menu);
	gtk_widget_show(status);
	gtk_window_resize(GTK_WINDOW(win), get_x_size(), get_y_size() );
	gtk_window_set_resizable(GTK_WINDOW(win), 1);
}



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
	// g_object_set (G_OBJECT (renderer), "foreground", "darkblue", NULL); // fixme: themes		
	g_object_set (G_OBJECT (column), "min-width", basewidth, "resizable", TRUE, NULL);
		
	if ( col_num > 1 ) {
		column = gtk_tree_view_column_new_with_attributes ( hdr2, renderer, "markup", 1, NULL);
		gtk_tree_view_column_set_sort_column_id(column, 1);
		gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview), column, 1);
		
		// g_object_set (G_OBJECT (renderer), "foreground", "darkblue", NULL); // fixme: themes		
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
 * run thesaurus search 
 *  
 * @parameter: caller - callback WidgetPointer
 * @parameter 'thes' - filename of the thesaurus
**/
void 
thesaurus(GtkWidget *caller, gchar *thes)
{
	if (caller == NULL ) return;
	if (thes == NULL) return;	
		
	GtkWidget       *entry = NULL;
	GtkWidget       *win = NULL;
	gchar *such = NULL;
	char line[200];
	gchar *reste;
	gchar *msg = NULL;
	GString *cmd = g_string_new("");
	GString *grep_out = g_string_new("");
	FILE *ptr;
	gint flag = 0;
	entry = lookup_widget(GTK_WIDGET(caller), "suchinput");
	win   = lookup_widget(GTK_WIDGET(caller), "gnome_ding");
	
	GtkListStore    *store = NULL;
	GtkTreeIter 	iter;
	
	store = gtk_list_store_new (1, G_TYPE_STRING);
	
	such  = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry) ));
	
	
	g_string_printf(cmd, "egrep -h -i -w '%s' '%s' | grep -v '^#'", 
	    /* (get_search_case_sense()  == FALSE) ? "-i" : " ", */
		/* (get_search_exact_words() == TRUE) ? "-w" : " ", */
		such, thes );
	
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
			g_string_printf(grep_out, _("Word not found or typo incorrect.\n <b>%s</b>"),  such );
			
			gtk_list_store_append (GTK_LIST_STORE(store), &iter);
    		gtk_list_store_set (GTK_LIST_STORE(store), &iter, 0,  grep_out->str, -1);
			
			gtk_list_store_append (GTK_LIST_STORE(store), &iter);
    		gtk_list_store_set (GTK_LIST_STORE(store), &iter, 0, " ", -1);
			
		} else {
			while ( strlen(grep_out->str) > 5 ) {
				gchar **result_line;
				gchar *reste = NULL;
				result_line = g_strsplit ( grep_out->str, "\n", 2);
				flag++;
				GString *out0 = g_string_sized_new( strlen(result_line[0] ) ) ;
       			gint i=0;
					
				gint st_len = strlen(result_line[0]);
				gint such_len = strlen(such);
				
				
				for(i=0; i < st_len; i++ ) {
                		if ( result_line[0][i] == '[' ) {
                        	out0 = g_string_append(out0, "[<i>");
                		} else if (result_line[0][i] == ']') {
							out0 = g_string_append(out0, "</i>]");
						} else if (result_line[0][i] == '{') {
							out0 = g_string_append(out0, "<i>");
						} else if (result_line[0][i] == '}') {
							out0 = g_string_append(out0, ".</i>");
						} else if (result_line[0][i] == ';') {
							out0 = g_string_append(out0, "\r");
						} else  {
							out0 = g_string_append_c(out0, result_line[0][i] );
						}
					}
					msg = g_strdup_printf( _("<b>Suggestion %i:</b>"), flag);
				
					gtk_list_store_append (GTK_LIST_STORE(store), &iter);
					gtk_list_store_set (GTK_LIST_STORE(store), &iter, 0,  msg, -1);
			
					gtk_list_store_append (GTK_LIST_STORE(store), &iter);
					gtk_list_store_set (GTK_LIST_STORE(store), &iter, 0, out0->str, -1);
					
					reste = g_string_free(out0, TRUE);
					grep_out = g_string_erase(grep_out, 0, strlen(result_line[0])+1 );
                    g_strfreev (result_line);
			}
		}
	}
	
	/* output */
	showtable(GTK_WIDGET(caller), store, 1, 200, _("Thesaurus (de)"), NULL);
    msg = g_strdup_printf( _("%i matches found to '%s'"), flag, such);
    print_to_status(GTK_WIDGET(caller), msg);
	
	reste = g_string_free(grep_out, TRUE);
	reste = g_string_free(cmd, TRUE);
	g_free(such);
	g_free(msg);
}



/**
 * @run spellchecker
 *
 * @parameter: caller - callback WidgetPointer
 * @parameter: lang   - chose language { ngerman, british, ... }
 *
 * @return: fill g_tree_list over callback-pointer with serach-results
**/
void
spellit(GtkWidget *caller, gchar *lang)
{
	if (caller == NULL ) return;
	if (lang == NULL) return;	
	
	GtkWidget       *entry = NULL;
	GtkWidget       *win = NULL;
	GtkListStore    *store = NULL;
	gchar *language = NULL;
	gchar *ok = NULL;
	gchar *sugg = NULL;
	gchar *such = NULL;
	gchar *match = NULL;
	gchar *output = NULL;
	
	entry = lookup_widget(GTK_WIDGET(caller), "suchinput");
	win   = lookup_widget(GTK_WIDGET(caller), "gnome_ding");

	such  = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry) ));
	match = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry) ));
	
	ok   = g_strdup( _("Spelling ok, no suggestions\n") ); 
	sugg  = g_strdup( _("Suggestions :") );
	store = gtk_list_store_new (1, G_TYPE_STRING);
	GtkTreeIter iter;
	// gtk_list_store_clear(store);
	
	
	if( strcmp (lang, "de_DE") == 0 ) {
		language = g_strdup( _("German spelling result") );
	} else if( strcmp (lang, "en_CA") == 0 ) {
		language = g_strdup( _("English (CA) spelling result") );
	} else if( strcmp (lang, "en_US") == 0 ) {
		language = g_strdup( _("English (US) spelling result") );
	} else if( strcmp (lang, "es") == 0 ) {
		language = g_strdup( _("Espain (ES) spelling result") );
	} else if( strcmp (lang, "fr") == 0 ) {
		language = g_strdup( _("Frensh (FR) spelling result") );
	} else if( strcmp (lang, "en") == 0 ) {
		language = g_strdup( _("English spelling result") );
	} else if (  strcmp (lang, "en_UK") == 0 ) {
	  language = g_strdup( _("English (UK) spelling result") );
		/* fixme: check not for wrong char, check for correct char */
		/* correct char, [0-9][a-z][A-Z] */
		if( (strchr(match, 'ä')) || (strchr(match, 'Ä')) ||
			(strchr(match, 'ü')) || (strchr(match, 'Ü')) ||
			(strchr(match, 'ö')) || (strchr(match, 'Ö')) ||
			(strchr(match, 'ß')) )   {

			gtk_list_store_append (GTK_LIST_STORE(store), &iter);
			gtk_list_store_set (GTK_LIST_STORE(store), &iter, 0,  _("Error :"), -1);
			
			gtk_list_store_append (GTK_LIST_STORE(store), &iter);
			gtk_list_store_set (GTK_LIST_STORE(store), &iter, 0, 
					_("Please don't use german special chars in english spellcheck.\n"), -1);
				
				
			showtable(GTK_WIDGET(caller), store, 1, 200, language, NULL);
				
			g_free(language);
			g_free(ok);				
			g_free(sugg);
			g_free(such);
			g_free(match);
        	return;
		}
	} else {
		/* unknow language selected */
		gtk_list_store_append (GTK_LIST_STORE(store), &iter);
		gtk_list_store_set (GTK_LIST_STORE(store), &iter, 0,  _("Error :"), -1);
			
		gtk_list_store_append (GTK_LIST_STORE(store), &iter);
		gtk_list_store_set (GTK_LIST_STORE(store), &iter, 0, _("Unknown language selected, please check it.\n"), -1);
		
		showtable(GTK_WIDGET(caller), store, 1, 200, _("Error message"), NULL);
		g_free(language);
		g_free(ok);				
		g_free(sugg);
		g_free(such);
		g_free(match);
		return;
	}
                                                                                                       
#ifdef USE_ASPELL
	/* the problem with the follow code is, that the api is in develoment
	   so the struct-names can be changed in future versions of aspell */
	AspellConfig *spell_config;
	AspellSpeller *spell_manager = 0;
	AspellCanHaveError *pos_err;
	int spell_correct;
	int ret;
    const AspellWordList *word_list;
    AspellStringEnumeration *suggestions;
	
	if ( get_debug() == TRUE ) {
		g_print( _("use aspell c-api\n"));
		g_print( _("search for '%s'\n"), match);
	}
	
	spell_config = new_aspell_config();
	aspell_config_remove(spell_config, "lang");
	ret = aspell_config_replace(spell_config, "lang", lang);
	if ( ret != 1 ) {
		if ( get_debug() == TRUE ) g_print( _("Error: Dictinory %s not found\n"), lang );
		gtk_list_store_append (GTK_LIST_STORE(store), &iter);
		gtk_list_store_set (GTK_LIST_STORE(store), &iter, 0,  _("Error :"), -1);
			
		gtk_list_store_append (GTK_LIST_STORE(store), &iter);
		gtk_list_store_set (GTK_LIST_STORE(store), &iter, 0, _("Dict not found"), -1);
		
		showtable(GTK_WIDGET(caller), store, 1, 500, language, NULL);
		g_free(language);
		g_free(ok);				
		g_free(sugg);
		g_free(output);
		g_free(such);
		g_free(match);
		return;
	}

	aspell_config_remove(spell_config, "sug-mode"); 
	if ( get_sug_mode() == 1 ) {
		aspell_config_replace(spell_config, "sug-mode", "ultra"); 
	} else if ( get_sug_mode() == 2 ) {
		aspell_config_replace(spell_config, "sug-mode", "fast"); 
	} else if ( get_sug_mode() == 4 ) {
		aspell_config_replace(spell_config, "bad-speller", "normal"); 
	} else {
		aspell_config_replace(spell_config, "sug-mode", "normal"); 
	}
	
	
	/* encoding: utf-8, iso8859-1, koi8-r, viscii, cp1252 */
	aspell_config_replace(spell_config, "encoding", "utf-8"); 
	
	pos_err = new_aspell_speller(spell_config);
	if( aspell_error_number(pos_err) != 0 ) {
		g_print( _("Error: %s\n") , aspell_error_message(pos_err) );
    	delete_aspell_can_have_error(pos_err);
		delete_aspell_config(spell_config);
		g_free(language);
		g_free(ok);				
		g_free(sugg);
		g_free(output);
		g_free(such);
		g_free(match);
		return;
	} else {
		spell_manager = to_aspell_speller(pos_err);
	}

	spell_correct = aspell_speller_check(spell_manager, such, strlen( match ) );
	if ( spell_correct == 0) {
		const char *word;
		GString *sug_list = g_string_new("");
		gchar *reste = NULL;
		
		g_string_printf(sug_list, _("Word not found or typo incorrect.\n <b>%s</b>"), 
			gtk_entry_get_text(GTK_ENTRY(entry) ) );
		gtk_list_store_append (GTK_LIST_STORE(store), &iter);
		gtk_list_store_set (GTK_LIST_STORE(store), &iter, 0,  sug_list->str, -1);
			
		gtk_list_store_append (GTK_LIST_STORE(store), &iter);
		gtk_list_store_set (GTK_LIST_STORE(store), &iter, 0, " ", -1);
		
		g_string_printf(sug_list, "");
		
		word_list = aspell_speller_suggest(spell_manager, match, strlen(match) );
		suggestions = aspell_word_list_elements( word_list );
 		while (( word = aspell_string_enumeration_next(suggestions)) != NULL) {
			g_string_append_printf(sug_list, " %s\r", word );
		}
		
		gtk_list_store_append (GTK_LIST_STORE(store), &iter);
		gtk_list_store_set (GTK_LIST_STORE(store), &iter, 0,  _("<b>Suggestions:</b>"), -1);
			
		gtk_list_store_append (GTK_LIST_STORE(store), &iter);
		gtk_list_store_set (GTK_LIST_STORE(store), &iter, 0, sug_list->str, -1);
		
		showtable(GTK_WIDGET(caller), store, 1, 500, language, NULL);
	
		reste = g_string_free(sug_list, TRUE);
	} else { 
		gtk_list_store_append (GTK_LIST_STORE(store), &iter);
		gtk_list_store_set (GTK_LIST_STORE(store), &iter, 0,  _("Spelling ok, no suggestions"), -1);
			
		gtk_list_store_append (GTK_LIST_STORE(store), &iter);
		gtk_list_store_set (GTK_LIST_STORE(store), &iter, 0, such, -1);
		
		showtable(GTK_WIDGET(caller), store, 1, 500, language, NULL);
	} 
	delete_aspell_can_have_error(pos_err);
	delete_aspell_config(spell_config);
#else
	GString *cmd = g_string_new("");
	gchar *reste = NULL;
	char buf[BUFSIZ];
	char *tmp , *dup, *head;
	
	if ( get_debug() == TRUE ) {
		g_print( _("use ispell pipe-mode\n"));
	}
	
	if (strcmp(lang, _("de_DE")) == 0) {
		g_string_printf(cmd, "echo '%s' | aspell -B -S -a -Tutf8 -d %s", 
		such, 
		"ngerman" );
	} else if (strcmp(lang, _("en_UK")) == 0) {
		g_string_printf(cmd, "echo '%s' | aspell -B -S -a -d %s", 
		such, 
		"british" );
	} else if (strcmp(lang, _("en_US")) == 0) {
		g_string_printf(cmd, "echo '%s' | aspell -B -S -a -d %s", 
		such, 
		"american" );
	} else if (strcmp(lang, _("en_CA")) == 0) {
		g_string_printf(cmd, "echo '%s' | aspell -B -S -a -d %s", 
		such, 
		"canadian" );
	} else if (strcmp(lang, _("fr")) == 0) {
		g_string_printf(cmd, "echo '%s' | aspell -B -S -a --ignore-accents -Tutf8 -d %s", 
		such, 
		"french" );
	} else  {
		g_string_printf(cmd, "echo '%s' | aspell -B -S -a -d %s", 
		such, 
		"english" );
	}
		
	if( get_debug() == TRUE ) {
		g_print("%s %s\n", _("command: "), 	cmd->str);	
	}
	
	/* get the results from ispell and print it */
	FILE *ptr;
	ptr = popen(cmd->str, "r");
	while (fgets(buf, BUFSIZ, ptr) != NULL)  {
		switch(buf[0]) {
			case '*':
				output = ok;
				head = sugg;
				break;
			case '&':
				dup = strstr(buf, ":");
				dup = strstr(dup, " ");
				output = g_locale_from_utf8(g_strdup(dup), -1, NULL, NULL, NULL);
				head = sugg;
				break;
			case '#':
				output = g_strdup( _("Typo or unknown word, no suggestions \n"));
				head = g_strdup( _("Error :"));
				break;
			case '+':
				dup = strstr(buf, " ");
				output = g_locale_from_utf8(g_strdup(dup), -1, NULL, NULL, NULL);
				head = g_strdup( _("root")); // wortstamm, fixme
				break;
			default :
				break;
		 }
         flag++;
	}
	pclose(ptr);
	reste = g_string_free(cmd, TRUE);

	/* output */
	gtk_list_store_append (GTK_LIST_STORE(store), &iter);
	gtk_list_store_set (GTK_LIST_STORE(store), &iter, 0,  head, -1);
			
	gtk_list_store_append (GTK_LIST_STORE(store), &iter);
	gtk_list_store_set (GTK_LIST_STORE(store), &iter, 0, output, -1);
	
	
	showtable(GTK_WIDGET(caller), store, 1, 500, language, NULL);

#endif /* use_aspell */
                                                                                                       
	/* cleanup status */
	cleanup_status(caller);
	g_free(language);
	g_free(ok);				
	g_free(sugg);
	g_free(output);
	g_free(such);
	g_free(match);
}


/**
 * use a ding dictinory to translate String(lang_from) ---> String(to)
 * 
 * @lang_from - language, form translate = string for tab-header
 * @lang_to   - language, to translate = string for tab-header
 * @dict - the filename of the uncompressed dict
**/
void
translation (GtkWidget *caller, gchar *lang_from, gchar *lang_to, gchar *dict)
{
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
	    (get_search_case_sense()  == FALSE) ? "-i" : " ",
		(get_search_exact_words() == TRUE) ? "-w" : " ",
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
	set_x_size(x);
	set_y_size(y);
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
        (strchr(match, '§')) ||
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
		
		/* pthread_t p;
		pthread_create (&p, NULL, my_wait_thread, (void *)sub_button );
		*/
		
		
        if (strcmp (suchart, _("Rechtschreibung (German)") ) == 0 ) {
			spellit(widget, "de_DE");
		} else if (strcmp (suchart, _("German <--> English")  ) == 0 ) {
			translation(widget, _("German"), _("English"), get_trans_de_en()  );
		} else if (strcmp (suchart, _("Thesaurus (de)")  ) == 0 ) {
			thesaurus(widget, get_thes_de()  );
		} else if (strcmp (suchart, _("Latin <--> German")  ) == 0 ) {
			translation(widget, _("Latin"), _("German"), get_trans_lat_de()  );
		} else if (strcmp (suchart, _("Spellcheck (Canadian)") ) == 0 ) {
			spellit(widget, "en_CA");
		} else if ( strcmp (suchart, _("Spellcheck (American)") ) == 0 ) {
			spellit(widget, "en_US");
		} else if ( strcmp (suchart, _("\303\251pelant (French)") ) == 0 ) {
			spellit(widget, "fr");
		} else if ( strcmp (suchart, _("Spellcheck (Spanish)") ) == 0 ) {
			spellit(widget, "es");
		} else {
			spellit(widget, "en_UK");
		}
		/*
		pthread_cancel (p);
		pthread_join (p, NULL);
		*/
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

#ifdef USE_ASPELL
	AspellConfig *spell_config;
	AspellDictInfoList * dlist;
  	AspellDictInfoEnumeration * dels;
  	const AspellDictInfo * entryd;
	
	spell_config = new_aspell_config();
	dlist = get_aspell_dict_info_list(spell_config);
	
	dels = aspell_dict_info_list_elements(dlist);
    	
	combo_entry = g_list_append (combo_entry, (gpointer) _("German <--> English") );
	combo_entry = g_list_append (combo_entry, (gpointer) _("Latin <--> German") );
	combo_entry = g_list_append (combo_entry, (gpointer) _("Thesaurus (de)") );
  	while ( (entryd = aspell_dict_info_enumeration_next(dels)) != 0) {
    	if ( strcmp(entryd->name, "de_DE") == 0 ) {
			combo_entry = g_list_append (combo_entry, (gpointer) _("Rechtschreibung (German)"));
		} else if ( strcmp(entryd->name, "en_GB") == 0 ) {
			combo_entry = g_list_append (combo_entry, (gpointer) _("Spellcheck (British)") );
		} else if ( strcmp(entryd->name, "en_CA") == 0 ) {
			combo_entry = g_list_append (combo_entry, (gpointer) _("Spellcheck (Canadian)") );
		} else if ( strcmp(entryd->name, "en_US") == 0 ) {
			combo_entry = g_list_append (combo_entry, (gpointer) _("Spellcheck (American)") );
		} else if ( strcmp(entryd->name, "fr_FR") == 0 ) {
			combo_entry = g_list_append (combo_entry, (gpointer) _("\303\251pelant (French)") );
		} 
           
  	}
	delete_aspell_config(spell_config);
   	delete_aspell_dict_info_enumeration(dels);
	
#else
	combo_entry = g_list_append (combo_entry, (gpointer) _("German <--> English") );
    combo_entry = g_list_append (combo_entry, (gpointer) _("Rechtschreibung (DE)") );
	combo_entry = g_list_append (combo_entry, (gpointer) _("Thesaurus (de)") );
	combo_entry = g_list_append (combo_entry, (gpointer) _("Spellcheck (British)") );
	/* combo_entry = g_list_append (combo_entry, (gpointer) _("Spellcheck (Canadian)") ); */
	/* combo_entry = g_list_append (combo_entry, (gpointer) _("Spellcheck (American)") ); */
	combo_entry = g_list_append (combo_entry, (gpointer) _("\303\251pelant (FR)") );
#endif	
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
                                                                                         
    list = lookup_widget(GTK_WIDGET(widget), "output");
    menu = lookup_widget(GTK_WIDGET(widget), "menubar1");
    status = lookup_widget(GTK_WIDGET(widget), "statusbar");
    win = lookup_widget(GTK_WIDGET(widget), "gnome_ding");
                                                                                                       
	if (gtk_window_get_resizable(GTK_WINDOW(win))) {
		gtk_widget_hide(list);
		gtk_widget_hide(menu);
		gtk_widget_hide(status);
		gtk_window_resize(GTK_WINDOW(win), get_x_size(), 40);
		gtk_window_set_resizable(GTK_WINDOW(win), 0);
	} else {
		unhide(widget);
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
		GtkWidget *case_sense;
		GtkWidget *exact_word;
		
		GtkWidget *aspell_sug;
		GtkWidget *aspell_char;
		
		case_sense = lookup_widget(pref_dlg, "grep_case");
		exact_word = lookup_widget(pref_dlg, "grep_word");
		aspell_sug  = lookup_widget(pref_dlg, "aspell_suggest");
		aspell_char = lookup_widget(pref_dlg, "aspell_charset");
		
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (case_sense), get_search_case_sense() );
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (exact_word), get_search_exact_words() );
		
		
		gtk_adjustment_set_value(
			gtk_range_get_adjustment( GTK_RANGE(aspell_sug )),
			(gdouble)(get_sug_mode() - 1 ) );
		
		gtk_widget_show(pref_dlg);
	}
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
		gtk_window_resize(GTK_WINDOW(win), get_x_size(), 40);
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
   unhide(menuitem);
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
 * only close the prefereces-dialog
**/
void
on_okbutton1_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_destroy(pref_dlg);
	pref_dlg = NULL;
}


/**
 * preferences-dialog : grep with/without exactly-word-match
**/
void
on_grep_word_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	gboolean value;
	GtkWidget *exact_word;
	exact_word = lookup_widget(pref_dlg, "grep_word");
	value = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (exact_word) );
	set_search_exact_words(value);
}

/**
 * preferences-dialog : grep with/without case-sensitive
**/
void
on_grep_case_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	gboolean value;
	GtkWidget *case_sense;
	
	case_sense = lookup_widget(pref_dlg, "grep_case");
	value = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (case_sense) );
	set_search_case_sense( value);
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
	set_sug_mode( 1+ (gint)( gtk_adjustment_get_value(adj) ));
}


/**
 * count entries in dictionary 
  less ger-eng.ding | grep -v '^#' | sed 's/:.*$//' | sed 's/|/\n/p' | wc -l
**/

