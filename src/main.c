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
 * main.c
 *
 * the main function and helpers for gconf and model handling
 *
 * begin                : Sat May 31 2003
 * copyright            : (C) 2003 by Heiko Abraham
 */


#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gnome.h>
#include <gconf/gconf-client.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "interface.h"
#include "support.h"
#include "main.h"


/**
 * @doc macro used by set_model_char() to store corresponding 
 *     string to enum
 * <BR>usage:  VALUESET( THES_IT, my_model.sz_thes_it)
 *
 * @parameter z - enum-type { THES_IT, DICT_DE_EN, .. }
 *
 * @parameter x - filename of the dictionary
 *
 * @author Heiko Abraham, <abrahamh@web.de>
 *
 * @see set_model_char
**/
#define VALUESET(z, x) \
	case z: \
		if(value != NULL) { \
		if (stat(value, &buf) == 0) { \
			g_string_printf( x, "%s", value ); \
		 } else {  \
			 g_string_printf( x , "(null)" ); \
		 } \
	} else { \
		g_string_printf( x , "(null)" ); \
	} \
	break;

	
	
/**
 * @doc macro used by get_model_bool() to return corresponding 
 *     boolean to enum
 * <BR>usage: CASE_MACRO1( ASPELL_AF, my_model.aspell_af )
 *
 * @parameter x -enum-type { APELL_AF, ASPELL_DE, ... }
 *
 * @parameter y - model-flag ( like my_model.aspell_af )
 *
 * @author Heiko Abraham, <abrahamh@web.de>
 *
 * @see get_model_bool
**/
#define CASE_MACRO1(x, y) \
	case x: \
		return y; \
		break;

	
/**
 * @doc macro used by set_model_bool() to store corresponding 
 *     boolean to enum
 * <BR>CASE_MACRO2( DICT_DE_EN, my_model.dict_de_es)
 *
 * @parameter x - enum-type { THES_IT, DICT_DE_EN, ... }
 * 	
 * @parameter y - filename for the corresponding dictionary
 * 
 * @author Heiko Abraham, <abrahamh@web.de>
 * 
 * @see set_model_bool
 * 
**/
// usage: 
#define	CASE_MACRO2(x, y) \
	case x: \
			y = value; \
			break;

// 	DEFAULT_MACRO(my_model.dict_de_en, my_model.sz_dict_de_en, "de-en.ding" )
#define DEFAULT_MACRO(x,y,z) \
	x  = FALSE; \
	g_string_printf(y, "%sshare/dict/%s", get_model_char(MAIN_PREFIX), z);
	
void model_alloc();
void model_free();
void model_default();
void model_read(GConfClient *gconf);
void model_write(GConfClient *gconf);
	
static struct my_ding_data my_model;



/**
 * @doc macro function to return a corresponding value of the
 *     internal data model to a flag
 * 
 * @parameter i - enum-type { MAIN_SIZE_X, ... }
 *
 * @return int - value corresponding to the model-flag
 * 
 * @author Heiko Abraham, <abrahamh@web.de>
 *
 * @see get_model_bool get_model_char
**/
gint get_model_int(MYTYPE i) {
	switch (i) {
		case MAIN_SIZE_X: 
			return my_model.win_size_x;
			break;
		case MAIN_SIZE_Y: 
			return my_model.win_size_y;
			break;
		case MAIN_SUGEST_MODE: 
			return my_model.suggestion_mode;
			break;
		case MAIN_COLUMN_NUM: 
			return my_model.column_num;
			break;
		default:
			g_message("get_model_int(%i) - unknown type\n", i);
			break;
	}
	return -1;
} // get_model_int


/**
 * @doc macro function to store a integer to a corresponding value 
 *      of the internal data model to a int
 * 
 * @parameter i - the enum-type for the  internal model flag
 * 
 * @parameter value - the integer value to be set to the model
 * 
 * @author Heiko Abraham, <abrahamh@web.de>
 *
 * @see set_model_bool set_model_char get_model_int
**/
void set_model_int(MYTYPE i, gint value) {
	switch (i) {
		case MAIN_SIZE_X: 
			if ( value < 20 ) value = 500;
			my_model.win_size_x = value;
			break;
		case MAIN_SIZE_Y: 
			if ( value < 20 ) value = 500;
			my_model.win_size_y = value;
			break;
		case MAIN_SUGEST_MODE: 
			if (value < 1) value = 1;
			if (value > 4) value = 4;
			my_model.suggestion_mode = value;
			break;
		case MAIN_COLUMN_NUM: 
			my_model.column_num = value;
			break;
		default:
			g_message("set_model_int(%i) - unknown type\n", i);
			break;
	}
} // set_model_int


/**
 * @doc return the boolean value to the given flag of the  internal
 *      data model
 * 
 * @parameter i - enum-type corresponding to the data model flag
 *
 * @return void 
 *
 * @author Heiko Abraham, <abrahamh@web.de>
 *
 * @see set_model_bool set_model_int get_model_int get_model_char
 *
**/
gboolean get_model_bool(MYTYPE i) {
	switch(i) {
		case MAIN_HIDDEN:
			return my_model.main_hidden;
			break;
		case MAIN_EXACT:
			return my_model.search_exact_words;
			break;
		case MAIN_CASE:
			return my_model.serach_case_sense;
			break;
		case MAIN_DEBUG:
			return my_model.main_debug;
			break;
		case MAIN_RESET:
			return my_model.main_reset;
			break;
		case MAIN_FONT_DEFAULT:
			return my_model.text_font_default;
			break;
		case MAIN_COLOR_DEFAULT:
			return my_model.text_color_default;
			break;
		
		// aspell languages
		CASE_MACRO1( ASPELL_AF, my_model.aspell_af)
		CASE_MACRO1( ASPELL_BG, my_model.aspell_bg)
 		CASE_MACRO1( ASPELL_BR, my_model.aspell_br)
		CASE_MACRO1( ASPELL_CA, my_model.aspell_ca)
		CASE_MACRO1( ASPELL_CH, my_model.aspell_ch )
		CASE_MACRO1( ASPELL_CS, my_model.aspell_cs )
		CASE_MACRO1( ASPELL_CY, my_model.aspell_cy)
		CASE_MACRO1( ASPELL_DA, my_model.aspell_da)
		CASE_MACRO1( ASPELL_DE, my_model.aspell_de)
		CASE_MACRO1( ASPELL_EL, my_model.aspell_el)
		CASE_MACRO1( ASPELL_EO, my_model.aspell_eo)
		CASE_MACRO1( ASPELL_ES, my_model.aspell_es)
		CASE_MACRO1( ASPELL_FO, my_model.aspell_fo)
		CASE_MACRO1( ASPELL_FR, my_model.aspell_fr)
		CASE_MACRO1( ASPELL_FRCH, my_model.aspell_frch)
		CASE_MACRO1( ASPELL_GA, my_model.aspell_ga)
		CASE_MACRO1( ASPELL_GB, my_model.aspell_gb)
		CASE_MACRO1( ASPELL_GL, my_model.aspell_gl)
		CASE_MACRO1( ASPELL_HR, my_model.aspell_hr)
		CASE_MACRO1( ASPELL_ID, my_model.aspell_id)
		CASE_MACRO1( ASPELL_IS, my_model.aspell_is)
		CASE_MACRO1( ASPELL_IT, my_model.aspell_it)
		CASE_MACRO1( ASPELL_MI, my_model.aspell_mi)
		CASE_MACRO1( ASPELL_MK, my_model.aspell_mk)
		CASE_MACRO1( ASPELL_MS, my_model.aspell_ms)
		CASE_MACRO1( ASPELL_MT, my_model.aspell_mt)
		CASE_MACRO1( ASPELL_NL, my_model.aspell_nl)
		CASE_MACRO1( ASPELL_NO, my_model.aspell_no)
		CASE_MACRO1( ASPELL_PL, my_model.aspell_pl)
		CASE_MACRO1( ASPELL_PT, my_model.aspell_pt)
		CASE_MACRO1( ASPELL_RO, my_model.aspell_ro)
		CASE_MACRO1( ASPELL_RU, my_model.aspell_ru)
		CASE_MACRO1( ASPELL_SK, my_model.aspell_sk)
		CASE_MACRO1( ASPELL_SL, my_model.aspell_sl)
		CASE_MACRO1( ASPELL_SV, my_model.aspell_sv)
		CASE_MACRO1( ASPELL_UC, my_model.aspell_uc)
		CASE_MACRO1( ASPELL_UK, my_model.aspell_uk)
		CASE_MACRO1( ASPELL_US, my_model.aspell_us)
		CASE_MACRO1( ASPELL_WA, my_model.aspell_wa)
		CASE_MACRO1( ASPELL_ZU, my_model.aspell_zu)
		
		// thesaurus
		CASE_MACRO1( THES_DE, my_model.thes_de)
		CASE_MACRO1( THES_IT, my_model.thes_it)
		CASE_MACRO1( THES_ES, my_model.thes_es)
		CASE_MACRO1( THES_FR, my_model.thes_fr)
		CASE_MACRO1( THES_EN, my_model.thes_en)
		CASE_MACRO1( THES_PT, my_model.thes_pt)
		CASE_MACRO1( THES_PL, my_model.thes_pl)

		// dictinory		
		CASE_MACRO1( DICT_LAT_DE, my_model.dict_lat_de)
		CASE_MACRO1( DICT_EN_LAT, my_model.dict_en_lat)
		CASE_MACRO1( DICT_NO_EN, my_model.dict_no_en)
		
		CASE_MACRO1( DICT_DE_EN,  my_model.dict_de_en)
		CASE_MACRO1( DICT_DE_ES,  my_model.dict_de_es)
		CASE_MACRO1( DICT_DE_FR,  my_model.dict_de_fr)
		CASE_MACRO1( DICT_DE_IT,  my_model.dict_de_it)
		CASE_MACRO1( DICT_DE_PT,  my_model.dict_de_pt)
		CASE_MACRO1( DICT_DE_NL,  my_model.dict_de_nl)
		
		CASE_MACRO1( DICT_EN_DE,  my_model.dict_en_de)
		CASE_MACRO1( DICT_EN_FR,  my_model.dict_en_fr)
		CASE_MACRO1( DICT_EN_ES,  my_model.dict_en_es)
		CASE_MACRO1( DICT_EN_PT,  my_model.dict_en_pt)
		CASE_MACRO1( DICT_EN_IT,  my_model.dict_en_it)
		CASE_MACRO1( DICT_EN_NO,  my_model.dict_en_no)
		CASE_MACRO1( DICT_EN_AR,  my_model.dict_en_ar)
		CASE_MACRO1( DICT_EN_SV,  my_model.dict_en_sv)
		
		CASE_MACRO1( DICT_FR_EN,  my_model.dict_fr_en)
		CASE_MACRO1( DICT_FR_ES,  my_model.dict_fr_es)
		CASE_MACRO1( DICT_FR_DE,  my_model.dict_fr_de)
		CASE_MACRO1( DICT_FR_IT,  my_model.dict_fr_it)
		CASE_MACRO1( DICT_FR_PT,  my_model.dict_fr_pt)
		
		CASE_MACRO1( DICT_ES_EN,  my_model.dict_es_en)
		CASE_MACRO1( DICT_ES_DE,  my_model.dict_es_de)
		CASE_MACRO1( DICT_ES_FR,  my_model.dict_es_fr)
		CASE_MACRO1( DICT_ES_IT,  my_model.dict_es_it)
		CASE_MACRO1( DICT_ES_PT,  my_model.dict_es_pt)
		
		CASE_MACRO1( DICT_IT_EN,  my_model.dict_it_en)
		CASE_MACRO1( DICT_IT_ES,  my_model.dict_it_es)
		CASE_MACRO1( DICT_IT_DE,  my_model.dict_it_de)
		CASE_MACRO1( DICT_IT_FR,  my_model.dict_it_fr)
		CASE_MACRO1( DICT_IT_PT,  my_model.dict_it_pt)
		
		default:
			g_message("get_model_bool(%i) - unknown type\n", i );
			break;
	}
	return FALSE;
} // get_model_bool


/**
 * @doc store a boolean to the internal data model defined be 
 *     the given flag.
 *
 * @parameter i - enum-type corresponding to the data model flag
 *
 * @parameter value - the boolean to be store
 *
 * @return void
 *
 * @author Heiko Abraham, <abrahamh@web.de>
 *
 * @see get_model_bool set_model_int get_model_int set_model_char
 *
**/
void set_model_bool(MYTYPE i, gboolean value) {
	switch(i) {
		case MAIN_HIDDEN:
			my_model.main_hidden = value;
			break;
		case MAIN_EXACT:
			my_model.search_exact_words = value;
			break;
		case MAIN_CASE:
			my_model.serach_case_sense = value;
			break;
		case MAIN_DEBUG:
			my_model.main_debug = value;
			break;
		case MAIN_RESET:
			my_model.main_reset = value;
			break;
		case MAIN_FONT_DEFAULT:
			my_model.text_font_default = value;
			break;
		case MAIN_COLOR_DEFAULT:
			my_model.text_color_default = value;
			break;
		
		// aspell languages
		CASE_MACRO2(ASPELL_AF, my_model.aspell_af)
		CASE_MACRO2(ASPELL_BG, my_model.aspell_bg)
		CASE_MACRO2(ASPELL_BR, my_model.aspell_br)
		CASE_MACRO2(ASPELL_CA, my_model.aspell_ca)
		CASE_MACRO2(ASPELL_CH, my_model.aspell_ch)
		CASE_MACRO2(ASPELL_CS, my_model.aspell_cs)
		CASE_MACRO2(ASPELL_CY, my_model.aspell_cy)
		CASE_MACRO2(ASPELL_DA, my_model.aspell_da)
		CASE_MACRO2(ASPELL_DE, my_model.aspell_de)
		CASE_MACRO2(ASPELL_EL, my_model.aspell_el)
		CASE_MACRO2(ASPELL_EO, my_model.aspell_eo)
		CASE_MACRO2(ASPELL_ES, my_model.aspell_es)
		CASE_MACRO2(ASPELL_FO, my_model.aspell_fo)
		CASE_MACRO2(ASPELL_FR, my_model.aspell_fr)
		CASE_MACRO2(ASPELL_FRCH, my_model.aspell_frch)
		CASE_MACRO2(ASPELL_GA, my_model.aspell_ga)
		CASE_MACRO2(ASPELL_GB, my_model.aspell_gb)
		CASE_MACRO2(ASPELL_GL, my_model.aspell_gl)
		CASE_MACRO2(ASPELL_ID, my_model.aspell_id)
		CASE_MACRO2(ASPELL_IT, my_model.aspell_it)
		CASE_MACRO2(ASPELL_IS, my_model.aspell_is)
		CASE_MACRO2(ASPELL_HR, my_model.aspell_hr)
		CASE_MACRO2(ASPELL_MI, my_model.aspell_mi)
		CASE_MACRO2(ASPELL_MK, my_model.aspell_mk)
		CASE_MACRO2(ASPELL_MS, my_model.aspell_ms)
		CASE_MACRO2(ASPELL_MT, my_model.aspell_mt)
		CASE_MACRO2(ASPELL_NL, my_model.aspell_nl)
		CASE_MACRO2(ASPELL_NO, my_model.aspell_no)
		CASE_MACRO2(ASPELL_PL, my_model.aspell_pl)
		CASE_MACRO2(ASPELL_PT, my_model.aspell_pt)
		CASE_MACRO2(ASPELL_RO, my_model.aspell_ro)
		CASE_MACRO2(ASPELL_RU, my_model.aspell_ru)
		CASE_MACRO2(ASPELL_SK, my_model.aspell_sk)
		CASE_MACRO2(ASPELL_SL, my_model.aspell_sl)
		CASE_MACRO2(ASPELL_SV, my_model.aspell_sv)
		CASE_MACRO2(ASPELL_UC, my_model.aspell_uc)
		CASE_MACRO2(ASPELL_UK, my_model.aspell_uk)
		CASE_MACRO2(ASPELL_US, my_model.aspell_us)
		CASE_MACRO2(ASPELL_WA, my_model.aspell_wa)
		CASE_MACRO2(ASPELL_ZU, my_model.aspell_zu)
		
		// thesaurus
		CASE_MACRO2(THES_DE, my_model.thes_de)
		CASE_MACRO2(THES_IT, my_model.thes_it)
		CASE_MACRO2(THES_ES, my_model.thes_es)
		CASE_MACRO2(THES_FR, my_model.thes_fr)
		CASE_MACRO2(THES_EN, my_model.thes_en)
		CASE_MACRO2(THES_PT, my_model.thes_pt)
		CASE_MACRO2(THES_PL, my_model.thes_pl)

		// dictinory		
		CASE_MACRO2(DICT_DE_EN, my_model.dict_de_en)
		CASE_MACRO2(DICT_DE_ES, my_model.dict_de_es)
		CASE_MACRO2(DICT_DE_FR, my_model.dict_de_fr)
		CASE_MACRO2(DICT_DE_IT, my_model.dict_de_it)
		CASE_MACRO2(DICT_DE_PT, my_model.dict_de_pt)
		CASE_MACRO2(DICT_DE_NL, my_model.dict_de_nl)
		
		CASE_MACRO2(DICT_EN_DE, my_model.dict_en_de)
		CASE_MACRO2(DICT_EN_FR, my_model.dict_en_fr)
		CASE_MACRO2(DICT_EN_ES,	my_model.dict_en_es)
		CASE_MACRO2(DICT_EN_IT, my_model.dict_en_it)
		CASE_MACRO2(DICT_EN_PT, my_model.dict_en_pt)
		CASE_MACRO2(DICT_EN_NO, my_model.dict_en_no)
		CASE_MACRO2(DICT_EN_AR, my_model.dict_en_ar)
		CASE_MACRO2(DICT_EN_SV, my_model.dict_en_sv)
		
		CASE_MACRO2(DICT_FR_DE, my_model.dict_fr_de)
		CASE_MACRO2(DICT_FR_EN, my_model.dict_fr_en)
		CASE_MACRO2(DICT_FR_ES, my_model.dict_fr_es)
		CASE_MACRO2(DICT_FR_IT, my_model.dict_fr_it)
		CASE_MACRO2(DICT_FR_PT, my_model.dict_fr_pt)
		
		CASE_MACRO2(DICT_ES_DE, my_model.dict_es_de)
		CASE_MACRO2(DICT_ES_EN, my_model.dict_es_en)
		CASE_MACRO2(DICT_ES_FR, my_model.dict_es_fr)
		CASE_MACRO2(DICT_ES_IT, my_model.dict_es_it)
		CASE_MACRO2(DICT_ES_PT, my_model.dict_es_pt)
		
		CASE_MACRO2(DICT_IT_DE, my_model.dict_it_de)
		CASE_MACRO2(DICT_IT_EN, my_model.dict_it_en)
		CASE_MACRO2(DICT_IT_ES, my_model.dict_it_es)
		CASE_MACRO2(DICT_IT_FR, my_model.dict_it_fr)
		CASE_MACRO2(DICT_IT_PT, my_model.dict_it_pt)
		
		CASE_MACRO2(DICT_NO_EN, my_model.dict_no_en)
		CASE_MACRO2(DICT_LAT_DE, my_model.dict_lat_de)
		CASE_MACRO2(DICT_EN_LAT, my_model.dict_en_lat)
		
		default:
			g_message("set-model-bool with unknown type\n");
			break;
	}
} // set_model_bool



/**
 * @doc return a pointer to const char* contain information off 
 * the internal data model like the filename of the dictionary.
 * or other configuration flags defined by the corresponding
 * enum-type.
 * 
 * @parameter i - enum-type like { DICT_DE_EN, ... }
 *
 * @return void 
 *
 * @author Heiko Abraham, <abrahamh@web.de>
 *
 * @see set_model_char get_model_bool get_model_int set_model_int
 *
**/
gchar *get_model_char(MYTYPE i) {
	switch (i) {
		CASE_MACRO1(MAIN_FONT, my_model.text_font->str)
		CASE_MACRO1(MAIN_COLOR1, my_model.text_color->str)
		CASE_MACRO1(MAIN_COLOR2, my_model.text_highlight->str)
		CASE_MACRO1(MAIN_COLUMN1, my_model.column1->str)
		CASE_MACRO1(MAIN_COLUMN2, my_model.column2->str)
		CASE_MACRO1(MAIN_PREFIX, my_model.main_prefix->str)
		CASE_MACRO1(MAIN_LATEST_DICT, my_model.main_latest_dict->str)
		
		CASE_MACRO1(THES_DE, my_model.sz_thes_de->str)
		CASE_MACRO1(THES_IT, my_model.sz_thes_it->str)
		CASE_MACRO1(THES_FR, my_model.sz_thes_fr->str)
		CASE_MACRO1(THES_EN, my_model.sz_thes_en->str)
		CASE_MACRO1(THES_ES, my_model.sz_thes_es->str)
		CASE_MACRO1(THES_PT, my_model.sz_thes_pt->str)
		CASE_MACRO1(THES_PL, my_model.sz_thes_pl->str)
		
		CASE_MACRO1(DICT_DE_EN, my_model.sz_dict_de_en->str)
		CASE_MACRO1(DICT_DE_ES, my_model.sz_dict_de_es->str)
		CASE_MACRO1(DICT_DE_FR, my_model.sz_dict_de_fr->str)
		CASE_MACRO1(DICT_DE_IT, my_model.sz_dict_de_it->str)
		CASE_MACRO1(DICT_DE_PT, my_model.sz_dict_de_pt->str)
		CASE_MACRO1(DICT_DE_NL, my_model.sz_dict_de_nl->str)

		CASE_MACRO1(DICT_EN_DE, my_model.sz_dict_en_de->str)
		CASE_MACRO1(DICT_EN_FR, my_model.sz_dict_en_fr->str)
		CASE_MACRO1(DICT_EN_ES, my_model.sz_dict_en_es->str)
		CASE_MACRO1(DICT_EN_PT, my_model.sz_dict_en_pt->str)
		CASE_MACRO1(DICT_EN_IT, my_model.sz_dict_en_it->str)
		CASE_MACRO1(DICT_EN_NO, my_model.sz_dict_en_no->str)
		CASE_MACRO1(DICT_EN_AR, my_model.sz_dict_en_ar->str)
		CASE_MACRO1(DICT_EN_SV, my_model.sz_dict_en_sv->str)

		CASE_MACRO1(DICT_FR_EN, my_model.sz_dict_fr_en->str)
		CASE_MACRO1(DICT_FR_ES, my_model.sz_dict_fr_es->str)
		CASE_MACRO1(DICT_FR_DE, my_model.sz_dict_fr_de->str)
		CASE_MACRO1(DICT_FR_IT, my_model.sz_dict_fr_it->str)
		CASE_MACRO1(DICT_FR_PT, my_model.sz_dict_fr_pt->str)
		
		CASE_MACRO1(DICT_ES_EN, my_model.sz_dict_es_en->str)
		CASE_MACRO1(DICT_ES_FR, my_model.sz_dict_es_fr->str)
		CASE_MACRO1(DICT_ES_DE, my_model.sz_dict_es_de->str)
		CASE_MACRO1(DICT_ES_IT, my_model.sz_dict_es_it->str)
		CASE_MACRO1(DICT_ES_PT, my_model.sz_dict_es_pt->str)
		
		CASE_MACRO1(DICT_IT_EN, my_model.sz_dict_it_en->str)
		CASE_MACRO1(DICT_IT_FR, my_model.sz_dict_it_fr->str)
		CASE_MACRO1(DICT_IT_DE, my_model.sz_dict_it_de->str)
		CASE_MACRO1(DICT_IT_ES, my_model.sz_dict_it_es->str)
		CASE_MACRO1(DICT_IT_PT, my_model.sz_dict_it_pt->str)

		CASE_MACRO1(DICT_NO_EN, my_model.sz_dict_no_en->str)
		CASE_MACRO1(DICT_LAT_DE, my_model.sz_dict_lat_de->str)
		CASE_MACRO1(DICT_EN_LAT, my_model.sz_dict_en_lat->str)
		
		default:
			// g_message("get-model-char with unknown type\n");
			return NULL;
			break;
	}
	return NULL;
} // get_model_char



/**
 * @doc store a copy of the give char array to an internal string 
 * to hold it into the internal data model. The data flag is 
 * defined by the given parameter i.
 * 
 * @parameter i - enum-type describt the data model flag (like THES_IT }
 *
 * @parameter value - pointer to a char-array, terminated by '\0'.
 *         ( like "/usr/share/dict/thes_it.ding" )
 *
 * @return void 
 *
 * @author Heiko Abraham, <abrahamh@web.de>
 *
 * @see get_model_char set_model_int get_model_int set_model_bool
**/
void set_model_char(MYTYPE i, gchar *value) {
	struct stat buf;
		
	switch (i) {
		case MAIN_FONT: 
			g_string_printf( my_model.text_font, "%s", value );
			break;
		case MAIN_COLOR1:
			g_string_printf( my_model.text_color, "%s", value ); 
			break;
		case MAIN_COLOR2:
			g_string_printf( my_model.text_highlight, "%s", value ); 
			break;
		case MAIN_COLUMN1:
			g_string_printf( my_model.column1, "%s", value ); 
			break;
		case MAIN_COLUMN2:
			g_string_printf( my_model.column2, "%s", value ); 
			break;
		case MAIN_PREFIX:
			g_string_printf( my_model.main_prefix, "%s", value ); 
			break;
		case MAIN_LATEST_DICT:
			g_string_printf( my_model.main_latest_dict, "%s", value ); 
			break;
		
		VALUESET(THES_DE, my_model.sz_thes_de);
		VALUESET(THES_IT, my_model.sz_thes_it);
		VALUESET(THES_FR, my_model.sz_thes_fr);
		VALUESET(THES_EN, my_model.sz_thes_en);
		VALUESET(THES_ES, my_model.sz_thes_es);
		VALUESET(THES_PT, my_model.sz_thes_pt);
		VALUESET(THES_PL, my_model.sz_thes_pl);
		
		VALUESET(DICT_DE_EN, my_model.sz_dict_de_en);
		VALUESET(DICT_DE_ES, my_model.sz_dict_de_es);
		VALUESET(DICT_DE_FR, my_model.sz_dict_de_fr);
		VALUESET(DICT_DE_IT, my_model.sz_dict_de_it);
		VALUESET(DICT_DE_PT, my_model.sz_dict_de_pt);
		VALUESET(DICT_DE_NL, my_model.sz_dict_de_nl);
		
		VALUESET(DICT_EN_DE, my_model.sz_dict_en_de);
		VALUESET(DICT_EN_PT, my_model.sz_dict_en_pt);
		VALUESET(DICT_EN_IT, my_model.sz_dict_en_it);
		VALUESET(DICT_EN_FR, my_model.sz_dict_en_fr);
		VALUESET(DICT_EN_ES, my_model.sz_dict_en_es);
		VALUESET(DICT_EN_NO, my_model.sz_dict_en_no);
		VALUESET(DICT_EN_AR, my_model.sz_dict_en_ar);
		VALUESET(DICT_EN_SV, my_model.sz_dict_en_sv);
		
		VALUESET(DICT_FR_EN, my_model.sz_dict_fr_en);
		VALUESET(DICT_FR_ES, my_model.sz_dict_fr_es);
		VALUESET(DICT_FR_DE, my_model.sz_dict_fr_de);
		VALUESET(DICT_FR_IT, my_model.sz_dict_fr_it);
		VALUESET(DICT_FR_PT, my_model.sz_dict_fr_pt);
		
		VALUESET(DICT_ES_EN, my_model.sz_dict_es_en);
		VALUESET(DICT_ES_FR, my_model.sz_dict_es_fr);
		VALUESET(DICT_ES_DE, my_model.sz_dict_es_de);
		VALUESET(DICT_ES_IT, my_model.sz_dict_es_it);
		VALUESET(DICT_ES_PT, my_model.sz_dict_es_pt);
		
		VALUESET(DICT_IT_EN, my_model.sz_dict_it_en);
		VALUESET(DICT_IT_ES, my_model.sz_dict_it_es);
		VALUESET(DICT_IT_DE, my_model.sz_dict_it_de);
		VALUESET(DICT_IT_FR, my_model.sz_dict_it_fr);
		VALUESET(DICT_IT_PT, my_model.sz_dict_it_pt);
		
		VALUESET(DICT_LAT_DE, my_model.sz_dict_lat_de);
		VALUESET(DICT_EN_LAT, my_model.sz_dict_en_lat);
		VALUESET(DICT_NO_EN, my_model.sz_dict_no_en);
	
		default:
			g_message("set-model-char with unknown type\n");
			return;
			break;
	}
	return;
} // set_model_char


/**
 * @doc a little helper funtion to determ, if we a inside the debug
 * mode.
 * 
 * @return bool - return true, if debug mode is enabled, else false.
 *
 * @author Heiko Abraham, <abrahamh@web.de>
 *
 * @see get_model_bool
 *
**/
gboolean  get_debug() { 
	return my_model.main_debug; 
}


/**
 * @doc a little helper funtion to return the pointer to the main window
 *
 * @return pointer to the GTK_WIDGET of the main window
 * 
 * @author Heiko Abraham, <abrahamh@web.de>
 *
 * @see get_model_bool get_model_int
**/
GtkWidget *get_mainwin() { 
	return my_model.win_main;
}


/**
 * @doc funtion to allocated all neccessary memory for the 
 * internal data model.
 * 
 * @return void
 *
 * @author Heiko Abraham, <abrahamh@web.de>
 *
 * @see model_default model_free model_read model_write
 *
**/
void model_alloc() {
	my_model.text_font = g_string_new("");      
	my_model.text_color = g_string_new("");
	my_model.text_highlight = g_string_new("");
	my_model.column1 = g_string_new("");
	my_model.column2 = g_string_new("");
	my_model.main_prefix = g_string_new("");
	my_model.main_latest_dict = g_string_new("");
	
	// init all GString's (empty)
	my_model.sz_thes_de = g_string_new(""); 
	my_model.sz_thes_en = g_string_new("");
	my_model.sz_thes_es = g_string_new("");
	my_model.sz_thes_it = g_string_new("");
	my_model.sz_thes_fr = g_string_new("");
	my_model.sz_thes_pt = g_string_new("");
	my_model.sz_thes_pl = g_string_new("");
	
	my_model.sz_dict_de_en  = g_string_new("");
	my_model.sz_dict_de_es = g_string_new("");
	my_model.sz_dict_de_fr = g_string_new("");
	my_model.sz_dict_de_it = g_string_new("");
	my_model.sz_dict_de_pt = g_string_new("");
	my_model.sz_dict_de_nl = g_string_new("");
	
	my_model.sz_dict_en_de = g_string_new("");
	my_model.sz_dict_en_es = g_string_new("");
	my_model.sz_dict_en_fr = g_string_new("");
	my_model.sz_dict_en_pt = g_string_new("");
	my_model.sz_dict_en_it = g_string_new("");
	my_model.sz_dict_en_no = g_string_new("");
	my_model.sz_dict_en_ar = g_string_new("");
	my_model.sz_dict_en_sv = g_string_new("");
	
	my_model.sz_dict_fr_en = g_string_new("");
	my_model.sz_dict_fr_es = g_string_new("");
	my_model.sz_dict_fr_de = g_string_new("");
	my_model.sz_dict_fr_it = g_string_new("");
	my_model.sz_dict_fr_pt = g_string_new("");
	
	my_model.sz_dict_es_en = g_string_new("");
	my_model.sz_dict_es_fr = g_string_new("");
	my_model.sz_dict_es_de = g_string_new("");
	my_model.sz_dict_es_it = g_string_new("");
	my_model.sz_dict_es_pt = g_string_new("");
	
	my_model.sz_dict_it_en = g_string_new("");
	my_model.sz_dict_it_es = g_string_new("");
	my_model.sz_dict_it_de = g_string_new("");
	my_model.sz_dict_it_fr = g_string_new("");
	my_model.sz_dict_it_pt = g_string_new("");
	
	my_model.sz_dict_no_en = g_string_new("");
	my_model.sz_dict_lat_de = g_string_new("");
	my_model.sz_dict_en_lat = g_string_new("");
} // model_alloc();


/**
 * @doc free all allocated memory of the internal data model
 * This function is typical called before exit the application.
 *
 * @return void 
 * 
 * @author Heiko Abraham, <abrahamh@web.de>
 *
 * @see model_alloc model_default model_read model_write
 *
**/
void model_free() {
	g_string_free(my_model.text_font , TRUE);
	g_string_free(my_model.text_color , TRUE);
	g_string_free(my_model.text_highlight , TRUE);
	g_string_free(my_model.column1, TRUE);
	g_string_free(my_model.column2, TRUE);
	g_string_free(my_model.main_prefix, TRUE);
	// free all GString's
	g_string_free(my_model.sz_thes_de , TRUE);
	g_string_free(my_model.sz_thes_es , TRUE);
	g_string_free(my_model.sz_thes_en , TRUE);
	g_string_free(my_model.sz_thes_fr , TRUE);
	g_string_free(my_model.sz_thes_it , TRUE);
	g_string_free(my_model.sz_thes_pt , TRUE);
	g_string_free(my_model.sz_thes_pl , TRUE);
	g_string_free(my_model.main_latest_dict, TRUE);
	
	g_string_free(my_model.sz_dict_de_en , TRUE);
	g_string_free(my_model.sz_dict_de_es , TRUE);
	g_string_free(my_model.sz_dict_de_fr , TRUE);
	g_string_free(my_model.sz_dict_de_it , TRUE);
	g_string_free(my_model.sz_dict_de_pt , TRUE);
	g_string_free(my_model.sz_dict_de_nl , TRUE);
	
	g_string_free(my_model.sz_dict_en_de , TRUE);
	g_string_free(my_model.sz_dict_en_fr , TRUE);
	g_string_free(my_model.sz_dict_en_es , TRUE);
	g_string_free(my_model.sz_dict_en_pt , TRUE);
	g_string_free(my_model.sz_dict_en_it , TRUE);
	g_string_free(my_model.sz_dict_en_no , TRUE);
	g_string_free(my_model.sz_dict_en_ar , TRUE);
	g_string_free(my_model.sz_dict_en_sv , TRUE);
	
	g_string_free(my_model.sz_dict_fr_en , TRUE);
	g_string_free(my_model.sz_dict_fr_es , TRUE);
	g_string_free(my_model.sz_dict_fr_de , TRUE);
	g_string_free(my_model.sz_dict_fr_it , TRUE);
	g_string_free(my_model.sz_dict_fr_pt , TRUE);
	
	g_string_free(my_model.sz_dict_es_en , TRUE);
	g_string_free(my_model.sz_dict_es_fr , TRUE);
	g_string_free(my_model.sz_dict_es_de , TRUE);
	g_string_free(my_model.sz_dict_es_it , TRUE);
	g_string_free(my_model.sz_dict_es_pt , TRUE);
	
	g_string_free(my_model.sz_dict_it_en , TRUE);
	g_string_free(my_model.sz_dict_it_es , TRUE);
	g_string_free(my_model.sz_dict_it_de , TRUE);
	g_string_free(my_model.sz_dict_it_fr , TRUE);
	g_string_free(my_model.sz_dict_it_pt , TRUE);
	
	g_string_free(my_model.sz_dict_no_en , TRUE);
	g_string_free(my_model.sz_dict_lat_de , TRUE);
	g_string_free(my_model.sz_dict_en_lat , TRUE);
} // model_free



/**
 * @doc 
 * 
 * @author Heiko Abraham, <abrahamh@web.de>
 *
**/
void model_read(GConfClient *gconf) {
	if( gconf_client_get_bool (gconf, "/apps/gnome-ding/main_reset", NULL) == TRUE) {
		set_model_bool(MAIN_RESET, TRUE);
		set_model_int(MAIN_SIZE_X,  gconf_client_get_int (gconf, "/apps/gnome-ding/main_size_x", NULL) );
		set_model_int(MAIN_SIZE_Y,  gconf_client_get_int (gconf, "/apps/gnome-ding/main_size_y", NULL) );
		set_model_int(MAIN_SUGEST_MODE, gconf_client_get_int (gconf, "/apps/gnome-ding/main_sug_mode", NULL) );

		set_model_bool(MAIN_HIDDEN, gconf_client_get_bool(gconf, "/apps/gnome-ding/main_hidden", NULL) );
		set_model_bool(MAIN_DEBUG,  gconf_client_get_bool(gconf, "/apps/gnome-ding/debug", NULL));
	
		set_model_bool(MAIN_FONT_DEFAULT,   gconf_client_get_bool(gconf, "/apps/gnome-ding/font_default", NULL));
		set_model_bool(MAIN_COLOR_DEFAULT,  gconf_client_get_bool(gconf, "/apps/gnome-ding/color_default", NULL));
		
		set_model_bool(MAIN_EXACT,  gconf_client_get_bool(gconf, "/apps/gnome-ding/main_exact_words", NULL));
		set_model_bool(MAIN_CASE,   gconf_client_get_bool(gconf, "/apps/gnome-ding/main_case_sense", NULL));

		set_model_char(MAIN_FONT,   gconf_client_get_string( gconf, "/apps/gnome-ding/font", NULL) );
		set_model_char(MAIN_COLOR1, gconf_client_get_string( gconf, "/apps/gnome-ding/color1", NULL) );
		set_model_char(MAIN_COLOR2, gconf_client_get_string( gconf, "/apps/gnome-ding/color2", NULL) );
		set_model_char(MAIN_LATEST_DICT, gconf_client_get_string( gconf, "/apps/gnome-ding/main_latest", NULL) );
		
		
		set_model_bool(THES_ES, gconf_client_get_bool(gconf, "/apps/gnome-ding/thes/thes_es", NULL));	
		set_model_bool(THES_DE, gconf_client_get_bool(gconf, "/apps/gnome-ding/thes/thes_de", NULL))	;
		set_model_bool(THES_EN, gconf_client_get_bool(gconf, "/apps/gnome-ding/thes/thes_en", NULL));	
		set_model_bool(THES_IT, gconf_client_get_bool(gconf, "/apps/gnome-ding/thes/thes_it", NULL));	
		set_model_bool(THES_FR, gconf_client_get_bool(gconf, "/apps/gnome-ding/thes/thes_fr", NULL));	
		set_model_bool(THES_PT, gconf_client_get_bool(gconf, "/apps/gnome-ding/thes/thes_pt", NULL));	
		set_model_bool(THES_PL, gconf_client_get_bool(gconf, "/apps/gnome-ding/thes/thes_pl", NULL));	
		
		set_model_bool(ASPELL_AF, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_af", NULL) );	
		set_model_bool(ASPELL_BG, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_bg", NULL) );	
		set_model_bool(ASPELL_BR, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_br", NULL) );	
		set_model_bool(ASPELL_CA, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_ca", NULL) );	
		set_model_bool(ASPELL_CS, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_cs", NULL) );	
		set_model_bool(ASPELL_CY, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_cy", NULL) );	
		set_model_bool(ASPELL_DA, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_da", NULL) );	
		set_model_bool(ASPELL_DE, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_de", NULL) );	
		set_model_bool(ASPELL_CH, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_ch", NULL) );	
		set_model_bool(ASPELL_EO, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_eo", NULL) );	
		set_model_bool(ASPELL_ES, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_es", NULL) );	
		set_model_bool(ASPELL_EL, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_el", NULL) );	
		set_model_bool(ASPELL_FO, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_fo", NULL) );	
		set_model_bool(ASPELL_FR, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_fr", NULL) );	
		set_model_bool(ASPELL_FRCH, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_frch", NULL) );	
		set_model_bool(ASPELL_GA, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_ga", NULL) );	
		set_model_bool(ASPELL_GB, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_gb", NULL) );	
		set_model_bool(ASPELL_GL, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_gl", NULL) );	
		set_model_bool(ASPELL_HR, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_hr", NULL) );	
		set_model_bool(ASPELL_ID, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_id", NULL) );	
		set_model_bool(ASPELL_IS, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_is", NULL) );	
		set_model_bool(ASPELL_IT, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_it", NULL) );	
		set_model_bool(ASPELL_MI, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_mi", NULL) );	
		set_model_bool(ASPELL_MK, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_mk", NULL) );	
		set_model_bool(ASPELL_MS, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_ms", NULL) );	
		set_model_bool(ASPELL_MT, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_mt", NULL) );	
		set_model_bool(ASPELL_NL, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_nl", NULL) );	
		set_model_bool(ASPELL_NO, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_no", NULL) );	
		set_model_bool(ASPELL_PT, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_pt", NULL) );	
		set_model_bool(ASPELL_RU, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_ru", NULL) );	
		set_model_bool(ASPELL_RO, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_ro", NULL) );	
		set_model_bool(ASPELL_SV, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_sv", NULL) );	
		set_model_bool(ASPELL_SL, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_sl", NULL) );	
		set_model_bool(ASPELL_SK, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_sk", NULL) );	
		set_model_bool(ASPELL_UK, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_uk", NULL) );	
		set_model_bool(ASPELL_US, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_us", NULL) );	
		set_model_bool(ASPELL_UC, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_uc", NULL) );	
		set_model_bool(ASPELL_WA, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_wa", NULL) );	
		set_model_bool(ASPELL_ZU, gconf_client_get_bool(gconf, "/apps/gnome-ding/aspell/aspell_zu", NULL) );	

		set_model_bool(DICT_LAT_DE, gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_lat_de", NULL) );	
		set_model_bool(DICT_EN_LAT, gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_en_lat", NULL) );	
		
		set_model_bool(DICT_DE_EN, gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_de_en", NULL) );	
		set_model_bool(DICT_DE_ES, gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_de_es", NULL) );	
		set_model_bool(DICT_DE_FR ,gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_de_fr", NULL) );	
		set_model_bool(DICT_DE_IT ,gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_de_it", NULL) );	
		set_model_bool(DICT_DE_PT ,gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_de_pt", NULL) );	
		set_model_bool(DICT_DE_NL ,gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_de_nl", NULL) );	

		set_model_bool(DICT_EN_DE ,gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_en_de", NULL) );	
		set_model_bool(DICT_EN_IT ,gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_en_it", NULL) );	
		set_model_bool(DICT_EN_FR ,gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_en_fr", NULL) );	
		set_model_bool(DICT_EN_PT ,gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_en_pt", NULL) );	
		set_model_bool(DICT_EN_ES ,gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_en_es", NULL) );	
		set_model_bool(DICT_EN_NO ,gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_en_no", NULL) );	
		set_model_bool(DICT_EN_AR ,gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_en_ar", NULL) );	
		set_model_bool(DICT_EN_SV ,gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_en_sv", NULL) );	

		set_model_bool(DICT_FR_EN, gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_fr_en", NULL) );	
		set_model_bool(DICT_FR_ES, gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_fr_es", NULL) );	
		set_model_bool(DICT_FR_DE ,gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_fr_de", NULL) );	
		set_model_bool(DICT_FR_IT ,gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_fr_it", NULL) );	
		set_model_bool(DICT_FR_PT ,gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_fr_pt", NULL) );	
		
		set_model_bool(DICT_ES_EN, gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_es_en", NULL) );	
		set_model_bool(DICT_ES_FR, gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_es_fr", NULL) );	
		set_model_bool(DICT_ES_DE ,gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_es_de", NULL) );	
		set_model_bool(DICT_ES_IT ,gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_es_it", NULL) );	
		set_model_bool(DICT_ES_PT ,gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_es_pt", NULL) );	
		
		set_model_bool(DICT_IT_EN, gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_it_en", NULL) );	
		set_model_bool(DICT_IT_ES, gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_it_es", NULL) );	
		set_model_bool(DICT_IT_DE ,gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_it_de", NULL) );	
		set_model_bool(DICT_IT_FR ,gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_it_fr", NULL) );	
		set_model_bool(DICT_IT_PT ,gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_it_pt", NULL) );	
		
		set_model_bool(DICT_NO_EN ,gconf_client_get_bool(gconf, "/apps/gnome-ding/dict/dict_no_en", NULL) );	
		
		set_model_char(DICT_LAT_DE, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_lat_de", NULL) );
		set_model_char(DICT_EN_LAT, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_en_lat", NULL) );

		set_model_char(DICT_DE_EN, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_de_en", NULL) );
		set_model_char(DICT_DE_ES, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_de_es", NULL) );
		set_model_char(DICT_DE_FR, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_de_fr", NULL) );
		set_model_char(DICT_DE_IT, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_de_it", NULL) );
		set_model_char(DICT_DE_PT, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_de_pt", NULL) );
		set_model_char(DICT_DE_NL, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_de_nl", NULL) );
	
		set_model_char(DICT_FR_EN, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_fr_en", NULL) );
		set_model_char(DICT_FR_ES, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_fr_es", NULL) );
		set_model_char(DICT_FR_DE, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_fr_de", NULL) );
		set_model_char(DICT_FR_IT, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_fr_it", NULL) );
		set_model_char(DICT_FR_PT, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_fr_pt", NULL) );

		set_model_char(DICT_ES_EN, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_es_en", NULL) );
		set_model_char(DICT_ES_FR, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_es_fr", NULL) );
		set_model_char(DICT_ES_DE, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_es_de", NULL) );
		set_model_char(DICT_ES_IT, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_es_it", NULL) );
		set_model_char(DICT_ES_PT, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_es_pt", NULL) );

		set_model_char(DICT_IT_EN, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_it_en", NULL) );
		set_model_char(DICT_IT_ES, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_it_es", NULL) );
		set_model_char(DICT_IT_DE, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_it_de", NULL) );
		set_model_char(DICT_IT_FR, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_it_fr", NULL) );
		set_model_char(DICT_IT_PT, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_it_pt", NULL) );

		set_model_char(DICT_EN_DE, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_en_de", NULL) );
		set_model_char(DICT_EN_IT, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_en_it", NULL) );
		set_model_char(DICT_EN_ES, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_en_es", NULL) );
		set_model_char(DICT_EN_PT, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_en_pt", NULL) );
		set_model_char(DICT_EN_FR, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_en_fr", NULL) );
		set_model_char(DICT_EN_NO, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_en_no", NULL) );
		set_model_char(DICT_EN_AR, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_en_ar", NULL) );
		set_model_char(DICT_EN_SV, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_en_sv", NULL) );
	
		set_model_char(DICT_NO_EN, gconf_client_get_string( gconf, "/apps/gnome-ding/dict/file_dict_no_en", NULL) );
	
		set_model_char(THES_DE, gconf_client_get_string( gconf, "/apps/gnome-ding/thes/file_thes_de", NULL) );
		set_model_char(THES_ES, gconf_client_get_string( gconf, "/apps/gnome-ding/thes/file_thes_es", NULL)  );
		set_model_char(THES_EN, gconf_client_get_string( gconf, "/apps/gnome-ding/thes/file_thes_en", NULL)  );
		set_model_char(THES_IT, gconf_client_get_string( gconf, "/apps/gnome-ding/thes/file_thes_it", NULL)  );
		set_model_char(THES_FR, gconf_client_get_string( gconf, "/apps/gnome-ding/thes/file_thes_fr", NULL)  );
		set_model_char(THES_PT, gconf_client_get_string( gconf, "/apps/gnome-ding/thes/file_thes_pt", NULL)  );
		set_model_char(THES_PL, gconf_client_get_string( gconf, "/apps/gnome-ding/thes/file_thes_pl", NULL)  );
	} else {
		// set to default value
		model_default();
	}
} // model_read


/**
 * @doc 
 * 
 * @author Heiko Abraham, <abrahamh@web.de>
 *
**/
void model_write(GConfClient *gconf) {
	gconf_client_set_int (gconf, "/apps/gnome-ding/main_size_x", get_model_int(MAIN_SIZE_X), NULL);
    gconf_client_set_int (gconf, "/apps/gnome-ding/main_size_y", get_model_int(MAIN_SIZE_Y), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/main_hidden", get_model_bool(MAIN_HIDDEN), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/debug",  get_model_bool(MAIN_DEBUG), NULL);
	gconf_client_set_int (gconf, "/apps/gnome-ding/main_sug_mode", get_model_int(MAIN_SUGEST_MODE), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/main_exact_words",  get_model_bool(MAIN_EXACT), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/main_case_sense",  get_model_bool(MAIN_CASE), NULL);

	gconf_client_set_bool(gconf, "/apps/gnome-ding/color_default",  get_model_bool(MAIN_COLOR_DEFAULT), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/font_default",  get_model_bool(MAIN_FONT_DEFAULT), NULL);
	
	gconf_client_set_string(gconf, "/apps/gnome-ding/font", get_model_char(MAIN_FONT), NULL);	
	gconf_client_set_string(gconf, "/apps/gnome-ding/color1", get_model_char(MAIN_COLOR1), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/color2", get_model_char(MAIN_COLOR2), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/main_latest", get_model_char(MAIN_LATEST_DICT), NULL);
	
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_af", get_model_bool(ASPELL_AF), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_bg", get_model_bool(ASPELL_BG), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_br", get_model_bool(ASPELL_BR), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_ca", get_model_bool(ASPELL_CA), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_ch", get_model_bool(ASPELL_CH), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_cs", get_model_bool(ASPELL_CS), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_cy", get_model_bool(ASPELL_CY), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_da", get_model_bool(ASPELL_DA), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_de", get_model_bool(ASPELL_DE), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_el", get_model_bool(ASPELL_EL), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_eo", get_model_bool(ASPELL_EO), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_es", get_model_bool(ASPELL_ES), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_fo", get_model_bool(ASPELL_FO), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_fr", get_model_bool(ASPELL_FR), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_frch", get_model_bool(ASPELL_FRCH), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_ga", get_model_bool(ASPELL_GA), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_gb", get_model_bool(ASPELL_GB), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_gl", get_model_bool(ASPELL_GL), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_it", get_model_bool(ASPELL_IT), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_id", get_model_bool(ASPELL_ID), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_is", get_model_bool(ASPELL_IS), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_mi", get_model_bool(ASPELL_MI), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_mk", get_model_bool(ASPELL_MK), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_ms", get_model_bool(ASPELL_MS), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_mt", get_model_bool(ASPELL_MT), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_nl", get_model_bool(ASPELL_NL), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_pl", get_model_bool(ASPELL_PL), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_pt", get_model_bool(ASPELL_PT), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_ro", get_model_bool(ASPELL_RO), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_ru", get_model_bool(ASPELL_RU), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_sk", get_model_bool(ASPELL_SK), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_sl", get_model_bool(ASPELL_SL), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_sv", get_model_bool(ASPELL_SV), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_uc", get_model_bool(ASPELL_UC), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_uk", get_model_bool(ASPELL_UK), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_us", get_model_bool(ASPELL_US), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_wa", get_model_bool(ASPELL_WA), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/aspell/aspell_zu", get_model_bool(ASPELL_ZU), NULL);
	
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_de_en", get_model_bool(DICT_DE_EN), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_de_es", get_model_bool(DICT_DE_ES), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_de_fr", get_model_bool(DICT_DE_FR), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_de_it", get_model_bool(DICT_DE_IT), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_de_pt", get_model_bool(DICT_DE_PT), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_de_nl", get_model_bool(DICT_DE_NL), NULL);
	
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_fr_en", get_model_bool(DICT_FR_EN), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_fr_es", get_model_bool(DICT_FR_ES), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_fr_de", get_model_bool(DICT_FR_DE), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_fr_it", get_model_bool(DICT_FR_IT), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_fr_pt", get_model_bool(DICT_FR_PT), NULL);
	
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_es_en", get_model_bool(DICT_ES_EN), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_es_fr", get_model_bool(DICT_ES_FR), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_es_de", get_model_bool(DICT_ES_DE), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_es_it", get_model_bool(DICT_ES_IT), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_es_pt", get_model_bool(DICT_ES_PT), NULL);
	
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_it_en", get_model_bool(DICT_IT_EN), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_it_es", get_model_bool(DICT_IT_ES), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_it_de", get_model_bool(DICT_IT_DE), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_it_fr", get_model_bool(DICT_IT_FR), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_it_pt", get_model_bool(DICT_IT_PT), NULL);
	
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_en_de", get_model_bool(DICT_EN_DE), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_en_es", get_model_bool(DICT_EN_ES), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_en_fr", get_model_bool(DICT_EN_FR), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_en_it", get_model_bool(DICT_EN_IT), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_en_pt", get_model_bool(DICT_EN_PT), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_en_no", get_model_bool(DICT_EN_NO), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_en_ar", get_model_bool(DICT_EN_AR), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_en_sv", get_model_bool(DICT_EN_SV), NULL);
	
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_no_en", get_model_bool(DICT_NO_EN), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_lat_de", get_model_bool(DICT_LAT_DE), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/dict/dict_en_lat", get_model_bool(DICT_EN_LAT), NULL);
	
	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_de_en", get_model_char(DICT_DE_EN), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_de_it", get_model_char(DICT_DE_IT), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_de_es", get_model_char(DICT_DE_ES), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_de_fr", get_model_char(DICT_DE_FR), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_de_pt", get_model_char(DICT_DE_PT), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_de_nl", get_model_char(DICT_DE_NL), NULL);

	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_fr_en", get_model_char(DICT_FR_EN), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_fr_it", get_model_char(DICT_FR_IT), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_fr_es", get_model_char(DICT_FR_ES), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_fr_de", get_model_char(DICT_FR_DE), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_fr_pt", get_model_char(DICT_FR_PT), NULL);
	
	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_es_en", get_model_char(DICT_ES_EN), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_es_it", get_model_char(DICT_ES_IT), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_es_fr", get_model_char(DICT_ES_FR), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_es_de", get_model_char(DICT_ES_DE), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_es_pt", get_model_char(DICT_ES_PT), NULL);
	
	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_it_en", get_model_char(DICT_IT_EN), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_it_fr", get_model_char(DICT_IT_FR), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_it_es", get_model_char(DICT_IT_ES), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_it_de", get_model_char(DICT_IT_DE), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_it_pt", get_model_char(DICT_IT_PT), NULL);

	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_en_de", get_model_char(DICT_EN_DE), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_en_it", get_model_char(DICT_EN_IT), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_en_es", get_model_char(DICT_EN_ES), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_en_fr", get_model_char(DICT_EN_FR), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_en_pt", get_model_char(DICT_EN_PT), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_en_no", get_model_char(DICT_EN_NO), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_en_ar", get_model_char(DICT_EN_AR), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_en_sv", get_model_char(DICT_EN_SV), NULL);
	
	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_no_en", get_model_char(DICT_NO_EN), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_lat_de", get_model_char(DICT_LAT_DE), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/dict/file_dict_en_lat", get_model_char(DICT_EN_LAT), NULL);
	
	gconf_client_set_bool(gconf, "/apps/gnome-ding/thes/thes_en",  get_model_bool(THES_EN), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/thes/thes_es",  get_model_bool(THES_ES), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/thes/thes_it",  get_model_bool(THES_IT), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/thes/thes_de",  get_model_bool(THES_DE), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/thes/thes_fr",  get_model_bool(THES_FR), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/thes/thes_pt",  get_model_bool(THES_PT), NULL);
	gconf_client_set_bool(gconf, "/apps/gnome-ding/thes/thes_pl",  get_model_bool(THES_PL), NULL);
		
	gconf_client_set_string(gconf, "/apps/gnome-ding/thes/file_thes_de", get_model_char(THES_DE), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/thes/file_thes_en", get_model_char(THES_EN), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/thes/file_thes_it", get_model_char(THES_IT), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/thes/file_thes_fr", get_model_char(THES_FR), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/thes/file_thes_es", get_model_char(THES_ES), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/thes/file_thes_pt", get_model_char(THES_PT), NULL);
	gconf_client_set_string(gconf, "/apps/gnome-ding/thes/file_thes_pl", get_model_char(THES_PL), NULL);

	gconf_client_set_bool(gconf, "/apps/gnome-ding/main_reset",  get_model_bool(MAIN_RESET), NULL);
} // model_write



/**
 * @doc 
 * 
 * @author Heiko Abraham, <abrahamh@web.de>
 *
**/
void model_default() {
	// set std value for model
	my_model.win_size_x = 300;
	my_model.win_size_y = 200;
	
	// my_model.win_main = NULL;
	my_model.suggestion_mode = 3;
	my_model.main_hidden = FALSE;
	my_model.search_exact_words = FALSE;
	my_model.serach_case_sense = FALSE;
	my_model.main_debug = FALSE;
	my_model.main_reset = TRUE;
	my_model.text_font_default = TRUE;
	my_model.text_color_default = TRUE;
	my_model.column_num = 1;
	
	// todo: load themes-default settings
	g_string_printf(my_model.text_font, "Monospace 12"); 
	g_string_printf(my_model.text_color, "#000000000000"); 
	g_string_printf(my_model.text_highlight, "#00000000ffff"); 
	
	DEFAULT_MACRO(my_model.thes_de, my_model.sz_thes_de, "thesaurus-de.ding" );
	DEFAULT_MACRO(my_model.thes_en, my_model.sz_thes_en, "thesaurus-en.ding" );
	DEFAULT_MACRO(my_model.thes_es, my_model.sz_thes_es, "thesaurus-es.ding" );
	DEFAULT_MACRO(my_model.thes_fr, my_model.sz_thes_fr, "thesaurus-fr.ding" );
	DEFAULT_MACRO(my_model.thes_it, my_model.sz_thes_it, "thesaurus-it.ding" );
	DEFAULT_MACRO(my_model.thes_pt, my_model.sz_thes_pt, "thesaurus-pt.ding" );
	DEFAULT_MACRO(my_model.thes_pl, my_model.sz_thes_pl, "thesaurus-pl.ding" );

	DEFAULT_MACRO(my_model.dict_de_en, my_model.sz_dict_de_en, "de-en.ding" );
	DEFAULT_MACRO(my_model.dict_de_es, my_model.sz_dict_de_es, "de-es.ding" );
	DEFAULT_MACRO(my_model.dict_de_fr, my_model.sz_dict_de_fr, "de-fr.ding" );
	DEFAULT_MACRO(my_model.dict_de_it, my_model.sz_dict_de_it, "de-it.ding" );
	DEFAULT_MACRO(my_model.dict_de_pt, my_model.sz_dict_de_pt, "de-pt.ding" );
	DEFAULT_MACRO(my_model.dict_de_nl, my_model.sz_dict_de_nl, "de-nl.ding" );
	
	DEFAULT_MACRO(my_model.dict_en_de, my_model.sz_dict_en_de, "en-de.ding" );
	DEFAULT_MACRO(my_model.dict_en_lat, my_model.sz_dict_en_lat, "en-lat.ding" );
	DEFAULT_MACRO(my_model.dict_en_fr, my_model.sz_dict_en_fr, "en-fr.ding" );
	DEFAULT_MACRO(my_model.dict_en_es, my_model.sz_dict_en_es, "en-es.ding" );
	DEFAULT_MACRO(my_model.dict_en_it, my_model.sz_dict_en_it, "en-it.ding" );
	DEFAULT_MACRO(my_model.dict_en_pt, my_model.sz_dict_en_pt, "en-pt.ding" );
	DEFAULT_MACRO(my_model.dict_en_no, my_model.sz_dict_en_no, "en-no.ding" );
	DEFAULT_MACRO(my_model.dict_en_ar, my_model.sz_dict_en_ar, "en-ar.ding" );
	DEFAULT_MACRO(my_model.dict_en_sv, my_model.sz_dict_en_sv, "en-sv.ding" );
	
	DEFAULT_MACRO(my_model.dict_fr_en, my_model.sz_dict_fr_en, "fr-en.ding" );
	DEFAULT_MACRO(my_model.dict_fr_es, my_model.sz_dict_fr_es, "fr-es.ding" );
	DEFAULT_MACRO(my_model.dict_fr_de, my_model.sz_dict_fr_de, "fr-de.ding" );
	DEFAULT_MACRO(my_model.dict_fr_it, my_model.sz_dict_fr_it, "fr-it.ding" );
	DEFAULT_MACRO(my_model.dict_fr_pt, my_model.sz_dict_fr_pt, "fr-pt.ding" );
	
	DEFAULT_MACRO(my_model.dict_es_en, my_model.sz_dict_es_en, "es-en.ding" );
	DEFAULT_MACRO(my_model.dict_es_fr, my_model.sz_dict_es_fr, "es-fr.ding" );
	DEFAULT_MACRO(my_model.dict_es_de, my_model.sz_dict_es_de, "es-de.ding" );
	DEFAULT_MACRO(my_model.dict_es_it, my_model.sz_dict_es_it, "es-it.ding" );
	DEFAULT_MACRO(my_model.dict_es_pt, my_model.sz_dict_es_pt, "es-pt.ding" );
	
	DEFAULT_MACRO(my_model.dict_it_en, my_model.sz_dict_it_en, "it-en.ding" );
	DEFAULT_MACRO(my_model.dict_it_es, my_model.sz_dict_it_es, "it-es.ding" );
	DEFAULT_MACRO(my_model.dict_it_de, my_model.sz_dict_it_de, "it-de.ding" );
	DEFAULT_MACRO(my_model.dict_it_fr, my_model.sz_dict_it_fr, "it-fr.ding" );
	DEFAULT_MACRO(my_model.dict_it_pt, my_model.sz_dict_it_pt, "it-pt.ding" );
	
	DEFAULT_MACRO(my_model.dict_no_en, my_model.sz_dict_no_en, "no-en.ding" );
	DEFAULT_MACRO(my_model.dict_lat_de, my_model.sz_dict_lat_de, "lat-de.ding" );
	
	my_model.aspell_af = FALSE;
	my_model.aspell_bg = FALSE;
	my_model.aspell_br = FALSE;
	my_model.aspell_ca = FALSE;
	my_model.aspell_ch = FALSE;
	my_model.aspell_cs = FALSE;
	my_model.aspell_cy = FALSE;
	my_model.aspell_da = FALSE;
	my_model.aspell_de = TRUE;
	my_model.aspell_el = FALSE;
	my_model.aspell_eo = FALSE;
	my_model.aspell_es = FALSE;
	my_model.aspell_fo = FALSE;
	my_model.aspell_fr = FALSE;
	my_model.aspell_frch = FALSE;
	my_model.aspell_ga = FALSE;
	my_model.aspell_gb = TRUE;
	my_model.aspell_gl = FALSE;
	my_model.aspell_hr = FALSE;
	my_model.aspell_id = FALSE;
	my_model.aspell_is = FALSE;
	my_model.aspell_it = FALSE;
	my_model.aspell_mi = FALSE;
	my_model.aspell_mk = FALSE;
	my_model.aspell_ms = FALSE;
	my_model.aspell_mt = FALSE;
	my_model.aspell_nl = FALSE;
	my_model.aspell_no = FALSE;
	my_model.aspell_pl = FALSE;
	my_model.aspell_pt = FALSE;
	my_model.aspell_ro = FALSE;
	my_model.aspell_ru = FALSE;
	my_model.aspell_sk = FALSE;
	my_model.aspell_sl = FALSE;
	my_model.aspell_sv = FALSE;
	my_model.aspell_uc = FALSE;
	my_model.aspell_uk = FALSE;
	my_model.aspell_us = FALSE;
	my_model.aspell_wa = FALSE;
	my_model.aspell_zu = FALSE;

} // model_default


/**
 * @doc c-main function
 *     the main function init the gnome application, init i18n,
 *     set the default internal model settings and load the
 *     user defined setting stored by the gconf2-enviroments
 *
 * @parameter argc - number of arguments (unused at moment)
 * 
 * @parameter argv - array of arguments (unused at moment)
 * 
 * @return 0 - every time
 *
 * @author Heiko Abraham, <abrahamh@web.de>
 *
**/
int
main (int argc, char *argv[])
{
  GtkWidget *gnome_ding = NULL;
  GConfClient *gconf = NULL;

#ifdef ENABLE_NLS
  bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);
#endif

  	gnome_program_init (PACKAGE, VERSION, LIBGNOMEUI_MODULE,
                      argc, argv,
                      GNOME_PARAM_APP_DATADIR, PACKAGE_DATA_DIR,
                      NULL);
 
	// get settings from gconf
	gconf = gconf_client_get_default ();
    gconf_client_add_dir(gconf, "/apps/gnome-ding", GCONF_CLIENT_PRELOAD_RECURSIVE, NULL);

	// setup the internal program model
	model_alloc();
	set_model_char(MAIN_PREFIX, PACKAGE_PREFIX_DIR);
	model_default();
	model_read(gconf);
	

	// create mainwin
	my_model.win_main = NULL;
 	gnome_ding = create_ding_mainwin();
	my_model.win_main = gnome_ding;
	
	// set mainwin window size 
	gtk_window_set_default_size( GTK_WINDOW(gnome_ding), 
		get_model_int(MAIN_SIZE_X),
		get_model_int(MAIN_SIZE_Y) );
		
	// fill widgets (dictionary-list ...)
	on_search_history_wordlist_init();
 	on_search_combobox_fill( gnome_ding );
	on_treeview_set_font(gnome_ding);
	on_treeview_set_color1(gnome_ding);	
	
	// setup the default contex-menu
	set_model_int(MAIN_COLUMN_NUM, 0);
	on_contex_menu_show(gnome_ding);
	
	// show main window and start main-loop
	gtk_widget_show (gnome_ding);
  	gtk_main (); 
	
	// destroy application, store settings 
	if ( get_debug() == TRUE ) {
		g_print("%s\n", _("write config, exit"));
	}
	
	
	// write setting by gconf
	model_write(gconf);
	model_free();
	on_search_history_wordlist_delete();
	gconf_client_remove_dir (gconf, "/apps/gnome-ding", NULL);
	g_object_unref (gconf);
	
  	return 0;
} // main
