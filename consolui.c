#include "consolui.h"

// **************************************************
// |	Window info									|
// **************************************************

//
CONSOLE_SCREEN_BUFFER_INFO GetConInfoUI(void) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    return info;
}

//
size_t GetConSizeXUI(CONSOLE_SCREEN_BUFFER_INFO* screenInfo) {
    CONSOLE_SCREEN_BUFFER_INFO info = screenInfo == NULL ? GetConInfoUI() : *screenInfo;
    return info.srWindow.Right - info.srWindow.Left + 1;
}

//
size_t GetConSizeYUI(CONSOLE_SCREEN_BUFFER_INFO* screenInfo) {
    CONSOLE_SCREEN_BUFFER_INFO info = screenInfo == NULL ? GetConInfoUI() : *screenInfo;
    return info.srWindow.Bottom - info.srWindow.Top + 1;
}

//
void GetConSizeUI(CONSOLE_SCREEN_BUFFER_INFO* screenInfo, uint32_t* x, uint32_t* y) {
    CONSOLE_SCREEN_BUFFER_INFO info = screenInfo == NULL ? GetConInfoUI() : *screenInfo;
    *x = info.srWindow.Right - info.srWindow.Left + 1;
    *y = info.srWindow.Bottom - info.srWindow.Top + 1;
}

//
uint16_t GetCaretXPosUI(CONSOLE_SCREEN_BUFFER_INFO* screenInfo) {
    CONSOLE_SCREEN_BUFFER_INFO info = screenInfo == NULL ? GetConInfoUI() : *screenInfo;
    return info.dwCursorPosition.X;
}

//
uint16_t GetCaretYPosUI(CONSOLE_SCREEN_BUFFER_INFO* screenInfo) {
    CONSOLE_SCREEN_BUFFER_INFO info = screenInfo == NULL ? GetConInfoUI() : *screenInfo;
    return info.dwCursorPosition.Y;
}

//
void GetCaretPosUI(uint16_t* x, uint16_t* y) {
    *x = GetCaretXPosUI(NULL);
    *y = GetCaretYPosUI(NULL);
}

// **************************************************
// |	IO Properties								|
// **************************************************

//
void SetCaretPosUI(int x, int y) {
    printf("%c[%u;%uH", CUI_ANSI_CHAR_ESC, (y > -1 ? y : GetCaretYPosUI(NULL)), (x > -1 ? x : GetCaretXPosUI(NULL)));        //Moving cursor using ANSI Escape Codes
}

//
void ModifiersUI(size_t argc, ...) {
    va_list list;
    va_start(list, argc);

    size_t allocSize = (argc * 4 + 3) * sizeof(char);
    char* args = malloc(allocSize);
    args[0] = '\0';

    for (int i = 0; i < argc - 1; i++)
        sprintf(args, "%s%d;", args, va_arg(list, int));

    sprintf(args, "%s%dm", args, va_arg(list, int));

    printf("%c[%s", CUI_ANSI_CHAR_ESC, args);

    free(args);

    va_end(list);
}

//
void AlignCaretVerticalUI(uint8_t alignment) {
    switch (alignment)
    {
    case CUI_ALIGN_TOP:
        SetCaretPosUI(-1, 0);
        break;

    case CUI_ALIGN_CENTER:
        SetCaretPosUI(-1, GetConSizeYUI(NULL) / 2);
        break;

    case CUI_ALIGN_BOTTOM:
        SetCaretPos(-1, GetConSizeYUI(NULL) - 1);
        break;

    default:
        break;
    }
}

//
void AlignCaretHorizontalUI(uint8_t alignment) {
    switch (alignment)
    {
    case CUI_ALIGN_LEFT:
        SetCaretPosUI(0, -1);
        break;

    case CUI_ALIGN_CENTER:
        SetCaretPosUI(GetConSizeXUI(NULL) / 2, -1);
        break;

    case CUI_ALIGN_RIGHT:
        SetCaretPosUI(GetConSizeXUI(NULL) - 1, -1);
        break;

    default:
        break;
    }
}

// **************************************************
// |	Window display								|
// **************************************************

//
void ClearUI(void) {
    system("cls");
}

//
void ResetUI(void) {
    printf("%c[%um", CUI_ANSI_CHAR_ESC, CUI_ANSI_RESET);
}

//
void SetBackgroundColorUI(uint8_t r, uint8_t g, uint8_t b) {
    printf("%c[48;2;%u;%u;%um", CUI_ANSI_CHAR_ESC, r, g, b);
}

//
void SetForegroundColorUI(uint8_t r, uint8_t g, uint8_t b) {
    printf("%c[38;2;%u;%u;%um", CUI_ANSI_CHAR_ESC, r, g, b);
}

//
void PrintUI(char* text) {
    printf(text);
}

// **************************************************
// |	Visual elements								|
// **************************************************

//
int PromptMenuScreenUI(char** menuItems, size_t itemCount, CUI_TEXTSTYLE* textStyle, CUI_SCREENSTYLE* screenStyle) {
    if (menuItems == NULL || itemCount < 1)      return -0x02;
    
    //Clean up
    ResetUI();
    ClearUI();

    CONSOLE_SCREEN_BUFFER_INFO screenInfo = GetConInfoUI();

    size_t selectedItem = 0;

    size_t buttonWidth = strlen(menuItems[GetLongestStringInArray(menuItems, itemCount)]);

    uint32_t startPointV = CalculateVerticalStartPointUI(screenStyle, screenInfo, itemCount);
    uint32_t startPointH = CalculateHorizontalStartPointUI(screenStyle, screenInfo, buttonWidth);    

    DrawMenuScreenUI(menuItems, itemCount, textStyle, screenStyle, selectedItem, screenInfo);
    DrawMenuItemsUI(menuItems, itemCount, buttonWidth, textStyle, screenStyle, startPointH, startPointV, selectedItem, screenInfo);

    while (1) {

        //Separate function maybe...?
        if (!IsWindowSizeSameUI(screenInfo)) {
            screenInfo = GetConInfoUI();
            startPointV = CalculateVerticalStartPointUI(screenStyle, screenInfo, itemCount);
            startPointH = CalculateHorizontalStartPointUI(screenStyle, screenInfo, buttonWidth);
            DrawMenuScreenUI(menuItems, itemCount, textStyle, screenStyle, selectedItem, screenInfo);
            DrawMenuItemsUI(menuItems, itemCount, buttonWidth, textStyle, screenStyle, startPointH, startPointV, selectedItem, screenInfo);
        }

        SetCaretPosUI(0, 0);
        char input = getch();
        //
        // ----
        // Separate function later...?
        //
        switch (input)
        {
        //ENTER
        case 13:
            ResetUI();
            SetCaretPos(0, 0);
            ClearUI();
            return selectedItem;
        //0 modifier - Key has multiple values
        case 0:
            input = getch();
            switch (input)
            {
            //F10 key
            case 68:
                ResetUI();
                SetCaretPos(0, 0);
                ClearUI();
                return -0x01;
                break;
            default:
                break;
            }
            break;
        //-32 modifier - Key has multiple values
        case -32:
            input = getch();
            switch (input)
            {
            //DOWN key
            case 80:
                if (selectedItem == itemCount - 1)
                    break;
                selectedItem++;
                DrawMenuItemsUI(menuItems, itemCount, buttonWidth, textStyle, screenStyle, startPointH, startPointV, selectedItem, screenInfo);
                break;
            //UP key
            case 72:
                if (selectedItem <= 0)
                    break;
                selectedItem--;
                DrawMenuItemsUI(menuItems, itemCount, buttonWidth, textStyle, screenStyle, startPointH, startPointV, selectedItem, screenInfo);
                break;
            default:
                break;
            }
            

        default:
            break;
        }

        //
        //  ----
        //

    }

    ShowCaret(NULL);
    ResetUI();
    return -0x01;

}
/*
*   Error values
* 
*   -0x01:  Menu session exited without selection
*   -0x02:  menuItems was NULL || itemCount < 1
*/

//
void DrawMenuItemsUI(char** menuItems, size_t itemCount, size_t longestItemWidth, CUI_TEXTSTYLE* textStyle, CUI_SCREENSTYLE* screenStyle, uint32_t startPointH, uint32_t startPointV, size_t selectedItem, CONSOLE_SCREEN_BUFFER_INFO screenInfo) {
    if (textStyle == NULL || screenStyle == NULL)       return;
    
    //Applying text styles
    SetForegroundColorUI(textStyle->textFGColorR, textStyle->textFGColorG, textStyle->textFGColorB);
    SetBackgroundColorUI(textStyle->textBGColorR, textStyle->textBGColorG, textStyle->textBGColorB);

    //Print menu items to screen
    for (size_t i = 0; i < itemCount; i++) {

        uint32_t textStartPointH = (startPointH + 3) + longestItemWidth / 2 - strlen(menuItems[i]) / 2;

        if (i == selectedItem) {
            SetForegroundColorUI(255 - textStyle->textFGColorR, 255 - textStyle->textFGColorG, 255 - textStyle->textFGColorB);
            SetBackgroundColorUI(255 - textStyle->textBGColorR, 255 - textStyle->textBGColorG, 255 - textStyle->textBGColorB);
        }
        SetCaretPosUI(startPointH, startPointV);
        printf("[ ");
        for (size_t space = 0; space < longestItemWidth + 2; space++)
            printf(" ");
        printf(" ]");

        SetCaretPosUI(textStartPointH, startPointV);
        printf("%s", menuItems[i]);

        startPointV += 2;

        if (i == selectedItem) {
            SetForegroundColorUI(textStyle->textFGColorR, textStyle->textFGColorG, textStyle->textFGColorB);
            SetBackgroundColorUI(textStyle->textBGColorR, textStyle->textBGColorG, textStyle->textBGColorB);
        }
    }
}

//
void DrawMenuScreenUI(char** menuItems, size_t itemCount, CUI_TEXTSTYLE* textStyle, CUI_SCREENSTYLE* screenStyle, size_t selectedItem, CONSOLE_SCREEN_BUFFER_INFO screenInfo) {
    if (textStyle == NULL || screenStyle == NULL)       return;
    SetCaretPosUI(0, 0);
    
    //Applying screen styles if there is any
    if (screenStyle != NULL) {
        //Background coloring
        SetBackgroundColorUI(screenStyle->screenBGColorR, screenStyle->screenBGColorG, screenStyle->screenBGColorB);
        if (screenStyle != NULL) {
            //Background coloring
            SetBackgroundColorUI(screenStyle->screenBGColorR, screenStyle->screenBGColorG, screenStyle->screenBGColorB);
            char* voidFilledArray = malloc((GetConSizeXUI(&screenInfo) + 2) * sizeof(char));
            for (size_t x = 0; x <= GetConSizeXUI(&screenInfo); x++)
                voidFilledArray[x] = 32;
            voidFilledArray[GetConSizeXUI(&screenInfo) + 1] = '\0';
            for (size_t y = 0; y < GetConSizeYUI(&screenInfo); y++) {
                SetCaretPosUI(0, y);
                printf("%s", voidFilledArray);
            }
            free(voidFilledArray);
        }
    }

    SetCaretPosUI(0, GetConSizeYUI(&screenInfo) - 3);

    for (size_t i = 0; i < GetConSizeXUI(&screenInfo); i++)
        printf("_");

    SetForegroundColorUI(textStyle->textFGColorR, textStyle->textFGColorG, textStyle->textFGColorB);
    SetBackgroundColorUI(textStyle->textBGColorR, textStyle->textBGColorG, textStyle->textBGColorB);

    for (size_t y = GetConSizeYUI(&screenInfo) - 2; y <= GetConSizeYUI(&screenInfo); y++) {
        SetCaretPosUI(0, y);
        for (size_t x = 0; x < GetConSizeXUI(&screenInfo); x++)
            printf(" ");
    }

    SetCaretPosUI(0, GetConSizeYUI(&screenInfo) - 1);
    printf("\tExit [F10]");

}

// **************************************************
// |	Scaling										|
// **************************************************

//
uint32_t CalculateVerticalStartPointUI(CUI_SCREENSTYLE* screenStyle, CONSOLE_SCREEN_BUFFER_INFO screenInfo, size_t menuItemCount) {
    //Vertical alignment calculation
    switch (screenStyle->verticalAlignment)
    {
        //Alignt items to top
    case CUI_ALIGN_TOP:
        return 1 + screenStyle->topOffset;
        break;
        //Alignt items to center
    case CUI_ALIGN_CENTER:
        return (GetConSizeYUI(&screenInfo) - (menuItemCount * 2 - 1)) / 2;
        break;
        //Alignt items to bottom
    case CUI_ALIGN_BOTTOM:
        return GetConSizeYUI(&screenInfo) - (menuItemCount * 2 - 1) - 3; //
        break;
        //On other values defaults to center alignment
    default:
        return (GetConSizeYUI(&screenInfo) - (menuItemCount * 2 - 1)) / 2;
        break;
    }
}

uint32_t CalculateHorizontalStartPointUI(CUI_SCREENSTYLE* screenStyle, CONSOLE_SCREEN_BUFFER_INFO screenInfo, size_t menuItemMaxLength) {
    //Horizontal alignment calculation
    switch (screenStyle->horizontalAlignment)
    {
        //Align items to left
    case CUI_ALIGN_LEFT:
        return 0;
        //Alignt items to center
    case CUI_ALIGN_CENTER:
        return (GetConSizeXUI(&screenInfo) - menuItemMaxLength) / 2 - 3;
        //Alignt items to right
    case CUI_ALIGN_RIGHT:
        return (GetConSizeXUI(&screenInfo) - menuItemMaxLength) - 5;
        //On other values defaults to center alignment
    default:
        return (GetConSizeXUI(&screenInfo) - menuItemMaxLength) / 2 - 3;
    }
}

//
bool IsWindowSizeSameUI(CONSOLE_SCREEN_BUFFER_INFO screenInfo) {
    CONSOLE_SCREEN_BUFFER_INFO newInfo = GetConInfoUI();
    return GetConSizeXUI(&screenInfo) == GetConSizeXUI(&newInfo) && GetConSizeYUI(&screenInfo) == GetConSizeYUI(&newInfo);
}

// **************************************************
// |	Supplementary functions						|
// **************************************************

//
size_t GetLongestStringInArray(char** array, size_t length) {
    size_t longestIndex = 0;

    for (size_t i = 0; i < length; i++)
        if (strlen(array[i]) > strlen(array[longestIndex]))
            longestIndex = i;

    return longestIndex;
}

//
void DebugMessage(char* msg) {
#ifdef KEY_DEBUG
    SetCaretPosUI(0, 0);
    ModifiersUI(2, CUI_ANSI_BG_BLACK, CUI_ANSI_FG_WHITE);
    for (size_t i = 0; i < 8; i++)
        printf(" ");
    printf("%s", msg);
#endif
}
