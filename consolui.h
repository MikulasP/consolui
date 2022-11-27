#include <windows.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h> 

/**
*	Struct to store text related modifiers
*/
typedef struct CUI_TEXTSTYLE {
	uint8_t textFGColorR;				///< Text foreground color red component
	uint8_t textFGColorG;				///< Text foreground color green component
	uint8_t textFGColorB;				///< Text foreground color blue component
	uint8_t textBGColorR;				///< Text background color red component
	uint8_t textBGColorG;				///< Text background color green component
	uint8_t textBGColorB;				///< Text background color blue component
} CUI_TEXTSTYLE;

/**
*	Struct to store screen related modifiers 
*		- Background color RGB components
*		- Vertical alignment on the screen
*		- Horizontal alignment on the screen
*		- Offset from the top of the screen in characters
*		- Offset from the left side of the screen in characters
*		- Line shifting
*		- Clear screen before or after a visual element
*/
typedef struct CUI_SCREENSTYLE {
	uint8_t screenBGColorR;				///< Background color red component
	uint8_t screenBGColorG;				///< Background color green component
	uint8_t screenBGColorB;				///< Background color blue component
	uint8_t verticalAlignment;			///< Menu list's vertical alignment
	uint8_t horizontalAlignment;		///< Menu list's horizontal alignment
	int topOffset;						///< Offset from the top of the screen
	int leftOffset;						///< Offset from the left side of the screen
	size_t lineShift;					///< Shift lines by x ammount. (Only used with menu buttons)
	bool cleanScreenBefore;				///< Clear screen contents before 
	bool cleanScreenAfter;				///< Clear screen contents after done
} CUI_SCREENSTYLE;

//**************************************************
//|	Window info									   |
//**************************************************

/**
*	Get console screen info
* 
*	@returns <CONSOLE_SCREEN_BUFFER_INFO>				Object containing the actual console screen buffer informations
*/
CONSOLE_SCREEN_BUFFER_INFO GetConInfoUI(void);

/**
*	Get console screen size in X dimension
* 
*	@param <CONSOLE_SCREEN_BUFFER_INFO*>screenInfoPtr	Pointer to a console screen buffer info. If NULL actual screen info will be used
*
*	@returns <size_t>									Number of columns in console window
*/
size_t GetConSizeXUI(CONSOLE_SCREEN_BUFFER_INFO* screenInfoPtr);

/**
*	Get console screen size in Y dimension
* 
*	@param <CONSOLE_SCREEN_BUFFER_INFO*>screenInfoPtr	Pointer to a console screen buffer info. If NULL actual screen info will be used
*
*	@returns <size_t>									Number of rows in console window
*/
size_t GetConSizeYUI(CONSOLE_SCREEN_BUFFER_INFO* screenInfoPtr);

/**
*	Get console screen size in X and Y dimension
*	
*	@param <uint16_t*>x									Variable to store the number of columns in
*	@param <uint16_t*>y									Variable to store the number of rows in
*	@param <CONSOLE_SCREEN_BUFFER_INFO*>screenInfoPtr	Pointer to a console screen buffer info. If NULL actual screen info will be used
*/
void GetConSizeXYUI(uint16_t* x, uint16_t* y, CONSOLE_SCREEN_BUFFER_INFO* screenInfoPtr);

/**
*	Get the caret's position on the X axis
* 
*	@param <CONSOLE_SCREEN_BUFFER_INFO*>screenInfoPtr	Pointer to a console screen buffer info. If NULL actual screen info will be used
*
*	@returns <uint16_t>									Caret's actual position on the X axis
*/
uint16_t GetCaretXPosUI(CONSOLE_SCREEN_BUFFER_INFO* screenInfoPtr);

/**
*	Get the caret's position on the Y axis
*
*	@param <CONSOLE_SCREEN_BUFFER_INFO*>screenInfoPtr	Pointer to a console screen buffer info. If NULL actual screen info will be used
*
*	@returns <uint16_t>									Caret's actual position on the Y axis
*/
uint16_t GetCaretYPosUI(CONSOLE_SCREEN_BUFFER_INFO* screenInfoPtr);

/**
*	Get caret's position on both axis
*
*	@param <uint16_t*>x									Pointer to store X position
*	@param <uint16_t*>y									Pointer to store Y position
*	@param <CONSOLE_SCREEN_BUFFER_INFO*>screenInfoPtr	Pointer to a console screen buffer info. If NULL actual screen info will be used
*/
void GetCaretPosUI(uint16_t* x, uint16_t* y, CONSOLE_SCREEN_BUFFER_INFO* screenInfoPtr);

//**************************************************
//|	IO Properties								   |
//**************************************************

/**
*	Move cursor up by <val> lines
*
*	@param <unsigned int>val							Value to move cursor
*/
///< void MoveCaretUpUI(size_t val);

/**
*	Move cursor down by <val> lines
*
*	@param <unsigned int>val							Value to move cursor
*/
///< void MoveCaretDownUI(size_t val);

/**
*	Move cursor right by <val> lines
*
*	@param <unsigned int>val							Value to move cursor
*/
///< void MoveCaretRight(size_t val);

/**
*	Move cursor left by <val> lines
*
*	@param <unsigned int>val							Value to move cursor
*/
///< void MoveCaretLeft(size_t val);

/**
*	Set cursor position in the console window (0,0 = upper left corner) | If passed coordinate < 0 the given coord. will be unchanged
*
*	@param <int>x										Horizontal position. If x < 0 position will remain the samem on the axis
*	@param <int>y										Vertical position. If y < 0 position will remain the samem on the axis
*/
void SetCaretPosUI(int x, int y);

/**
*	Modify the appearance of printed text. Only valid for prints after call.
*
*	@param <unsigned int>argc							Number of modifiers passed to function
*/
void ModifiersUI(size_t argc, ...);

/**
*	Set vertical cursor position (TOP, CENTER, BOTTOM)
*
*	@param <unsigned int>alignment						Cursor alignment (defined value expected)
*/
void AlignCaretVerticalUI(uint8_t alignment);

/**
*	Set horizontal cursor position (LEFT, CENTER, RIGHT)
*
*	@param <unsigned int>alignment						Cursor alignment (defined value expected)
*/
void AlignCaretHorizontalUI(uint8_t alignment);

//**************************************************
//|	Window display								   |
//**************************************************

/**
*	Clear console window 
*/
void ClearUI(void);

/**
*	Reset screen to default parameters
*/
void ResetUI(void);

/**
*	Full reset of the UI (inc. clearing screen, reseting styles and caret pos.)
*/
void CleanUpUI(void);

/**
*	Set console cell background color. Valid until reset or change
*
*	@param <uint8_t>r									Background color red component
*	@param <uint8_t>g									Background color green component
*	@param <uint8_t>b									Background color blue component
*/
void SetBackgroundColorUI(uint8_t r, uint8_t g, uint8_t b);

/**
*	Set console cell foreground color. Valid until reset or change
*
*	@param <uint8_t>r									Text color red component
*	@param <uint8_t>g									Text color green component
*	@param <uint8_t>b									Text color blue component
*/
void SetForegroundColorUI(uint8_t r, uint8_t g, uint8_t b);

/**
*	Print text to conole with set modifiers. (Pls just use printf)
*
*	@param <char*>text									Text to print
*/
void PrintUI(char* text);


///< DELETE
void PreventResizeUI(void);

//**************************************************
//|	Visual elements								   |
//**************************************************

/**
*	Prompt a screen with a selection of items. 
*
*	@param <char**>menuItems							List of selectable menu items
*	@param <size_t>itemCount							Menu item counts
*	@param <CUI_TEXTSTYLE*>textStylePtr					Pointe to printed text modifiers. If NULL default values will be used
*	@param <CUI_SCREENSTYLE*>screenStylePtr				Pointer to screen modifiers. If NULL default values will be used
*
*	@returns <int>										Index of selected item, or negative on error (See documentation page -X- )
*/
int PromptMenuScreenUI(char** menuItems, size_t itemCount, CUI_TEXTSTYLE* textStylePtr, CUI_SCREENSTYLE* menuStylePtr);
/*
*	Error values
*	
*	-0x01:  Menu session exited without selection (How did u do it??)
*	-0x02:  menuItems NULL or itemCount < 1
*/

/**
*	Draw selectable menu items to screen.
*
*	@param <char**>menuItems							List of selectable menu items
*	@param <size_t>itemCount							Menu item counts
*	@param <size_t>longestMenuItemWidth					Length of the longest menu item in characters
*	@param <CUI_TEXTSTYLE*>textStylePtr					Pointe to printed text modifiers. (Function returns if NULL)
*	@param <CUI_SCREENSTYLE*>screenStylePtr				Pointer to screen modifiers. (Function returns if NULL)
*	@param <size_t>startPointH							X coordinate of menu's top left corner
*	@param <size_t>startPointV							Y coordinate of menu's top left corner
*	@param <int>selectedItem							Highlight the selected menu item.
*	@param <CONSOLE_SCREEN_BUFFER_INFO>	screenInfo		Screen info for scaling
*
*/
void DrawMenuItemsUI(char** menuItems, size_t itemCount, size_t longestMenuItemWidth, CUI_TEXTSTYLE* textStylePtr, CUI_SCREENSTYLE* screenStylePtr, size_t startPointH, size_t startPointV, int selectedItem, CONSOLE_SCREEN_BUFFER_INFO screenInfo);

/**
*	Draw menu background and def. menu bar.
*
*	@param <char**>menuItems							List of selectable menu items
*	@param <size_t>itemCount							Menu item counts
*	@param <CUI_TEXTSTYLE*>textStylePtr					Pointe to printed text modifiers. (Function returns if NULL)
*	@param <CUI_SCREENSTYLE*>screenStylePtr				Pointer to screen modifiers. (Function returns if NULL)
*	@param <CONSOLE_SCREEN_BUFFER_INFO>screenInfo		Screen buffer info for scaling
*/
void DrawMenuScreenUI(char** menuItems, size_t itemCount, CUI_TEXTSTYLE* textStylePtr, CUI_SCREENSTYLE* screenStylePtr, CONSOLE_SCREEN_BUFFER_INFO screenInfo);

/**
*	Fill background with given color
*	
*	@param <CUI_SCREENSTYLE>style						Screen modifiers
*	@param <CONSOLE_SCREEN_BUFFER_INFO*>screenInfoPtr	Pointer to screen buffer info. If NULL actual buffer info will be used
*/
void DrawBackgroundColorUI(CUI_SCREENSTYLE style, CONSOLE_SCREEN_BUFFER_INFO* screenInfoPtr);

/**
*	Text input prompt
*	
*	@param <char*>title									Title of the popup
*	@param <char*>label									Label nex to the input
*	@param <CUI_TEXTSTYLE*>textStyle					Pointe to printed text modifiers. If NULL default values will be used
*	@param <CUI_SCREENSTYLE*>screenStyle				Pointer to screen modifiers. If NULL default values will be used
*	@returns <char*>									Pointer to a dinamicaly allocated array with the result. User MUST free array after use.
*/
char* TextInputPopUpUI(char* title, char* label, CUI_TEXTSTYLE* textStylePtr, CUI_SCREENSTYLE* screenStylePtr, CONSOLE_SCREEN_BUFFER_INFO* bufferInfoPtr);

/**
*	Print multiple lines, formatted (e.g. ASCII art)
* 
*	@param <char*>text									Text to print on the screen. Lines separated by [separator] param.
*	@param <char>separator								Character separating lines
*	@param <CUI_TEXTSTYLE*>testStylePtr					Pointer to text modifiers. If NULL default values will be used
*	@param <CUI_SCREENSTYLE*>screenStylePtr				Pointer to screen modifiers. If NULL default values will be used
*	@param <CONSOLE_SCREEN_BUFFER_INFO*>bufferInfoPtr	Pointer to screen buffer info. If NULL actual screen info will be used
*/
void MultilinePrintUI(char* text, char separator, CUI_TEXTSTYLE* textStylePtr, CUI_SCREENSTYLE* screenStylePtr, CONSOLE_SCREEN_BUFFER_INFO* bufferInfoPtr);

//**************************************************
//|	Scaling										   |
//**************************************************

/**
*	Calculate vertical start point for a given number of items, according to the screenStyle alignment value
* 
*	@param <CUI_SCREENSTYLE*>screenStylePtr				Pointer to screen modifiers. (Function returns 0 if NULL)
*	@param <CONSOLE_SCREEN_BUFFER_INFO>screenInfo		Screen buffer info for scaling
*	@param <size_t>itemCount							Item count to scale alignment
*	@param <bool>lineSpacing							Place an emty line nex to each element
* 
*	@returns <size_t>									Vertical start point for the given element(s) according to alignment
*/
size_t CalculateVerticalStartPointUI(CUI_SCREENSTYLE* screenStylePtr, CONSOLE_SCREEN_BUFFER_INFO screenInfo, size_t itemCount, bool lineSpacing);

/**
*	Calculate horizontal start point for a given number of items, according to the screenStyle alignment value
*
*	@param <CUI_SCREENSTYLE*>screenStylePtr				Pointer to screen modifiers. (Function returns 0 if NULL)
*	@param <CONSOLE_SCREEN_BUFFER_INFO>screenInfo		Screen buffer info for scaling
*	@param <size_t>itemCount							Item width to scale alignment
*
*	@returns <size_t>									Horizontal start point for the given element(s) according to alignment
*/
size_t CalculateHorizontalStartPointUI(CUI_SCREENSTYLE* screenStylePtr, CONSOLE_SCREEN_BUFFER_INFO screenInfo, size_t itemWidth);

/**
*	Chack if window size changed
* 
*	@param <CONSOLE_SCREEN_BUFFER_INFO>screenInfo		Screen buffer info for previous window size reference
* 
*	@returns <bool>										True: If window changed size | False: Window size is the same
*/
bool IsWindowSizeSameUI(CONSOLE_SCREEN_BUFFER_INFO screenInfo);

//**************************************************
//|	Other UI related							   |
//**************************************************

/**
*	Get a CUI_SCREENSTYLE with the default parameters defined
* 
*	@returns <CUI_SCREENSTYLE>
*/
CUI_SCREENSTYLE GetDefaultSreenStyleUI(void);

/**
*	Get a CUI_TEXTSTYLE with the default parameters defined
* 
*	@returns <CUI_TEXTSTYLE>
*/
CUI_TEXTSTYLE GetDefaultTextStyleUI(void);

//**************************************************
//|	Supplementary functions						   |
//**************************************************

/**
*	Get longest string in a 2D array
* 
*	@param <char**>array								2D array to check
*	@param <size_t>length								Number of elements in the array
* 
*	@returns <size_t>									Index of the longest item or 0 if array NULL.
*/
size_t GetLongestStringInArray(char** array, size_t length);

/**
*	Get array element length up to a given character
* 
*	@param <char*>str									Pointer to a char array
*	@param <char>limit									The char to measure the length up to
* 
*	@returns <size_t>									Array element count up to given char or 0 if array NULL
*/
size_t GetStrLenChar(char* str, char limit);

/**
*	Count the number of a given character in an array
* 
*	@param <char*>str									Array to count elements in
*	@param <char>limit									The char to count
* 
*	@returns <size_t>									How many times the character appears in the array, or 0 if array NULL
*/
size_t GetStrCharCount(char* str, char toCount);

//**************************************************
//|	ANSI Constans								   |
//**************************************************

#define CUI_ANSI_CHAR_ESC		0x1B		///< ESC character code

///< ANSI codes for text styles
#define	CUI_ANSI_RESET			0x00		///< Reset text style
#define	CUI_ANSI_SET_BOLD		0x01		///< Bold font ANSI constant
#define	CUI_ANSI_SET_FAINT		0x02		///< Faint font ANSI constant
#define	CUI_ANSI_SET_ITALIC		0x03		///< Italic font ANSI constant
#define	CUI_ANSI_SET_SULINE		0x04		///< Single underline ANSI constant
#define	CUI_ANSI_SET_STRIKE		0x09		///< Striked font ANSI constant
#define	CUI_ANSI_SET_DULINE		0x15		///< Dual underline ANSI constant
#define	CUI_ANSI_RST_BOLD		0x16		///< Reset bold fonts ANSI constant
#define	CUI_ANSI_RST_FAINT		0x16		///< Reset faint fonts ANSI constant
#define	CUI_ANSI_RST_ITALIC		0x17		///< Reset italic fonts ANSI constant
#define	CUI_ANSI_RST_ULINE		0x18		///< Reset single underline fonts ANSI constant
#define	CUI_ANSI_RST_STRIKE		0x1D		///< Reset striked fonts ANSI constant

///< ANSI foreground color codes

#define	CUI_ANSI_FG_RESET		0x00		///< Reset foreground color
#define	CUI_ANSI_FG_BLACK		0x1E		///< Foreground black color ANSI constant
#define	CUI_ANSI_FG_RED			0x1F		///< Foreground red color ANSI constant
#define	CUI_ANSI_FG_GREEN		0x20		///< Foreground green color ANSI constant
#define	CUI_ANSI_FG_YELLOW		0x21		///< Foreground yellow color ANSI constant
#define	CUI_ANSI_FG_BLUE		0x22		///< Foreground blue color ANSI constant
#define	CUI_ANSI_FG_MAGENTA		0x23		///< Foreground magenta color ANSI constant
#define	CUI_ANSI_FG_CYAN		0x24		///< Foreground cyan color ANSI constant
#define	CUI_ANSI_FG_WHITE		0x25		///< Foreground white color ANSI constant
#define	CUI_ANSI_FG_DEFAULT		0x27		///< Default foreground color ANSI constant

///< ANSI backround color codes
#define	CUI_ANSI_BG_RESET		0x00		///< Reset background color
#define	CUI_ANSI_BG_BLACK		0x28		///< Background black color ANSI constant
#define	CUI_ANSI_BG_RED			0x29		///< Background red color ANSI constant
#define	CUI_ANSI_BG_GREEN		0x2A		///< Background green color ANSI constant
#define	CUI_ANSI_BG_YELLOW		0x2B		///< Background yellow color ANSI constant
#define	CUI_ANSI_BG_BLUE		0x2C		///< Background blue color ANSI constant
#define	CUI_ANSI_BG_MAGENTA		0x2D		///< Background magenta color ANSI constant
#define	CUI_ANSI_BG_CYAN		0x2E		///< Background cyan color ANSI constant
#define	CUI_ANSI_BG_WHITE		0x2F		///< Background white color ANSI constant
#define	CUI_ANSI_BG_DEFAULT		0x31		///< Default background color ANSI constant

///< Text alignment labels
#define CUI_ALIGN_LEFT			0x01		///< Constant value for left alignemnt
#define CUI_ALIGN_RIGHT			0x02		///< Constant value for right alignemnt
#define CUI_ALIGN_TOP			0x03		///< Constant value for top alignemnt
#define CUI_ALIGN_BOTTOM		0x04		///< Constant value for bottom alignemnt
#define CUI_ALIGN_CENTER		0x05		///< Constant value for center alignemnt

///<  **************************************************
///<  |	Default values								|
///<  **************************************************

///< CUI_SCREENSTYLE defaults
#define CUI_SCREENS_DEF_BG_R		0x00				///< Default screen background color red component
#define CUI_SCREENS_DEF_BG_G		0x00				///< Default screen background color green component
#define CUI_SCREENS_DEF_BG_B		0x00				///< Default screen background color blue component
#define CUI_SCREEN_DEF_ALIGN_V		CUI_ALIGN_CENTER	///< Default vertical alignment
#define CUI_SCREEN_DEF_ALIGN_H		CUI_ALIGN_CENTER	///< Default horizontal alignment
#define CUI_SCREEN_DEF_CLR_B		true				///< Whether to clear screen before display
#define CUI_SCREEN_DEF_CLR_A		true				///< Whether to clear screen after done

///< CUI_TEXTSTYLE defaults
#define CUI_TEXT_DEF_FG_R			0xFF				///< Default text foreground color red component
#define CUI_TEXT_DEF_FG_G			0xFF				///< Default text foreground color green component
#define CUI_TEXT_DEF_FG_B			0xFF				///< Default text foreground color blue component
#define CUI_TEXT_DEF_BG_R			0x00				///< Default text background color red component
#define CUI_TEXT_DEF_BG_G			0x00				///< Default text background color green component
#define CUI_TEXT_DEF_BG_B			0x00				///< Default text background color blue component

///< 
#define CUI_MENU_BUTTON_OP '{'							///< Menu button opening char
#define CUI_MENU_BUTTON_CL '}'							///< Menu button closing char
#define CUI_MENU_EXIT_LABEL "Exit application"			///< Label next to menu bar exit button [F10]

#define CUI_MENU_LABEL		"Code by: Mikuas Peter Marcell - SYZMBA"		///< Label on the console menu bar

#define CUI_POPUP_INPUT_WIDTH		30					///< PopUp window input width in characters
