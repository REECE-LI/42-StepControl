//
// Created by 22482 on 2024/4/12.
//

#ifndef STM32F303_FW_OLED_TASK_H
#define STM32F303_FW_OLED_TASK_H

#include "simpleui_ui.h"

extern OLED_Base oled;
extern SimpleUIUI simpleUIUI;

void OledTaskInit();

void OledGetInfo(void *info);

void OledSelectClick(bool isUp);

void OledSelectLongPress();


#endif //STM32F303_FW_OLED_TASK_H
