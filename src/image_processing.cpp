#include "image_processing.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Bellek Yönetimi

unsigned char*** createImageArray(int height, int width) {
    unsigned char*** image = new unsigned char**[height];
    for (int i = 0; i < height; i++) {
        image[i] = new unsigned char*[width];
        for (int j = 0; j < width; j++) {
            image[i][j] = new unsigned char[3];
        }
    }
    return image;
}

unsigned char** createGrayArray(int height, int width) {
    unsigned char** image = new unsigned char*[height];
    for (int i = 0; i < height; i++) {
        image[i] = new unsigned char[width];
    }
    return image;
}

void freeImageArray(unsigned char*** image, int height, int width) {
    if (!image) return;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            delete[] image[i][j];
        }
        delete[] image[i];
    }
    delete[] image;
}

void freeGrayArray(unsigned char** image, int height) {
    if (!image) return;
    for (int i = 0; i < height; i++) {
        delete[] image[i];
    }
    delete[] image;
}

// BMP İşlemleri

void saveBMP(const char* filename, unsigned char*** image, BMPHeader header, BMPInfoHeader infoHeader, int padding) {
    std::ofstream out(filename, std::ios::binary);
    out.write((char*)&header, sizeof(header));
    out.write((char*)&infoHeader, sizeof(infoHeader));

    for (int i = 0; i < infoHeader.height; i++) {
        for (int j = 0; j < infoHeader.width; j++) {
            out.write((char*)image[i][j], 3);
        }
        for (int k = 0; k < padding; k++) {
            out.put(0);
        }
    }
    out.close();
}

void convertGrayToBMP(unsigned char** srcGray, unsigned char*** dstRGB, int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            unsigned char value = srcGray[i][j];
            dstRGB[i][j][0] = value; // B
            dstRGB[i][j][1] = value; // G
            dstRGB[i][j][2] = value; // R
        }
    }
}

// Renk Uzayları ve Filtreler

void extractChannels(unsigned char*** src, unsigned char*** rImg, unsigned char*** gImg, unsigned char*** bImg, int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            bImg[i][j][0] = src[i][j][0]; bImg[i][j][1] = 0; bImg[i][j][2] = 0;
            gImg[i][j][0] = 0; gImg[i][j][1] = src[i][j][1]; gImg[i][j][2] = 0;
            rImg[i][j][0] = 0; rImg[i][j][1] = 0; rImg[i][j][2] = src[i][j][2];
        }
    }
}

void convertToGray(unsigned char*** src, unsigned char** dst, int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int gray = 0.299 * src[i][j][2] + 0.587 * src[i][j][1] + 0.114 * src[i][j][0];
            dst[i][j] = std::clamp(gray, 0, 255);
        }
    }
}

void RGBtoHSI(unsigned char*** src, HSI** dst, int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double b = src[i][j][0] / 255.0;
            double g = src[i][j][1] / 255.0;
            double r = src[i][j][2] / 255.0;

            double numerator = 0.5 * ((r - g) + (r - b));
            double denominator = sqrt((r - g) * (r - g) + (r - b) * (g - b));
            double theta = 0;

            if (denominator != 0) {
                double value = std::clamp(numerator / denominator, -1.0, 1.0);
                theta = acos(value);
            }

            double h = (b <= g) ? theta : (2 * M_PI - theta);
            double minRGB = std::min({r, g, b});
            double s = (r + g + b != 0) ? (1 - (3 * minRGB / (r + g + b))) : 0;
            double intensity = (r + g + b) / 3.0;

            dst[i][j].h = h;
            dst[i][j].s = s;
            dst[i][j].i = intensity;
        }
    }
}

void HSItoRGB(HSI** src, unsigned char*** dst, int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double H = src[i][j].h;
            double S = src[i][j].s;
            double I = src[i][j].i;
            double R, G, B;

            if (H < 2 * M_PI / 3) {
                B = I * (1 - S);
                R = I * (1 + (S * cos(H)) / cos(M_PI / 3 - H));
                G = 3 * I - (R + B);
            } else if (H < 4 * M_PI / 3) {
                H = H - 2 * M_PI / 3;
                R = I * (1 - S);
                G = I * (1 + (S * cos(H)) / cos(M_PI / 3 - H));
                B = 3 * I - (R + G);
            } else {
                H = H - 4 * M_PI / 3;
                G = I * (1 - S);
                B = I * (1 + (S * cos(H)) / cos(M_PI / 3 - H));
                R = 3 * I - (G + B);
            }

            dst[i][j][2] = std::clamp(R * 255.0, 0.0, 255.0);
            dst[i][j][1] = std::clamp(G * 255.0, 0.0, 255.0);
            dst[i][j][0] = std::clamp(B * 255.0, 0.0, 255.0);
        }
    }
}

void RGBtoYCbCr(unsigned char*** src, YCbCr** dst, int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double B = src[i][j][0];
            double G = src[i][j][1];
            double R = src[i][j][2];

            dst[i][j].y  =  0.299000 * R + 0.587000 * G + 0.114000 * B;
            dst[i][j].cb = -0.168736 * R - 0.331264 * G + 0.500000 * B + 128;
            dst[i][j].cr =  0.500000 * R - 0.418688 * G - 0.081312 * B + 128;
        }
    }
}

void YCbCrtoRGB(YCbCr** src, unsigned char*** dst, int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double Y = src[i][j].y;
            double Cb = src[i][j].cb;
            double Cr = src[i][j].cr;

            double R = Y + 1.402 * (Cr - 128);
            double G = Y - 0.344136 * (Cb - 128) - 0.714136 * (Cr - 128);
            double B = Y + 1.772 * (Cb - 128);

            dst[i][j][2] = std::clamp(R, 0.0, 255.0);
            dst[i][j][1] = std::clamp(G, 0.0, 255.0);
            dst[i][j][0] = std::clamp(B, 0.0, 255.0);
        }
    }
}

// Histogram ve Filtreler

void equalizeRGB(unsigned char*** src, unsigned char*** dst, int height, int width) {
    int rHist[256] = {0}, gHist[256] = {0}, bHist[256] = {0};
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            bHist[src[i][j][0]]++; gHist[src[i][j][1]]++; rHist[src[i][j][2]]++;
        }
    }

    int rCDF[256] = {0}, gCDF[256] = {0}, bCDF[256] = {0};
    rCDF[0] = rHist[0]; gCDF[0] = gHist[0]; bCDF[0] = bHist[0];
    for (int i = 1; i < 256; i++) {
        rCDF[i] = rCDF[i - 1] + rHist[i];
        gCDF[i] = gCDF[i - 1] + gHist[i];
        bCDF[i] = bCDF[i - 1] + bHist[i];
    }

    int totalPixels = width * height;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            dst[i][j][0] = (bCDF[src[i][j][0]] * 255) / totalPixels;
            dst[i][j][1] = (gCDF[src[i][j][1]] * 255) / totalPixels;
            dst[i][j][2] = (rCDF[src[i][j][2]] * 255) / totalPixels;
        }
    }
}

void equalizeHSI(HSI** hsiImg, int height, int width) {
    int intensityHistogram[256] = {0};
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int intensity = std::clamp((int)(hsiImg[i][j].i * 255), 0, 255);
            intensityHistogram[intensity]++;
        }
    }

    int intensityCDF[256] = {0};
    intensityCDF[0] = intensityHistogram[0];
    for (int i = 1; i < 256; i++) {
        intensityCDF[i] = intensityCDF[i - 1] + intensityHistogram[i];
    }

    int totalPixels = width * height;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int oldIntensity = std::clamp((int)(hsiImg[i][j].i * 255), 0, 255);
            hsiImg[i][j].i = ((intensityCDF[oldIntensity] * 255) / totalPixels) / 255.0;
        }
    }
}

void equalizeYCbCr(YCbCr** ycbcrImg, int height, int width) {
    int yHistogram[256] = {0};
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int yValue = std::clamp((int)ycbcrImg[i][j].y, 0, 255);
            yHistogram[yValue]++;
        }
    }

    int yCDF[256] = {0};
    yCDF[0] = yHistogram[0];
    for (int i = 1; i < 256; i++) {
        yCDF[i] = yCDF[i - 1] + yHistogram[i];
    }

    int totalPixels = width * height;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int oldY = std::clamp((int)ycbcrImg[i][j].y, 0, 255);
            ycbcrImg[i][j].y = (yCDF[oldY] * 255) / totalPixels;
        }
    }
}

// --- Filtreleme ve Kenar Algılama ---

void applyBlur(unsigned char** src, unsigned char** dst, int height, int width) {
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            int sum = 1 * src[i - 1][j - 1] + 2 * src[i - 1][j] + 1 * src[i - 1][j + 1] +
                      2 * src[i][j - 1]     + 4 * src[i][j]    + 2 * src[i][j + 1] +
                      1 * src[i + 1][j - 1] + 2 * src[i + 1][j] + 1 * src[i + 1][j + 1];
            dst[i][j] = sum / 16;
        }
    }
    // Kenar piksellerini doldurma
    for (int i = 0; i < height; i++) {
        dst[i][0] = src[i][0];
        dst[i][width - 1] = src[i][width - 1];
    }
    for (int j = 0; j < width; j++) {
        dst[0][j] = src[0][j];
        dst[height - 1][j] = src[height - 1][j];
    }
}

void applySobel(unsigned char** src, unsigned char** dstSobel, int height, int width, int threshold) {
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            int gx = -1 * src[i - 1][j - 1] + 1 * src[i - 1][j + 1] +
                     -2 * src[i][j - 1]     + 2 * src[i][j + 1] +
                     -1 * src[i + 1][j - 1] + 1 * src[i + 1][j + 1];

            int gy = -1 * src[i - 1][j - 1] - 2 * src[i - 1][j] - 1 * src[i - 1][j + 1] +
                      1 * src[i + 1][j - 1] + 2 * src[i + 1][j] + 1 * src[i + 1][j + 1];

            int magnitude = sqrt(gx * gx + gy * gy);
            dstSobel[i][j] = (magnitude > threshold) ? 255 : 0;
        }
    }
}

// Performans Değerlendirme Fonksiyonları

double calculateMSE(unsigned char*** orig, unsigned char*** proc, int height, int width) {
    double errorSum = 0.0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int c = 0; c < 3; c++) { // R, G, B kanalları için
                double diff = (double)orig[i][j][c] - (double)proc[i][j][c];
                errorSum += diff * diff;
            }
        }
    }
    return errorSum / (height * width * 3.0);
}

double calculatePSNR(double mse) {
    if (mse == 0) return 99.0; 
    return 10.0 * log10((255.0 * 255.0) / mse);
}

double calculateSNR(unsigned char*** orig, unsigned char*** proc, int height, int width) {
    double signalPower = 0.0;
    double noisePower = 0.0;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int c = 0; c < 3; c++) {
                double origVal = (double)orig[i][j][c];
                double procVal = (double)proc[i][j][c];
                
                signalPower += origVal * origVal;
                noisePower += (origVal - procVal) * (origVal - procVal);
            }
        }
    }
    if (noisePower == 0) return 99.0;
    return 10.0 * log10(signalPower / noisePower);
}
