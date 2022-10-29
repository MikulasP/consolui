#include "consolui.h"


CONSOLE_SCREEN_BUFFER_INFO GetConInfo(void) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    return info;
}

//
size_t GetConSizeX(void) {
    CONSOLE_SCREEN_BUFFER_INFO info = GetConInfo();
    return info.srWindow.Right - info.srWindow.Left + 1;
}

//
size_t GetConSizeY(void) {
    CONSOLE_SCREEN_BUFFER_INFO info = GetConInfo();
    return info.srWindow.Bottom - info.srWindow.Top + 1;
}

//
void GetConSizeXY(unsigned int* x, unsigned int* y) {
    CONSOLE_SCREEN_BUFFER_INFO info = GetConInfo();
    *x = info.srWindow.Right - info.srWindow.Left + 1;
    *y = info.srWindow.Bottom - info.srWindow.Top + 1;
}

//
void SetCursorPosUI(unsigned int x, unsigned int y) {
    printf("%c[%u;%uH", CUI_ANSI_CHAR_ESC, y, x);        //Moving cursor using ANSI Escape Codes
}

//
void ModifiersUI(unsigned int argc, ...) {
    va_list list;
    va_start(list, argc);

    char* args = malloc((argc * 3) * sizeof(char));
    args[0] = '\0';

    for (int i = 0; i < argc - 1; i++)
        sprintf(args, "%s%d;", args, va_arg(list, int));

    sprintf(args, "%s%dm", args, va_arg(list, int));

    printf("%c[%s", CUI_ANSI_CHAR_ESC, args);

    free(args);

    va_end(list);
}

//
void ClearUI(void) {
    system("clear");
}

//
void ResetUI(void) {
    printf("%c[%um", CUI_ANSI_CHAR_ESC, CUI_ANSI_RESET);
}

//
void SetBackgroundUI(uint8_t r, uint8_t g, uint8_t b) {
    printf("%c[48;2;%u;%u;%um", CUI_ANSI_CHAR_ESC, r, g, b);
}

void SetForegroundUI(uint8_t r, uint8_t g, uint8_t b) {
    printf("%c[38;2;%u;%u;%um", CUI_ANSI_CHAR_ESC, r, g, b);
}

void PrintUI(char* text) {
    printf(text);
}