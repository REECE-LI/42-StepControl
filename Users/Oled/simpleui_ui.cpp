//
// Created by 22482 on 2024/3/31.
//

#include "simpleui_ui.h"

#include "cmath"
#include "tim.h"

void SimpleUIUI::DrawLabel(const SimpleUIUI::Label_t *_label) {
    oled->ShowString(_label->window.startX, _label->window.startY, (char *) _label->label.c_str(),
                     SimpleUIBase::FONT_SIZE_16,
                     SimpleUIBase::BLACK_POINT);
}

void SimpleUIUI::DrawSelectBox(const SimpleUIUI::SelectBox_t *_selectBox) {
    uint8_t wide = _selectBox->selectWindow.endX - _selectBox->selectWindow.startX;

    oled->ShowString(_selectBox->label.window.startX, _selectBox->label.window.startY,
                     (char *) _selectBox->label.label.c_str(), SimpleUIBase::FONT_SIZE_12,
                     SimpleUIBase::WHITE_POINT);
    for (uint8_t i = 0; i < _selectBox->selectNum; i++) {
        oled->ShowString(_selectBox->selectWindow.startX + SelectBoxXAdaptive(wide, _selectBox->selectList[i].length()),
                         _selectBox->selectWindow.startY + SelectBoxYAdaptive(i),
                         (char *) _selectBox->selectList[i].c_str(), SimpleUIBase::FONT_SIZE_12,
                         SimpleUIBase::WHITE_POINT);
    }
}

uint8_t SimpleUIUI::SelectBoxXAdaptive(uint8_t _boxWidth, uint8_t _stringWidth) {
    return (uint8_t) ((_boxWidth - _stringWidth * 6) * 0.5);
}

uint8_t SimpleUIUI::SelectBoxYAdaptive(uint8_t _index) {
    if (_index)
        return 12 * _index + 2;
    else
        return 2;
}

void SimpleUIUI::RunMain(ChaoticSelectBox_t *_chaoticSelectBox, uint8_t _index) {
    static bool isFirst = true;

    uiState = Main_ing;

    targetCursor = SetCursor(_chaoticSelectBox, _index);

    targetBoxBackGround = targetCursor;
    if (isFirst) {
        selectBoxBackGround = targetBoxBackGround;
        selectedLabel.startX = selectBox->selectWindow.startX +
                               SelectBoxXAdaptive(selectBox->selectWindow.endX - selectBox->selectWindow.startX,
                                                  selectBox->selectList[selectBox->currentIndex].length());
        selectedLabel.startY = selectBox->selectWindow.startY + SelectBoxYAdaptive(selectBox->currentIndex);
        isFirst = false;
    }

    oled->LoadFrameBuffer(0);

    oled->SaveFrameBuffer(1);

    _chaoticSelectBox->currentIndex = _index;

    // selectBox 是chaoticSelectBox中label元素的第_index个
    selectBox = _chaoticSelectBox->selectBoxesLabel[_index];

    if (lastUiState == SelectBox_ed) {
        selectedLabel.startX = selectBox->selectWindow.startX +
                               SelectBoxXAdaptive(selectBox->selectWindow.endX - selectBox->selectWindow.startX,
                                                  selectBox->selectList[selectBox->currentIndex].length());
        selectedLabel.startY = selectBox->selectWindow.startY + SelectBoxYAdaptive(selectBox->currentIndex);
    } else if (lastUiState == Main_ed) {
        selectedLabel.startX = selectBox->selectWindow.startX +
                               SelectBoxXAdaptive(selectBox->selectWindow.endX - selectBox->selectWindow.startX,
                                                  selectBox->selectList[selectBox->currentIndex].length());
        selectedLabel.startY = selectBox->selectWindow.startY + SelectBoxYAdaptive(0);
    }


    targetSelectedLabel.startX = selectedLabel.startX;
    targetSelectedLabel.startY = selectBox->selectWindow.startY + SelectBoxYAdaptive(0);

    uiState = Main_ed;
}

void SimpleUIUI::RunSelectBox(SimpleUIUI::SelectBox_t *_selectBox, uint8_t _index) {
    static bool isFirst = true;
    uiState = SelectBox_ing;

    if (isFirst) {
        selectCursor = SetCursor(_selectBox, _selectBox->currentIndex);
        isFirst = false;
    }

    oled->LoadFrameBuffer(0);

    targetBoxBackGround = SetSelectBox(_selectBox);
    if (lastUiState == Main_ed) {
        selectBoxBackGround = targetCursor;
    }

    // 设置光标目标位置
    targetCursor = SetCursor(_selectBox, _index);

    oled->SaveFrameBuffer(1);
    _selectBox->currentIndex = _index;

    selectedLabel.startX = _selectBox->selectWindow.startX +
                           SelectBoxXAdaptive(_selectBox->selectWindow.endX - _selectBox->selectWindow.startX,
                                              _selectBox->selectList[_index].length());
    selectedLabel.startY = _selectBox->selectWindow.startY + SelectBoxYAdaptive(_index);
    targetSelectedLabel = selectedLabel;
    uiState = SelectBox_ed;
}

void SimpleUIUI::RefreshCursorBox() {
    static bool isFirst = true;
    uint16_t wide = selectCursor.endX - selectCursor.startX;
    uint16_t height = selectCursor.endY - selectCursor.startY;
    if (wide <= 5 || height <= 5)
        return;
#if Enable_Animate
    AnimateCursor(&targetCursor, &selectCursor);
#else
    selectCursor = targetCursor;
#endif

    if (uiState == Main_ed)
        return;
    oled->DrawRRect(selectCursor.startX, selectCursor.startY, selectCursor.endX - selectCursor.startX,
                    selectCursor.endY - selectCursor.startY,
                    2,
                    SimpleUIBase::OR_POINT);
}

void
SimpleUIUI::AnimateCursor(SimpleUIBase::SimpleWindow_t *_target, SimpleUIBase::SimpleWindow_t *_value) {
    static float valueStartX, valueStartY, valueEndX, valueEndY;
    static SimpleUIBase::SimpleWindow_t *lastTarget;
    if (isFirstRunCursor) {
        lastTarget = _target;
        valueStartX = _value->startX;
        valueStartY = _value->startY;
        valueEndX = _value->endX;
        valueEndY = _value->endY;
        isFirstRunCursor = false;
    }
    int16_t xStartGap = _target->startX - _value->startX;
    int16_t xEndGap = _target->endX - _value->endX;
    int16_t yStartGap = _target->startY - _value->startY;
    int16_t yEndGap = _target->endY - _value->endY;


    if (lastTarget->startX != _target->startX || lastTarget->startY != _target->startY ||
        lastTarget->endX != _target->endX || lastTarget->endY != _target->endY) {
        valueStartX = _value->startX;
        valueStartY = _value->startY;
        valueEndX = _value->endX;
        valueEndY = _value->endY;
    }

    valueStartX += (float) xStartGap / ANIMATE_SPEED;
    _value->startX = xStartGap == 0 ? _target->startX : (int16_t) valueStartX;
    valueEndX += (float) xEndGap / ANIMATE_SPEED;
    _value->endX = xEndGap == 0 ? _target->endX : (int16_t) valueEndX;

    valueStartY += (float) yStartGap / ANIMATE_SPEED;
    _value->startY = yStartGap == 0 ? _target->startY : (int16_t) valueStartY;
    valueEndY += (float) yEndGap / ANIMATE_SPEED;
    _value->endY = yEndGap == 0 ? _target->endY : (int16_t) valueEndY;
    lastTarget = _target;
}

void
SimpleUIUI::AnimateBackGround(SimpleUIBase::SimpleWindow_t *_target, SimpleUIBase::SimpleWindow_t *_value) {
    static float valueStartX, valueStartY, valueEndX, valueEndY;
    static SimpleUIBase::SimpleWindow_t *lastTarget;
    if (isFirstRunBackGround) {
        lastTarget = _target;
        valueStartX = _value->startX;
        valueStartY = _value->startY;
        valueEndX = _value->endX;
        valueEndY = _value->endY;
        isFirstRunBackGround = false;
    }
    int16_t xStartGap = _target->startX - _value->startX;
    int16_t xEndGap = _target->endX - _value->endX;
    int16_t yStartGap = _target->startY - _value->startY;
    int16_t yEndGap = _target->endY - _value->endY;

    if (lastTarget->startX != _target->startX || lastTarget->startY != _target->startY ||
        lastTarget->endX != _target->endX || lastTarget->endY != _target->endY) {
        valueStartX = _value->startX;
        valueStartY = _value->startY;
        valueEndX = _value->endX;
        valueEndY = _value->endY;
    }

    valueStartX += (float) xStartGap / ANIMATE_SPEED;
    _value->startX = xStartGap == 0 ? _target->startX : (int16_t) valueStartX;
    valueEndX += (float) xEndGap / ANIMATE_SPEED;
    _value->endX = xEndGap == 0 ? _target->endX : (int16_t) valueEndX;

    valueStartY += (float) yStartGap / ANIMATE_SPEED;
    _value->startY = yStartGap == 0 ? _target->startY : (int16_t) valueStartY;
    valueEndY += (float) yEndGap / ANIMATE_SPEED;
    _value->endY = yEndGap == 0 ? _target->endY : (int16_t) valueEndY;
    lastTarget = _target;
}

#include "motor.h"

extern Motor motor;

void SimpleUIUI::Refresh() {
    static float Speed, Position;
    static char speedStr[10], positionStr[10];

    if (uiState == Main_ing || uiState == SelectBox_ing)
        return;

    if (lastUiState != uiState) {
        isFirstRunCursor = true;
        isFirstRunBackGround = true;
    }


    if (!isAnimateFinish) {
        oled->LoadFrameBuffer(1);
        switch (uiState) {
            case Main_ed:
            case SelectBox_ed:
                RefreshSelectBackGround(selectBox);
                RefreshCursorBox();
                break;
            default:
                return;
        }
    } else {
        oled->LoadFrameBuffer(2);
    }

    if (selectBoxBackGround.startX == targetBoxBackGround.startX &&
        selectBoxBackGround.startY == targetBoxBackGround.startY &&
        selectBoxBackGround.endX == targetBoxBackGround.endX && selectBoxBackGround.endY == targetBoxBackGround.endY &&
        uiState == Main_ed && !isAnimateFinish) {
        isAnimateFinish = true;
        oled->SaveFrameBuffer(2);
        oled->LoadFrameBuffer(2);
//        if (HAL_TIM_STATE_READY == HAL_TIM_Base_GetState(&htim3))
        HAL_TIM_Base_Start_IT(&htim3);
    } else if (!(selectBoxBackGround.startX == targetBoxBackGround.startX &&
                 selectBoxBackGround.startY == targetBoxBackGround.startY &&
                 selectBoxBackGround.endX == targetBoxBackGround.endX &&
                 selectBoxBackGround.endY == targetBoxBackGround.endY) || uiState != Main_ed) {
        isAnimateFinish = false;
    }
    if (uiState == Main_ed) {
        Speed = motor.controller->GetVelocity();
        Position = motor.controller->GetPosition();

        sprintf(speedStr, "%+.2f", Speed);
        sprintf(positionStr, "%+.2f", Position);
        oled->ShowString(50, 42, (char *) speedStr, SimpleUIBase::FONT_SIZE_12,
                         SimpleUIBase::BLACK_POINT);
        oled->ShowString(70, 53, (char *) positionStr, SimpleUIBase::FONT_SIZE_12,
                         SimpleUIBase::BLACK_POINT);
    }


    lastUiState = uiState;


    oled->Refresh();


}

SimpleUIBase::SimpleWindow_t SimpleUIUI::SetCursor(void *_setCursor, uint8_t _index) {
    SimpleUIBase::SimpleWindow_t cursor;
    if (uiState == Main_ing) {
        auto *chaoticSelectBox = (ChaoticSelectBox_t *) _setCursor;
        cursor = {
                .startX = (int16_t) (chaoticSelectBox->selectBoxesLabel[_index]->label.window.startX - 2),
                .startY = chaoticSelectBox->selectBoxesLabel[_index]->label.window.startY,
                .endX = (int16_t) (chaoticSelectBox->selectBoxesLabel[_index]->label.window.startX +
                                   chaoticSelectBox->selectBoxesLabel[_index]->label.label.length() * 6),
                .endY = (int16_t) (chaoticSelectBox->selectBoxesLabel[_index]->label.window.startY + 10)
        };
    } else if (uiState == SelectBox_ing) {
        auto *selectBox = (SelectBox_t *) _setCursor;
        cursor = {
                .startX = (int16_t) (selectBox->selectWindow.startX +
                                     SelectBoxXAdaptive(selectBox->selectWindow.endX - selectBox->selectWindow.startX,
                                                        selectBox->selectList[_index].length()) - 1),
                .startY = (int16_t) (selectBox->selectWindow.startY + SelectBoxYAdaptive(_index)),
                .endX = (int16_t) (selectBox->selectWindow.startX +
                                   SelectBoxXAdaptive(selectBox->selectWindow.endX - selectBox->selectWindow.startX,
                                                      selectBox->selectList[_index].length()) - 1 +
                                   selectBox->selectList[_index].length() * 6 + 2),
                .endY = (int16_t) (selectBox->selectWindow.startY + SelectBoxYAdaptive(_index) + 10)
        };
    }
    return cursor;
}

SimpleUIBase::SimpleWindow_t SimpleUIUI::SetSelectBox(void *_selectBox) {
    SimpleUIBase::SimpleWindow_t selectWindow;
    auto *box = (SelectBox_t *) _selectBox;

    selectWindow = {
            .startX = static_cast<int16_t>(box->label.window.startX - 2),
            .startY = box->selectWindow.startY,
            .endX = static_cast<int16_t>(box->selectWindow.endX),
            .endY = static_cast<int16_t>(box->selectWindow.endY)
    };
    return selectWindow;
}

void SimpleUIUI::RefreshSelectBackGround(SimpleUIUI::SelectBox_t *_box) {
    // 画出选择背景框
    static uint16_t wide;
    static uint16_t height;

    wide = selectBoxBackGround.endX - selectBoxBackGround.startX;
    height = selectBoxBackGround.endY - selectBoxBackGround.startY;
    if (wide <= 5 || height <= 5)
        return;

#if Enable_Animate
    AnimateBackGround(&targetBoxBackGround, &selectBoxBackGround);
    AnimateSelectedLabel(&targetSelectedLabel, &selectedLabel);
#else
    selectBoxBackGround = targetBoxBackGround;
    selectedLabel = targetSelectedLabel;
#endif
    // Draw Mask


#if 1
    if (uiState == SelectBox_ed) {
        oled->DisMask4Point(0, 0, DIS_WIDE, DIS_HIGH, 1, SimpleUIBase::BLACK_POINT);
        for (uint8_t i = 0; i < MainChaoticSelectBox.selectNum; i++) {
            if (i == MainChaoticSelectBox.currentIndex)
                continue;
            oled->ShowString(MainChaoticSelectBox.selectBoxesLabel[i]->selectWindow.startX +
                             SelectBoxXAdaptive(MainChaoticSelectBox.selectBoxesLabel[i]->selectWindow.endX -
                                                MainChaoticSelectBox.selectBoxesLabel[i]->selectWindow.startX,
                                                MainChaoticSelectBox.selectBoxesLabel[i]->selectList[
                                                        MainChaoticSelectBox.selectBoxesLabel[i]->currentIndex].length()),
                             MainChaoticSelectBox.selectBoxesLabel[i]->selectWindow.startY +
                             SelectBoxYAdaptive(0),
                             (char *) MainChaoticSelectBox.selectBoxesLabel[i]->selectList[
                                     MainChaoticSelectBox.selectBoxesLabel[i]->currentIndex].c_str(),
                             SimpleUIBase::FONT_SIZE_12,
                             SimpleUIBase::OR_POINT);
        }
        oled->DrawRRect(selectBoxBackGround.startX, selectBoxBackGround.startY,
                        selectBoxBackGround.endX - selectBoxBackGround.startX,
                        selectBoxBackGround.endY - selectBoxBackGround.startY,
                        2,
                        SimpleUIBase::BLACK_POINT);
        // 白色的字
        oled->ShowString(_box->label.window.startX, _box->label.window.startY,
                         (char *) _box->label.label.c_str(), SimpleUIBase::FONT_SIZE_12,
                         SimpleUIBase::WHITE_POINT);


        for (uint8_t i = 0; i < _box->selectNum; i++) {
            oled->ShowString(_box->selectWindow.startX +
                             SelectBoxXAdaptive(_box->selectWindow.endX - _box->selectWindow.startX,
                                                _box->selectList[i].length()),
                             _box->selectWindow.startY + SelectBoxYAdaptive(i),
                             (char *) _box->selectList[i].c_str(), SimpleUIBase::FONT_SIZE_12,
                             SimpleUIBase::OR_POINT);
        }
    } else if (uiState == Main_ed) {
        for (uint8_t i = 0; i < MainChaoticSelectBox.selectNum; i++) {
            if (i == MainChaoticSelectBox.currentIndex)
                continue;
            oled->ShowString(MainChaoticSelectBox.selectBoxesLabel[i]->selectWindow.startX +
                             SelectBoxXAdaptive(MainChaoticSelectBox.selectBoxesLabel[i]->selectWindow.endX -
                                                MainChaoticSelectBox.selectBoxesLabel[i]->selectWindow.startX,
                                                MainChaoticSelectBox.selectBoxesLabel[i]->selectList[
                                                        MainChaoticSelectBox.selectBoxesLabel[i]->currentIndex].length()),
                             MainChaoticSelectBox.selectBoxesLabel[i]->selectWindow.startY +
                             SelectBoxYAdaptive(0),
                             (char *) MainChaoticSelectBox.selectBoxesLabel[i]->selectList[
                                     MainChaoticSelectBox.selectBoxesLabel[i]->currentIndex].c_str(),
                             SimpleUIBase::FONT_SIZE_12,
                             SimpleUIBase::OR_POINT);
        }
        oled->DrawRRect(selectBoxBackGround.startX, selectBoxBackGround.startY,
                        selectBoxBackGround.endX - selectBoxBackGround.startX,
                        selectBoxBackGround.endY - selectBoxBackGround.startY,
                        2,
                        SimpleUIBase::OR_POINT);

        oled->ShowString(selectedLabel.startX, selectedLabel.startY,

                         (char *) _box->selectList[_box->currentIndex].c_str(), SimpleUIBase::FONT_SIZE_12,
                         SimpleUIBase::OR_POINT);


    }

#endif
}

// 这个函数只用到了 Window_t 结构体的 startX 和 startY 成员变量，
void SimpleUIUI::AnimateSelectedLabel(SimpleUIBase::SimpleWindow_t *_target,
                                      SimpleUIBase::SimpleWindow_t *_value) {
    static float valueStartX, valueStartY;
    static SimpleUIBase::SimpleWindow_t *lastTarget;
    if (isFirstRunSelectedLabel) {
        lastTarget = _target;
        valueStartX = _value->startX;
        valueStartY = _value->startY;
        isFirstRunBackGround = false;
    }
    int16_t xStartGap = _target->startX - _value->startX;
    int16_t yStartGap = _target->startY - _value->startY;

    if (lastTarget->startX != _target->startX || lastTarget->startY != _target->startY) {
        valueStartX = _value->startX;
        valueStartY = _value->startY;
    }
    valueStartX += (float) xStartGap / ANIMATE_SPEED;
    _value->startX = xStartGap == 0 ? _target->startX : (int16_t) valueStartX;

    valueStartY += (float) yStartGap / ANIMATE_SPEED;
    _value->startY = yStartGap == 0 ? _target->startY : (int16_t) valueStartY;

    lastTarget = _target;
}
