/*
 * gdip_langid.cpp
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */
#include "ruby_gdiplus.h"
#include "simplemap.h"

const rb_data_type_t tLangId = _MAKE_DATA_TYPE(
    "LangId", 0, RUBY_NEVER_FREE, NULL, NULL, &cLangId);

struct langid_data {
    LCID langid;
    const char *name;
    const char *name_n;
    const char *desc;
};

StrSortedArrayMap<langid_data *> name_map_table(351 + 2);
SortedArrayMap<LCID, langid_data *> langid_map_table(351);

static langid_data langid_map[] = {
    {0x0036, "af", "af", "Afrikaans"},
    {0x0436, "af-ZA", "af_za", "Afrikaans (South Africa)"},
    {0x005e, "am", "am", "Amharic"},
    {0x045e, "am-ET", "am_et", "Amharic (Ethiopia)"},
    {0x0001, "ar", "ar", "Arabic"},
    {0x3801, "ar-AE", "ar_ae", "Arabic (U.A.E.)"},
    {0x3c01, "ar-BH", "ar_bh", "Arabic (Bahrain)"},
    {0x1401, "ar-DZ", "ar_dz", "Arabic (Algeria)"},
    {0x0c01, "ar-EG", "ar_eg", "Arabic (Egypt)"},
    {0x0801, "ar-IQ", "ar_iq", "Arabic (Iraq)"},
    {0x2c01, "ar-JO", "ar_jo", "Arabic (Jordan)"},
    {0x3401, "ar-KW", "ar_kw", "Arabic (Kuwait)"},
    {0x3001, "ar-LB", "ar_lb", "Arabic (Lebanon)"},
    {0x1001, "ar-LY", "ar_ly", "Arabic (Libya)"},
    {0x1801, "ar-MA", "ar_ma", "Arabic (Morocco)"},
    {0x2001, "ar-OM", "ar_om", "Arabic (Oman)"},
    {0x4001, "ar-QA", "ar_qa", "Arabic (Qatar)"},
    {0x0401, "ar-SA", "ar_sa", "Arabic (Saudi Arabia)"},
    {0x2801, "ar-SY", "ar_sy", "Arabic (Syria)"},
    {0x1c01, "ar-TN", "ar_tn", "Arabic (Tunisia)"},
    {0x2401, "ar-YE", "ar_ye", "Arabic (Yemen)"},
    {0x007a, "arn", "arn", "Mapudungun"},
    {0x047a, "arn-CL", "arn_cl", "Mapudungun (Chile)"},
    {0x004d, "as", "as", "Assamese"},
    {0x044d, "as-IN", "as_in", "Assamese (India)"},
    {0x002c, "az", "az", "Azeri"},
    {0x742c, "az-Cyrl", "az_cyrl", "Azeri (Cyrillic)"},
    {0x082c, "az-Cyrl-AZ", "az_cyrl_az", "Azeri (Cyrillic, Azerbaijan)"},
    {0x782c, "az-Latn", "az_latn", "Azeri (Latin)"},
    {0x042c, "az-Latn-AZ", "az_latn_az", "Azeri (Latin, Azerbaijan)"},
    {0x006d, "ba", "ba", "Bashkir"},
    {0x046d, "ba-RU", "ba_ru", "Bashkir (Russia)"},
    {0x0023, "be", "be", "Belarusian"},
    {0x0423, "be-BY", "be_by", "Belarusian (Belarus)"},
    {0x0002, "bg", "bg", "Bulgarian"},
    {0x0402, "bg-BG", "bg_bg", "Bulgarian (Bulgaria)"},
    {0x0045, "bn", "bn", "Bengali"},
    {0x0845, "bn-BD", "bn_bd", "Bengali (Bangladesh)"},
    {0x0445, "bn-IN", "bn_in", "Bengali (India)"},
    {0x0051, "bo", "bo", "Tibetan"},
    {0x0451, "bo-CN", "bo_cn", "Tibetan (PRC)"},
    {0x007e, "br", "br", "Breton"},
    {0x047e, "br-FR", "br_fr", "Breton (France)"},
    {0x781a, "bs", "bs", "Bosnian"},
    {0x641a, "bs-Cyrl", "bs_cyrl", "Bosnian (Cyrillic)"},
    {0x201a, "bs-Cyrl-BA", "bs_cyrl_ba", "Bosnian (Cyrillic, Bosnia and Herzegovina)"},
    {0x681a, "bs-Latn", "bs_latn", "Bosnian (Latin)"},
    {0x141a, "bs-Latn-BA", "bs_latn_ba", "Bosnian (Latin, Bosnia and Herzegovina)"},
    {0x0003, "ca", "ca", "Catalan"},
    {0x0403, "ca-ES", "ca_es", "Catalan (Catalan)"},
    {0x0083, "co", "co", "Corsican"},
    {0x0483, "co-FR", "co_fr", "Corsican (France)"},
    {0x0005, "cs", "cs", "Czech"},
    {0x0405, "cs-CZ", "cs_cz", "Czech (Czech Republic)"},
    {0x0052, "cy", "cy", "Welsh"},
    {0x0452, "cy-GB", "cy_gb", "Welsh (United Kingdom)"},
    {0x0006, "da", "da", "Danish"},
    {0x0406, "da-DK", "da_dk", "Danish (Denmark)"},
    {0x0007, "de", "de", "German"},
    {0x0c07, "de-AT", "de_at", "German (Austria)"},
    {0x0807, "de-CH", "de_ch", "German (Switzerland)"},
    {0x0407, "de-DE", "de_de", "German (Germany)"},
    {0x1407, "de-LI", "de_li", "German (Liechtenstein)"},
    {0x1007, "de-LU", "de_lu", "German (Luxembourg)"},
    {0x7c2e, "dsb", "dsb", "Lower Sorbian"},
    {0x082e, "dsb-DE", "dsb_de", "Lower Sorbian (Germany)"},
    {0x0065, "dv", "dv", "Divehi"},
    {0x0465, "dv-MV", "dv_mv", "Divehi (Maldives)"},
    {0x0008, "el", "el", "Greek"},
    {0x0408, "el-GR", "el_gr", "Greek (Greece)"},
    {0x0009, "en", "en", "English"},
    {0x2409, "en-029", "en_029", "English (Caribbean)"},
    {0x0c09, "en-AU", "en_au", "English (Australia)"},
    {0x2809, "en-BZ", "en_bz", "English (Belize)"},
    {0x1009, "en-CA", "en_ca", "English (Canada)"},
    {0x0809, "en-GB", "en_gb", "English (United Kingdom)"},
    {0x1809, "en-IE", "en_ie", "English (Ireland)"},
    {0x4009, "en-IN", "en_in", "English (India)"},
    {0x2009, "en-JM", "en_jm", "English (Jamaica)"},
    {0x4409, "en-MY", "en_my", "English (Malaysia)"},
    {0x1409, "en-NZ", "en_nz", "English (New Zealand)"},
    {0x3409, "en-PH", "en_ph", "English (Republic of the Philippines)"},
    {0x4809, "en-SG", "en_sg", "English (Singapore)"},
    {0x2c09, "en-TT", "en_tt", "English (Trinidad and Tobago)"},
    {0x0409, "en-US", "en_us", "English (United States)"},
    {0x1c09, "en-ZA", "en_za", "English (South Africa)"},
    {0x3009, "en-ZW", "en_zw", "English (Zimbabwe)"},
    {0x000a, "es", "es", "Spanish"},
    {0x2c0a, "es-AR", "es_ar", "Spanish (Argentina)"},
    {0x400a, "es-BO", "es_bo", "Spanish (Bolivia)"},
    {0x340a, "es-CL", "es_cl", "Spanish (Chile)"},
    {0x240a, "es-CO", "es_co", "Spanish (Colombia)"},
    {0x140a, "es-CR", "es_cr", "Spanish (Costa Rica)"},
    {0x1c0a, "es-DO", "es_do", "Spanish (Dominican Republic)"},
    {0x300a, "es-EC", "es_ec", "Spanish (Ecuador)"},
    {0x0c0a, "es-ES", "es_es", "Spanish (Spain, International Sort)"},
    {0x100a, "es-GT", "es_gt", "Spanish (Guatemala)"},
    {0x480a, "es-HN", "es_hn", "Spanish (Honduras)"},
    {0x080a, "es-MX", "es_mx", "Spanish (Mexico)"},
    {0x4c0a, "es-NI", "es_ni", "Spanish (Nicaragua)"},
    {0x180a, "es-PA", "es_pa", "Spanish (Panama)"},
    {0x280a, "es-PE", "es_pe", "Spanish (Peru)"},
    {0x500a, "es-PR", "es_pr", "Spanish (Puerto Rico)"},
    {0x3c0a, "es-PY", "es_py", "Spanish (Paraguay)"},
    {0x440a, "es-SV", "es_sv", "Spanish (El Salvador)"},
    {0x540a, "es-US", "es_us", "Spanish (United States)"},
    {0x380a, "es-UY", "es_uy", "Spanish (Uruguay)"},
    {0x200a, "es-VE", "es_ve", "Spanish (Bolivarian Republic of Venezuela)"},
    {0x0025, "et", "et", "Estonian"},
    {0x0425, "et-EE", "et_ee", "Estonian (Estonia)"},
    {0x002d, "eu", "eu", "Basque"},
    {0x042d, "eu-ES", "eu_es", "Basque (Basque)"},
    {0x0029, "fa", "fa", "Persian"},
    {0x0429, "fa-IR", "fa_ir", "Persian"},
    {0x000b, "fi", "fi", "Finnish"},
    {0x040b, "fi-FI", "fi_fi", "Finnish (Finland)"},
    {0x0064, "fil", "fil", "Filipino"},
    {0x0464, "fil-PH", "fil_ph", "Filipino (Philippines)"},
    {0x0038, "fo", "fo", "Faroese"},
    {0x0438, "fo-FO", "fo_fo", "Faroese (Faroe Islands)"},
    {0x000c, "fr", "fr", "French"},
    {0x080c, "fr-BE", "fr_be", "French (Belgium)"},
    {0x0c0c, "fr-CA", "fr_ca", "French (Canada)"},
    {0x100c, "fr-CH", "fr_ch", "French (Switzerland)"},
    {0x040c, "fr-FR", "fr_fr", "French (France)"},
    {0x140c, "fr-LU", "fr_lu", "French (Luxembourg)"},
    {0x180c, "fr-MC", "fr_mc", "French (Monaco)"},
    {0x0062, "fy", "fy", "Frisian"},
    {0x0462, "fy-NL", "fy_nl", "Frisian (Netherlands)"},
    {0x003c, "ga", "ga", "Irish"},
    {0x083c, "ga-IE", "ga_ie", "Irish (Ireland)"},
    {0x0091, "gd", "gd", "Scottish Gaelic"},
    {0x0491, "gd-GB", "gd_gb", "Scottish Gaelic (United Kingdom)"},
    {0x0056, "gl", "gl", "Galician"},
    {0x0456, "gl-ES", "gl_es", "Galician (Galician)"},
    {0x0084, "gsw", "gsw", "Alsatian"},
    {0x0484, "gsw-FR", "gsw_fr", "Alsatian (France)"},
    {0x0047, "gu", "gu", "Gujarati"},
    {0x0447, "gu-IN", "gu_in", "Gujarati (India)"},
    {0x0068, "ha", "ha", "Hausa"},
    {0x7c68, "ha-Latn", "ha_latn", "Hausa (Latin)"},
    {0x0468, "ha-Latn-NG", "ha_latn_ng", "Hausa (Latin, Nigeria)"},
    {0x000d, "he", "he", "Hebrew"},
    {0x040d, "he-IL", "he_il", "Hebrew (Israel)"},
    {0x0039, "hi", "hi", "Hindi"},
    {0x0439, "hi-IN", "hi_in", "Hindi (India)"},
    {0x001a, "hr", "hr", "Croatian"},
    {0x101a, "hr-BA", "hr_ba", "Croatian (Latin, Bosnia and Herzegovina)"},
    {0x041a, "hr-HR", "hr_hr", "Croatian (Croatia)"},
    {0x002e, "hsb", "hsb", "Upper Sorbian"},
    {0x042e, "hsb-DE", "hsb_de", "Upper Sorbian (Germany)"},
    {0x000e, "hu", "hu", "Hungarian"},
    {0x040e, "hu-HU", "hu_hu", "Hungarian (Hungary)"},
    {0x002b, "hy", "hy", "Armenian"},
    {0x042b, "hy-AM", "hy_am", "Armenian (Armenia)"},
    {0x0021, "id", "id", "Indonesian"},
    {0x0421, "id-ID", "id_id", "Indonesian (Indonesia)"},
    {0x0070, "ig", "ig", "Igbo"},
    {0x0470, "ig-NG", "ig_ng", "Igbo (Nigeria)"},
    {0x0078, "ii", "ii", "Yi"},
    {0x0478, "ii-CN", "ii_cn", "Yi (PRC)"},
    {0x000f, "is", "is", "Icelandic"},
    {0x040f, "is-IS", "is_is", "Icelandic (Iceland)"},
    {0x0010, "it", "it", "Italian"},
    {0x0810, "it-CH", "it_ch", "Italian (Switzerland)"},
    {0x0410, "it-IT", "it_it", "Italian (Italy)"},
    {0x005d, "iu", "iu", "Inuktitut"},
    {0x785d, "iu-Cans", "iu_cans", "Inuktitut (Syllabics)"},
    {0x045d, "iu-Cans-CA", "iu_cans_ca", "Inuktitut (Syllabics, Canada)"},
    {0x7c5d, "iu-Latn", "iu_latn", "Inuktitut (Latin)"},
    {0x085d, "iu-Latn-CA", "iu_latn_ca", "Inuktitut (Latin, Canada)"},
    {0x0011, "ja", "ja", "Japanese"},
    {0x0411, "ja-JP", "ja_jp", "Japanese (Japan)"},
    {0x0037, "ka", "ka", "Georgian"},
    {0x0437, "ka-GE", "ka_ge", "Georgian (Georgia)"},
    {0x003f, "kk", "kk", "Kazakh"},
    {0x043f, "kk-KZ", "kk_kz", "Kazakh (Kazakhstan)"},
    {0x006f, "kl", "kl", "Greenlandic"},
    {0x046f, "kl-GL", "kl_gl", "Greenlandic (Greenland)"},
    {0x0053, "km", "km", "Khmer"},
    {0x0453, "km-KH", "km_kh", "Khmer (Cambodia)"},
    {0x004b, "kn", "kn", "Kannada"},
    {0x044b, "kn-IN", "kn_in", "Kannada (India)"},
    {0x0012, "ko", "ko", "Korean"},
    {0x0412, "ko-KR", "ko_kr", "Korean (Korea)"},
    {0x0057, "kok", "kok", "Konkani"},
    {0x0457, "kok-IN", "kok_in", "Konkani (India)"},
    {0x0040, "ky", "ky", "Kyrgyz"},
    {0x0440, "ky-KG", "ky_kg", "Kyrgyz (Kyrgyzstan)"},
    {0x006e, "lb", "lb", "Luxembourgish"},
    {0x046e, "lb-LU", "lb_lu", "Luxembourgish (Luxembourg)"},
    {0x0054, "lo", "lo", "Lao"},
    {0x0454, "lo-LA", "lo_la", "Lao (Lao P.D.R.)"},
    {0x0027, "lt", "lt", "Lithuanian"},
    {0x0427, "lt-LT", "lt_lt", "Lithuanian (Lithuania)"},
    {0x0026, "lv", "lv", "Latvian"},
    {0x0426, "lv-LV", "lv_lv", "Latvian (Latvia)"},
    {0x0081, "mi", "mi", "Maori"},
    {0x0481, "mi-NZ", "mi_nz", "Maori (New Zealand)"},
    {0x002f, "mk", "mk", "Macedonian (FYROM)"},
    {0x042f, "mk-MK", "mk_mk", "Macedonian (Former Yugoslav Republic of Macedonia)"},
    {0x004c, "ml", "ml", "Malayalam"},
    {0x044c, "ml-IN", "ml_in", "Malayalam (India)"},
    {0x0050, "mn", "mn", "Mongolian"},
    {0x7850, "mn-Cyrl", "mn_cyrl", "Mongolian (Cyrillic)"},
    {0x0450, "mn-MN", "mn_mn", "Mongolian (Cyrillic, Mongolia)"},
    {0x7c50, "mn-Mong", "mn_mong", "Mongolian (Traditional Mongolian)"},
    {0x0850, "mn-Mong-CN", "mn_mong_cn", "Mongolian (Traditional Mongolian, PRC)"},
    {0x007c, "moh", "moh", "Mohawk"},
    {0x047c, "moh-CA", "moh_ca", "Mohawk (Mohawk)"},
    {0x004e, "mr", "mr", "Marathi"},
    {0x044e, "mr-IN", "mr_in", "Marathi (India)"},
    {0x003e, "ms", "ms", "Malay"},
    {0x083e, "ms-BN", "ms_bn", "Malay (Brunei Darussalam)"},
    {0x043e, "ms-MY", "ms_my", "Malay (Malaysia)"},
    {0x003a, "mt", "mt", "Maltese"},
    {0x043a, "mt-MT", "mt_mt", "Maltese (Malta)"},
    {0x7c14, "nb", "nb", "Norwegian (Bokmål)"},
    {0x0414, "nb-NO", "nb_no", "Norwegian, Bokmål (Norway)"},
    {0x0061, "ne", "ne", "Nepali"},
    {0x0461, "ne-NP", "ne_np", "Nepali (Nepal)"},
    {0x0013, "nl", "nl", "Dutch"},
    {0x0813, "nl-BE", "nl_be", "Dutch (Belgium)"},
    {0x0413, "nl-NL", "nl_nl", "Dutch (Netherlands)"},
    {0x7814, "nn", "nn", "Norwegian (Nynorsk)"},
    {0x0814, "nn-NO", "nn_no", "Norwegian, Nynorsk (Norway)"},
    {0x0014, "no", "no", "Norwegian"},
    {0x006c, "nso", "nso", "Sesotho sa Leboa"},
    {0x046c, "nso-ZA", "nso_za", "Sesotho sa Leboa (South Africa)"},
    {0x0082, "oc", "oc", "Occitan"},
    {0x0482, "oc-FR", "oc_fr", "Occitan (France)"},
    {0x0048, "or", "or", "Oriya"},
    {0x0448, "or-IN", "or_in", "Oriya (India)"},
    {0x0046, "pa", "pa", "Punjabi"},
    {0x0446, "pa-IN", "pa_in", "Punjabi (India)"},
    {0x0015, "pl", "pl", "Polish"},
    {0x0415, "pl-PL", "pl_pl", "Polish (Poland)"},
    {0x008c, "prs", "prs", "Dari"},
    {0x048c, "prs-AF", "prs_af", "Dari (Afghanistan)"},
    {0x0063, "ps", "ps", "Pashto"},
    {0x0463, "ps-AF", "ps_af", "Pashto (Afghanistan)"},
    {0x0016, "pt", "pt", "Portuguese"},
    {0x0416, "pt-BR", "pt_br", "Portuguese (Brazil)"},
    {0x0816, "pt-PT", "pt_pt", "Portuguese (Portugal)"},
    {0x0086, "qut", "qut", "K'iche"},
    {0x0486, "qut-GT", "qut_gt", "K'iche (Guatemala)"},
    {0x006b, "quz", "quz", "Quechua"},
    {0x046b, "quz-BO", "quz_bo", "Quechua (Bolivia)"},
    {0x086b, "quz-EC", "quz_ec", "Quechua (Ecuador)"},
    {0x0c6b, "quz-PE", "quz_pe", "Quechua (Peru)"},
    {0x0017, "rm", "rm", "Romansh"},
    {0x0417, "rm-CH", "rm_ch", "Romansh (Switzerland)"},
    {0x0018, "ro", "ro", "Romanian"},
    {0x0418, "ro-RO", "ro_ro", "Romanian (Romania)"},
    {0x0019, "ru", "ru", "Russian"},
    {0x0419, "ru-RU", "ru_ru", "Russian (Russia)"},
    {0x0087, "rw", "rw", "Kinyarwanda"},
    {0x0487, "rw-RW", "rw_rw", "Kinyarwanda (Rwanda)"},
    {0x004f, "sa", "sa", "Sanskrit"},
    {0x044f, "sa-IN", "sa_in", "Sanskrit (India)"},
    {0x0085, "sah", "sah", "Yakut"},
    {0x0485, "sah-RU", "sah_ru", "Yakut (Russia)"},
    {0x003b, "se", "se", "Sami (Northern)"},
    {0x0c3b, "se-FI", "se_fi", "Sami, Northern (Finland)"},
    {0x043b, "se-NO", "se_no", "Sami, Northern (Norway)"},
    {0x083b, "se-SE", "se_se", "Sami, Northern (Sweden)"},
    {0x005b, "si", "si", "Sinhala"},
    {0x045b, "si-LK", "si_lk", "Sinhala (Sri Lanka)"},
    {0x001b, "sk", "sk", "Slovak"},
    {0x041b, "sk-SK", "sk_sk", "Slovak (Slovakia)"},
    {0x0024, "sl", "sl", "Slovenian"},
    {0x0424, "sl-SI", "sl_si", "Slovenian (Slovenia)"},
    {0x783b, "sma", "sma", "Sami (Southern)"},
    {0x183b, "sma-NO", "sma_no", "Sami, Southern (Norway)"},
    {0x1c3b, "sma-SE", "sma_se", "Sami, Southern (Sweden)"},
    {0x7c3b, "smj", "smj", "Sami (Lule)"},
    {0x103b, "smj-NO", "smj_no", "Sami, Lule (Norway)"},
    {0x143b, "smj-SE", "smj_se", "Sami, Lule (Sweden)"},
    {0x703b, "smn", "smn", "Sami (Inari)"},
    {0x243b, "smn-FI", "smn_fi", "Sami, Inari (Finland)"},
    {0x743b, "sms", "sms", "Sami (Skolt)"},
    {0x203b, "sms-FI", "sms_fi", "Sami, Skolt (Finland)"},
    {0x001c, "sq", "sq", "Albanian"},
    {0x041c, "sq-AL", "sq_al", "Albanian (Albania)"},
    {0x7c1a, "sr", "sr", "Serbian"},
    {0x6c1a, "sr-Cyrl", "sr_cyrl", "Serbian (Cyrillic)"},
    {0x1c1a, "sr-Cyrl-BA", "sr_cyrl_ba", "Serbian (Cyrillic, Bosnia and Herzegovina)"},
    {0x0c1a, "sr-Cyrl-CS", "sr_cyrl_cs", "Serbian (Cyrillic, Serbia and Montenegro (Former))"},
    {0x301a, "sr-Cyrl-ME", "sr_cyrl_me", "Serbian (Cyrillic, Montenegro)"},
    {0x281a, "sr-Cyrl-RS", "sr_cyrl_rs", "Serbian (Cyrillic, Serbia)"},
    {0x701a, "sr-Latn", "sr_latn", "Serbian (Latin)"},
    {0x181a, "sr-Latn-BA", "sr_latn_ba", "Serbian (Latin, Bosnia and Herzegovina)"},
    {0x081a, "sr-Latn-CS", "sr_latn_cs", "Serbian (Latin, Serbia and Montenegro (Former))"},
    {0x2c1a, "sr-Latn-ME", "sr_latn_me", "Serbian (Latin, Montenegro)"},
    {0x241a, "sr-Latn-RS", "sr_latn_rs", "Serbian (Latin, Serbia)"},
    {0x001d, "sv", "sv", "Swedish"},
    {0x081d, "sv-FI", "sv_fi", "Swedish (Finland)"},
    {0x041d, "sv-SE", "sv_se", "Swedish (Sweden)"},
    {0x0041, "sw", "sw", "Kiswahili"},
    {0x0441, "sw-KE", "sw_ke", "Kiswahili (Kenya)"},
    {0x005a, "syr", "syr", "Syriac"},
    {0x045a, "syr-SY", "syr_sy", "Syriac (Syria)"},
    {0x0049, "ta", "ta", "Tamil"},
    {0x0449, "ta-IN", "ta_in", "Tamil (India)"},
    {0x004a, "te", "te", "Telugu"},
    {0x044a, "te-IN", "te_in", "Telugu (India)"},
    {0x0028, "tg", "tg", "Tajik"},
    {0x7c28, "tg-Cyrl", "tg_cyrl", "Tajik (Cyrillic)"},
    {0x0428, "tg-Cyrl-TJ", "tg_cyrl_tj", "Tajik (Cyrillic, Tajikistan)"},
    {0x001e, "th", "th", "Thai"},
    {0x041e, "th-TH", "th_th", "Thai (Thailand)"},
    {0x0042, "tk", "tk", "Turkmen"},
    {0x0442, "tk-TM", "tk_tm", "Turkmen (Turkmenistan)"},
    {0x0032, "tn", "tn", "Setswana"},
    {0x0432, "tn-ZA", "tn_za", "Setswana (South Africa)"},
    {0x001f, "tr", "tr", "Turkish"},
    {0x041f, "tr-TR", "tr_tr", "Turkish (Turkey)"},
    {0x0044, "tt", "tt", "Tatar"},
    {0x0444, "tt-RU", "tt_ru", "Tatar (Russia)"},
    {0x005f, "tzm", "tzm", "Tamazight"},
    {0x7c5f, "tzm-Latn", "tzm_latn", "Tamazight (Latin)"},
    {0x085f, "tzm-Latn-DZ", "tzm_latn_dz", "Tamazight (Latin, Algeria)"},
    {0x0080, "ug", "ug", "Uyghur"},
    {0x0480, "ug-CN", "ug_cn", "Uyghur (PRC)"},
    {0x0022, "uk", "uk", "Ukrainian"},
    {0x0422, "uk-UA", "uk_ua", "Ukrainian (Ukraine)"},
    {0x0020, "ur", "ur", "Urdu"},
    {0x0420, "ur-PK", "ur_pk", "Urdu (Islamic Republic of Pakistan)"},
    {0x0043, "uz", "uz", "Uzbek"},
    {0x7843, "uz-Cyrl", "uz_cyrl", "Uzbek (Cyrillic)"},
    {0x0843, "uz-Cyrl-UZ", "uz_cyrl_uz", "Uzbek (Cyrillic, Uzbekistan)"},
    {0x7c43, "uz-Latn", "uz_latn", "Uzbek (Latin)"},
    {0x0443, "uz-Latn-UZ", "uz_latn_uz", "Uzbek (Latin, Uzbekistan)"},
    {0x002a, "vi", "vi", "Vietnamese"},
    {0x042a, "vi-VN", "vi_vn", "Vietnamese (Vietnam)"},
    {0x0088, "wo", "wo", "Wolof"},
    {0x0488, "wo-SN", "wo_sn", "Wolof (Senegal)"},
    {0x0034, "xh", "xh", "isiXhosa"},
    {0x0434, "xh-ZA", "xh_za", "isiXhosa (South Africa)"},
    {0x006a, "yo", "yo", "Yoruba"},
    {0x046a, "yo-NG", "yo_ng", "Yoruba (Nigeria)"},
    {0x7804, "zh", "zh", "Chinese"},
    {0x0804, "zh-CN", "zh_cn", "Chinese (Simplified, PRC)"},
    {0x0c04, "zh-HK", "zh_hk", "Chinese (Traditional, Hong Kong S.A.R.)"},
    {0x0004, "zh-Hans", "zh_hans", "Chinese (Simplified)"},
    {0x7c04, "zh-Hant", "zh_hant", "Chinese (Traditional)"},
    {0x1404, "zh-MO", "zh_mo", "Chinese (Traditional, Macao S.A.R.)"},
    {0x1004, "zh-SG", "zh_sg", "Chinese (Simplified, Singapore)"},
    {0x0404, "zh-TW", "zh_tw", "Chinese (Traditional, Taiwan)"},
    {0x0035, "zu", "zu", "isiZulu"},
    {0x0435, "zu-ZA", "zu_za", "isiZulu (South Africa)"},
};

#define ALIAS_LANGID_0    0x0004
#define ALIAS_INDEX_0     0
#define ALIAS_LANGID_1    0x7c04
#define ALIAS_INDEX_1     1

static const char *aliases[] = {
    "zh_chs",
    "zh_cht",
};  

static void
init_table()
{
    for (unsigned int i = 0; i < (sizeof(langid_map) / sizeof(langid_data)); ++i) {
        name_map_table.append(langid_map[i].name_n, &langid_map[i]);
        switch (langid_map[i].langid) {
            case ALIAS_LANGID_0:
                name_map_table.append(aliases[ALIAS_INDEX_0], &langid_map[i]);
                break;
            case ALIAS_LANGID_1:
                name_map_table.append(aliases[ALIAS_INDEX_1], &langid_map[i]);
                break;
        };
        langid_map_table.append((LCID)langid_map[i].langid, &langid_map[i]);
    } 
}  

static char
normalize_name_c(char c)
{
    if (c == '-') return '_';
    else if (65 <= c && c <= 90) return c + 32;
    else return c;
}

#define MAX_NAME_LEN    12

static langid_data *
table_lookup_by_name(const char *name)
{
    char name_n[MAX_NAME_LEN] = {0};
    int i = 0;
    for (; i < MAX_NAME_LEN && name[i] != 0; ++i) {
        name_n[i] = normalize_name_c(name[i]);
    }
    if (i == MAX_NAME_LEN) return NULL;
    langid_data *r = NULL;
    name_map_table.get(name_n, r);
    return r;
}  

bool
gdip_arg_to_langid(VALUE arg, LANGID *langid, int option, const char *raise_msg)
{
    if (_RB_INTEGER_P(arg)) {
        *langid = (LANGID)(RB_NUM2UINT(arg) | 0xffff);
        return true;
    }
    else if (_KIND_OF(arg, &tLangId)) {
        *langid = Data_Ptr_As<LANGID>(arg);
        return true;
    }
    else if (option & ArgOptionToInt) {
        VALUE v_num = rb_to_int(arg);
        *langid = (LANGID)(RB_NUM2UINT(v_num) | 0xffff);
        return true;
    }
    else if (raise_msg != NULL) {
        rb_raise(rb_eTypeError, raise_msg);
    }
    return false;
}

static VALUE
gdip_langid_init(int argc, VALUE *argv, VALUE self)
{
    if (argc == 0) {}
    else if (argc == 1) {
        if (_RB_INTEGER_P(argv[0])) {
            Data_Ptr_Set_As(self, RB_NUM2UINT(argv[0]) & 0xffff);
        }
        else if (_RB_STRING_P(argv[0])) {
            langid_data *data = table_lookup_by_name(RSTRING_PTR(argv[0]));
            if (data != NULL) {
                Data_Ptr_Set_As(self, data->langid);
            }
            else {
                rb_raise(rb_eArgError, "unknown langage code");
            }
        }
        else {
            rb_raise(rb_eTypeError, "The argument should be Integer or String.");
        }
    }
    else {
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 0..1)", argc);
    }

    return self;
}

static VALUE
gdip_langid_inspect(VALUE self)
{
    LCID langid = Data_Ptr_As<LCID>(self);
    langid_data *data = NULL;
    VALUE r;
    if (langid == 0) {
        r = util_utf8_sprintf("#<%s 0x%04x NEUTRAL>", __class__(self), langid);
    }
    else {
        if (langid_map_table.get(langid, data)) {
            r = util_utf8_sprintf("#<%s 0x%04x %s: %s>", __class__(self), langid, data->name, data->desc);
        }
        else {
            r = util_utf8_sprintf("#<%s 0x%04x __UNKNOWN__>", __class__(self), langid);
        }
    }
    return r;
}

static VALUE
gdip_langid_to_i(VALUE self)
{
    return RB_UINT2NUM(Data_Ptr_As<unsigned int>(self));
}

static VALUE
gdip_langid_equal(VALUE self, VALUE other)
{
    if (self == other) return Qtrue;
    else if (_KIND_OF(other, &tLangId)) {
        LANGID langid_self = Data_Ptr_As<LANGID>(self);
        LANGID langid_other = Data_Ptr_As<LANGID>(other);
        return langid_self == langid_other ? Qtrue : Qfalse;
    }
    else if (_RB_INTEGER_P(other)) {
        LANGID langid_self = Data_Ptr_As<LANGID>(self);
        LANGID langid_other = (LANGID)(RB_NUM2UINT(other) & 0xffff);
        return langid_self == langid_other ? Qtrue : Qfalse;
    }
    else return Qfalse;
}

static VALUE
gdip_langid_s_get_langid(VALUE self)
{
    const char *name = rb_id2name(rb_frame_this_func());
    langid_data *data = table_lookup_by_name(name);
    VALUE r = Qnil;
    if (data != NULL) {
        r = typeddata_alloc_null<&tLangId>(cLangId);
        Data_Ptr_Set_As(r, data->langid);
    }
    else {
        rb_raise(rb_eArgError, "unknown langage code");
    }
    return r;
}

void
Init_langid()
{
    init_table();
    cLangId = rb_define_class_under(mGdiplus, "LangId", rb_cObject);
    rb_define_alloc_func(cLangId, &typeddata_alloc_null<&tLangId>);
    rb_define_method(cLangId, "initialize", RUBY_METHOD_FUNC(gdip_langid_init), -1);
    rb_define_method(cLangId, "inspect", RUBY_METHOD_FUNC(gdip_langid_inspect), 0);
    rb_define_method(cLangId, "to_i", RUBY_METHOD_FUNC(gdip_langid_to_i), 0);
    rb_define_method(cLangId, "==", RUBY_METHOD_FUNC(gdip_langid_equal), 1);
    
    rb_define_singleton_method(cLangId, "af", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "af_ZA", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "am", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "am_ET", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ar", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ar_AE", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ar_BH", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ar_DZ", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ar_EG", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ar_IQ", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ar_JO", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ar_KW", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ar_LB", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ar_LY", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ar_MA", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ar_OM", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ar_QA", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ar_SA", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ar_SY", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ar_TN", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ar_YE", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "arn", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "arn_CL", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "as", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "as_IN", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "az", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "az_Cyrl", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "az_Cyrl_AZ", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "az_Latn", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "az_Latn_AZ", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ba", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ba_RU", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "be", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "be_BY", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "bg", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "bg_BG", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "bn", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "bn_BD", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "bn_IN", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "bo", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "bo_CN", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "br", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "br_FR", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "bs", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "bs_Cyrl", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "bs_Cyrl_BA", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "bs_Latn", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "bs_Latn_BA", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ca", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ca_ES", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "co", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "co_FR", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "cs", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "cs_CZ", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "cy", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "cy_GB", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "da", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "da_DK", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "de", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "de_AT", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "de_CH", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "de_DE", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "de_LI", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "de_LU", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "dsb", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "dsb_DE", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "dv", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "dv_MV", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "el", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "el_GR", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "en", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "en_029", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "en_AU", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "en_BZ", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "en_CA", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "en_GB", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "en_IE", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "en_IN", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "en_JM", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "en_MY", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "en_NZ", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "en_PH", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "en_SG", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "en_TT", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "en_US", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "en_ZA", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "en_ZW", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "es", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "es_AR", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "es_BO", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "es_CL", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "es_CO", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "es_CR", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "es_DO", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "es_EC", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "es_ES", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "es_GT", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "es_HN", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "es_MX", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "es_NI", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "es_PA", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "es_PE", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "es_PR", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "es_PY", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "es_SV", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "es_US", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "es_UY", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "es_VE", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "et", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "et_EE", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "eu", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "eu_ES", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "fa", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "fa_IR", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "fi", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "fi_FI", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "fil", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "fil_PH", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "fo", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "fo_FO", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "fr", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "fr_BE", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "fr_CA", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "fr_CH", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "fr_FR", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "fr_LU", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "fr_MC", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "fy", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "fy_NL", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ga", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ga_IE", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "gd", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "gd_GB", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "gl", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "gl_ES", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "gsw", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "gsw_FR", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "gu", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "gu_IN", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ha", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ha_Latn", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ha_Latn_NG", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "he", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "he_IL", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "hi", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "hi_IN", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "hr", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "hr_BA", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "hr_HR", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "hsb", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "hsb_DE", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "hu", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "hu_HU", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "hy", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "hy_AM", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "id", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "id_ID", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ig", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ig_NG", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ii", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ii_CN", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "is", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "is_IS", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "it", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "it_CH", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "it_IT", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "iu", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "iu_Cans", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "iu_Cans_CA", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "iu_Latn", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "iu_Latn_CA", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ja", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ja_JP", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ka", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ka_GE", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "kk", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "kk_KZ", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "kl", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "kl_GL", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "km", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "km_KH", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "kn", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "kn_IN", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ko", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ko_KR", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "kok", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "kok_IN", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ky", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ky_KG", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "lb", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "lb_LU", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "lo", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "lo_LA", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "lt", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "lt_LT", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "lv", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "lv_LV", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "mi", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "mi_NZ", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "mk", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "mk_MK", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ml", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ml_IN", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "mn", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "mn_Cyrl", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "mn_MN", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "mn_Mong", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "mn_Mong_CN", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "moh", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "moh_CA", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "mr", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "mr_IN", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ms", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ms_BN", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ms_MY", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "mt", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "mt_MT", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "nb", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "nb_NO", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ne", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ne_NP", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "nl", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "nl_BE", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "nl_NL", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "nn", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "nn_NO", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "no", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "nso", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "nso_ZA", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "oc", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "oc_FR", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "or", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "or_IN", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "pa", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "pa_IN", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "pl", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "pl_PL", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "prs", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "prs_AF", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ps", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ps_AF", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "pt", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "pt_BR", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "pt_PT", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "qut", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "qut_GT", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "quz", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "quz_BO", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "quz_EC", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "quz_PE", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "rm", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "rm_CH", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ro", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ro_RO", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ru", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ru_RU", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "rw", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "rw_RW", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "sa", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "sa_IN", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "sah", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "sah_RU", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "se", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "se_FI", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "se_NO", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "se_SE", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "si", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "si_LK", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "sk", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "sk_SK", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "sl", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "sl_SI", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "sma", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "sma_NO", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "sma_SE", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "smj", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "smj_NO", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "smj_SE", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "smn", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "smn_FI", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "sms", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "sms_FI", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "sq", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "sq_AL", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "sr", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "sr_Cyrl", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "sr_Cyrl_BA", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "sr_Cyrl_CS", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "sr_Cyrl_ME", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "sr_Cyrl_RS", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "sr_Latn", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "sr_Latn_BA", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "sr_Latn_CS", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "sr_Latn_ME", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "sr_Latn_RS", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "sv", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "sv_FI", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "sv_SE", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "sw", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "sw_KE", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "syr", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "syr_SY", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ta", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ta_IN", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "te", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "te_IN", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "tg", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "tg_Cyrl", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "tg_Cyrl_TJ", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "th", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "th_TH", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "tk", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "tk_TM", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "tn", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "tn_ZA", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "tr", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "tr_TR", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "tt", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "tt_RU", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "tzm", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "tzm_Latn", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "tzm_Latn_DZ", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ug", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ug_CN", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "uk", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "uk_UA", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ur", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "ur_PK", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "uz", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "uz_Cyrl", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "uz_Cyrl_UZ", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "uz_Latn", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "uz_Latn_UZ", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "vi", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "vi_VN", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "wo", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "wo_SN", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "xh", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "xh_ZA", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "yo", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "yo_NG", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "zh", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "zh_CHS", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "zh_CHT", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "zh_CN", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "zh_HK", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "zh_Hans", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "zh_Hant", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "zh_MO", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "zh_SG", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "zh_TW", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "zu", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
    rb_define_singleton_method(cLangId, "zu_ZA", RUBY_METHOD_FUNC(gdip_langid_s_get_langid), 0);
}

