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
 * thesaurus.c
 *
 * all callbacks thesaurus search
 *
 * begin                : Sat May 31 2003
 * copyright            : (C) 2003 by Heiko Abraham
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gnome.h>

#include "support.h"
#include "interface.h"
#include "callbacks.h"
#include "main.h"


/** 
 *
 * @doc  this function is called to search for a given word inside
 *       the thesaurus
 *       <BR>It search through the file by using grep. 
 *
 * @parameter caller - callback WidgetPointer
 * @parameter thes - filename of the thesaurus
 * @parameter msg2 - string to show on list header as description
 *
 * @return void - function display the result inside the list-view
 *
 * @author Heiko Abraham, <abrahamh@web.de>
 *
**/
void 
run_thesaurus(GtkWidget *caller, gchar *thes, gchar *msg2)
{
	if (caller == NULL ) return;
	if (thes == NULL) return;	
		
	set_model_int(MAIN_COLUMN_NUM, 1);
	on_contex_menu_show(caller);
	GtkWidget *entry = NULL;
	GtkWidget *win = NULL;
	gchar *such = NULL;
	gchar *msg = NULL;
	char line[200];
	gchar *reste;
	GString *cmd = g_string_new("");
	GString *grep_out = g_string_new("");
	FILE *ptr;
	gint flag = 0;
	GtkListStore *store = NULL;
	GtkTreeIter iter;

	win   = get_mainwin();
	if ( win == NULL ) return;
	entry = lookup_widget( win, "search_input");
	store = gtk_list_store_new (1, G_TYPE_STRING);
	such  = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry) ));
	g_string_printf(cmd, "egrep -h -i -w '%s' '%s' | grep -v '^#'", 
	    /* (get_search_case_sense()  == FALSE) ? "-i" : " ", */
		/* (get_model_bool(MAIN_EXACT) == TRUE) ? "-w" : " ", */
		such, thes );
	
	if ( get_debug() == TRUE) {
		g_printf("call thesaurus '%s' - '%s'\n", thes, such);
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
							out0 = g_string_append(out0, " \r ");
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
	showtable(GTK_WIDGET(caller), store, 1, 200, msg2, NULL);
    msg = g_strdup_printf( _("%i matches found to '%s'"), flag, such);
    on_statusbar_print(GTK_WIDGET(caller), msg);
	
	reste = g_string_free(grep_out, TRUE);
	reste = g_string_free(cmd, TRUE);
	g_free(such);
	g_free(msg);
}
