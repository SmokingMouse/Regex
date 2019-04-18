#pragma once

#include<stdint.h>
#include<vector>

std::vector<uint32_t> match_points(const char* text, const char* re);
bool match_all_text(const char* text, const char* re);