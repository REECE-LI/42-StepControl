//
// Created by 22482 on 2024/3/31.
//

#ifndef SIMPLEUI_BASE_CFG_H
#define SIMPLEUI_BASE_CFG_H

#define    Enable_Animate  1

#define     DIS_HIGH      64    // 屏幕尺寸
#define     DIS_WIDE      128

#define     KKUI_Gram_High (DIS_HIGH/8)
#define     KKUI_Gram_Wide DIS_WIDE

#define    KKUI_Gram_Size (KKUI_Gram_High * KKUI_Gram_Wide)
#define    KKUI_Gram_Num  3
#define    KKUI_Grams_Size (KKUI_Gram_Size * KKUI_Gram_Num)

#define OLED_CMD  0    //写命令
#define OLED_DATA 1    //写数据

// 是否开启中文字体
#define KKUI_CH_FONT_EN         0

#define     MENU_MENU_FONT_HIGH         12  // 菜单字体高度
#define     MENU_MENU_FONT_WIDTH        6   // 菜单字体宽度
#define     MENU_MENU_CURSOR_SIZE       16  // 菜单光标高度
#define     MENU_MENU_CURSOR_SHIFT      2   // (MENU_MENU_CURSOR_SIZE - MENU_MENU_FONT_HIGH) / 2
#define     MENU_MENU_MARGIN    2   // 前后空余

#define     MENU_MENU_ICON_SIZE         32  // 菜单图标宽度高度

#define RESET_ALL   0
#define SET_ALL     1

#define KKUI_DRAW_UPPER_RIGHT   0x01
#define KKUI_DRAW_UPPER_LEFT    0x02
#define KKUI_DRAW_LOWER_LEFT    0x04
#define KKUI_DRAW_LOWER_RIGHT   0x08
#define KKUI_DRAW_ALL (KKUI_DRAW_UPPER_RIGHT|KKUI_DRAW_UPPER_LEFT|KKUI_DRAW_LOWER_LEFT|KKUI_DRAW_LOWER_RIGHT)

#define constrain(amt, low, high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))


#endif //SIMPLEUI_BASE_CFG_H
