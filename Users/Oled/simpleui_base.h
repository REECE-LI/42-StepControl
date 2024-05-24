//
// Created by 22482 on 2023/12/11.
//

#ifndef SIMPLEUI_BASE_H
#define SIMPLEUI_BASE_H

#include <stdint-gcc.h>
#include "simpleui_base_cfg.h"

class SimpleUIBase {
public:

    typedef struct {
        int16_t startX;
        int16_t startY;
        int16_t endX;
        int16_t endY;
    } SimpleWindow_t;

    typedef enum {
        BLACK_POINT = 0,
        WHITE_POINT,
        OR_POINT,
    } PointColor_e;

    typedef enum {
//        FONT_SIZE_8 = 8,   to small to show
        FONT_SIZE_12 = 12,
        FONT_SIZE_16 = 16,
        FONT_SIZE_24 = 24,
    } FontSize_e;

    virtual void Init() = 0;

    virtual void Refresh() = 0;

//    uint8_t (*KKUI_Gram_Addr)[DIS_WIDE];

    void DisClear(PointColor_e _color);

    void ClearDisWindows();

    void SetDisWindows(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h);

    void DrawPoint(uint16_t _x, uint16_t _y, PointColor_e _color);

    void
    DisMask4Point(uint16_t _x, uint16_t _y, uint16_t _sizex, uint16_t _sizey, uint8_t _value, PointColor_e _color);

    void DrawHLine(uint16_t _x, uint16_t _y, uint16_t _len, PointColor_e _color);

    void DrawVLine(uint16_t _x, uint16_t _y, uint16_t _len, PointColor_e _color);

    void DrawLine(uint16_t _x1, uint16_t _y1, uint16_t _x2, uint16_t _y2, PointColor_e _color);

    void DrawDisc(uint16_t _x, uint16_t _y, uint16_t _r, uint16_t _option, PointColor_e _color);

    void DrawCircle(uint16_t _x, uint16_t _y, uint16_t _r, PointColor_e _color);

    void DrawRect(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h, PointColor_e _color);

    void DrawRRect(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h, uint16_t _r, PointColor_e _color);

    void DrawBox(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h, PointColor_e _color);

    void DrawRBox(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h, uint16_t _r, PointColor_e _color);

    void ShowChar(uint16_t _x, uint16_t _y, char _chr, FontSize_e _fontSize, PointColor_e _color);

    void ShowString(uint16_t _x, uint16_t _y, char *_str, FontSize_e _fontSize, PointColor_e _color);

    void SaveFrameBuffer(uint8_t _inxdex);

    void LoadFrameBuffer(uint8_t _inxdex);

    void ANDFrameBuffer(uint8_t _inxdex);

    void ORFrameBuffer(uint8_t _inxdex);

    void XORFrameBuffer(uint8_t _inxdex);

#if KKUI_CH_FONT_EN

    void ShowChinese(uint16_t _x, uint16_t _y, uint8_t *ch, uint8_t _size, PointColor_e _color);

#endif

    void ShowPicture(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h, uint8_t BMP[], PointColor_e _color);

    uint64_t GetTick();


protected:
    uint8_t KKUI_Gram_Addr[KKUI_Gram_High][KKUI_Gram_Wide];

    uint64_t uiTick = 0;

private:
    virtual void SPI_WriteByte(uint8_t _dat) = 0;

    virtual void SPI_WriteBytes(uint8_t *_dat, uint8_t _len) = 0;

    virtual void WR_Byte(uint8_t _dat, uint8_t _mode) = 0;

    int8_t baseFrameBuffer[KKUI_Gram_Num][KKUI_Gram_Size];// = new int8_t[KKUI_Grams_Size];



    SimpleWindow_t simpleWindow = {
            .startX = 0,
            .startY = 0,
            .endX = DIS_WIDE,
            .endY = DIS_HIGH
    };
};


#endif //SIMPLEUI_BASE_H
