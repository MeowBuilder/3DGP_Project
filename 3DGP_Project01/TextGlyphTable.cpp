#include "TextGlyphTable.h"

static const wchar_t* GLYPH_EMPTY[GLYPH_HEIGHT] = {
	L"       ",
	L"       ",
	L"       ",
	L"       ",
	L"       ",
	L"       ",
	L"       "
};

static const wchar_t* GLYPH_3[GLYPH_HEIGHT] = {
	L" ##### ",
	L"     # ",
	L"     # ",
	L"  #### ",
	L"     # ",
	L"     # ",
	L" ##### "
};

static const wchar_t* GLYPH_D[GLYPH_HEIGHT] = {
	L"#####  ",
	L"#    # ",
	L"#     #",
	L"#     #",
	L"#     #",
	L"#    # ",
	L"#####  "
};


static const wchar_t* GLYPH_��[GLYPH_HEIGHT] = {
	L"### # #",
	L"  # # #",
	L"  # # #",
	L" # ####",
	L" #  # #",
	L"#   # #",
	L"#   # #"
};

static const wchar_t* GLYPH_��[GLYPH_HEIGHT] = {
	L" ###  #",
	L"#   # #",
	L" ###  #",
	L"       ",
	L" ##### ",
	L" #   # ",
	L" ##### "
};

static const wchar_t* GLYPH_��[GLYPH_HEIGHT] = {
	L" ##### ",
	L"  # #  ",
	L"  # #  ",
	L"  # #  ",
	L" #####",
	L"       ",
	L"#######"
};

static const wchar_t* GLYPH_��[GLYPH_HEIGHT] = {
	L" ##### ",
	L"     # ",
	L" ##### ",
	L" #     ",
	L" ##### ",
	L"   #   ",
	L"#######"
};

static const wchar_t* GLYPH_��[GLYPH_HEIGHT] = {
	L" ##### ",
	L"     # ",
	L"     # ",
	L"     # ",
	L"     # ",
	L"       ",
	L"#######"
};

static const wchar_t* GLYPH_��[GLYPH_HEIGHT] = {
	L"### # #",
	L"  # # #",
	L"  # # #",
	L"### ###",
	L"#   # #",
	L"#   # #",
	L"### # #"
};

static const wchar_t* GLYPH_��[GLYPH_HEIGHT] = {
	L" #### #",
	L" #  # #",
	L" #### #",
	L"      #",
	L"  ### #",
	L" #   # ",
	L"  ###  "
};

static const wchar_t* GLYPH_1[GLYPH_HEIGHT] = {
	L"   #   ",
	L"  ##   ",
	L"   #   ",
	L"   #   ",
	L"   #   ",
	L"   #   ",
	L" ##### "
};

static const wchar_t* GLYPH_��[GLYPH_HEIGHT] = {
	L"###  # ",
	L"  #  # ",
	L"  #  # ",
	L"       ",
	L" ##### ",
	L" #   # ",
	L" ##### "
};

static const wchar_t* GLYPH_��[GLYPH_HEIGHT] = {
	L"### # #",
	L"#   # #",
	L"#   # #",
	L"### ###",
	L"#   # #",
	L"#   # #",
	L"### # #"
};

static const wchar_t* GLYPH_��[GLYPH_HEIGHT] = {
	L"   #   ",
	L"  # #  ",
	L" #   # ",
	L"#######",
	L"   #   ",
	L"#      ",
	L"#######"
};

const wchar_t** GetGlyph(wchar_t ch)
{
	switch (ch)
	{
	case L'��': return GLYPH_��;
	case L'��': return GLYPH_��;
	case L'��': return GLYPH_��;
	case L'3': return GLYPH_3;
	case L'D': return GLYPH_D;
	case L'��': return GLYPH_��;
	case L'��': return GLYPH_��;
	case L'��': return GLYPH_��;
	case L'��': return GLYPH_��;
	case L'��': return GLYPH_��;
	case L'��': return GLYPH_��;
	case L'��': return GLYPH_��;
	case L'1': return GLYPH_1;
	default:    return nullptr;
	}
}