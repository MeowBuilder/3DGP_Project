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


static const wchar_t* GLYPH_게[GLYPH_HEIGHT] = {
	L"### # #",
	L"  # # #",
	L"  # # #",
	L" # ####",
	L" #  # #",
	L"#   # #",
	L"#   # #"
};

static const wchar_t* GLYPH_임[GLYPH_HEIGHT] = {
	L" ###  #",
	L"#   # #",
	L" ###  #",
	L"       ",
	L" ##### ",
	L" #   # ",
	L" ##### "
};

static const wchar_t* GLYPH_프[GLYPH_HEIGHT] = {
	L" ##### ",
	L"  # #  ",
	L"  # #  ",
	L"  # #  ",
	L" #####",
	L"       ",
	L"#######"
};

static const wchar_t* GLYPH_로[GLYPH_HEIGHT] = {
	L" ##### ",
	L"     # ",
	L" ##### ",
	L" #     ",
	L" ##### ",
	L"   #   ",
	L"#######"
};

static const wchar_t* GLYPH_그[GLYPH_HEIGHT] = {
	L" ##### ",
	L"     # ",
	L"     # ",
	L"     # ",
	L"     # ",
	L"       ",
	L"#######"
};

static const wchar_t* GLYPH_래[GLYPH_HEIGHT] = {
	L"### # #",
	L"  # # #",
	L"  # # #",
	L"### ###",
	L"#   # #",
	L"#   # #",
	L"### # #"
};

static const wchar_t* GLYPH_밍[GLYPH_HEIGHT] = {
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

static const wchar_t* GLYPH_김[GLYPH_HEIGHT] = {
	L"###  # ",
	L"  #  # ",
	L"  #  # ",
	L"       ",
	L" ##### ",
	L" #   # ",
	L" ##### "
};

static const wchar_t* GLYPH_태[GLYPH_HEIGHT] = {
	L"### # #",
	L"#   # #",
	L"#   # #",
	L"### ###",
	L"#   # #",
	L"#   # #",
	L"### # #"
};

static const wchar_t* GLYPH_순[GLYPH_HEIGHT] = {
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
	case L'김': return GLYPH_김;
	case L'태': return GLYPH_태;
	case L'순': return GLYPH_순;
	case L'3': return GLYPH_3;
	case L'D': return GLYPH_D;
	case L'게': return GLYPH_게;
	case L'임': return GLYPH_임;
	case L'프': return GLYPH_프;
	case L'로': return GLYPH_로;
	case L'그': return GLYPH_그;
	case L'래': return GLYPH_래;
	case L'밍': return GLYPH_밍;
	case L'1': return GLYPH_1;
	default:    return nullptr;
	}
}