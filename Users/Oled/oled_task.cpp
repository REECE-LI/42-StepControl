//
// Created by 22482 on 2024/4/12.
//

#include "oled_task.h"
#include "simpleui_ui_cfg.h"
#include "motor.h"
#include "main.h"
#include "tim.h"

OLED_Base oled;
SimpleUIUI simpleUIUI(&oled);

void OledTaskInit() {
    oled.Init();
    oled.DisClear(SimpleUIBase::WHITE_POINT);
    simpleUIUI.DrawLabel(&XDrive_Interface_Label);
    oled.Refresh();
}

void OledGetInfo(void *info) {
    // do something...
}

void OledSelectClick(bool isUp) {
    // do something...
}

void OledSelectLongPress() {
    // do something...
}