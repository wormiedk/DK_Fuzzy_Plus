#pragma once

#include "string.h"
#include "pebble_os.h"

void fuzzy_time(int hours, int minutes, char* str_line1, char* str_line2, char* str_line3);
void status_bar(PblTm* t, char* str_topbar, char* str_bottombar);
