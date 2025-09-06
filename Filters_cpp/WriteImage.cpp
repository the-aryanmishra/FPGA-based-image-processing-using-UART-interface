#include "WriteImage.h"

int main()
{
    string filename = "Image4.bmp";
    ifstream file(filename, ios::binary);

    if (!file) {
        cerr << "Error: Could not open the BMP file." << endl;
        exit(1);
    }

    uint8_t fileHeader[14];
    file.read(reinterpret_cast<char*>(fileHeader), 14);
    uint8_t infoHeader[40];
    file.read(reinterpret_cast<char*>(infoHeader), 40);

    int width = *reinterpret_cast<int*>(&infoHeader[4]);
    int height = *reinterpret_cast<int*>(&infoHeader[8]);
 
    int rowSize = (3 * width + 3) & (~3);

    vector<uint8_t> rowBuffer(rowSize);
    vector<vector<int>> pixelData;
    for (int i = height - 1; i >= 0; --i)
    {
        vector<int> row;
        file.read(reinterpret_cast<char*>(rowBuffer.data()), rowSize);
        for (int j = 0; j < width; ++j) 
        {
            vector<int> pixel;

            int blue = rowBuffer[3 * j];
            int green = rowBuffer[3 * j + 1];
            int red = rowBuffer[3 * j + 2];

            row.push_back(red);
            row.push_back(green);
            row.push_back(blue);

        }
        pixelData.push_back(row);
    }
    file.close();

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < 83; j = j + 3)
        {
            for(int k = 0; k < 3; k++)
            {
                pixelData[i].push_back(pixelData[i][0]);  
                pixelData[i].erase(pixelData[i].begin()); 
            }
        }
    }

    ofstream outFile("Image.txt");

    if (!outFile)
        cerr << "Error: Could not create the output file." << endl;
    else{
        for (int i = 0; i < pixelData.size(); i++) 
        {
            for (int j = 0; j < pixelData[0].size()/3; j++) 
            {
                outFile << pixelData[i][3*j] << " " << pixelData[i][3*j+1] << " " << pixelData[i][3*j+2] << endl;
            }
        }
    }

    cout << "RGB values stored in Image.txt" << endl;
    outFile.close();
    return 0;
}