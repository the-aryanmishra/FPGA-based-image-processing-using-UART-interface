#include <iostream>
#include <fstream>
#include <vector>
#include "WriteBMP.h"
#define height 150
#define width 200

using namespace std;

int main()
{
    ifstream f("Image.txt");
    if (!f.is_open()) {
        cerr << "Error opening the file!";
        return 1;
    }
    string pixel;
    int number = 0;
    int tempData[height*3*width];
    int count = 0;

    cout << "Obtaining Pixels." << endl;
    for(int i = 0; i < width*height; i++)
    {
        getline(f,pixel);
        int num_len = 0;
        int number = 0;
        for(int j = 0; j < pixel.size()+1; j++)
        {
            int digit = (int)(pixel[j]-'0');
            // if(num_len == 3)
            // {
            //     tempData[count] = number;
            //     count++;
            //     number = 0;
            //     num_len = 0;
            // }
            if(digit != -16 && digit != -48)
            {
                number = number * 10 + digit;
                num_len++;
            }
            else
            {
                tempData[count] = number;
                count++;
                number = 0;
                num_len = 0;
            }
        }
    }
    f.close();

    int image[height][3*width];
    int row = 0;
    int col = 0;

    cout << "Entering 2D storing loop." << endl;
    for(int i = 0; i < height*3*width; i++)
    {
        // if(i % (3*width) == 0 && i > 0)
        if(col == 3*width)
        {
            row++;
            col = 0;
        }
        image[row][col] = tempData[i];
        col++;
    }
    cout << "2D Array Created." << endl;



    int grayscale = 0;

    int brighten = 0;
    float amount = -0.50;

    int flip = 0;
    int flipHorizontal = 1;
    int flipVertical = 1;

    int invert = 0;

    int sepia = 1;

    if(grayscale)
    {
        for(int i = 0; i < height; i++)
        {
            for(int j = 0; j < width; j++)
            {
                //Y=0.299⋅R+0.587⋅G+0.114⋅B
                int gray = (int)min((int)(0.299*image[i][3*j] + 0.587*image[i][3*j+1] + 0.114*image[i][3*j+2]), 255); //calculate the grayscale value
                image[i][3*j] = gray;
                image[i][3*j+1] = gray;
                image[i][3*j+2] = gray;
            }
        }
    }

    if(brighten)
    {
        for (int i = 0; i < height; i++) 
    {
        for (int j = 0; j < width; j++) 
        {
            int r = 3 * j;
            int g = 3 * j + 1;
            int b = 3 * j + 2;
            if(amount >= -1 && amount <= 1) 
            { 
                float newRed, newGreen, newBlue;
                newRed = image[i][r] + (255*amount);
                newGreen = image[i][g] + (255*amount);
                newBlue = image[i][b] + (255*amount);

                if(newRed > 255)
                    newRed = 255;
                else if(newRed < 0)
                    newRed = 0;
                
                if(newGreen > 255)
                    newGreen = 255;
                else if(newGreen < 0)
                    newGreen = 0;
                
                if(newBlue > 255)
                    newBlue = 255;
                else if(newBlue < 0)
                    newBlue = 0;
                
                image[i][r] = (int)newRed;
                image[i][g] = (int)newGreen;
                image[i][b] = (int)newBlue;
            }
        }
    }
    }

    if(flip)
    {
        if (flipHorizontal) 
        {
            for(int i = 0; i < height; i++)
            {
                int start = 0;
                int end = width - 1;
                while(start < end)
                {
                    int r = image[i][3*start];
                    int g = image[i][3*start + 1];
                    int b = image[i][3*start + 2];

                    image[i][3*start] = image[i][3*end];
                    image[i][3*start + 1] = image[i][3*end + 1];
                    image[i][3*start + 2] = image[i][3*end + 2];

                    image[i][3*end] = r;
                    image[i][3*end + 1] = g;
                    image[i][3*end + 2] = b;

                    start++;
                    end--;
                }
            }
        }
        if (flipVertical)
        {
            for(int i = 0; i < 3*width; i++)
            {
                int start = 0;
                int end = height-1;
                while(start < end)
                {
                    int temp = image[start][i];
                    image[start][i] = image[end][i];
                    image[end][i] = temp;
                    start++;
                    end--;
                }
            }    
        }
    }

    if(invert)
    {
        for (int i = 0; i < height; i++) 
        {
            for (int j = 0; j < width; j++) 
            {
                image[i][3*j] = 255-image[i][3*j];    //invert the pixel
                image[i][3*j+1] = 255-image[i][3*j+1];
                image[i][3*j+2] = 255-image[i][3*j+2];
            }
        }
    }

    if(sepia)
    {
        for(int i = 0; i < height; i++)
        {
            for(int j = 0; j < width; j++)
            {
                int r = 3 * j;
                int g = 3 * j + 1;
                int b = 3 * j + 2;
                //Y=0.299⋅R+0.587⋅G+0.114⋅B
                image[i][r] = (int) (min(0.393*image[i][r] + 0.769*image[i][g] + 0.189*image[i][b], 255.0));    //calculate the sepia value
                image[i][g] = (int) (min(0.349*image[i][r] + 0.686*image[i][g] + 0.168*image[i][b], 255.0));
                image[i][b] = (int) (min(0.272*image[i][r] + 0.534*image[i][g] + 0.131*image[i][b], 255.0));
            }
        }
    }

    // ofstream outFile("Image2.txt");
    // if (!outFile)
    //     cerr << "Error: Could not create the output file." << endl;
    // else{
    //     for (int i = 0; i < height; i++) 
    //     {
    //         for (int j = 0; j < width; j++) 
    //         {
    //             outFile << image[i][3*j] << " " << image[i][3*j+1] << " " << image[i][3*j+2] << endl;
    //         }
    //     }
    //     cout << "Done writing again" << endl;
    // }
    // outFile.close();

    vector<vector<int>> pixelData;
    for(int i = 0; i < height; i++)
    {
        vector<int> v;
        for(int j = 0; j < width; j++)
        {
                v.push_back(image[i][3*j]);
                v.push_back(image[i][3*j+1]);
                v.push_back(image[i][3*j+2]);
        }
        pixelData.push_back(v);
    }
    WriteBMP("Result.bmp", pixelData, height, width);



    return 0;
}