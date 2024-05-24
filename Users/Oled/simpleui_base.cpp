//
// Created by 22482 on 2023/12/11.
//

#include "simpleui_base.h"
#include "kkgui_font.h"
#include <cmath>
#include "cstring"


void SimpleUIBase::DisClear(PointColor_e _color) {
    for (uint16_t i = 0; i < KKUI_Gram_High; i++) {
        for (uint16_t j = 0; j < KKUI_Gram_Wide; j++) {
            KKUI_Gram_Addr[i][j] = _color == BLACK_POINT ? 0x00 : 0xff;
        }
    }
}

void SimpleUIBase::ClearDisWindows() {
    simpleWindow.startX = 0;
    simpleWindow.startY = 0;
    simpleWindow.endX = DIS_WIDE;
    simpleWindow.endY = DIS_HIGH;
}

void SimpleUIBase::SetDisWindows(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h) {
    simpleWindow.startX = _x < 0 ? 0 : _x;
    simpleWindow.startY = _y < 0 ? 0 : _y;
    simpleWindow.endX = constrain(_x + _w, 0, DIS_WIDE);
    simpleWindow.endY = constrain(_y + _h, 0, DIS_HIGH);
}

void SimpleUIBase::DrawPoint(uint16_t _x, uint16_t _y, PointColor_e _color) {
    if (_x < simpleWindow.startX || _x >= simpleWindow.endX || _y < simpleWindow.startY || _y >= simpleWindow.endY) {
        return;
    }
    switch (_color) {
        case BLACK_POINT:
            KKUI_Gram_Addr[_y / 8][_x] &= ~(1 << (_y % 8));
            break;
        case WHITE_POINT:
            KKUI_Gram_Addr[_y / 8][_x] |= 1 << (_y % 8);
            break;
        case OR_POINT:
            KKUI_Gram_Addr[_y / 8][_x] ^= 1 << (_y % 8);
            break;
        default:
            break;
    }
}

void SimpleUIBase::DisMask4Point(uint16_t _x, uint16_t _y, uint16_t _sizex, uint16_t _sizey, uint8_t _value,
                                 SimpleUIBase::PointColor_e _color) {
    if (_x < simpleWindow.startX || _x >= simpleWindow.endX || _y < simpleWindow.startY || _y >= simpleWindow.endY) {
        return;
    }
    for (uint16_t a = _x; a < _x + _sizex; a++) {
        for (uint16_t b = _y; b < _y + _sizey; b++) {
            if (_value & 0x01 && !(b % 2) && !(a % 2))
                DrawPoint(a, b, _color);
            if (_value & 0x02 && (b % 2) && !(a % 2))
                DrawPoint(a, b, _color);
            if (_value & 0x04 && !(b % 2) && (a % 2))
                DrawPoint(a, b, _color);
            if (_value & 0x08 && (b % 2) && (a % 2))
                DrawPoint(a, b, _color);
        }
    }
}

void SimpleUIBase::DrawHLine(uint16_t _x, uint16_t _y, uint16_t _len, SimpleUIBase::PointColor_e _color) {
    if (_x < simpleWindow.startX || _x >= simpleWindow.endX || _y < simpleWindow.startY || _y >= simpleWindow.endY) {
        return;
    }
    for (uint16_t i = 0; i < _len; i++) {
        DrawPoint(_x + i, _y, _color);
    }
}

void SimpleUIBase::DrawVLine(uint16_t _x, uint16_t _y, uint16_t _len, SimpleUIBase::PointColor_e _color) {
    if (_x < simpleWindow.startX || _x >= simpleWindow.endX || _y < simpleWindow.startY || _y >= simpleWindow.endY) {
        return;
    }
    for (uint16_t i = 0; i < _len; i++) {
        DrawPoint(_x, _y + i, _color);
    }
}

void SimpleUIBase::DrawLine(uint16_t _x1, uint16_t _y1, uint16_t _x2, uint16_t _y2, SimpleUIBase::PointColor_e _color) {
    uint16_t xErr = 0, yErr = 0, deltaX, deltaY, Distance;
    uint16_t incX, incY, uRow, uCol;
    deltaX = _x2 - _x1;
    deltaY = _y2 - _y1;
    uRow = _x1;
    uCol = _y1;
    if (deltaX > 0)incX = 1;
    else if (deltaX == 0)incX = 0;
    else {
        incX = -1;
        deltaX = -deltaX;
    }
    if (deltaY > 0)incY = 1;
    else if (deltaY == 0)incY = 0;
    else {
        incY = -1;
        deltaY = -deltaY;
    }
    if (deltaX > deltaY)Distance = deltaX;
    else Distance = deltaY;
    for (uint16_t i = 0; i < Distance + 1; i++) {
        DrawPoint(uRow, uCol, _color);
        xErr += deltaX;
        yErr += deltaY;
        if (xErr > Distance) {
            xErr -= Distance;
            uRow += incX;
        }
        if (yErr > Distance) {
            yErr -= Distance;
            uCol += incY;
        }
    }
}

void
SimpleUIBase::DrawDisc(uint16_t _x, uint16_t _y, uint16_t _r, uint16_t _option, SimpleUIBase::PointColor_e _color) {
    uint16_t a, b;
    a = 1;
    b = _r;

    // todo: 懒得抄这个
}

void SimpleUIBase::DrawCircle(uint16_t _x, uint16_t _y, uint16_t _r, SimpleUIBase::PointColor_e _color) {
    DrawDisc(_x, _y, _r, KKUI_DRAW_ALL, _color);
}

// rect 是有填充的
// box 是没有填充的
void SimpleUIBase::DrawRect(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h, SimpleUIBase::PointColor_e _color) {
    for (uint16_t i = _x; i < _x + _w; i++)
        for (uint16_t j = _y; j < _y + _h; j++)
            DrawPoint(i, j, _color);
}

void
SimpleUIBase::DrawRRect(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h, uint16_t _r,
                        SimpleUIBase::PointColor_e _color) {
    uint16_t a, b;
    a = 1;
    b = _r;
    while (b > a) {
        b = (uint16_t) (sqrt(_r * _r - a * a) + 0.43f);
        if (b - a <= 1 && b >= (uint16_t) (sqrt(_r * _r - (a - 1) * (a - 1)) + 0.43f))
            break;
        if (b - a < 1) break;
        DrawHLine(_x + _w - _r + a, _y + _r - b, b - a, _color);
        DrawHLine(_x + _w - _r + a + 1, _y + _r - 1 - a, b - a - 1, _color);

        DrawHLine(_x + _r - 1 - a, _y + _r - b, b - a, _color);
        DrawHLine(_x + _r - b, _y + _r - 1 - a, b - a - 1, _color);

        DrawHLine(_x + _w - _r + a, _y + _h - _r + a, b - a, _color);
        DrawHLine(_x + _w - _r + a + 1, _y + _h - _r + a, b - a - 1, _color);

        DrawHLine(_x + _r - 1 - a, _y + _h - _r + a, b - a, _color);
        DrawHLine(_x + _r - b, _y + _h - _r + a, b - a - 1, _color);
        a++;
    }
    DrawRect(_x + _r - 1, _y, _w - 2 * _r + 2, _r - 1, _color);
    DrawRect(_x + _r - 1, _y + _h - _r + 1, _w - 2 * _r + 2, _r - 1, _color);
    DrawRect(_x, _y + _r - 1, _w, _h - 2 * _r + 2, _color);
}

void SimpleUIBase::DrawBox(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h, SimpleUIBase::PointColor_e _color) {
    DrawHLine(_x, _y, _w, _color);
    DrawHLine(_x, _y + _h - 1, _w, _color);
    DrawVLine(_x, _y + 1, _h - 2, _color);
    DrawVLine(_x + _w - 1, _y + 1, _h - 2, _color);
}

void
SimpleUIBase::DrawRBox(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h, uint16_t _r,
                       SimpleUIBase::PointColor_e _color) {
    uint16_t a, b;
    a = 1;
    b = _r;
    while (b > a) {
        b = (uint16_t) (sqrt(_r * _r - a * a) + 0.43f);
        if (b - a <= 1 && b >= (uint16_t) (sqrt(_r * _r - (a - 1) * (a - 1)) + 0.43f))
            break;
        if (b - a < 1) break;
        DrawHLine(_x + _w - _r + a, _y + _r - b, b - a, _color);
        DrawHLine(_x + _w - _r + a + 1, _y + _r - 1 - a, b - a - 1, _color);

        DrawHLine(_x + _r - 1 - a, _y + _r - b, b - a, _color);
        DrawHLine(_x + _r - b, _y + _r - 1 - a, b - a - 1, _color);

        DrawHLine(_x + _w - _r + a, _y + _h - _r + a, b - a, _color);
        DrawHLine(_x + _w - _r + a + 1, _y + _h - _r + a, b - a - 1, _color);

        DrawHLine(_x + _r - 1 - a, _y + _h - _r + a, b - a, _color);
        DrawHLine(_x + _r - b, _y + _h - _r + a, b - a - 1, _color);
        a++;
    }
    DrawHLine(_x + _r - 1, _y, _w - 2 * _r + 2, _color);
    DrawHLine(_x + _r - 1, _y + _h - 1, _w - 2 * _r + 2, _color);
    DrawVLine(_x, _y + _r - 1, _h - 2 * _r + 2, _color);
    DrawVLine(_x + _w - 1, _y + _r - 1, _h - 2 * _r + 2, _color);

}

void
SimpleUIBase::ShowChar(uint16_t _x, uint16_t _y, char _chr, FontSize_e _fontSize, SimpleUIBase::PointColor_e _color) {
    uint8_t fontSize, chr1, i, m, temp;
    uint16_t x0 = _x, y0 = _y;
    uint8_t *charData;

    if (_chr < ' ' || _chr > '~') return;
    chr1 = _chr - ' ';
    switch (_fontSize) {
        case FONT_SIZE_12:
            charData = (uint8_t *) ui_asc2_1206[chr1];
            fontSize = 12;
            break;
        case FONT_SIZE_16:
            charData = (uint8_t *) ui_asc2_1608[chr1];
            fontSize = 16;
            break;
        case FONT_SIZE_24:
            charData = (uint8_t *) ui_asc2_2412[chr1];
            fontSize = 36;
            break;
        default:
            return;
    }
    for (i = 0; i < fontSize; i++) {
        temp = charData[i];
        for (m = 0; m < 8; m++) {
            if (temp & 0x01) DrawPoint(_x, _y, _color);
            temp >>= 1;
            _y++;
        }
        _x++;
        if (fontSize > 16) {
            if (((_x - x0) == fontSize / 2) || ((_x - x0) == fontSize / 3)) {
                _x = x0;
                y0 = y0 + 8;
            }
        } else if (fontSize <= 16) {
            if (((_x - x0) == fontSize / 2)) {
                _x = x0;
                y0 = y0 + 8;
            }
        }

        _y = y0;
    }
}

#if KKUI_CH_FONT_EN
void SimpleUIBase::UIShowChinese(uint16_t _x, uint16_t _y, uint8_t *ch, uint8_t _size, SimpleUIBase::PointColor_e _color) {

}
#endif

void SimpleUIBase::ShowPicture(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h, uint8_t BMP[],
                               SimpleUIBase::PointColor_e _color) {
    uint16_t j = 0;
    uint8_t i, n, temp, m;
    uint16_t x0 = _x, y0 = _y;
    _h = _h / 8 + (_h % 8 ? 1 : 0);
    switch (_color) {
        case SimpleUIBase::BLACK_POINT: {
            for (n = 0; n < _h; n++) {
                for (i = 0; i < _w; i++) {
                    temp = BMP[j++];
                    for (m = 0; m < 8; m++) {
                        if (temp & 0x01) DrawPoint(_x, _y, SimpleUIBase::WHITE_POINT);
                        temp >>= 1;
                        _y++;
                    }
                    _x++;
                    if ((_x - x0) == _w) {
                        _x = x0;
                        y0 = y0 + 8;
                    }
                    _y = y0;
                }
            }
            break;
        }
        case SimpleUIBase::WHITE_POINT: {
            for (n = 0; n < _h; n++) {
                for (i = 0; i < _w; i++) {
                    temp = BMP[j++];
                    for (m = 0; m < 8; m++) {
                        DrawPoint(_x, _y, (temp & 0x01) ? SimpleUIBase::WHITE_POINT : SimpleUIBase::BLACK_POINT);
                        temp >>= 1;
                        _y++;
                    }
                    _x++;
                    if ((_x - x0) == _w) {
                        _x = x0;
                        y0 = y0 + 8;
                    }
                    _y = y0;
                }
            }
            break;
        }
        case SimpleUIBase::OR_POINT: {
            for (n = 0; n < _h; n++) {
                for (i = 0; i < _w; i++) {
                    temp = BMP[j++];
                    for (m = 0; m < 8; m++) {
                        if (temp & 0x01) DrawPoint(_x, _y, SimpleUIBase::OR_POINT);
                        temp >>= 1;
                        _y++;
                    }
                    _x++;
                    if ((_x - x0) == _w) {
                        _x = x0;
                        y0 = y0 + 8;
                    }
                    _y = y0;
                }
            }
            break;
        }
    }
}


void SimpleUIBase::ShowString(uint16_t _x, uint16_t _y, char *_str, SimpleUIBase::FontSize_e _fontSize,
                              SimpleUIBase::PointColor_e _color) {
    uint16_t x = _x, y = _y;
    while (1) {
        if (*_str == 0) break;
        if ((*_str >= ' ') && (*_str <= '~')) {
            ShowChar(x, y, *_str, _fontSize, _color);
            x += (uint8_t) _fontSize / 2;
            _str++;
        } else if ((_str[0] > 0xA0) && (_str[1] > 0xA0)) {
#if KKUI_CH_FONT_EN
            uiShowChinese(x, y, chr, size1, mode);
#endif
            x += (uint8_t) _fontSize;
            _str += 2;
        } else if (*_str == 0x0a) {
            x = _x;
            y += (uint8_t) _fontSize;
            _str++;
        } else
            _str++;
    }
}

void SimpleUIBase::SaveFrameBuffer(uint8_t _index) {
    memcpy(baseFrameBuffer[_index], KKUI_Gram_Addr, KKUI_Gram_Size);
    DisClear(WHITE_POINT);
}

void SimpleUIBase::LoadFrameBuffer(uint8_t _inxdex) {
    DisClear(WHITE_POINT);
    memcpy(KKUI_Gram_Addr, baseFrameBuffer[_inxdex], KKUI_Gram_Size);
}

void SimpleUIBase::ANDFrameBuffer(uint8_t _inxdex) {
    for (uint16_t i = 0; i < KKUI_Gram_High; i++) {
        for (uint16_t j = 0; j < KKUI_Gram_Wide; j++) {
            KKUI_Gram_Addr[i][j] &= baseFrameBuffer[_inxdex][i * KKUI_Gram_Wide + j];
        }
    }
}

void SimpleUIBase::ORFrameBuffer(uint8_t _inxdex) {
    for (uint16_t i = 0; i < KKUI_Gram_High; i++) {
        for (uint16_t j = 0; j < KKUI_Gram_Wide; j++) {
            KKUI_Gram_Addr[i][j] |= baseFrameBuffer[_inxdex][i * KKUI_Gram_Wide + j];
        }
    }
}

void SimpleUIBase::XORFrameBuffer(uint8_t _inxdex) {
    for (uint16_t i = 0; i < KKUI_Gram_High; i++) {
        for (uint16_t j = 0; j < KKUI_Gram_Wide; j++) {
            KKUI_Gram_Addr[i][j] ^= baseFrameBuffer[_inxdex][i * KKUI_Gram_Wide + j];
        }
    }
}

uint64_t SimpleUIBase::GetTick() {
    return uiTick;
}
