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
 * myaspell.c
 *
 * all callbacks for aspell search
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

#include "support.h"
#include "interface.h"
#include "callbacks.h"
#include "main.h"

/**
 *
 * @doc macro to fill header of list with information
 *     <BR> usage:  SPELL_FILL( "de_DE", _("German spelling result") );
 *
 * @parameter x - string for aspell language (like "de_DE")
 *
 * @parameter y - corresponding string for list header (like _("German spelling result")
 *
**/
 #define SPELL_FILL(x, y) \
	(strcmp (lang, x) == 0) ) { \
		language = g_strdup( y );

		
		
/**
 * @doc this function is called to search for a given word inside
 *       aspell dictionary
 *
 * @parameter: caller - callback WidgetPointer
 * @parameter: lang   - chose language { de, fr, de_DE, ... }
 *
 * @return: void - fill g_tree_list over callback-pointer with serach-results
**/
void
run_spellchecker(GtkWidget *caller, gchar *lang)
{
	if (caller == NULL ) return;
	if (lang == NULL) return;	
	
	set_model_int(MAIN_COLUMN_NUM, 1);
	on_contex_menu_show(caller);
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
	
	win   = get_mainwin();
	if (win == NULL) return;
		
	entry = lookup_widget( win, "search_input");
	such  = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry) ));
	match = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry) ));
	ok   = g_strdup( _("Spelling ok, no suggestions\n") ); 
	sugg  = g_strdup( _("Suggestions :") );
	store = gtk_list_store_new (1, G_TYPE_STRING);
	
	// fill top-level description
	if( SPELL_FILL( "de_DE", _("German spelling result") );
	} else if( SPELL_FILL( "de_CH", _("German (CH) spelling result") );
	} else if( SPELL_FILL( "af", _("Afrikans (AF) spelling result") );
	} else if( SPELL_FILL( "bg", _("Bulgarian (BG) spelling result") );
	} else if( SPELL_FILL( "br", _("Breton (BR) spelling result") );
	} else if( SPELL_FILL( "ca", _("Catalan (CA) spelling result") );
	} else if( SPELL_FILL( "cs", _("Czech (CS) spelling result") );
	} else if( SPELL_FILL( "cy", _("Welsh (CY) spelling result") );
	} else if( SPELL_FILL( "da", _("Danish (DA) spelling result") );
	} else if( SPELL_FILL( "eo", _("Esperanto (EO) spelling result") ); 
	} else if( SPELL_FILL( "el", _("Greek (EL) spelling result") );
	} else if( SPELL_FILL( "es", _("Spanish (ES) spelling result") );
	} else if( SPELL_FILL( "fo", _("Faroese (FO) spelling result") );
	} else if( SPELL_FILL( "ga", _("Irish, Gaelic (GA) spelling result") );
	} else if( SPELL_FILL( "gl", _("Gallegan (GL) spelling result") );
	} else if( SPELL_FILL( "hr", _("Croatian (HR) spelling result") );
	} else if( SPELL_FILL( "fr_FR" , _("French (FR) spelling result") );
	} else if( SPELL_FILL( "fr_CH", _("French (CH) spelling result") );
	} else if( SPELL_FILL( "id", _("Indonesian (ID) spelling result") );
	} else if( SPELL_FILL( "it", _("Italian (IT) spelling result") );
	} else if( SPELL_FILL( "is", _("Icelandic (IS) spelling result") );
	} else if( SPELL_FILL( "mi", _("Maori (MI) spelling result") );
	} else if( SPELL_FILL( "mk", _("Makasar (MK) spelling result") );
	} else if( SPELL_FILL( "ms", _("Malay (MS) spelling result") );
	} else if( SPELL_FILL( "mt", _("Maltese (MT) spelling result") );
	} else if( SPELL_FILL( "nl", _("Dutch (NL) spelling result") );
	} else if( SPELL_FILL( "no", _("Norwegian (NO) spelling result") );
	// } else if( SPELL_FILL( "pt_BR", _("Portuguese (BR) spelling result") );
	} else if( SPELL_FILL( "pt", _("Portuguese (PT) spelling result") );
	} else if( SPELL_FILL( "ru", _("Russian (RU) spelling result") );
	} else if( SPELL_FILL( "ro", _("Romanian (RO) spelling result") );
	} else if( SPELL_FILL( "sk", _("Slovak (SK) spelling result") );
	} else if( SPELL_FILL( "sl", _("Slovenian (SL) spelling result") );
	} else if( SPELL_FILL( "sv", _("Swedish (SV) spelling result") );
	} else if( SPELL_FILL( "uk", _("Ukrainian (UK) spelling result") );
	} else if( SPELL_FILL( "en_CA", _("English (CA) spelling result") );
	} else if( SPELL_FILL( "en_US", _("English (US) spelling result") );
	} else if( SPELL_FILL( "wa"   , _("Walloon (WA) spelling result") );
	} else if( SPELL_FILL( "zu"   , _("Zulu (ZU) spelling result") );
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

		gtk_list_store_append (GTK_LIST_STORE(store), &iter);
		gtk_list_store_set (GTK_LIST_STORE(store), &iter, 0,  _("<b>Suggestions:</b>"), -1);
		
 		while (( word = aspell_string_enumeration_next(suggestions)) != NULL) {
			g_string_printf(sug_list, "%s", word );

			gtk_list_store_append (GTK_LIST_STORE(store), &iter);
			gtk_list_store_set (GTK_LIST_STORE(store), &iter, 0, sug_list->str, -1);
		}
		
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
	on_statusbar_cleanup(caller);
	g_free(language);
	g_free(ok);				
	g_free(sugg);
	g_free(output);
	g_free(such);
	g_free(match);
}
