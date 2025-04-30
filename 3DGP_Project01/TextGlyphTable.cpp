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

static const wchar_t* GLYPH_1[GLYPH_HEIGHT] = {
	L"   #   ",
	L"  ##   ",
	L"   #   ",
	L"   #   ",
	L"   #   ",
	L"   #   ",
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

static const wchar_t* GLYPH_A[GLYPH_HEIGHT] = {
	L" ##### ",
	L"#     #",
	L"#     #",
	L"#######",
	L"#     #",
	L"#     #",
	L"#     #"
};

static const wchar_t* GLYPH_E[GLYPH_HEIGHT] = {
	L"#######",
	L"#      ",
	L"#      ",
	L"###### ",
	L"#      ",
	L"#      ",
	L"#######"
};

static const wchar_t* GLYPH_L[GLYPH_HEIGHT] = {
	L"#      ",
	L"#      ",
	L"#      ",
	L"#      ",
	L"#      ",
	L"#      ",
	L"#######"
};

static const wchar_t* GLYPH_N[GLYPH_HEIGHT] = {
	L"#     #",
	L"##    #",
	L"# #   #",
	L"#  #  #",
	L"#   # #",
	L"#    ##",
	L"#     #"
};

static const wchar_t* GLYPH_O[GLYPH_HEIGHT] = {
	L" ##### ",
	L"#     #",
	L"#     #",
	L"#     #",
	L"#     #",
	L"#     #",
	L" ##### "
};

static const wchar_t* GLYPH_R[GLYPH_HEIGHT] = {
	L"###### ",
	L"#     #",
	L"#     #",
	L"###### ",
	L"#   #  ",
	L"#    # ",
	L"#     #"
};

static const wchar_t* GLYPH_I[GLYPH_HEIGHT] = {
	L" ##### ",
	L"   #   ",
	L"   #   ",
	L"   #   ",
	L"   #   ",
	L"   #   ",
	L" ##### "
};

static const wchar_t* GLYPH_S[GLYPH_HEIGHT] = {
	L" ##### ",
	L"#     #",
	L"#      ",
	L" ##### ",
	L"      #",
	L"#     #",
	L" ##### "
};

static const wchar_t* GLYPH_T[GLYPH_HEIGHT] = {
	L"#######",
	L"   #   ",
	L"   #   ",
	L"   #   ",
	L"   #   ",
	L"   #   ",
	L"   #   "
};

static const wchar_t* GLYPH_U[GLYPH_HEIGHT] = {
	L"#     #",
	L"#     #",
	L"#     #",
	L"#     #",
	L"#     #",
	L"#     #",
	L" ##### "
};

static const wchar_t* GLYPH_V[GLYPH_HEIGHT] = {
	L"#     #",
	L"#     #",
	L"#     #",
	L"#     #",
	L" #   # ",
	L"  # #  ",
	L"   #   "
};

static const wchar_t* GLYPH_TWODASH[GLYPH_HEIGHT] = {
	L"       ",
	L"       ",
	L"       ",
	L" ##### ",
	L"       ",
	L"       ",
	L"       "
};

static const wchar_t* GLYPH_2[GLYPH_HEIGHT] = {
	L" ##### ",
	L"#     #",
	L"      #",
	L"  #### ",
	L" #     ",
	L"#      ",
	L"#######"
};

static const wchar_t* GLYPH_Y[GLYPH_HEIGHT] = {
	L"#     #",
	L" #   # ",
	L"  # #  ",
	L"   #   ",
	L"   #   ",
	L"   #   ",
	L"   #   "
};

static const wchar_t* GLYPH_W[GLYPH_HEIGHT] = {
	L"#     #",
	L"#     #",
	L"#  #  #",
	L"#  #  #",
	L"#  #  #",
	L"#  #  #",
	L" ## ## "
};

static const wchar_t* GLYPH_Exclamation[GLYPH_HEIGHT] = {
	L"   #   ",
	L"   #   ",
	L"   #   ",
	L"   #   ",
	L"       ",
	L"   #   ",
	L"   #   "
};

const wchar_t** GetGlyph(wchar_t ch)
{
	switch (ch)
	{
	case L'김': return GLYPH_김;
	case L'태': return GLYPH_태;
	case L'순': return GLYPH_순;
	case L'3': return GLYPH_3;
	case L'1': return GLYPH_1;
	case L'2': return GLYPH_2;
	case L'D': return GLYPH_D;

	case L'게': return GLYPH_게;
	case L'임': return GLYPH_임;
	case L'프': return GLYPH_프;
	case L'로': return GLYPH_로;
	case L'그': return GLYPH_그;
	case L'래': return GLYPH_래;
	case L'밍': return GLYPH_밍;

	case L'A': return GLYPH_A;
	case L'E': return GLYPH_E;
	case L'L': return GLYPH_L;
	case L'N': return GLYPH_N;
	case L'O': return GLYPH_O;
	case L'R': return GLYPH_R;
	case L'I': return GLYPH_I;
	case L'S': return GLYPH_S;
	case L'T': return GLYPH_T;
	case L'U': return GLYPH_U;
	case L'V': return GLYPH_V;
	case L'-': return GLYPH_TWODASH;
	case L'Y': return GLYPH_Y;
	case L'W': return GLYPH_W;
	case L'!': return GLYPH_Exclamation;

	default:    return GLYPH_EMPTY;
	}
}
