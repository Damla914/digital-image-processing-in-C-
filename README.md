# Digital Image Processing in C++

A C++ implementation of fundamental digital image processing algorithms developed from scratch without using external libraries such as OpenCV.

This project focuses on implementing and comparing different image enhancement techniques using multiple color spaces, including RGB, HSI, and YCbCr.


## Features

- Read and write 24-bit BMP image files
- RGB channel separation
- Histogram calculation
- Histogram Equalization in RGB color space
- RGB → HSI color space conversion
- HSI → RGB conversion
- Histogram Equalization using the Intensity channel (HSI)
- RGB → YCbCr color space conversion
- YCbCr → RGB conversion
- Histogram Equalization using the Luminance channel (YCbCr)
- Grayscale image conversion
- Gaussian Blur (3×3 kernel)
- Sobel Edge Detection


## Project Structure

```
├── README.md
├── LICENSE
│
├── src/
│   └── main.cpp
│   └── image_processing.cpp
│   └── image_processing.h
│   └── types.h
│
├── images/
│   ├── lena.bmp
│   ├── red.bmp
│   ├── green.bmp
│   ├── blue.bmp
│   ├── equalized_rgb.bmp
│   ├── equalized_hsi.bmp
│   ├── equalized_ycbcr.bmp
│   ├── gray.bmp
│   ├── blur.bmp
│   └── sobel.bmp
│
└── report/
    └── digital_image_processing_report.pdf
```

## Implemented Algorithms

### 1. RGB Histogram Equalization

Histogram Equalization is independently applied to the Red, Green and Blue channels.

Although this increases image contrast, independent equalization may distort color relationships and produce unnatural colors.


### 2. HSI Histogram Equalization

The RGB image is converted into the HSI color space.

Histogram Equalization is applied only to the **Intensity (I)** channel while preserving Hue and Saturation.

Finally, the image is converted back to RGB.

This approach significantly improves contrast while preserving natural colors.


### 3. YCbCr Histogram Equalization

The RGB image is transformed into YCbCr.

Histogram Equalization is performed only on the **Luminance (Y)** component.

Since chrominance information remains unchanged, this method preserves image colors more effectively than direct RGB equalization.


### 4. Grayscale Conversion

The grayscale image is generated using the standard luminance equation:

```
Gray = 0.299R + 0.587G + 0.114B
```


### 5. Gaussian Blur

A 3×3 Gaussian kernel is used for image smoothing:

```
1 2 1
2 4 2
1 2 1
```

The kernel is normalized by dividing the weighted sum by **16**.


### 6. Sobel Edge Detection

The Sobel operator detects horizontal and vertical image gradients using two convolution masks.

Gradient magnitude is calculated as

```
Magnitude = sqrt(Gx² + Gy²)
```

to highlight image edges.


## Experimental Results

| Enhancement Method | MSE | PSNR (dB) | SNR (dB) |
|-------------------|---------:|---------:|---------:|
| RGB Equalization | 2384.88 | 14.3561 | 9.2186 |
| HSI Equalization | 979.322 | 18.2215 | 13.0840 |
| YCbCr Equalization | 647.113 | 20.0210 | 14.8834 |

The experimental results demonstrate that luminance-based histogram equalization methods outperform direct RGB equalization both quantitatively and visually.


## Sample Outputs

The project generates the following images:

- red.bmp
- green.bmp
- blue.bmp
- equalized_rgb.bmp
- equalized_hsi.bmp
- equalized_ycbcr.bmp
- gray.bmp
- blur.bmp
- sobel.bmp


## Technologies

- C++
- BMP File Format
- Histogram Equalization
- RGB Color Space
- HSI Color Space
- YCbCr Color Space
- Gaussian Filtering
- Sobel Edge Detection

