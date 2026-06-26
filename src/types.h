#ifndef TYPES_H
#define TYPES_H

#include <cstdint>

#pragma pack(push, 1)
struct BMPHeader {
    uint16_t type;
    uint32_t size;
    uint16_t res1, res2;
    uint32_t offset;
};

struct BMPInfoHeader {
    uint32_t size;
    int32_t  width;
    int32_t  height;
    uint16_t planes;
    uint16_t bitCount;
    uint32_t compression;
    uint32_t sizeImage;
    int32_t  xPelsPerMeter;
    int32_t  yPelsPerMeter;
    uint32_t clrUsed;
    uint32_t clrImportant;
};
#pragma pack(pop)

struct HSI {
    double h;
    double s;
    double i;
};

struct YCbCr {
    double y;
    double cb;
    double cr;
};

#endif