#include "WriteBMP.h"

void WriteBMP(const string filename, vector<vector<int>> pixelData, int height, int width) {
    ofstream file(filename, ios::binary);

    // if (!file) {
    //     cerr << "Error: Could not create the BMP file." << endl;
    //     exit(1);
    // }

    // BMP Header (14 bytes)
    uint8_t fileHeader[14] = {
        'B', 'M',                          // Signature
        0, 0, 0, 0,                        // File size (will be set later)
        0, 0,                              // Reserved
        0, 0,                              // Reserved
        54, 0, 0, 0                        // Pixel data offset (54 bytes)
    };

    // BMP Info Header (40 bytes)
    uint8_t infoHeader[40] = {
        40, 0, 0, 0,                      // Header size
        0, 0, 0, 0,                       // Image width (will be set later)
        0, 0, 0, 0,                       // Image height (will be set later)
        1, 0,                             // Number of color planes (must be 1)
        24, 0,                            // Bits per pixel (24 for RGB)
        0, 0, 0, 0,                       // Compression (0 = no compression)
        0, 0, 0, 0,                       // Image size (can be 0 for no compression)
        0, 0, 0, 0,                       // Horizontal resolution (pixels per meter)
        0, 0, 0, 0,                       // Vertical resolution (pixels per meter)
        0, 0, 0, 0,                       // Number of colors (0 = default)
        0, 0, 0, 0                        // Important colors (0 = all colors are important)
    };

    *reinterpret_cast<int*>(&infoHeader[4]) = width;
    *reinterpret_cast<int*>(&infoHeader[8]) = height;

    // Calculate the row size (each row must be a multiple of 4 bytes)
    int rowSize = (3 * width + 3) & (~3);
    int imageSize = rowSize * height;

    *reinterpret_cast<int*>(&fileHeader[2]) = 54 + imageSize;

    file.write(reinterpret_cast<char*>(fileHeader), 14);
    file.write(reinterpret_cast<char*>(infoHeader), 40);

    vector<uint8_t> rowBuffer(rowSize);

    // for(int i = 0; i < height; i++)
    // {
    //     for(int j = 0; j < 28; j++)
    //     {
    //         vector<int> pixel = pixelData[i].front();
    //         pixelData[i].erase(pixelData[i].begin());
    //         pixelData[i].push_back(pixel);
    //     }
    // }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++) {

            rowBuffer[3 * j] = static_cast<uint8_t>(pixelData[i][3*j+2]);      //Blue
            rowBuffer[3 * j + 1] = static_cast<uint8_t>(pixelData[i][3*j+1]);  //Green
            rowBuffer[3 * j + 2] = static_cast<uint8_t>(pixelData[i][3*j]);  //Red

        }
        file.write(reinterpret_cast<char*>(rowBuffer.data()), rowSize);
    }

    file.close();
    cout << "BMP image created: " << filename << endl;
}