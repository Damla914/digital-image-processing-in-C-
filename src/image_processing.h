#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include "types.h" 

// Bellek Yönetimi
unsigned char*** createImageArray(int height, int width);
unsigned char** createGrayArray(int height, int width);
void freeImageArray(unsigned char*** image, int height, int width);
void freeGrayArray(unsigned char** image, int height);

// BMP İşlemleri
void saveBMP(const char* filename, unsigned char*** image, BMPHeader header, BMPInfoHeader infoHeader, int padding);
void convertGrayToBMP(unsigned char** srcGray, unsigned char*** dstRGB, int height, int width);

// Renk Uzayları ve Filtreler
void extractChannels(unsigned char*** src, unsigned char*** rImg, unsigned char*** gImg, unsigned char*** bImg, int height, int width);
void convertToGray(unsigned char*** src, unsigned char** dst, int height, int width);
void RGBtoHSI(unsigned char*** src, HSI** dst, int height, int width);
void HSItoRGB(HSI** src, unsigned char*** dst, int height, int width);
void RGBtoYCbCr(unsigned char*** src, YCbCr** dst, int height, int width);
void YCbCrtoRGB(YCbCr** src, unsigned char*** dst, int height, int width);

// Histogram ve Filtreler
void equalizeRGB(unsigned char*** src, unsigned char*** dst, int height, int width);
void equalizeHSI(HSI** hsiImg, int height, int width);
void equalizeYCbCr(YCbCr** ycbcrImg, int height, int width);
void applyBlur(unsigned char** src, unsigned char** dst, int height, int width);
void applySobel(unsigned char** src, unsigned char** dstSobel, int height, int width, int threshold);

// Performans Değerlendirme Fonksiyonları
double calculateMSE(unsigned char*** orig, unsigned char*** proc, int height, int width);
double calculatePSNR(double mse);
double calculateSNR(unsigned char*** orig, unsigned char*** proc, int height, int width);

#endif