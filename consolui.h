#include "debugmalloc.h"

#include <windows.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <conio.h>
#include <stdio.h>

typedef struct CUI_TEXTSTYLE {
	uint8_t textFGColorR;				//Text foreground color red component
	uint8_t textFGColorG;				//Text foreground color green component
	uint8_t textFGColorB;				//Text foreground color blue component
	uint8_t textBGColorR;				//Text background color red component
	uint8_t textBGColorG;				//Text background color green component
	uint8_t textBGColorB;				//Text background color blue component
} CUI_TEXTSTYLE;

typedef struct CUI_SCREENSTYLE {
	uint8_t screenBGColorR;				//Background color red component
	uint8_t screenBGColorG;				//Background color green component
	uint8_t screenBGColorB;				//Background color blue component
	uint8_t verticalAlignment;			//Menu list's vertical alignment (not yet functional)
	uint8_t horizontalAlignment;		//Menu list's horizontal alignment (not yet functional)
	size_t topOffset;					//Offset from the top of the screen (not yet functional)
	uint8_t border;						//Border around the menu screen (not yet functional)
} CUI_SCREENSTYLE;

// **************************************************
// |	Window info									|
// **************************************************

/**
	Get console screen info
 
	@returns CONSOLE_SCREEN_BUFFER_INFO
*/
CONSOLE_SCREEN_BUFFER_INFO GetConInfoUI(void);

/**
	Get console screen size in X dimension

	@returns <size_t>					Number of columns in console window
*/
size_t GetConSizeXUI(void);

/**
	Get console screen size in Y dimension

	@returns <size_t>					Number of rows in console window
*/
size_t GetConSizeYUI(void);

/**
	Get console screen size in X and Y dimension
	
	@param <unsigned int*>x				Variable to store the number of columns in
	@param <unsigned int*>y				Variable to store the number of rows in
*/
void GetConSizeXYUI(uint32_t* x, uint32_t* y);

//
uint16_t GetCaretXPosUI(void);

//
uint16_t GetCaretYPosUI(void);

//
void GetCaretPosUI(uint32_t* x, uint32_t* y);

// **************************************************
// |	IO Properties								|
// **************************************************

/**
	Move cursor up by <val> lines

	@param <unsigned int>val			Value to move cursor
*/
void MoveCaretUpUI(size_t val);

/**
	Move cursor down by <val> lines

	@param <unsigned int>val			Value to move cursor
*/
void MoveCaretDownUI(size_t val);

/**
	Move cursor right by <val> lines

	@param <unsigned int>val			Value to move cursor
*/
void MoveCaretRight(size_t val);

/**
	Move cursor left by <val> lines

	@param <unsigned int>val			Value to move cursor
*/
void MoveCaretLeft(size_t val);

/**
	Set cursor position in the console window (0,0 = upper left corner) | If passed coordinate < 0 the given coord. will be unchanged

	@param <int>x						Horizontal position
	@param <int>y						Vertical position
*/
void SetCaretPosUI(int x, int y);

/**
	Modify the appearance of the printed text. Only valid for prints after call.

	@param <unsigned int>argc			Number of modifiers passed to function
*/
void ModifiersUI(size_t argc, ...);

/**
	Set vertical cursor position (TOP, CENTER, BOTTOM)

	@param <unsigned int>alignment		Cursor alignment (defined value expected)
*/
void AlignCaretVerticalUI(uint8_t alignment);

/**
	Set horizontal cursor position (LEFT, CENTER, RIGHT)

	@param <unsigned int>alignment		Cursor alignment (defined value expected)
*/
void AlignCaretHorizontalUI(uint8_t alignment);

// **************************************************
// |	Window display								|
// **************************************************

/**
	Clear console window 
*/
void ClearUI(void);

/**
	Reset screen to default
*/
void ResetUI(void);

/**
	Set console cell background color. Valid until reset or change

	@param <uint8_t>r					Background color red component
	@param <uint8_t>g					Background color green component
	@param <uint8_t>b					Background color blue component

*/
void SetBackgroundColorUI(uint8_t r, uint8_t g, uint8_t b);

/**
	Set console cell foreground color. Valid until reset or change

	@param <uint8_t>r					Text color red component
	@param <uint8_t>g					Text color green component
	@param <uint8_t>b					Text color blue component

*/
void SetForegroundColorUI(uint8_t r, uint8_t g, uint8_t b);

/**
	Print text to conole with set modifiers. (Pls just use printf)

	@param <char*>text					Text to print
*/
void PrintUI(char* text);

// **************************************************
// |	Visual elements								|
// **************************************************

/**
	Prompt a screen with a selection of items. 

	@param <char**>menuitems							List of selectable menu items
	@param <size_t>itemCount							Menu item counts
	@param <CUI_TEXTSTYLE*>textStyle					Pointe to printed text modifiers. If NULL default values will be used
	@param <CUI_SCREENSTYLE*>screenStyle				Pointer to screen modifiers. If NULL default values will be used

	@returns <int>										Index of selected item, or negative on error (See documentation page -X- )
*/
int PromptMenuScreenUI(char** menuItems, size_t itemCount, CUI_TEXTSTYLE* textStyle, CUI_SCREENSTYLE* menuStyle);

/**
	Draw selectable menu items to screen.

	@param <char**>menuItems							List of selectable menu items
	@param <size_t>itemCount							Menu item counts
	@param
	@param <CUI_TEXTSTYLE*>textStyle					Pointe to printed text modifiers. (Function returns if NULL)
	@param <CUI_SCREENSTYLE*>screenStyle				Pointer to screen modifiers. (Function returns if NULL)
	@param <uint32_t>startPointH
	@param <uint32_t>startPointV
	@param <size_t>selectedItem							Highlight the selected menu item.
	@param <CONSOLE_SCREEN_BUFFER_INFO>	screenInfo		Screen info for scaling

*/
void DrawMenuItemsUI(char** menuItems, size_t itemCount, size_t longestMenuItemWidth, CUI_TEXTSTYLE* textStyle, CUI_SCREENSTYLE* screenStyle, uint32_t startPointH, uint32_t startPointV, size_t selectedItem, CONSOLE_SCREEN_BUFFER_INFO screenInfo);

/**
	Draw menu background and def. menu bar.

	@param <char**>menuItems							List of selectable menu items
	@param <size_t>itemCount							Menu item counts
	@param <CUI_TEXTSTYLE*>textStyle					Pointe to printed text modifiers. (Function returns if NULL)
	@param <CUI_SCREENSTYLE*>screenStyle				Pointer to screen modifiers. (Function returns if NULL)
	@param <size_t>selectedItem							Highlight the selected menu item.
	@param <CONSOLE_SCREEN_BUFFER_INFO>	screenInfo		Screen info for scaling

*/
void DrawMenuScreenUI(char** menuItems, size_t itemCount, CUI_TEXTSTYLE* textStyle, CUI_SCREENSTYLE* menuStyle, size_t selectedItem, CONSOLE_SCREEN_BUFFER_INFO screenInfo);

// **************************************************
// |	Scaling										|
// **************************************************

//
uint32_t CalculateVerticalStartPointUI(CUI_SCREENSTYLE* screenStyle, CONSOLE_SCREEN_BUFFER_INFO screenInfo, size_t menuItemCount);

//
uint32_t CalculateHorizontalStartPointUI(CUI_SCREENSTYLE* screenStyle, CONSOLE_SCREEN_BUFFER_INFO screenInfo, size_t menuItemMaxLength);

//
bool IsWindowSizeSameUI(CONSOLE_SCREEN_BUFFER_INFO screenInfo);

// **************************************************
// |	Supplementary functions						|
// **************************************************

//Get longest string's index
size_t GetLongestStringInArray(char** array, size_t length);

//
void DebugMessage(char msg);

// **************************************************
// |	ANSI Constans								|
// **************************************************

#define CUI_ANSI_CHAR_ESC		0x1B		//

//ANSI codes for text styles
#define	CUI_ANSI_RESET			0x00		//
#define	CUI_ANSI_SET_BOLD		0x01		//
#define	CUI_ANSI_SET_FAINT		0x02		//
#define	CUI_ANSI_SET_ITALIC		0x03		//
#define	CUI_ANSI_SET_SULINE		0x04		//
#define	CUI_ANSI_SET_BLINK		0x05		//
#define	CUI_ANSI_SET_INVERSE	0x07		//
#define	CUI_ANSI_SET_HIDDEN		0x08		//
#define	CUI_ANSI_SET_STRIKE		0x09		//
#define	CUI_ANSI_SET_DULINE		0x15		//
#define	CUI_ANSI_RST_BOLD		0x16		//
#define	CUI_ANSI_RST_FAINT		0x16		//
#define	CUI_ANSI_RST_ITALIC		0x17		//
#define	CUI_ANSI_RST_ULINE		0x18		//
#define	CUI_ANSI_RST_BLINK		0x19		//
#define	CUI_ANSI_RST_INVERSE	0x1B		//
#define	CUI_ANSI_RST_HIDDEN		0x1C		//
#define	CUI_ANSI_RST_STRIKE		0x1D		//

//ANSI foreground color codes
#define	CUI_ANSI_FG_RESET		0x00		//
#define	CUI_ANSI_FG_BLACK		0x1E		//
#define	CUI_ANSI_FG_RED			0x1F		//
#define	CUI_ANSI_FG_GREEN		0x20		//
#define	CUI_ANSI_FG_YELLOW		0x21		//
#define	CUI_ANSI_FG_BLUE		0x22		//
#define	CUI_ANSI_FG_MAGENTA		0x23		//
#define	CUI_ANSI_FG_CYAN		0x24		//
#define	CUI_ANSI_FG_WHITE		0x25		//
#define	CUI_ANSI_FG_DEFAULT		0x27		//

//ANSI backround color codes
#define	CUI_ANSI_BG_RESET		0x00		//Reset background color
#define	CUI_ANSI_BG_BLACK		0x28		//
#define	CUI_ANSI_BG_RED			0x29		//
#define	CUI_ANSI_BG_GREEN		0x2A		//
#define	CUI_ANSI_BG_YELLOW		0x2B		//
#define	CUI_ANSI_BG_BLUE		0x2C		//
#define	CUI_ANSI_BG_MAGENTA		0x2D		//
#define	CUI_ANSI_BG_CYAN		0x2E		//
#define	CUI_ANSI_BG_WHITE		0x2F		//
#define	CUI_ANSI_BG_DEFAULT		0x31		//

//Text alignment labels
#define CUI_ALIGN_LEFT			0x01		//
#define CUI_ALIGN_RIGHT			0x02		//
#define CUI_ALIGN_TOP			0x03		//
#define CUI_ALIGN_BOTTOM		0x04		//
#define CUI_ALIGN_CENTER		0x05		//

// **************************************************
// |	Dev Constans								|
// **************************************************

//#define KEY_DEBUG
