#pragma once

#include <unordered_map>
#include "stdafx.h"

constexpr int GLYPH_WIDTH = 7;
constexpr int GLYPH_HEIGHT = 7;

const wchar_t** GetGlyph(wchar_t ch);
