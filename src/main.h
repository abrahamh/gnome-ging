#include <gnome.h>


#ifndef __MAIN_H_
#define __MAIN_H_

struct my_ding_data {
	gint win_size_x;
	gint win_size_y;
	
	GtkWidget *win_main;
	gint suggestion_mode;
	gboolean main_hidden;
	gboolean search_exact_words;
	gboolean serach_case_sense;
	gboolean main_debug;
	gboolean main_reset;
	
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
	
	gboolean dict_de_en;  // german - english
	GString *sz_dict_de_en;
	gboolean dict_de_es;  // german - spanish
	GString *sz_dict_de_es;
	gboolean dict_de_fr;  // german - french
	GString *sz_dict_de_fr;
	gboolean dict_de_pt;  
	GString *sz_dict_de_pt;
	gboolean dict_de_it;  
	GString *sz_dict_de_it;
	
	gboolean dict_en_de;  
	GString *sz_dict_en_de;
	gboolean dict_en_es;  
	GString *sz_dict_en_es;
	gboolean dict_en_fr;  
	GString *sz_dict_en_fr;
	gboolean dict_en_pt;  
	GString *sz_dict_en_pt;
	gboolean dict_en_it;  
	GString *sz_dict_en_it;
	gboolean dict_en_no;  
	GString *sz_dict_en_no;
	
	gboolean dict_no_en;  
	GString *sz_dict_no_en;
	gboolean dict_lat_de; // latin - german
	GString *sz_dict_lat_de;
	
	
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
	// gboolean aspell_en;  // English - unused (use us,gb, uc)
	gboolean aspell_eo;  // Esperanto
	gboolean aspell_es;  // spanish
	// gboolean aspell_et;  // Estonian - unused
	// gboolean aspell_eu;  // Basque - unused
	// gboolean aspell_fa;  // Persian - unused
	// gboolean aspell_fi;  // Finnish - unused
	// gboolean aspell_fj;  // Fijian  - unused
	gboolean aspell_fo;  // Faroese
	gboolean aspell_fr;  // french
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
	// gboolean aspell_id;  // Indonesian - unused
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
	// gboolean aspell_mi;  // Maori - unused
	// gboolean aspell_mk;  // Makasar - unused
	// gboolean aspell_ml;  // Malayalam - unused
	// gboolean aspell_mn;  // Mongolian - unused
	// gboolean aspell_mo;  // Moldavian - unused
	// gboolean aspell_mr;  // Marathi - unused
	// gboolean aspell_ms;  // Malay - unused
	// gboolean aspell_mt;  // Maltese - unused
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


typedef enum _mytype {
	MAIN_SIZE_X,
	MAIN_SIZE_Y,
	MAIN_SUGEST_MODE,
	MAIN_HIDDEN,
	MAIN_EXACT,
	MAIN_CASE,
	MAIN_DEBUG,
	MAIN_RESET,
	ASPELL_AA,  // Flags for Aspell
	ASPELL_AB,
	ASPELL_AE,
	ASPELL_AF,
	ASPELL_AK,
	ASPELL_AN,
	ASPELL_AR,
	ASPELL_AS,
	ASPELL_AV,
	ASPELL_AY,
	ASPELL_AZ,
	ASPELL_BA,
	ASPELL_BE,
	ASPELL_BG,
	ASPELL_BH,
	ASPELL_BI,
	ASPELL_BM,
	ASPELL_BN,
	ASPELL_BO,
	ASPELL_BR,
	ASPELL_BS,
	ASPELL_CA,
	ASPELL_CE,
	ASPELL_CH,
	ASPELL_CO,
	ASPELL_CR,
	ASPELL_CS,
	ASPELL_CU,
	ASPELL_CV,
	ASPELL_CY,
	ASPELL_DA,
	ASPELL_DECH, // german swiss
	ASPELL_DE, // german, german
	ASPELL_DV,
	ASPELL_DZ,
	ASPELL_EE,
	ASPELL_EL,
	ASPELL_EN,
	ASPELL_EO,
	ASPELL_ES,
	ASPELL_ET,
	ASPELL_EU,
	ASPELL_FA,
	ASPELL_FF,
	ASPELL_FI,
	ASPELL_FJ,
	ASPELL_FO,
	ASPELL_FR,
	ASPELL_FY,
	ASPELL_GA,
	ASPELL_GB, // english, great brit.
	ASPELL_GD,
	ASPELL_GL,
	ASPELL_GN,
	ASPELL_GU,
	ASPELL_GV,
	ASPELL_HA,
	ASPELL_HE,
	ASPELL_HI,
	ASPELL_HO,
	ASPELL_HR,
	ASPELL_HT,
	ASPELL_HU,
	ASPELL_HY,
	ASPELL_HZ,
	ASPELL_IA,
	ASPELL_ID,
	ASPELL_IE,
	ASPELL_IG,
	ASPELL_II,
	ASPELL_IK,
	ASPELL_IO,
	ASPELL_IS,
	ASPELL_IT,
	ASPELL_IU,
	ASPELL_JA,
	ASPELL_JV,
	ASPELL_KA,
	ASPELL_KG,
	ASPELL_KI,
	ASPELL_KJ,
	ASPELL_KK,
	ASPELL_KL,
	ASPELL_KM,
	ASPELL_KN,
	ASPELL_KO,
	ASPELL_KR,
	ASPELL_KS,
	ASPELL_KU,
	ASPELL_KV,
	ASPELL_KW,
	ASPELL_KY,
	ASPELL_LA,
	ASPELL_LB,
	ASPELL_LG,
	ASPELL_LI,
	ASPELL_LN,
	ASPELL_LO,
	ASPELL_LT,
	ASPELL_LU,
	ASPELL_LV,
	ASPELL_MG,
	ASPELL_MH,
	ASPELL_MI,
	ASPELL_MK,
	ASPELL_ML,
	ASPELL_MN,
	ASPELL_MO,
	ASPELL_MR,
	ASPELL_MS,
	ASPELL_MT,
	ASPELL_MY,
	ASPELL_NA,
	ASPELL_NB,
	ASPELL_ND,
	ASPELL_NE,
	ASPELL_NG,
	ASPELL_NL,
	ASPELL_NN,
	ASPELL_NO,
	ASPELL_NR,
	ASPELL_NV,
	ASPELL_NY,
	ASPELL_OC,
	ASPELL_OJ,
	ASPELL_OR,
	ASPELL_OS,
	ASPELL_PA,
	ASPELL_PI,
	ASPELL_PL,
	ASPELL_PS,
	ASPELL_PT,
	ASPELL_RM,
	ASPELL_RO,
	ASPELL_RU,
	ASPELL_SA,
	ASPELL_SC,
	ASPELL_SD,
	ASPELL_SE,
	ASPELL_SG,
	ASPELL_SI,
	ASPELL_SK,
	ASPELL_SL,
	ASPELL_SM,
	ASPELL_SN,
	ASPELL_SO,
	ASPELL_SQ,
	ASPELL_SR,
	ASPELL_SS,
	ASPELL_ST,
	ASPELL_SU,
	ASPELL_SV,
	ASPELL_SW,
	ASPELL_TA,
	ASPELL_TE,
	ASPELL_TG,
	ASPELL_TK,
	ASPELL_TL,
	ASPELL_TN,
	ASPELL_TO,
	ASPELL_TR,
	ASPELL_TS,
	ASPELL_TT,
	ASPELL_TW,
	ASPELL_TY,
	ASPELL_UC, // english canadian
	ASPELL_UG,
	ASPELL_UK, // uskrain
	ASPELL_US, // english amer.
	ASPELL_UZ,
	ASPELL_VE,
	ASPELL_WA,
	ASPELL_WO,
	ASPELL_XH,
	ASPELL_YI,
	ASPELL_YO,
	ASPELL_ZA,
	ASPELL_ZU,
	
	THES_DE,  // Flags for thesaurus
	THES_IT,
	THES_FR,
	THES_EN,
	THES_ES,
	THES_PT,
	
	DICT_DE_EN, // flags for Dict's
	DICT_DE_ES,
	DICT_DE_FR,
	DICT_DE_IT,
	DICT_DE_PT,
	
	DICT_EN_DE,
	DICT_EN_FR,
	DICT_EN_ES,
	DICT_EN_IT,
	DICT_EN_PT,
	DICT_EN_NO,
	
	DICT_NO_EN,
	DICT_LAT_DE,
	
	END_MARKER
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
