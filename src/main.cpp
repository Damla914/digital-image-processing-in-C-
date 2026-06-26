#include <iostream>
#include <fstream>
#include "image_processing.h" 

int main() {
   
    std::ifstream file("lena.bmp", std::ios::binary);
    if (!file) {
        std::cerr << "File is not found" << std::endl;
        return 1;
    }

    BMPHeader header;
    BMPInfoHeader infoHeader;
    file.read((char*)&header, sizeof(header));
    file.read((char*)&infoHeader, sizeof(infoHeader));

    if (header.type != 0x4D42) {
        std::cout << "BMP degil!" << std::endl;
        return 1;
    }

    int width = infoHeader.width;
    int height = infoHeader.height;
    int padding = (4 - (width * 3) % 4) % 4;

    file.seekg(header.offset, std::ios::beg);

    unsigned char*** image = createImageArray(height, width);
    unsigned char*** redImage = createImageArray(height, width);
    unsigned char*** greenImage = createImageArray(height, width);
    unsigned char*** blueImage = createImageArray(height, width);
    unsigned char*** equalizedImage = createImageArray(height, width);
    unsigned char*** hsiEqualizedImage = createImageArray(height, width);
    unsigned char*** ycbcrEqualizedImage = createImageArray(height, width);
    unsigned char*** grayBMP = createImageArray(height, width);
    unsigned char*** blurBMP = createImageArray(height, width);
    unsigned char*** sobelBMP = createImageArray(height, width);

    unsigned char** grayImage = createGrayArray(height, width);
    unsigned char** blurImage = createGrayArray(height, width);
    unsigned char** sobelImage = createGrayArray(height, width);

    HSI** hsiImage = new HSI*[height];
    YCbCr** ycbcrImage = new YCbCr*[height];
    for (int i = 0; i < height; i++) {
        hsiImage[i] = new HSI[width];
        ycbcrImage[i] = new YCbCr[width];
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            file.read((char*)image[i][j], 3);
        }
        file.ignore(padding);
    }
    file.close(); 

    extractChannels(image, redImage, greenImage, blueImage, height, width);
    saveBMP("red.bmp", redImage, header, infoHeader, padding);
    saveBMP("green.bmp", greenImage, header, infoHeader, padding);
    saveBMP("blue.bmp", blueImage, header, infoHeader, padding);

    equalizeRGB(image, equalizedImage, height, width);
    saveBMP("equalized_rgb.bmp", equalizedImage, header, infoHeader, padding);

    RGBtoHSI(image, hsiImage, height, width);
    equalizeHSI(hsiImage, height, width);
    HSItoRGB(hsiImage, hsiEqualizedImage, height, width);
    saveBMP("equalized_hsi.bmp", hsiEqualizedImage, header, infoHeader, padding);

    RGBtoYCbCr(image, ycbcrImage, height, width);
    equalizeYCbCr(ycbcrImage, height, width);
    YCbCrtoRGB(ycbcrImage, ycbcrEqualizedImage, height, width);
    saveBMP("equalized_ycbcr.bmp", ycbcrEqualizedImage, header, infoHeader, padding);

    convertToGray(image, grayImage, height, width);
    convertGrayToBMP(grayImage, grayBMP, height, width);
    saveBMP("gray.bmp", grayBMP, header, infoHeader, padding);

  
    applyBlur(grayImage, blurImage, height, width);
    convertGrayToBMP(blurImage, blurBMP, height, width);
    saveBMP("blur.bmp", blurBMP, header, infoHeader, padding);

    applySobel(blurImage, sobelImage, height, width, 100);
    convertGrayToBMP(sobelImage, sobelBMP, height, width);
    saveBMP("sobel.bmp", sobelBMP, header, infoHeader, padding);

    std::cout << "\nPERFORMENCE EVALUATION\n" << std::endl;

    double mseRGB = calculateMSE(image, equalizedImage, height, width);
    std::cout << "RGB Equalization:" << std::endl;
    std::cout << " MSE: " << mseRGB << std::endl;
    std::cout << " PSNR: " << calculatePSNR(mseRGB) << " dB" << std::endl;
    std::cout << " SNR: " << calculateSNR(image, equalizedImage, height, width) << " dB\n" << std::endl;

    double mseHSI = calculateMSE(image, hsiEqualizedImage, height, width);
    std::cout << "HSI Equalization:" << std::endl;
    std::cout << " MSE: " << mseHSI << std::endl;
    std::cout << " PSNR: " << calculatePSNR(mseHSI) << " dB" << std::endl;
    std::cout << " SNR: " << calculateSNR(image, hsiEqualizedImage, height, width) << " dB\n" << std::endl;

    double mseYCbCr = calculateMSE(image, ycbcrEqualizedImage, height, width);
    std::cout << "YCbCr Equalization:" << std::endl;
    std::cout << " MSE: " << mseYCbCr << std::endl;
    std::cout << " PSNR: " << calculatePSNR(mseYCbCr) << " dB" << std::endl;
    std::cout << " SNR: " << calculateSNR(image, ycbcrEqualizedImage, height, width) << " dB\n" << std::endl;

    freeImageArray(image, height, width);
    freeImageArray(redImage, height, width);
    freeImageArray(greenImage, height, width);
    freeImageArray(blueImage, height, width);
    freeImageArray(equalizedImage, height, width);
    freeImageArray(hsiEqualizedImage, height, width);
    freeImageArray(ycbcrEqualizedImage, height, width);
    freeImageArray(grayBMP, height, width);
    freeImageArray(blurBMP, height, width);
    freeImageArray(sobelBMP, height, width);

    freeGrayArray(grayImage, height);
    freeGrayArray(blurImage, height);
    freeGrayArray(sobelImage, height);

    for (int i = 0; i < height; i++) {
        delete[] hsiImage[i];
        delete[] ycbcrImage[i];
    }
    delete[] hsiImage;
    delete[] ycbcrImage;

    return 0;
}