//
// Created by 22482 on 2024/3/31.
//

#ifndef SIMPLEUI_UI_H
#define SIMPLEUI_UI_H

#include <string>
#include "oled_base.h"

#define ANIMATE_SPEED 5

class SimpleUIUI {
public:
    typedef struct {
        SimpleUIBase::SimpleWindow_t window;
        const std::string label;
    } Label_t;

    typedef struct {
        Label_t label;
        SimpleUIBase::SimpleWindow_t selectWindow;
        uint8_t selectNum;
        const std::string *selectList;
        uint8_t currentIndex;
        enum {
            expand = 0,
            merge,
        } selectState;
        uint8_t selectIndex;
    } SelectBox_t;

    typedef struct {
        SelectBox_t **selectBoxesLabel;
        uint8_t selectNum;
        uint8_t currentIndex;
        uint8_t selectIndex;
    } ChaoticSelectBox_t;

    typedef enum {
        Main_ing = 0,
        Main_ed,
        SelectBox_ing,
        SelectBox_ed,
    } UIState_e;

    explicit SimpleUIUI(OLED_Base *_oled) : oled(_oled) {}

    void DrawLabel(const Label_t *_label);

    void DrawSelectBox(const SelectBox_t *_selectBox);

    void RunMain(ChaoticSelectBox_t *_chaoticSelectBox, uint8_t _index);

    void RunSelectBox(SelectBox_t *_selectBox, uint8_t _index);

    void Refresh();

    UIState_e uiState = Main_ed;

    SimpleUIUI::SelectBox_t *selectBox;

    bool keyPress = true;

    bool isAnimateFinish = false;
private:
    OLED_Base *oled;

    UIState_e lastUiState = Main_ed;

//    uint8_t index = 0;


    SimpleUIBase::SimpleWindow_t tempWindow = {0};

    bool isFirstRunBackGround = true;

    bool isFirstRunCursor = true;

    bool isFirstRunSelectedLabel = true;

    SimpleUIBase::SimpleWindow_t selectCursor, targetCursor;

    SimpleUIBase::SimpleWindow_t selectBoxBackGround, targetBoxBackGround;

    SimpleUIBase::SimpleWindow_t selectedLabel, targetSelectedLabel;

    void RefreshMain();

    void RefreshCursorBox();

    void RefreshSelectBackGround(SimpleUIUI::SelectBox_t *_box);

    static uint8_t SelectBoxXAdaptive(uint8_t _boxWidth, uint8_t _stringWidth);

    static uint8_t SelectBoxYAdaptive(uint8_t _index);

    SimpleUIBase::SimpleWindow_t SetCursor(void *_setCursor, uint8_t _index);

    SimpleUIBase::SimpleWindow_t SetSelectBox(void *_selectBox);

    void AnimateCursor(SimpleUIBase::SimpleWindow_t *_target, SimpleUIBase::SimpleWindow_t *_value);

    void AnimateBackGround(SimpleUIBase::SimpleWindow_t *_target, SimpleUIBase::SimpleWindow_t *_value);

    void AnimateSelectedLabel(SimpleUIBase::SimpleWindow_t *_target, SimpleUIBase::SimpleWindow_t *_value);

};

extern SimpleUIUI::ChaoticSelectBox_t MainChaoticSelectBox;

#endif //SIMPLEUI_UI_H
