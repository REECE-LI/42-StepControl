//
// Created by 22482 on 2024/3/29.
//

#ifndef OLED_BASE_H
#define OLED_BASE_H

#include "simpleui_base.h"
#include "stdbool.h"


class OLED_Base : public SimpleUIBase {
public:
    typedef enum {
        DEGREE_0 = 0,
        DEGREE_90,
        DEGREE_180,
        DEGREE_270,
    } DisplayDegree_e;

    explicit OLED_Base() = default;

    void Init() override;

    void Refresh() override;

    void Rotate(DisplayDegree_e _degree);

    void Brightness(uint8_t _i);

    void Mirror(bool _enable);

    void Inverse(bool _enable);

    void Enable();

    void Disable();


private:

    uint8_t oledBuffer[KKUI_Gram_High][KKUI_Gram_Wide];

    void SPI_WriteByte(uint8_t _dat) override;

    void SPI_WriteBytes(uint8_t *_dat, uint8_t _len) override;

    void WR_Byte(uint8_t _dat, uint8_t _mode) override;

};


#endif //OLED_BASE_H
