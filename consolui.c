#include "consolui.h"

// **************************************************
// |	Window info									|
// **************************************************

//-
CONSOLE_SCREEN_BUFFER_INFO GetConInfoUI(void) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    return info;
}

//-
size_t GetConSizeXUI(CONSOLE_SCREEN_BUFFER_INFO* screenInfoPtr) {
    CONSOLE_SCREEN_BUFFER_INFO info = screenInfoPtr == NULL ? GetConInfoUI() : *screenInfoPtr;
    return info.srWindow.Right - info.srWindow.Left + 1;
}

//-
size_t GetConSizeYUI(CONSOLE_SCREEN_BUFFER_INFO* screenInfoPtr) {
    CONSOLE_SCREEN_BUFFER_INFO info = screenInfoPtr == NULL ? GetConInfoUI() : *screenInfoPtr;
    return info.srWindow.Bottom - info.srWindow.Top + 1;
}

//-
void GetConSizeXYUI(uint16_t* x, uint16_t* y, CONSOLE_SCREEN_BUFFER_INFO* screenInfoPtr) {
    CONSOLE_SCREEN_BUFFER_INFO info = screenInfoPtr == NULL ? GetConInfoUI() : *screenInfoPtr;
    *x = info.srWindow.Right - info.srWindow.Left + 1;
    *y = info.srWindow.Bottom - info.srWindow.Top + 1;
}

//-
uint16_t GetCaretXPosUI(CONSOLE_SCREEN_BUFFER_INFO* screenInfoPtr) {
    CONSOLE_SCREEN_BUFFER_INFO info = screenInfoPtr == NULL ? GetConInfoUI() : *screenInfoPtr;
    return info.dwCursorPosition.X;
}

//-
uint16_t GetCaretYPosUI(CONSOLE_SCREEN_BUFFER_INFO* screenInfoPtr) {
    CONSOLE_SCREEN_BUFFER_INFO info = screenInfoPtr == NULL ? GetConInfoUI() : *screenInfoPtr;
    return info.dwCursorPosition.Y;
}

//-
void GetCaretPosUI(uint16_t* x, uint16_t* y, CONSOLE_SCREEN_BUFFER_INFO* screenInfoPtr) {
    *x = GetCaretXPosUI(screenInfoPtr);
    *y = GetCaretYPosUI(screenInfoPtr);
}

// **************************************************
// |	IO Properties								|
// **************************************************

//-
void SetCaretPosUI(int x, int y) {
    printf("%c[%u;%uH", CUI_ANSI_CHAR_ESC, (y > -1 ? y : GetCaretYPosUI(NULL)), (x > -1 ? x : GetCaretXPosUI(NULL)));        //Moving cursor using ANSI Escape Codes
}

//-
void ModifiersUI(size_t argc, ...) {
    va_list list;
    va_start(list, argc);

    size_t allocSize = (argc * 4 + 3) * sizeof(char);
    char* args = (char*)malloc(allocSize);
    args[0] = '\0';

    for (int i = 0; i < argc - 1; i++)
        sprintf_s(args, allocSize, "%s%d;", args, va_arg(list, int));

    sprintf_s(args, allocSize, "%s%dm", args, va_arg(list, int));

    printf("%c[%s", CUI_ANSI_CHAR_ESC, args);

    free(args);

    va_end(list);
}

//-
void AlignCaretVerticalUI(uint8_t alignment) {
    switch (alignment)
    {
    case CUI_ALIGN_TOP:
        SetCaretPosUI(-1, 0);
        break;

    case CUI_ALIGN_CENTER:
        SetCaretPosUI(-1, (int)GetConSizeYUI(NULL) / 2);
        break;

    case CUI_ALIGN_BOTTOM:
        SetCaretPos(-1, (int)GetConSizeYUI(NULL) - 1);
        break;

    default:
        break;
    }
}

//-
void AlignCaretHorizontalUI(uint8_t alignment) {
    switch (alignment)
    {
    case CUI_ALIGN_LEFT:
        SetCaretPosUI(0, -1);
        break;

    case CUI_ALIGN_CENTER:
        SetCaretPosUI( (int)GetConSizeXUI(NULL) / 2, -1);
        break;

    case CUI_ALIGN_RIGHT:
        SetCaretPosUI( (int)GetConSizeXUI(NULL) - 1, -1);
        break;

    default:
        break;
    }
}

// **************************************************
// |	Window display								|
// **************************************************

//-
void ClearUI(void) {
    system("cls");
}

//-
void ResetUI(void) {
    printf("%c[%um", CUI_ANSI_CHAR_ESC, CUI_ANSI_RESET);
}

//-
void CleanUpUI(void) {
    ResetUI();
    SetCaretPosUI(0, 0);
    ClearUI();
}

//-
void SetBackgroundColorUI(uint8_t r, uint8_t g, uint8_t b) {
    printf("%c[48;2;%u;%u;%um", CUI_ANSI_CHAR_ESC, r, g, b);
}

//-
void SetForegroundColorUI(uint8_t r, uint8_t g, uint8_t b) {
    printf("%c[38;2;%u;%u;%um", CUI_ANSI_CHAR_ESC, r, g, b);
}

//-
void PrintUI(char* text) {
    printf(text);
}

// **************************************************
// |	Visual elements								|
// **************************************************

//-
int PromptMenuScreenUI(char** menuItems, size_t itemCount, CUI_TEXTSTYLE* textStylePtr, CUI_SCREENSTYLE* menuStylePtr) {
    if (menuItems == NULL || itemCount < 1)      return -0x02;

    CUI_SCREENSTYLE screenStyle = menuStylePtr == NULL ? GetDefaultSreenStyleUI() : *menuStylePtr;
    CUI_TEXTSTYLE textStyle = textStylePtr == NULL ? GetDefaultTextStyleUI() : *textStylePtr;

    if (screenStyle.cleanScreenBefore)
        CleanUpUI();

    CONSOLE_SCREEN_BUFFER_INFO screenInfo = GetConInfoUI();

    int selectedItem = 0;

    size_t buttonWidth = strlen(menuItems[GetLongestStringInArray(menuItems, itemCount)]);

    size_t startPointV = CalculateVerticalStartPointUI(&screenStyle, screenInfo, itemCount, true);
    size_t startPointH = CalculateHorizontalStartPointUI(&screenStyle, screenInfo, buttonWidth);

    DrawMenuScreenUI(menuItems, itemCount, &textStyle, &screenStyle, screenInfo);
    DrawMenuItemsUI(menuItems, itemCount, buttonWidth, &textStyle, &screenStyle, startPointH, startPointV, selectedItem, screenInfo);

    while (1) {

        //Separate function maybe...?
        if (!IsWindowSizeSameUI(screenInfo)) {
            screenInfo = GetConInfoUI();
            startPointV = CalculateVerticalStartPointUI(&screenStyle, screenInfo, itemCount, true);
            startPointH = CalculateHorizontalStartPointUI(&screenStyle, screenInfo, buttonWidth);
            DrawMenuScreenUI(menuItems, itemCount, &textStyle, &screenStyle, screenInfo);
            DrawMenuItemsUI(menuItems, itemCount, buttonWidth, &textStyle, &screenStyle, startPointH, startPointV, selectedItem, screenInfo);
        }
        SetCaretPosUI(0, 0);
        char input = _getch();
        //
        // ----
        // Separate function later...?
        //
        switch (input)
        {
            //ENTER
        case 13:
            if (screenStyle.cleanScreenAfter)
                CleanUpUI();
            return selectedItem;
            //0 modifier - Key has multiple values
        case 0:
            input = _getch();
            switch (input)
            {
                //F10 key
            case 68:
                if (screenStyle.cleanScreenAfter)
                    CleanUpUI();
                return -0x01;
            default:
                break;
            }
            break;
            //-32 modifier - Key has multiple values
        case -32:
            input = _getch();
            switch (input)
            {
                //DOWN key
            case 80:
                selectedItem = selectedItem >= itemCount - 1 ? 0 : selectedItem + 1;
                DrawMenuItemsUI(menuItems, itemCount, buttonWidth, &textStyle, &screenStyle, startPointH, startPointV, selectedItem, screenInfo);
                break;
                //UP key
            case 72:
                selectedItem = selectedItem <= 0 ? (int)itemCount - 1 : selectedItem - 1;
                DrawMenuItemsUI(menuItems, itemCount, buttonWidth, &textStyle, &screenStyle, startPointH, startPointV, selectedItem, screenInfo);
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

    if (screenStyle.cleanScreenAfter)
        CleanUpUI();
    return -0x01;

}

//-
void DrawMenuItemsUI(char** menuItems, size_t itemCount, size_t longestItemWidth, CUI_TEXTSTYLE* textStylePtr, CUI_SCREENSTYLE* screenStylePtr, size_t startPointH, size_t startPointV, int selectedItem, CONSOLE_SCREEN_BUFFER_INFO screenInfo) {
    if (textStylePtr == NULL || screenStylePtr == NULL)       return;

    //Applying text styles
    SetForegroundColorUI(textStylePtr->textFGColorR, textStylePtr->textFGColorG, textStylePtr->textFGColorB);
    SetBackgroundColorUI(textStylePtr->textBGColorR, textStylePtr->textBGColorG, textStylePtr->textBGColorB);

    startPointH -= 3;   //Offset for the opening anc closing braces (3 chars each)

    //Print menu items to screen
    for (size_t i = 0; i < itemCount; i++) {

        startPointH += screenStylePtr->lineShift;

        size_t textStartPointH = (startPointH + 3) + longestItemWidth / 2 - strlen(menuItems[i]) / 2;

        if (i == selectedItem) {
            SetForegroundColorUI(255 - textStylePtr->textFGColorR, 255 - textStylePtr->textFGColorG, 255 - textStylePtr->textFGColorB);
            SetBackgroundColorUI(255 - textStylePtr->textBGColorR, 255 - textStylePtr->textBGColorG, 255 - textStylePtr->textBGColorB);
        }
        SetCaretPosUI( (int)startPointH,  (int)startPointV);

        //Temp. solutions
#ifdef CUI_MENU_BUTTON_OP
        printf("%c ", CUI_MENU_BUTTON_OP);
#else
        printf("  ");
#endif
        for (size_t space = 0; space < longestItemWidth + 2; space++)
            printf(" ");
#ifdef CUI_MENU_BUTTON_CL
        printf(" %c", CUI_MENU_BUTTON_CL);
#else
        printf("  ");
#endif

        SetCaretPosUI( (int)textStartPointH, (int)startPointV);
        printf("%s", menuItems[i]);

        startPointV += 2;

        //Will be better, I swear
        if (i == selectedItem) {
            SetForegroundColorUI(textStylePtr->textFGColorR, textStylePtr->textFGColorG, textStylePtr->textFGColorB);
            SetBackgroundColorUI(textStylePtr->textBGColorR, textStylePtr->textBGColorG, textStylePtr->textBGColorB);
        }
    }
}

//-
void DrawMenuScreenUI(char** menuItems, size_t itemCount, CUI_TEXTSTYLE* textStyle, CUI_SCREENSTYLE* screenStyle, CONSOLE_SCREEN_BUFFER_INFO screenInfo) {
    if (textStyle == NULL || screenStyle == NULL)       return;

    //Applying background coloring
    DrawBackgroundColorUI(*screenStyle, &screenInfo);

    SetCaretPosUI(0, (int)GetConSizeYUI(&screenInfo) - 3);
    for (size_t i = 0; i < GetConSizeXUI(&screenInfo); i++)
        printf("_");
    SetForegroundColorUI(textStyle->textFGColorR, textStyle->textFGColorG, textStyle->textFGColorB);
    SetBackgroundColorUI(textStyle->textBGColorR, textStyle->textBGColorG, textStyle->textBGColorB);

    //Bottom menu
    for (size_t y = GetConSizeYUI(&screenInfo) - 2; y <= GetConSizeYUI(&screenInfo); y++) {
        SetCaretPosUI(0, (int)y);
        for (size_t x = 0; x < GetConSizeXUI(&screenInfo); x++)
            printf(" ");
    }

    SetCaretPosUI(0, (int)GetConSizeYUI(&screenInfo) - 1);
#ifdef CUI_MENU_EXIT_LABEL
    printf("\t%s [F10]", CUI_MENU_EXIT_LABEL);
#else
    printf("\tExit [F10]");
#endif // CUI_MENU_EXIT_LABEL

    //Label
#ifdef CUI_MENU_LABEL
    CUI_SCREENSTYLE labelStyle = GetDefaultSreenStyleUI();
    labelStyle.horizontalAlignment = CUI_ALIGN_RIGHT;
    labelStyle.leftOffset = -8;
    SetCaretPosUI( (int)CalculateHorizontalStartPointUI(&labelStyle, screenInfo, strlen(CUI_MENU_LABEL)), (int)GetConSizeYUI(&screenInfo) - 1);
    printf("%s", CUI_MENU_LABEL);
#endif

}

//-
void DrawBackgroundColorUI(CUI_SCREENSTYLE style, CONSOLE_SCREEN_BUFFER_INFO* screenInfo) {
    CONSOLE_SCREEN_BUFFER_INFO info = screenInfo == NULL ? GetConInfoUI() : *screenInfo;

    SetCaretPosUI(0, 0);

    //Applying screen styles if there is any
    SetBackgroundColorUI(style.screenBGColorR, style.screenBGColorG, style.screenBGColorB);

    char* voidFilledArray = (char*)malloc((GetConSizeXUI(&info) + 2) * sizeof(char));
    for (size_t x = 0; x <= GetConSizeXUI(&info); x++)
        voidFilledArray[x] = 32;
    voidFilledArray[GetConSizeXUI(&info) + 1] = '\0';

    for (size_t y = 0; y < GetConSizeYUI(&info); y++) {
        SetCaretPosUI(0, (int)y);
        printf("%s", voidFilledArray);
    }

    free(voidFilledArray);
}

//
char* TempTextInput(char* title, char* label) {
    printf("\n%s", title);

    char buffer[150];

    printf("\n\n%s: ", label);
    scanf_s(" %[^\n]", buffer);

    char* inputData = (char*)malloc((strlen(buffer) + 1) * sizeof(char));

    strcpy_s(inputData, ((strlen(buffer) + 1) * sizeof(char)), buffer);

    return inputData;
}

//-
char* TextInputPopUpUI(char* title, char* label, CUI_TEXTSTYLE* textStyle, CUI_SCREENSTYLE* screenStyle, CONSOLE_SCREEN_BUFFER_INFO* bufferInfo) {
    //PopUp window size

    return TempTextInput(title, label);

    //
    //      Not finished function
    //

    CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo = bufferInfo == NULL ? GetConInfoUI() : *bufferInfo;

    size_t titleLength = title == NULL ? 0 : strlen(title);
    size_t labelLength = label == NULL ? 0 : strlen(label);
    size_t windowLength = labelLength + CUI_POPUP_INPUT_WIDTH + 7;  //+7 is for padding inside the window and between label and input field.
    const size_t windowHeigth = 5;      //Windows height will be always 5 chars (at least for now...)

    //Upper left corner of window
    size_t startPointH = (GetConSizeXUI(&screenBufferInfo) - windowLength) / 2;     //Horizontal start point
    size_t startPointV = (GetConSizeYUI(&screenBufferInfo) - windowHeigth) / 2;     //Vertical start point

    //Buffer behind window
    CHAR_INFO* hiddenPart = (CHAR_INFO*)malloc((windowHeigth * windowLength) * sizeof(CHAR_INFO));
    COORD hiddenPartSize = { (short)windowLength, (short)windowHeigth };
    COORD hiddenPartPos = { (short)startPointH, (short)startPointV };
    SMALL_RECT hiddenPartRect = { (short)startPointH, (short)startPointV, (short)(startPointH + windowLength), (short)(startPointV + windowHeigth) };     //Left, Top, Right, Bottom

    //GetData


    //PopUp
    SetCaretPosUI( (int)startPointH, (int)startPointV);
    //...

    free(hiddenPart);
}

//-
void MultilinePrintUI(char* text, char separator, CUI_TEXTSTYLE* textStylePtr, CUI_SCREENSTYLE* screenStylePtr, CONSOLE_SCREEN_BUFFER_INFO* bufferInfoPtr) {
    if (text == NULL)   return;

    //
    CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo = bufferInfoPtr == NULL ? GetConInfoUI() : *bufferInfoPtr;
    CUI_TEXTSTYLE textStyle = textStylePtr == NULL ? GetDefaultTextStyleUI() : *textStylePtr;
    CUI_SCREENSTYLE screenStyle = screenStylePtr == NULL ? GetDefaultSreenStyleUI() : *screenStylePtr;

    screenStyle.verticalAlignment = CUI_ALIGN_TOP;

    size_t width = GetStrLenChar(text, separator);
    size_t heigth = GetStrCharCount(text, separator);

    size_t startPointH = CalculateHorizontalStartPointUI(&screenStyle, screenBufferInfo, width);
    size_t startPointV = CalculateVerticalStartPointUI(&screenStyle, screenBufferInfo, heigth, false);

    SetForegroundColorUI(textStyle.textFGColorR, textStyle.textFGColorG, textStyle.textFGColorB);
    SetBackgroundColorUI(textStyle.textBGColorR, textStyle.textBGColorG, textStyle.textBGColorB);

    SetCaretPosUI((int)startPointH, (int)startPointV++);

    size_t textIndex = 0;
    while (text[textIndex] != '\0')
        if (text[textIndex++] == separator)
            SetCaretPosUI((int)startPointH, (int)startPointV++);
        else
            printf("%c", text[textIndex]);

}

// **************************************************
// |	Scaling										|
// **************************************************

//-
size_t CalculateVerticalStartPointUI(CUI_SCREENSTYLE* screenStyle, CONSOLE_SCREEN_BUFFER_INFO screenInfo, size_t itemCount, bool lineSpacing) {
    if (screenStyle == NULL)    return 0;
    //Vertical alignment calculation
    switch (screenStyle->verticalAlignment)
    {
        //Alignt items to top
    case CUI_ALIGN_TOP:
        return 0 + screenStyle->topOffset;
        //Alignt items to center
    case CUI_ALIGN_CENTER:
        return ((GetConSizeYUI(&screenInfo) - (itemCount * (lineSpacing ? 2 : 1) - (lineSpacing ? 1 : 0))) / 2) + screenStyle->topOffset;
        //Alignt items to bottom
    case CUI_ALIGN_BOTTOM:
        return (GetConSizeYUI(&screenInfo) - (itemCount * (lineSpacing ? 2 : 1) - (lineSpacing ? 1 : 0))) + screenStyle->topOffset; //
    default:
        return 0;
    }
}

//-
size_t CalculateHorizontalStartPointUI(CUI_SCREENSTYLE* screenStyle, CONSOLE_SCREEN_BUFFER_INFO screenInfo, size_t itemWidth) {
    if (screenStyle == NULL)    return 0;
    //Horizontal alignment calculation
    switch (screenStyle->horizontalAlignment)
    {
        //Align items to left
    case CUI_ALIGN_LEFT:
        return 0 + screenStyle->leftOffset;
        //Alignt items to center
    case CUI_ALIGN_CENTER:
        return (GetConSizeXUI(&screenInfo) - itemWidth) / 2 + screenStyle->leftOffset;
        //Alignt items to right
    case CUI_ALIGN_RIGHT:
        return (GetConSizeXUI(&screenInfo) - itemWidth) + screenStyle->leftOffset;
    default:
        return 0;
    }
}

//-
bool IsWindowSizeSameUI(CONSOLE_SCREEN_BUFFER_INFO screenInfo) {
    CONSOLE_SCREEN_BUFFER_INFO newInfo = GetConInfoUI();
    return GetConSizeXUI(&screenInfo) == GetConSizeXUI(&newInfo) && GetConSizeYUI(&screenInfo) == GetConSizeYUI(&newInfo);
}

// **************************************************
// |	Other UI related							|
// **************************************************

//-
CUI_SCREENSTYLE GetDefaultSreenStyleUI(void) {
    CUI_SCREENSTYLE scrStyle = { CUI_SCREENS_DEF_BG_R, CUI_SCREENS_DEF_BG_G, CUI_SCREENS_DEF_BG_B, CUI_SCREEN_DEF_ALIGN_V, CUI_SCREEN_DEF_ALIGN_H, 0, CUI_SCREEN_DEF_CLR_B, CUI_SCREEN_DEF_CLR_B };
    return scrStyle;
}

//-
CUI_TEXTSTYLE GetDefaultTextStyleUI(void) {
    CUI_TEXTSTYLE txtStyle = { CUI_TEXT_DEF_FG_R, CUI_TEXT_DEF_FG_G, CUI_TEXT_DEF_FG_B, CUI_TEXT_DEF_BG_R , CUI_TEXT_DEF_BG_G, CUI_TEXT_DEF_BG_B };
    return txtStyle;
}

// **************************************************
// |	Supplementary functions						|
// **************************************************

//-
size_t GetLongestStringInArray(char** array, size_t length) {
    if (array == NULL || length < 1)      return 0;
    size_t longestIndex = 0;
    for (size_t i = 0; i < length; i++)
        if (strlen(array[i]) > strlen(array[longestIndex]))
            longestIndex = i;
    return longestIndex;
}

//-
size_t GetStrLenChar(char* str, char limit) {
    if (str == NULL)        return 0;
    size_t length = 0;
    while ((*str) != limit && *(str++) != '\0')
        length++;
    return length;
}

//-
size_t GetStrCharCount(char* str, char toCount) {
    uint32_t count = 0;
    while (*(str++) != '\0')
        if ((*str) == toCount)
            count++;
    return count;
}
