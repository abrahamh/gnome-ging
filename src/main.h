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
 * main.h
 *
 * begin                : Sat May 31 2003
 * copyright            : (C) 2003 by Heiko Abraham
 */
 
 #include <gnome.h>


#ifndef __MAIN_H_
#define __MAIN_H_

/**
 *
 * @doc follow struct represent the internal data model. 
 *      It contain all value and flags that are neccessary 
 *      for application configuration and to hold importend
 *      information about dictionaries and internal settings.
 *      <BR>This struct should not stored global, access 
 *      over get_model_ and set_model_ - funtions
 *
 * @see set_model_int set_model_bool set_model_char get_model_bool get_model_int get_model_char
**/
struct my_ding_data {
	gint win_size_x;		// x size of main window
	gint win_size_y;        // y size of main window
	
	GtkWidget *win_main;	// pointer to main window
	gint suggestion_mode;	// suggest mode for aspell
	gboolean main_hidden;	// hide/unhide mode of main window
	gboolean search_exact_words;	// thes/dict - search only exact words
	gboolean serach_case_sense;		// thes/dict - search case sensitive
	gboolean main_debug;	// debug mode (output to stdout)
	gboolean main_reset;	// reset-flag (false==on) - reset interal data model
	GString  *main_prefix;  // application prefix like "/usr/local/"
 	GString *main_latest_dict;   // string that contain the latest selected dictionary
	
	gint column_num;    	// num of column (for contex menu, copy result)
	GString *column1;		// String to hold left column cell content (contex)
	GString *column2;		// String to hold right column cell content (contex)
	
	GString *text_color;   // packed, unsigned, 8bit  {alpha, red, green, blue }
	GString *text_font;    // packed, unsigned, 8bit  {alpha, red, green, blue }
	GString *text_highlight;    // packed, unsigned, 8bit  {alpha, red, green, blue }
	gboolean text_font_default; // use theme font
	gboolean text_color_default; // use theme color
	
	// gboolean thes_LANGUAGE = flag, if LANGUAGE available and use inside
	//                          the selection combo box
	// GString sz_thes_LANGUAGE = filename of the thesaurus, or NULL or
	//                            "(NULL)" if not available
	
	gboolean thes_de;  // german
	GString *sz_thes_de;
	gboolean thes_en;  // english
	GString *sz_thes_en;
	gboolean thes_es;  // spanish
	GString *sz_thes_es;
	gboolean thes_fr;  // french
	GString *sz_thes_fr;
	gboolean thes_it;  // italian
	GString *sz_thes_it;
	gboolean thes_pt;  // pourtoges
	GString *sz_thes_pt;
	gboolean thes_pl;  // Polish
	GString *sz_thes_pl;

	gboolean dict_de_en;  // german - english
	GString *sz_dict_de_en;
	gboolean dict_de_es;  // german - spanish
	GString *sz_dict_de_es;
	gboolean dict_de_fr;  // german - french
	GString *sz_dict_de_fr;
	gboolean dict_de_pt;  // german - pourtoges
	GString *sz_dict_de_pt;
	gboolean dict_de_it;  // german - italian
	GString *sz_dict_de_it;
	gboolean dict_de_nl;  // german - dutch
	GString *sz_dict_de_nl;
	
	gboolean dict_en_de;  // english - german
	GString *sz_dict_en_de;
	gboolean dict_en_es;  // english - spanish
	GString *sz_dict_en_es;
	gboolean dict_en_fr;  // english - french
	GString *sz_dict_en_fr;
	gboolean dict_en_pt;  // english - pourtoges
	GString *sz_dict_en_pt;
	gboolean dict_en_it;  // english - italian
	GString *sz_dict_en_it;
	gboolean dict_en_no;  // english - norwegian
	GString *sz_dict_en_no;
	gboolean dict_en_ar;  // english - arabic
	GString *sz_dict_en_ar;
	gboolean dict_en_sv;  // english - swedish
	GString *sz_dict_en_sv;
	
	gboolean dict_fr_de;  // french - german
	GString *sz_dict_fr_de;
	gboolean dict_fr_es;  // french - spanish
	GString *sz_dict_fr_es;
	gboolean dict_fr_en;  // french - english
	GString *sz_dict_fr_en;
	gboolean dict_fr_pt;  // french - pourtoges
	GString *sz_dict_fr_pt;
	gboolean dict_fr_it;  // french - italian
	GString *sz_dict_fr_it;
	
	gboolean dict_es_de; 
	GString *sz_dict_es_de;
	gboolean dict_es_en; 
	GString *sz_dict_es_en;
	gboolean dict_es_pt;
	GString *sz_dict_es_pt;
	gboolean dict_es_fr;
	GString *sz_dict_es_fr;
	gboolean dict_es_it;
	GString *sz_dict_es_it;
	
	gboolean dict_it_de;
	GString *sz_dict_it_de;
	gboolean dict_it_en;
	GString *sz_dict_it_en;
	gboolean dict_it_fr;
	GString *sz_dict_it_fr;
	gboolean dict_it_es;
	GString *sz_dict_it_es;
	gboolean dict_it_pt;
	GString *sz_dict_it_pt;
	
	gboolean dict_no_en;  // norwegian - english
	GString *sz_dict_no_en;
	gboolean dict_lat_de; // latin - german
	GString *sz_dict_lat_de;
	gboolean dict_en_lat;	// englisch - latin	
	GString *sz_dict_en_lat;
	
	
	// gboolean aspell_ab;  // Abkhazian - unused
	// gboolean aspell_ae;  // Avestan - unused
	gboolean aspell_af;  // Afrikaans
	// gboolean aspell_an;  // Aragonese - unused
	// gboolean aspell_ar;  // Arabic - unused
	// gboolean aspell_as;  // Assamese - unused
	// gboolean aspell_ay;  // Aymara - unused
	// gboolean aspell_az;  // Azerbaijani - unused
	// gboolean aspell_ba;  // Bashkir - unused
	// gboolean aspell_be;  // Belarusian - unused
	gboolean aspell_bg;  // Bulgarian
	// gboolean aspell_bh;  // Bihari - unused
	// gboolean aspell_bn;  // Bengali - unused
	// gboolean aspell_bo;  // Tibetan - unused
	gboolean aspell_br;  // Breton
	// gboolean aspell_bs;  // Bosnian - unused
	gboolean aspell_ca;  // Catalan/Valencian
	// gboolean aspell_ce;  // Chechen - unused
	gboolean aspell_ch;  // german swiss
	// gboolean aspell_co;  // Corsican - unused
	// gboolean aspell_cr;  // Cree - unused
	gboolean aspell_cs;  // Czech
	gboolean aspell_cy;  // Welsh
	gboolean aspell_da;  // Danish
	gboolean aspell_de;  // german de_DE
	// gboolean aspell_dv;  // Divehi - unused
	// gboolean aspell_dz;  // Dzongkha - unused
	gboolean aspell_el;  // Greek
	// gboolean aspell_en;  // English - unused (see us,gb, uc)
	gboolean aspell_eo;  // Esperanto
	gboolean aspell_es;  // spanish
	// gboolean aspell_et;  // Estonian - unused
	// gboolean aspell_eu;  // Basque - unused
	// gboolean aspell_fa;  // Persian - unused
	// gboolean aspell_fi;  // Finnish - unused
	// gboolean aspell_fj;  // Fijian  - unused
	gboolean aspell_fo;  // Faroese
	gboolean aspell_fr;  // french
	gboolean aspell_frch;  // french, swiss
	// gboolean aspell_fy;  // Frisian - unused
	gboolean aspell_ga;  // Irish
	gboolean aspell_gb;  // english great brit.
	// gboolean aspell_gd;  // Scottish Gaelic - unused
	gboolean aspell_gl;  // Gallegan
	// gboolean aspell_gn;  // Guarani - unused
	// gboolean aspell_gu;  // Gujarati - unused
	// gboolean aspell_gv;  // Manx - unused
	// gboolean aspell_ha;  // Hausa - unused
	// gboolean aspell_he;  // Hebrew - unused
	// gboolean aspell_hi;  // Hindi - unused
	gboolean aspell_hr;  // Croatian
	// gboolean aspell_hu;  // Hungarian - unused
	// gboolean aspell_hy;  // Armenian - unused
	// gboolean aspell_ia;  // Interlingua - unused
	gboolean aspell_id;  // Indonesian 
	// gboolean aspell_io;  // Ido - unused
	gboolean aspell_is;  // Icelandic
	gboolean aspell_it;  // Italian
	// gboolean aspell_iu;  // Inuktitut - unused
	// gboolean aspell_ja;  // Japanese - unused
	// gboolean aspell_ka;  // Georgian - unused
	// gboolean aspell_kk;  // Kazakh - unused
	// gboolean aspell_kl;  // Kalaallisut/Greenlandic - unused
	// gboolean aspell_km;  // Khmer - unused
	// gboolean aspell_kn;  // Kannada - unused
	// gboolean aspell_ko;  // Korean - unused
	// gboolean aspell_kr;  // Kanuri - unused
	// gboolean aspell_ks;  // Kashmiri - unused
	// gboolean aspell_ku;  // Kurdish - unused
	// gboolean aspell_kv;  // Komi - unused
	// gboolean aspell_kw;  // Cornish - unused
	// gboolean aspell_ky;  // Kirghiz - unused
	// gboolean aspell_la;  // Latin - unused
	// gboolean aspell_lo;  // Lao - unused
	// gboolean aspell_lt;  // Lithuanian - unused
	// gboolean aspell_lv;  // Latvian - unused
	gboolean aspell_mi;  // Maori 
	gboolean aspell_mk;  // Makasar 
	// gboolean aspell_ml;  // Malayalam - unused
	// gboolean aspell_mn;  // Mongolian - unused
	// gboolean aspell_mo;  // Moldavian - unused
	// gboolean aspell_mr;  // Marathi - unused
	gboolean aspell_ms;  // Malay 
	gboolean aspell_mt;  // Maltese 
	// gboolean aspell_my;  // Burmese - unused
	// gboolean aspell_nb;  // Norwegian Bokmal - unused
	// gboolean aspell_ne;  // Nepali - unused
	gboolean aspell_nl;  // Dutch
	// gboolean aspell_nn;  // Norwegian Nynorsk - unused
	gboolean aspell_no;  // Norwegian
	// gboolean aspell_nv;  // Navajo - unused
	// gboolean aspell_oc;  // Occitan/Provencal - unused
	// gboolean aspell_oj;  // Ojibwa - unused
	// gboolean aspell_or;  // Oriya - unused
	// gboolean aspell_os;  // Ossetic - unused
	// gboolean aspell_pa;  // Punjabi - unused
	// gboolean aspell_pi;  // Pali - unused
	gboolean aspell_pl;  // Polish
	// gboolean aspell_ps;  // Pushto - unused
	gboolean aspell_pt;  // Portuguese
	// gboolean aspell_qu;  // Quechua - unused
	// gboolean aspell_rm;  // Raeto-Romance - unused
	gboolean aspell_ro;  // Romanian
	gboolean aspell_ru;  // Russian
	// gboolean aspell_sa;  // Sanskrit - unused
	// gboolean aspell_sd;  // Sindhi - unused
	// gboolean aspell_se;  // Northern Sami - unused
	gboolean aspell_sk;  // Slovak
	gboolean aspell_sl;  // Slovenian
	// gboolean aspell_sn;  // Shona - unused
	// gboolean aspell_so;  // Somali - unused
	// gboolean aspell_sq;  // Albanian - unused
	// gboolean aspell_sr;  // Serbian - unused
	// gboolean aspell_su;  // Sundanese - unused
	gboolean aspell_sv;  // Swedish
	// gboolean aspell_sw;  // Swahili - unused
	// gboolean aspell_ta;  // Tamil -unused
	// gboolean aspell_te;  // Telugu - unused
	// gboolean aspell_tg;  // Tajik - unused
	// gboolean aspell_tk;  // Turkmen - unused
	// gboolean aspell_tl;  // Tagalog - unused
	// gboolean aspell_tr;  // Turkish - unused
	// gboolean aspell_tt;  // Tatar - unused
	// gboolean aspell_ty;  // Tahitian - unused
	gboolean aspell_uc;  // english canadian
	// gboolean aspell_ug;  // Uighur - unused
	gboolean aspell_uk;  // Ukrainian
	// gboolean aspell_ur;  // Urdu - unused
	gboolean aspell_us;  // english american
	// gboolean aspell_uz;  // Uzbek - unused
	// gboolean aspell_vi;  // Vietnamese - unused
	// gboolean aspell_vo;  // Volapuk - unused
	gboolean aspell_wa;  // Walloon
	// gboolean aspell_yi;  // Yiddish - unused
	// gboolean aspell_yo;  // Yoruba - unused
	gboolean aspell_zu;  // Zulu
	// future  gboolean aspell_aa   Afar
	// future  gboolean aspell_ak   Akan
	// future  gboolean aspell_av   Avaric
	// future  gboolean aspell_bi   Bislama
	// future  gboolean aspell_bm   Bambara
	// future  gboolean aspell_cu   Old Slavonic
	// future  gboolean aspell_ee   Ewe
	// future  gboolean aspell_ff   Fulah
	// future  gboolean aspell_ho   Hiri Motu
	// future  gboolean aspell_ht   Haitian Creole
	// future  gboolean aspell_hz   Herero
	// future  gboolean aspell_ie   Interlingue
	// future  gboolean aspell_ig   Igbo
	// future  gboolean aspell_ii   Sichuan Yi
	// future  gboolean aspell_ik   Inupiaq
	// future  gboolean aspell_kg   Kongo
	// future  gboolean aspell_ki   Kikuyu/Gikuyu
	// future  gboolean aspell_kj   Kwanyama
	// future  gboolean aspell_lb   Luxembourgish
	// future  gboolean aspell_lg   Ganda
	// future  gboolean aspell_li   Limburgan
	// future  gboolean aspell_ln   Lingala
	// future  gboolean aspell_lu   Luba-Katanga
	// future  gboolean aspell_mg   Malagasy
	// future  gboolean aspell_mh   Marshallese
	// future  gboolean aspell_na   Nauru
	// future  gboolean aspell_nd   North Ndebele
	// future  gboolean aspell_ng   Ndonga
	// future  gboolean aspell_nr   South Ndebele
	// future  gboolean aspell_ny   Nyanja
	// future  gboolean aspell_rn   Rundi
	// future  gboolean aspell_rw   Kinyarwanda
	// future  gboolean aspell_sc   Sardinian
	// future  gboolean aspell_sg   Sango
	// future  gboolean aspell_si   Sinhalese
	// future  gboolean aspell_sm   Samoan
	// future  gboolean aspell_ss   Swati
	// future  gboolean aspell_st   Southern Sotho
	// future  gboolean aspell_tn   Tswana
	// future  gboolean aspell_to   Tonga
	// future  gboolean aspell_ts   Tsonga
	// future  gboolean aspell_tw   Twi
	// future  gboolean aspell_ve   Venda
	// future  gboolean aspell_wo   Wolof
	// future  gboolean aspell_xh   Xhosa
	// future  gboolean aspell_za   Zhuang
};	


/**
 *
 * @doc follow enum-type is used to store/return a value to/from
 *      the internal data model by using get_model_{bool,int,char}
 *      or set_model_{bool,int,char}
 *
 * @see set_model_bool get_model_bool
 *
**/
typedef enum _mytype {
	MAIN_SIZE_X,
	MAIN_SIZE_Y,
	MAIN_SUGEST_MODE,
	MAIN_HIDDEN,
	MAIN_EXACT,
	MAIN_CASE,
	MAIN_DEBUG,
	MAIN_RESET,
	MAIN_PREFIX,
	MAIN_FONT,
	MAIN_FONT_DEFAULT,
	MAIN_COLOR_DEFAULT,
	MAIN_COLOR1,
	MAIN_COLOR2,
	MAIN_COLUMN_NUM,
	MAIN_COLUMN1,
	MAIN_COLUMN2,
	MAIN_LATEST_DICT,
	
	// Flags for Aspell
	ASPELL_AA,  // "aa", _("Afar"),
	ASPELL_AB,  // "ab", _("Abkhazian"),
	ASPELL_AE,  // "ae", _("Avestan"), 
	ASPELL_AF,  // "af", _("Afrikaans"),
	ASPELL_AK,  // "ak", _("Akan"),
	ASPELL_AN,  // "an", -("Aragonese"),
	ASPELL_AR,  // "ar", _("Arabic"), 
	ASPELL_AS,  // "as", _("Assamese"),
	ASPELL_AV,  // "av", _("Avaric"),
	ASPELL_AY,  // "ay",  _("Aymara"),
	ASPELL_AZ,  // "az",  _("Azerbaijani"),
	ASPELL_BA,  // "ba",  _("Bashkir"),
	ASPELL_BE,  // "be",  _("Belarusian"),
	ASPELL_BG,  // "bg",  _("Bulgarian"),
	ASPELL_BH,  // "bh",  _("Bihari"),
	ASPELL_BI,  // "bi", _("Bislama"),
	ASPELL_BM,  // "bm", _("Bambara"),
	ASPELL_BN,  // "bn",  _("Bengali"),
	ASPELL_BO,  // "bo",  _("Tibetan"),
	ASPELL_BR,  // "br",  _("Breton"),
	ASPELL_BS,  // "bs",  _("Bosnian"),
	ASPELL_CA,  // "ca",  _("Catalan/Valencian"),
	ASPELL_CE,  // "ce",  _("Chechen"),
	ASPELL_CH,  // german swiss
	ASPELL_CO,  // "co",  _("Corsican"),
	ASPELL_CR,  // "cr",  _("Cree"),
	ASPELL_CS,  // "cs",  _("Czech"),
	ASPELL_CU,  // "cu", _("Old Slavonic"),
	ASPELL_CV,  // "ee", _("Ewe"),
	ASPELL_CY,  // "cy",  _("Welsh"),
	ASPELL_DA,  // "da",  _("Danish"),
	ASPELL_DE,  // "de",  _("German"),
	ASPELL_DV,  // "dv",  _("Divehi"),
	ASPELL_DZ,  // "dz",  _("Dzongkha"),
	ASPELL_EE,
	ASPELL_EL,  // "el",  _("Greek"),
	ASPELL_EN,  // "en",  _("English"),
	ASPELL_EO,  // "eo",  _("Esperanto"),
	ASPELL_ES,  // "es",  _("Spanish"),
	ASPELL_ET,  // "et",  _("Estonian"),
	ASPELL_EU,  // "eu",  _("Basque"),
	ASPELL_FA,  // "fa",  _("Persian"),
	ASPELL_FF,  // "ff", _("Fulah"),
	ASPELL_FI,  // "fi",  _("Finnish"),
	ASPELL_FJ,  // "fj",  _("Fijian"),
	ASPELL_FO,  // "fo",  _("Faroese"),
	ASPELL_FR,  // "fr",  _("France"),
	ASPELL_FRCH, // french, swiss
	ASPELL_FY,  // "fy",  _("Frisian"),
	ASPELL_GA,  // ga",  _("Irish"),
	ASPELL_GB,  // english, great brit.
	ASPELL_GD,  // "gd",  _("Scottish Gaelic"),
	ASPELL_GL,  // "gl",  _("Gallegan"),
	ASPELL_GN,  // "gn",  _("Guarani"),
	ASPELL_GU,  // "gu",  _("Gujarati"),
	ASPELL_GV,  // "gv",  _("Manx"),
	ASPELL_HA,  // "ha",  _("Hausa"),
	ASPELL_HE,  // "he",  _("Hebrew"),
	ASPELL_HI,  // "hi",  _("Hindi"),
	ASPELL_HO,  // "ho", _("Hiri Motu"),
	ASPELL_HR,  // "hr",  _("Croatian"),
	ASPELL_HT,  // "ht", _("Haitian Creole"),
	ASPELL_HU,  // "hu",  _("Hungarian"),
	ASPELL_HY,  // "hy",  _("Armenian"),
	ASPELL_HZ,  // "hz", _("Herero"),
	ASPELL_IA,  // "ia",  _("Interlingua"),
	ASPELL_ID,  // "id",  _("Indonesian"), 
	ASPELL_IE,  // "ie", _("Interlingue"),
	ASPELL_IG,  // "ig", _("Igbo"),
	ASPELL_II,  // "ii", _("Sichuan Yi"),
	ASPELL_IK,  // "ik", _("Inupiaq"),
	ASPELL_IO,  // "io",  _("Ido"),
	ASPELL_IS,  // "is",  _("Icelandic"),
	ASPELL_IT,  // "it",  _("Italian"),
	ASPELL_IU,  // "iu",  _("Inuktitut"),
	ASPELL_JA,  // "ja",  -("Japanese"),
	ASPELL_JV,
	ASPELL_KA,  // "ka",  _("Georgian"),
	ASPELL_KG,  // "kg", _("Kongo"),
	ASPELL_KI,  // "ki", _("Kikuyu/Gikuyu"),
	ASPELL_KJ,  // "kj", _("Kwanyama"),
	ASPELL_KK,  // "kk",  _("Kazakh"),
	ASPELL_KL,  // "kl",  _("Kalaallisut/Greenlandic"),
	ASPELL_KM,  // "km",  _("Khmer"),
	ASPELL_KN,  // "kn",  _("Kannada"),
	ASPELL_KO,  // "ko",  _("Korean"),
	ASPELL_KR,  // "kr",  _("Kanuri"),
	ASPELL_KS,  // "ks",  _("Kashmiri"),
	ASPELL_KU,  // "ku",  _("Kurdish"),
	ASPELL_KV,  // "kv",  _("Komi"),
	ASPELL_KW,  // "kw",  _("Cornish"),
	ASPELL_KY,  // "ky",  _("Kirghiz"),
	ASPELL_LA,  // "la",  _("Latin"),
	ASPELL_LB,  // "lb", _("Luxembourgish"),
	ASPELL_LG,  // "lg", _("Ganda"),
	ASPELL_LI,  // "li", _("Limburgan"),
	ASPELL_LN,  // "ln", _("Lingala"),
	ASPELL_LO,  // "lo",  _("Lao"),
	ASPELL_LT,  // "lt",  _("Lithuanian"),
	ASPELL_LU,  // "lu", _("Luba-Katanga"),
	ASPELL_LV,  // "lv",  _("Latvian"),
	ASPELL_MG,  // "mg", _("Malagasy"),
	ASPELL_MH,  // "mh", _("Marshallese"),
	ASPELL_MI,  // "mi",  _("Maori"),
	ASPELL_MK,  // "mk",  _("Makasar"),
	ASPELL_ML,  // "ml",  _("Malayalam"),
	ASPELL_MN,  // "mn",  _("Mongolian"),
	ASPELL_MO,  // "mo",  _("Moldavian"),
	ASPELL_MR,  // "mr",  _("Marathi"),
	ASPELL_MS,  // "ms",  _("Malay"),
	ASPELL_MT,  // "mt",  _("Maltese"),
	ASPELL_MY,  // "my",  _("Burmese"),
	ASPELL_NA,  // "na", _("Nauru"),
	ASPELL_NB,  // "nb",  _("Norwegian Bokmal"),
	ASPELL_ND,  // "nd", _("North Ndebele"),
	ASPELL_NE,  // "ne",  _("Nepali"),
	ASPELL_NG,  // "ng", _("Ndonga"),
	ASPELL_NL,  // "nl",  _("Dutch"),
	ASPELL_NN,  // "nn",  _("Norwegian Nynorsk"),
	ASPELL_NO,  // "no",  _("Norwegian"),
	ASPELL_NR,  // "nr", _("South Ndebele"),
	ASPELL_NV,  // "nv",  _("Navajo"),
	ASPELL_NY,  // "ny", _("Nyanja"),
	ASPELL_OC,  // "oc",  _("Occitan/Provencal"),
	ASPELL_OJ,  // "oj",  _("Ojibwa"),
	ASPELL_OR,  // "or",  _("Oriya"),
	ASPELL_OS,  // "os",  _("Ossetic"),
	ASPELL_PA,  // "pa",  _("Punjabi"),
	ASPELL_PI,  // "pi",  _("Pali"),
	ASPELL_PL,  // "pl",  _("Polish"),
	ASPELL_PS,  // "ps",  _("Pushto"),
	ASPELL_PT,  // "pt",  _("Portuguese"),
	ASPELL_QU,  // "qu",  _("Quechua"),
	ASPELL_RN,  // "rn",  _("Rundi"),
	ASPELL_RM,  // "rm",  _("Raeto-Romance"),
	ASPELL_RO,  // "ro",  _("Romanian"),
	ASPELL_RU,  // "ru",  _("Russian"),
	ASPELL_RW,  // "rw", _("Kinyarwanda"),
	ASPELL_SA,  // "sa",  _("Sanskrit"),
	ASPELL_SC,  // "sc", _("Sardinian"),
	ASPELL_SD,  // "sd",  _("Sindhi"),
	ASPELL_SE,  // "se",  _("Northern Sami"),
	ASPELL_SG,  // "sg", _("Sango"),
	ASPELL_SI,  // "si", _("Sinhalese"),
	ASPELL_SK,  // "sk",  _("Slovak"),
	ASPELL_SL,  // "sl",  _("Slovenian"),
	ASPELL_SM,  // "sm", _("Samoan"),
	ASPELL_SN,  // "sn",  _("Shona"),
	ASPELL_SO,  // "so",  _("Somali"),
	ASPELL_SQ,  // "sq",  _("Albanian"),
	ASPELL_SR,  // "sr",  _("Serbian"),
	ASPELL_SS,  // "ss", _("Swati"),
	ASPELL_ST,  // "st", _("Southern Sotho"),
	ASPELL_SU,  // "su",  _("Sundanese"),
	ASPELL_SV,  // "sv",  _("Swedish"),
	ASPELL_SW,  // "sw",  _("Swahili"),
	ASPELL_TA,  // "ta",  _("Tamil"),
	ASPELL_TE,  // "te",  _("Telugu"),
	ASPELL_TG,  // "tg",  _("Tajik"),
	ASPELL_TK,  // "tk",  _("Turkmen"),
	ASPELL_TL,  // "tl",  _("Tagalog"),
	ASPELL_TN,  // "tn", _("Tswana"),
	ASPELL_TO,  // "to", _("Tonga"),
	ASPELL_TR,  // "tr",  _("Turkish"),
	ASPELL_TS,  // "ts", _("Tsonga"),
	ASPELL_TT,  // "tt",  _("Tatar"),
	ASPELL_TW,  // "tw", _("Twi"),
	ASPELL_TY,  // "ty",  _("Tahitian"),
	ASPELL_UC,  // 	"uc",  _("English/Canadian"),
	ASPELL_UG,  // "ug",  _("Uighur"),
	ASPELL_UK,  // "uk",  _("Ukrainian"),
	ASPELL_US,  // "us",  _("English/American"),
	ASPELL_UR,  // "ur",  _("Urdu"),
	ASPELL_UZ,  // "uz",  _("Uzbek"), 
	ASEPLL_VI,  // "vi",  _("Vietnamese"),
	ASPELL_VE,  // "ve", _("Venda"),
	ASPELL_VO,  // "vo",  _("Volapuk"),
	ASPELL_WA,  // "wa",  _("Walloon"),
	ASPELL_WO,  // "wo", _("Wolof"),
	ASPELL_XH,  // "xh", _("Xhosa"),
	ASPELL_YI,  // "yi",  _("Yiddish"),
	ASPELL_YO,  // "yo",  _("Yoruba"),
	ASPELL_ZA,  // "za", _("Zhuang")
	ASPELL_ZU,  // "zu",  _("Zulu")

	// ######### Flage for Thesaurus
	THES_DE,  	// german
	THES_IT,  	// italian
	THES_FR,  	// french
	THES_EN, 	// english
	THES_ES,	// spanish
	THES_PT,	// pourtoges
	THES_PL,	// plolish
	
	// ######## Flags for dictionaries
	DICT_DE_EN, 	// german-english
	DICT_DE_ES,		// german-spanish
	DICT_DE_FR,		// german-frensh
	DICT_DE_IT,		// german-italian
	DICT_DE_PT,		// german-pourtoges
	DICT_DE_NL,		// german-dutch
	
	DICT_EN_DE,		// english-german
	DICT_EN_FR,		// english-french
	DICT_EN_ES,		// english-spanish
	DICT_EN_IT,		// english-italian
	DICT_EN_PT,		// english-pourtoges
	DICT_EN_NO,		// english-norwegian
	DICT_EN_AR,		// english-arabic
	DICT_EN_SV,		// english-swedish

	DICT_FR_DE,		// french-german
	DICT_FR_EN,		// french-english
	DICT_FR_ES,		// french-spanish
	DICT_FR_IT,		// french-italian
	DICT_FR_PT,		// french-pourtoges

	DICT_ES_DE,
	DICT_ES_EN,
	DICT_ES_FR,
	DICT_ES_IT,
	DICT_ES_PT,
	
	DICT_IT_DE,
	DICT_IT_EN,
	DICT_IT_FR,
	DICT_IT_ES,
	DICT_IT_PT,
	
	DICT_NO_EN,		// norwegian
	DICT_LAT_DE,	// later-german
	DICT_EN_LAT,	// english-german
	
	END_MARKER		// unused
} MYTYPE;


/* some helper functions to commit model data */
gint get_model_int(MYTYPE i);
void set_model_int(MYTYPE i, gint value);

gboolean get_model_bool(MYTYPE i);
void set_model_bool(MYTYPE i, gboolean value);

gchar *get_model_char(MYTYPE i);
void set_model_char(MYTYPE i, gchar *value);

void model_default();

gboolean  get_debug();
GtkWidget *get_mainwin();

#endif // __MAIN_H_
