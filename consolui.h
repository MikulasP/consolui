#include <windows.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

// **************************************************
// *	ANSI Constans
// **************************************************

#define CUI_ANSI_CHAR_ESC		0x1B

//ANSI codes for text styles

#define	CUI_ANSI_RESET			0x00
#define	CUI_ANSI_SET_BOLD		0x01
#define	CUI_ANSI_SET_FAINT		0x02
#define	CUI_ANSI_SET_ITALIC		0x03
#define	CUI_ANSI_SET_SULINE		0x04
#define	CUI_ANSI_SET_BLINK		0x05
#define	CUI_ANSI_SET_INVERSE	0x07
#define	CUI_ANSI_SET_HIDDEN		0x08
#define	CUI_ANSI_SET_STRIKE		0x09
#define	CUI_ANSI_SET_DULINE		0x15
//
#define	CUI_ANSI_RST_BOLD		0x16
#define	CUI_ANSI_RST_FAINT		0x16
#define	CUI_ANSI_RST_ITALIC		0x17
#define	CUI_ANSI_RST_ULINE		0x18
#define	CUI_ANSI_RST_BLINK		0x19
#define	CUI_ANSI_RST_INVERSE	0x1B
#define	CUI_ANSI_RST_HIDDEN		0x1C
#define	CUI_ANSI_RST_STRIKE		0x1D

//ANSI foreground color codes

#define	CUI_ANSI_FG_RESET		0x00
#define	CUI_ANSI_FG_BLACK		0x1E
#define	CUI_ANSI_FG_RED			0x1F
#define	CUI_ANSI_FG_GREEN		0x20
#define	CUI_ANSI_FG_YELLOW		0x21
#define	CUI_ANSI_FG_BLUE		0x22
#define	CUI_ANSI_FG_MAGENTA		0x23
#define	CUI_ANSI_FG_CYAN		0x24
#define	CUI_ANSI_FG_WHITE		0x25
#define	CUI_ANSI_FG_DEFAULT		0x27

//ANSI backround color codes

#define	CUI_ANSI_BG_RESET		0x00
#define	CUI_ANSI_BG_BLACK		0x28
#define	CUI_ANSI_BG_RED			0x29
#define	CUI_ANSI_BG_GREEN		0x2A
#define	CUI_ANSI_BG_YELLOW		0x2B
#define	CUI_ANSI_BG_BLUE		0x2C
#define	CUI_ANSI_BG_MAGENTA		0x2D
#define	CUI_ANSI_BG_CYAN		0x2E
#define	CUI_ANSI_BG_WHITE		0x2F
#define	CUI_ANSI_BG_DEFAULT		0x31


// **************************************************
// |	Window info									|
// **************************************************

/**
	Get console screen info
 
	@returns CONSOLE_SCREEN_BUFFER_INFO
*/
CONSOLE_SCREEN_BUFFER_INFO GetConInfo(void);

/**
	Get console screen size in X dimension

	@returns size_t - Number of columns in console window
*/
size_t GetConSizeX(void);

/**
	Get console screen size in Y dimension

	@returns size_t - Number of rows in console window
*/
size_t GetConSizeY(void);

/**
	Get console screen size in X and Y dimension

	@param <unsigned int*>x		Variable to store the number of columns in
	@param <unsigned int*>y		Variable to store the number of rows in
*/
void GetConSizeXY(unsigned int* x, unsigned int* y);


// **************************************************
// |	IO Properties								|
// **************************************************


/**
	Set cursor position in the console window (0,0 = upper left corner)

	@param <unsigned int>x		Horizontal position
	@param <unsigned int>y		Vertical position
*/
void SetCursorPosUI(unsigned int x, unsigned int y);

/**
	Modify the appearance of the printed text. Only valid for prints after call.

	@param <unsigned int>argc		Number of modifiers passed to function
*/
void ModifiersUI(unsigned int argc, ...);


// **************************************************
// |	Window display								|
// **************************************************

/**
	Clear console window 
*/
void ClearUI(void);

/**
	Reset to default modifiers
*/
void ResetUI(void);

/**
	Set console cell background color. Valid until reset or change

	@param <uint8_t>r		color red component
	@param <uint8_t>g		color green component
	@param <uint8_t>b		color blue component

*/
void SetBackgroundUI(uint8_t r, uint8_t g, uint8_t b);

/**
	Set console cell foreground color. Valid until reset or change

	@param <uint8_t>r		color red component
	@param <uint8_t>g		color green component
	@param <uint8_t>b		color blue component

*/
void SetForegroundUI(uint8_t r, uint8_t g, uint8_t b);

/**
	Print text to conole with set modifiers

	@param <char*>text		Text to print
*/
void PrintUI(char* text);
