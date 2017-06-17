/*
 * gdip_font.cpp
 * Copyright (c) 2017 Yagi Sumiya 
 * Released under the MIT License.
 */
#include "ruby_gdiplus.h"

const rb_data_type_t tLangId = _MAKE_DATA_TYPE(
    "LangId", 0, RUBY_NEVER_FREE, NULL, NULL, &cLangId);
