#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gnome.h>

#ifdef USE_ASPELL
#  include <aspell.h>
#endif

#include "support.h"
#include "interface.h"
#include "callbacks.h"
#include "main.h"

// usage:  SPELL_FILL( "de_DE", _("German spelling result") );
#define SPELL_FILL(x, y) \
	(strcmp (lang, x) == 0) ) { \
		language = g_strdup( y );

/**
 * @run spellchecker
 *
 * @parameter: caller - callback WidgetPointer
 * @parameter: lang   - chose language { ngerman, british, ... }
 *
 * @return: fill g_tree_list over callback-pointer with serach-results
**/
void
run_spellchecker(GtkWidget *caller, gchar *lang)
{
	if (caller == NULL ) return;
	if (lang == NULL) return;	
	
	GtkWidget       *entry = NULL;
	GtkWidget       *win = NULL;
	GtkListStore    *store = NULL;
	GtkTreeIter iter;
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
	
	// gtk_list_store_clear(store);
	
	// fill top-level description
	if( SPELL_FILL( "de_DE", _("German spelling result") );
	} else if( SPELL_FILL( "en_CA", _("English (CA) spelling result") );
	} else if( SPELL_FILL( "en_US", _("English (US) spelling result") );
	} else if( SPELL_FILL( "es"   , _("Spanish (ES) spelling result") );
	} else if( SPELL_FILL( "fr"   , _("Frensh (FR) spelling result") );
	} else if( SPELL_FILL( "en"   , _("English spelling result") );
	} else if( SPELL_FILL( "en_UK", _("English (UK) spelling result") );
		/* fixme: check not for wrong char, check for correct char */
		/* correct char, [0-9][a-z][A-Z] */
		if( FALSE /* (strchr(match, 'ä')) || (strchr(match, 'Ä')) ||
			(strchr(match, 'ü')) || (strchr(match, 'Ü')) ||
			(strchr(match, 'ö')) || (strchr(match, 'Ö')) ||
			(strchr(match, 'ß')) */ )   {

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
	if ( get_model_int(MAIN_SUGEST_MODE)  == 1 ) {
		aspell_config_replace(spell_config, "sug-mode", "ultra"); 
	} else if ( get_model_int(MAIN_SUGEST_MODE) == 2 ) {
		aspell_config_replace(spell_config, "sug-mode", "fast"); 
	} else if ( get_model_int(MAIN_SUGEST_MODE) == 4 ) {
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
	
	if (strcmp(lang, "de_DE") == 0) {
		g_string_printf(cmd, "echo '%s' | aspell -B -S -a -Tutf8 -d %s", 
		such, 
		"ngerman" );
	} else if (strcmp(lang, "en_UK") == 0) {
		g_string_printf(cmd, "echo '%s' | aspell -B -S -a -d %s", 
		such, 
		"british" );
	} else if (strcmp(lang, "en_US") == 0) {
		g_string_printf(cmd, "echo '%s' | aspell -B -S -a -d %s", 
		such, 
		"american" );
	} else if (strcmp(lang, "en_CA") == 0) {
		g_string_printf(cmd, "echo '%s' | aspell -B -S -a -d %s", 
		such, 
		"canadian" );
	} else if (strcmp(lang, "fr") == 0) {
		g_string_printf(cmd, "echo '%s' | aspell -B -S -a --ignore-accents -Tutf8 -d %s", 
		such, 
		"french" );
	} else if (strcmp(lang, "es") == 0) {
		g_string_printf(cmd, "echo '%s' | aspell -B -S -a -Tutf8 -d %s", 
		such, 
		"spanish" );
	} else if (strcmp(lang, "it") == 0) {
		g_string_printf(cmd, "echo '%s' | aspell -B -S -a -Tutf8 -d %s", 
		such, 
		"italian" );
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
