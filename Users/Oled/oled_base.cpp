//
// Created by 22482 on 2024/3/29.
//

#include "oled_base.h"
#include "main.h"
#include "spi.h"

void OLED_Base::Init() {

    HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_RESET);
    HAL_Delay(300);
    HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_SET);

    WR_Byte(0xAE, OLED_CMD);//--turn off oled panel
    WR_Byte(0x00, OLED_CMD);//---set low column address
    WR_Byte(0x10, OLED_CMD);//---set high column address
    WR_Byte(0x40, OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    WR_Byte(0x81, OLED_CMD);//--set contrast control register
    WR_Byte(0xFF, OLED_CMD); // Set SEG Output Current Brightness
    WR_Byte(0xA1, OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    WR_Byte(0xC0, OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    WR_Byte(0xA6, OLED_CMD);//--set normal display
    WR_Byte(0xA8, OLED_CMD);//--set multiplex ratio(1 to 64)
    WR_Byte(0x3f, OLED_CMD);//--1/64 duty
    WR_Byte(0xD3, OLED_CMD);//-set display offset Shift Mapping RAM Counter (0x00~0x3F)
    WR_Byte(0x00, OLED_CMD);//-not offset
    WR_Byte(0xd5, OLED_CMD);//--set display clock divide ratio/oscillator frequency
    WR_Byte(0x80, OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
    WR_Byte(0xD9, OLED_CMD);//--set pre-charge period
    WR_Byte(0xF1, OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    WR_Byte(0xDA, OLED_CMD);//--set com pins hardware configuration
    WR_Byte(0x12, OLED_CMD);
    WR_Byte(0xDB, OLED_CMD);//--set vcomh
    WR_Byte(0x40, OLED_CMD);//Set VCOM Deselect Level
    WR_Byte(0x20, OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
    WR_Byte(0x02, OLED_CMD);//
    WR_Byte(0x8D, OLED_CMD);//--set Charge Pump enable/disable
    WR_Byte(0x14, OLED_CMD);//--set(0x10) disable
    WR_Byte(0xA4, OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
    WR_Byte(0xA6, OLED_CMD);// Disable Inverse Display On (0xa6/a7)
    WR_Byte(0xAF, OLED_CMD); /*display ON*/
    HAL_Delay(100);
}

void OLED_Base::Refresh() {
    uint8_t i, n;
    for (i = 0; i < 8; i++) {
        WR_Byte(0xb0 + i, OLED_CMD);    //设置页地址（0~7）
        WR_Byte(0x00, OLED_CMD);      //设置显示位置—列低地址
        WR_Byte(0x10, OLED_CMD);      //设置显示位置—列高地址
        SPI_WriteBytes(&KKUI_Gram_Addr[i][0], 128);
    } //更新显示
//    uiTick += _tick;
}

void OLED_Base::SPI_WriteByte(uint8_t _dat) {
    HAL_SPI_Transmit(&hspi1, &_dat, 1, 1000);
}

void OLED_Base::SPI_WriteBytes(uint8_t *_dat, uint8_t _len) {
    HAL_SPI_Transmit(&hspi1, _dat, _len, 1000);
}

void OLED_Base::WR_Byte(uint8_t _dat, uint8_t _mode) {
    if (_mode)
//        digitalWrite(_dc, HIGH);
        HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET);
    else
//        digitalWrite(_dc, LOW);
        HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET);

    SPI_WriteByte(_dat);

//    digitalWrite(_dc, HIGH);
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET);
}

void OLED_Base::Rotate(OLED_Base::DisplayDegree_e _degree) {
    if (_degree == DEGREE_0) {
        WR_Byte(0xC8, OLED_CMD);
        WR_Byte(0xA0, OLED_CMD);

    }
    if (_degree == DEGREE_180) {
        WR_Byte(0xA1, OLED_CMD);
        WR_Byte(0xC0, OLED_CMD);
    }
}

void OLED_Base::Brightness(uint8_t _i) {
    WR_Byte(0x81, OLED_CMD);
    WR_Byte(_i, OLED_CMD);
}

void OLED_Base::Mirror(bool _enable) {
//    WR_Byte(0x81, OLED_CMD);
    if (_enable) {
        WR_Byte(0xA0, OLED_CMD);
    } else {
        WR_Byte(0xA1, OLED_CMD);
    }
}

void OLED_Base::Enable() {
    WR_Byte(0x8D, OLED_CMD);//电荷泵使能
    WR_Byte(0x14, OLED_CMD);//开启电荷泵
    WR_Byte(0xAF, OLED_CMD);//点亮屏幕
}

void OLED_Base::Disable() {
    WR_Byte(0x8D, OLED_CMD);//电荷泵使能
    WR_Byte(0x10, OLED_CMD);//关闭电荷泵
    WR_Byte(0xAE, OLED_CMD);//关闭屏幕
}

void OLED_Base::Inverse(bool _enable) {
    if (_enable) {
        WR_Byte(0xA7, OLED_CMD);
    } else {
        WR_Byte(0xA6, OLED_CMD);
    }
}


