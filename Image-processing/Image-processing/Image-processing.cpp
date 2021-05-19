#include "pch.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;
using namespace System;
using namespace System::Drawing;

// ex 1
void normalizationRGB();
void normalizationMono();

// ex 2
void filtrationRGB();
Color filtrationPixelRGB(Bitmap^ source, int maskCenter, int kz, int kx);
void filtrationMono();
Color filtrationPixelMono(Bitmap^ source, int maskCenter, int kz, int kx);

// ex 3
void openingMono();
Color openingPixelMono(Bitmap^ source, int** mask, int maskCenter, int kz, int kx, int type);
void openingLog();
Color openingPixelLog(Bitmap^ source, int** mask, int maskCenter, int kz, int kx, int type);

// ex 4
void labeling();
void recursiveFinder(int x, int z, int current_label, Bitmap^ bmp, Bitmap^ bmpTmp);



int main()
{
    // zad 1
    std::cout << "exercise 1\n";
    std::cout << "normalization RGB\n";
    std::cout << "===========================================\n\n";
    normalizationRGB();
    std::cout << std::endl;
    std::cout << "normalization Mono\n";
    std::cout << "===========================================\n\n";
    normalizationMono();

    // zad 2
    std::cout << std::endl;
    std::cout << "exercise 2\n";
    std::cout << "filtration RGB\n";
    std::cout << "===========================================\n\n";
    filtrationRGB();
    std::cout << std::endl;
    std::cout << "filtration Mono\n";
    std::cout << "===========================================\n\n";
    filtrationMono();

    // zad 3
    std::cout << std::endl;
    std::cout << "exercise 3\n";
    std::cout << "opening Logic\n";
    std::cout << "===========================================\n\n";
    openingLog();
    std::cout << std::endl;
    std::cout << "opening Mono\n";
    std::cout << "===========================================\n\n";
    openingMono();

    // zad 4
    std::cout << std::endl;
    std::cout << "exercise 4\n";
    std::cout << "labeling\n";
    std::cout << "===========================================\n\n";
    labeling();

    return 0;
}



// zad 1
void normalizationMono()
{
    //===========================================================
    // create functions
    //===========================================================

    int noPoints;
    std::cout << "number of points (min 3):\t";
    std::cin >> noPoints;
    if (noPoints < 3)
    {
        std::cout << "error! minimum 3 points required!\nexiting...\n";
        exit(EXIT_FAILURE);
    }

    auto pointsArr = new int[noPoints + 2][2];
    auto functionArr = new double[noPoints + 1][2];
    pointsArr[0][0] = 0; pointsArr[0][1] = 0;
    pointsArr[noPoints + 1][0] = 255; pointsArr[noPoints + 1][1] = 255;

    std::cout << std::endl << "===========================================" << std::endl;
    std::cout << "enter the points\nvalues between (0,255)" << std::endl;
    std::cout << "===========================================" << std::endl;
    for (int i = 1; i < noPoints + 1; i++)
    {
        std::cout << i << "_x_: ";
        std::cin >> pointsArr[i][0];
        std::cout << std::endl;

        std::cout << i << "_y_: ";
        std::cin >> pointsArr[i][1];
        std::cout << std::endl;

        if (!(pointsArr[i][0] >= 0 && pointsArr[i][0] <= 255) ||   // x: (0,255)
            !(pointsArr[i][1] >= 0 && pointsArr[i][1] <= 255) ||     // y: (0,255)
            (pointsArr[i][0] <= pointsArr[i - 1][0]))
        {
            std::cout << "error! this must be correct function!\nexiting...\n";
            exit(EXIT_FAILURE);
        }

        if ((pointsArr[i][0] == pointsArr[i - 1][0]) && (pointsArr[i][1] == pointsArr[i - 1][1]))
        {
            std::cout << "error! incorrect points!\nexiting...\n";
            exit(EXIT_FAILURE);
        }
    }

    if (pointsArr[1][0] == 0 || pointsArr[noPoints][0] == 255)
    {
        std::cout << "error! conditions: x1 != 0 || x" << noPoints << " != 255\nexiting...\n";
        exit(EXIT_FAILURE);
    }

    // create functions y=ax+b
    for (int i = 0; i < noPoints + 1; i++)
    {
        // a
        functionArr[i][0] = (double)(pointsArr[i][1] - pointsArr[i + 1][1]) / (double)(pointsArr[i][0] - pointsArr[i + 1][0]);
        // b
        functionArr[i][1] = pointsArr[i][1] - functionArr[i][0] * pointsArr[i][0];
    }

    //===========================================================
    // loading image
    //===========================================================

    string fileName;

    std::cout << "insert image name:\n(possible moves example: ../../file.png)\n";
    std::cin >> fileName;

    Bitmap^ bmp, ^ bmpNew;
    bmp = gcnew Bitmap(gcnew String(fileName.c_str()), true);
    bmpNew = gcnew Bitmap(bmp->Width, bmp->Height);

    int wys, szer, kx, kz;
    int mono = 0;
    Color Px;

    wys = bmp->Height;
    szer = bmp->Width;

    for (kz = 0; kz < wys; kz++)
    {
        for (kx = 0; kx < szer; kx++)
        {
            Px = bmp->GetPixel(kx, kz);

            //mono = (int)(0.299)* (double)Px.R + (0.587) * (double)Px.G + (0.114) * (double)Px.B;

            mono = Px.R;

            for (int i = 1; i <= noPoints + 1; i++)
            {
                if (mono <= pointsArr[i][0] && mono > pointsArr[i - 1][0])
                {
                    mono = functionArr[i - 1][0] * mono + functionArr[i - 1][1];

                    if (mono < 0) mono = 0;
                    if (mono > 255) mono = 255;
                }

            }

            bmpNew->SetPixel(kx, kz, Px.FromArgb(mono, mono, mono));
        }
    }


    bmpNew->Save("../normalizationMono_new.png");

    delete[] pointsArr;
    delete[] functionArr;
    return;
}

void normalizationRGB()
{
    //===========================================================
    // create functions
    //===========================================================

    int noPoints;
    std::cout << "number of points (min 3):\t";
    std::cin >> noPoints;
    if (noPoints < 3)
    {
        std::cout << "error! minimum 3 points required!\nexiting...\n";
        exit(EXIT_FAILURE);
    }

    auto pointsArr = new int[noPoints + 2][2];
    auto functionArr = new double[noPoints + 1][2];
    pointsArr[0][0] = 0; pointsArr[0][1] = 0;
    pointsArr[noPoints + 1][0] = 255; pointsArr[noPoints + 1][1] = 255;

    std::cout << std::endl << "===========================================" << std::endl;
    std::cout << "enter the points\nvalues between (0,255)" << std::endl;
    std::cout << "===========================================" << std::endl;
    for (int i = 1; i < noPoints + 1; i++)
    {
        std::cout << i << "_x_: ";
        std::cin >> pointsArr[i][0];
        std::cout << std::endl;

        std::cout << i << "_y_: ";
        std::cin >> pointsArr[i][1];
        std::cout << std::endl;

        if (!(pointsArr[i][0] >= 0 && pointsArr[i][0] <= 255) ||   // x: (0,255)
            !(pointsArr[i][1] >= 0 && pointsArr[i][1] <= 255) ||   // y: (0,255)
            (pointsArr[i][0] <= pointsArr[i - 1][0]))
        {
            std::cout << "error! this must be correct function!\nexiting...\n";
            exit(EXIT_FAILURE);
        }

        if ((pointsArr[i][0] == pointsArr[i - 1][0]) && (pointsArr[i][1] == pointsArr[i - 1][1]))
        {
            std::cout << "error! incorrect points!\nexiting...\n";
            exit(EXIT_FAILURE);
        }
    }

    if (pointsArr[1][0] == 0 || pointsArr[noPoints][0] == 255)
    {
        std::cout << "error! conditions: x1 != 0 || x" << noPoints << " != 255\nexiting...\n";
        exit(EXIT_FAILURE);
    }

    // create functions y=ax+b
    for (int i = 0; i < noPoints + 1; i++)
    {
        // a
        functionArr[i][0] = (double)(pointsArr[i][1] - pointsArr[i + 1][1]) / (double)(pointsArr[i][0] - pointsArr[i + 1][0]);
        // b
        functionArr[i][1] = pointsArr[i][1] - functionArr[i][0] * pointsArr[i][0];
    }

    //===========================================================
    // loading image
    //===========================================================

    string fileName;

    std::cout << "insert image name:\n(possible moves example: ../../file.png)\n";
    std::cin >> fileName;

    Bitmap^ bmp, ^ bmpNew;
    bmp = gcnew Bitmap(gcnew String(fileName.c_str()), true);
    bmpNew = gcnew Bitmap(bmp->Width, bmp->Height);

    int wys, szer, kx, kz;
    int tmpR = 0, tmpG = 0, tmpB = 0;
    Color Px;

    wys = bmp->Height;
    szer = bmp->Width;

    for (kz = 0; kz < wys; kz++)
    {
        for (kx = 0; kx < szer; kx++)
        {
            Px = bmp->GetPixel(kx, kz);

            for (int i = 1; i <= noPoints + 1; i++)
            {
                if (Px.R <= pointsArr[i][0] && Px.R > pointsArr[i - 1][0])
                {
                    tmpR = functionArr[i - 1][0] * Px.R + functionArr[i - 1][1];

                    if (tmpR < 0) tmpR = 0;
                    if (tmpR > 255) tmpR = 255;
                }

                if (Px.G <= pointsArr[i][0] && Px.G > pointsArr[i - 1][0])
                {
                    tmpG = functionArr[i - 1][0] * Px.G + functionArr[i - 1][1];

                    if (tmpG < 0) tmpG = 0;
                    if (tmpG > 255) tmpG = 255;
                }

                if (Px.B <= pointsArr[i][0] && Px.B > pointsArr[i - 1][0])
                {
                    tmpB = functionArr[i - 1][0] * Px.B + functionArr[i - 1][1];

                    if (tmpB < 0) tmpB = 0;
                    if (tmpB > 255) tmpB = 255;
                }
            }

            bmpNew->SetPixel(kx, kz, Px.FromArgb(tmpR, tmpG, tmpB));
        }
    }

    bmpNew->Save("../normalizationRGB_new.png");

    delete[] pointsArr;
    delete[] functionArr;
    return;
}

// zad 2
void filtrationRGB()
{
    //===========================================================
    // mask creation
    //===========================================================

    int maskSize;
    std::cout << "insert mask size (int, odd number) -> enter:\t";
    std::cin >> maskSize;
    if (maskSize <= 0 || maskSize % 2 == 0)
    {
        std::cout << "blad! incorrect mask size!\nexiting...\n";
        exit(EXIT_FAILURE);
    }

    //===========================================================
    // loading image
    //===========================================================

    string fileName;
    Bitmap^ bmp, ^ bmpNew;
    int kx, kz;
    Color Px;

    std::cout << "insert image name:\n(possible moves example: ../../file.png)\n";
    std::cin >> fileName;

    bmp = gcnew Bitmap(gcnew String(fileName.c_str()), true);
    bmpNew = gcnew Bitmap(bmp->Width, bmp->Height);

    //===========================================================
    // new image creation
    //===========================================================

    int maskCenter = maskSize / 2;

    for (kz = 0; kz < bmp->Height; kz++)
    {
        for (kx = 0; kx < bmp->Width; kx++)
        {
            // black frames
            if (kx - maskCenter < 0 || kx + maskCenter > bmp->Width - 1
                || kz - maskCenter < 0 || kz + maskCenter > bmp->Height - 1)
                bmpNew->SetPixel(kx, kz, Px.FromArgb(0, 0, 0));
            else
                bmpNew->SetPixel(kx, kz, filtrationPixelRGB(bmp, maskCenter, kz, kx));
        }
    }

    bmpNew->Save("../filtrationRGB_new.png");
    return;
}

Color filtrationPixelRGB(Bitmap^ source, int maskCenter, int kz, int kx)
{
    Color Px;
    double normR, normG, normB;
    double stdDevR, stdDevG, stdDevB;           // standard deviation
    double varianceR, varianceG, varianceB;
    double avgR = 0, avgG = 0, avgB = 0;        // average
    int maskSize = ((2 * maskCenter + 1) * (2 * maskCenter + 1));
    int minR, minG, minB, maxR, maxG, maxB;

    // mask
    for (int i = 0; i <= 2 * maskCenter; i++)
    {
        minR = 0; maxR = 0;
        minG = 0; maxG = 0;
        minB = 0; maxB = 0;
        for (int j = 0; j <= 2 * maskCenter; j++)
        {
            Px = source->GetPixel(kx - maskCenter + i, kz - maskCenter + j);

            if (minR > Px.R) minR = Px.R;
            if (maxR < Px.R) maxR = Px.R;

            if (minG > Px.G) minG = Px.G;
            if (maxG < Px.G) maxG = Px.G;

            if (minB > Px.B) minB = Px.B;
            if (maxB < Px.B) maxB = Px.B;

            avgR += Px.R;
            avgG += Px.G;
            avgB += Px.B;

            varianceR += (Px.R * Px.R);
            varianceG += (Px.G * Px.G);
            varianceB += (Px.B * Px.B);
        }
    }

    avgR /= maskSize;
    avgG /= maskSize;
    avgB /= maskSize;

    varianceR /= maskSize;
    varianceG /= maskSize;
    varianceB /= maskSize;

    stdDevR = sqrt(varianceR - avgR * avgR);
    stdDevG = sqrt(varianceG - avgG * avgG);
    stdDevB = sqrt(varianceB - avgB * avgB);


    // norm
    stdDevR = (stdDevR - minR) / (maxR - minR) * 255;
    stdDevG = (stdDevG - minG) / (maxG - minG) * 255;
    stdDevB = (stdDevB - minB) / (maxB - minB) * 255;

    if (stdDevR < 0 || isnan(stdDevR)) stdDevR = 0;
    else if (stdDevR > 255) stdDevR = 255;

    if (stdDevG < 0 || isnan(stdDevG)) stdDevG = 0;
    else if (stdDevG > 255) stdDevG = 255;

    if (stdDevB < 0 || isnan(stdDevB)) stdDevB = 0;
    else if (stdDevB > 255) stdDevB = 255;

    return Px.FromArgb(stdDevR, stdDevG, stdDevB);
}

void filtrationMono()
{
    //===========================================================
    // mask creation
    //===========================================================

    int maskSize;
    std::cout << "insert mask size (int, odd number) -> enter:\t";
    std::cin >> maskSize;
    if (maskSize <= 0 || maskSize % 2 == 0)
    {
        std::cout << "blad! incorrect mask size!\nexiting...\n";
        exit(EXIT_FAILURE);
    }

    //===========================================================
    // loading image
    //===========================================================

    string fileName;
    Bitmap^ bmp, ^ bmpNew;
    int kx, kz;
    Color Px;

    std::cout << "insert image name:\n(possible moves example: ../../file.png)\n";
    std::cin >> fileName;

    bmp = gcnew Bitmap(gcnew String(fileName.c_str()), true);
    bmpNew = gcnew Bitmap(bmp->Width, bmp->Height);

    //===========================================================
    // new image creation
    //===========================================================

    int maskCenter = maskSize / 2;

    for (kz = 0; kz < bmp->Height; kz++)
    {
        for (kx = 0; kx < bmp->Width; kx++)
        {
            // black frames
            if (kx - maskCenter < 0 || kx + maskCenter > bmp->Width - 1
                || kz - maskCenter < 0 || kz + maskCenter > bmp->Height - 1)
                bmpNew->SetPixel(kx, kz, Px.FromArgb(0, 0, 0));
            else
                bmpNew->SetPixel(kx, kz, filtrationPixelMono(bmp, maskCenter, kz, kx));
        }
    }

    bmpNew->Save("../filtrationMono_new.png");
    return;
}

Color filtrationPixelMono(Bitmap^ source, int maskCenter, int kz, int kx)
{
    Color Px;
    double norm;
    double stdDev;
    double variance;
    double avg;
    int maskSize = ((2 * maskCenter + 1) * (2 * maskCenter + 1));
    int min, max;

    // mask
    for (int i = 0; i <= 2 * maskCenter; i++)
    {
        min = 0; max = 0;
        for (int j = 0; j <= 2 * maskCenter; j++)
        {
            Px = source->GetPixel(kx - maskCenter + i, kz - maskCenter + j);

            if (min > Px.R) min = Px.R;
            if (max < Px.R) max = Px.R;

            avg += Px.R;
            variance += (Px.R * Px.R);
        }
    }

    avg /= maskSize;

    variance /= maskSize;

    stdDev = sqrt(variance - avg * avg);


    // norm
    stdDev = (stdDev - min) / (max - min) * 255;

    if (stdDev < 0 || isnan(stdDev)) stdDev = 0;
    else if (stdDev > 255) stdDev = 255;

    return Px.FromArgb(stdDev, stdDev, stdDev);
}


// zad 3
void openingMono()
{
    //===========================================================
    // mask creation
    //===========================================================

    int radius, maskSize;
    std::cout << "insert mask radius (int) -> enter:\t";
    std::cin >> radius;
    if (radius <= 0)
    {
        std::cout << "error! incorrect mask radius!\nexiting...\n";
        exit(EXIT_FAILURE);
    }

    maskSize = radius * 2 - 1;

    int** mask = new int* [maskSize];
    for (int i = 0; i < maskSize; i++)
    {
        mask[i] = new int[maskSize];
        for (int j = 0; j < maskSize; j++)
        {
            // (x-a)^2 + (y-b)^2 = r^2
            if ((i - (radius - 1)) * (i - (radius - 1)) + (j - (radius - 1)) * (j - (radius - 1)) <= radius * radius)
                mask[i][j] = 1;
            else mask[i][j] = 0;
            //std::cout << mask[i][j] << "  ";
        }
        //std::cout << std::endl;
    }

    //===========================================================
    // loading image
    //===========================================================

    string fileName;
    Bitmap^ bmp, ^ bmpTmp, ^ bmpNew;
    int kx, kz;
    Color Px;

    std::cout << "insert image name:\n(possible moves example: ../../file.png)\n";
    std::cin >> fileName;

    bmp = gcnew Bitmap(gcnew String(fileName.c_str()), true);
    bmpTmp = gcnew Bitmap(bmp->Width, bmp->Height);
    bmpNew = gcnew Bitmap(bmp->Width, bmp->Height);

    //===========================================================
    // new image creation
    //===========================================================

    int maskCenter = radius - 1;

    int last1, last2;

    for (kz = 0; kz < bmp->Height; kz++)
    {
        for (kx = 0; kx < bmp->Width; kx++)
        {
            bmpTmp->SetPixel(kx, kz, openingPixelMono(bmp, mask, maskCenter, kz, kx, 1));
        }
    }

    for (kz = 0; kz < bmp->Height; kz++)
    {
        for (kx = 0; kx < bmp->Width; kx++)
        {
            bmpNew->SetPixel(kx, kz, openingPixelMono(bmpTmp, mask, maskCenter, kz, kx, 2));
            last1 = kx; last2 = kz;
        }
    }

    bmpNew->Save("../openingMono_new.png");

    delete[] mask;
    return;
}

Color openingPixelMono(Bitmap^ source, int** mask, int maskCenter, int kz, int kx, int type)
{
    Color Px;
    double avg;
    int maskSize = ((2 * maskCenter + 1) * (2 * maskCenter + 1));
    int value = 1;

    if (type == 1) value = 255;
    else if (type == 2) value = 0;

    for (int i = 0; i <= 2 * maskCenter; i++)
    {
        for (int j = 0; j <= 2 * maskCenter; j++)
        {
            if (mask[i][j] != 1) continue;

            if (((kx - maskCenter + i) < 0) || ((kz - maskCenter + j) < 0) || (kx - maskCenter + i) >= source->Width || (kz - maskCenter + j) >= source->Height)
                continue;

            Px = source->GetPixel(kx - maskCenter + i, kz - maskCenter + j);

            if ((type == 1) && (value > Px.R)) value = Px.R;         // erosion
            else if ((type == 2) && (value < Px.R)) value = Px.R;    // dilation
        }
    }
    return Px.FromArgb(value, value, value);
}

void openingLog()
{
    //===========================================================
    // mask creation
    //===========================================================

    int radius, maskSize;
    std::cout << "insert mask radius (int) -> enter:\t";
    std::cin >> radius;
    if (radius <= 0)
    {
        std::cout << "error! incorrect mask radius!\nexiting...\n";
        exit(EXIT_FAILURE);
    }

    maskSize = radius * 2 - 1;

    int** mask = new int* [maskSize];
    for (int i = 0; i < maskSize; i++)
    {
        mask[i] = new int[maskSize];
        for (int j = 0; j < maskSize; j++)
        {
            // (x-a)^2 + (y-b)^2 = r^2
            if ((i - (radius - 1)) * (i - (radius - 1)) + (j - (radius - 1)) * (j - (radius - 1)) <= radius * radius)
                mask[i][j] = 1;
            else mask[i][j] = 0;
        }
    }

    //===========================================================
    // loading image
    //===========================================================

    Bitmap^ bmp, ^ bmpTmp, ^ bmpNew;
    int kx, kz;
    Color Px;
    string fileName;

    std::cout << "insert image name:\n(possible moves example: ../../file.png)\n";
    std::cin >> fileName;

    bmp = gcnew Bitmap(gcnew String(fileName.c_str()), true);
    bmpTmp = gcnew Bitmap(bmp->Width, bmp->Height);
    bmpNew = gcnew Bitmap(bmp->Width, bmp->Height);

    //===========================================================
    // new image creation
    //===========================================================

    int maskCenter = radius - 1;

    int last1, last2;

    for (kz = 0; kz < bmp->Height; kz++)
    {
        for (kx = 0; kx < bmp->Width; kx++)
        {
            bmpTmp->SetPixel(kx, kz, openingPixelLog(bmp, mask, maskCenter, kz, kx, 1));
        }
    }

    for (kz = 0; kz < bmp->Height; kz++)
    {
        for (kx = 0; kx < bmp->Width; kx++)
        {
            bmpNew->SetPixel(kx, kz, openingPixelLog(bmpTmp, mask, maskCenter, kz, kx, 2));
            last1 = kx; last2 = kz;
        }
    }

    bmpNew->Save("../openingLog_new.png");

    delete[] mask;
    return;
}

Color openingPixelLog(Bitmap^ source, int** mask, int maskCenter, int kz, int kx, int type)
{
    Color Px;
    double avg;
    int maskSize = ((2 * maskCenter + 1) * (2 * maskCenter + 1));
    int value = 1;

    if (type == 1) value = 255;
    else if (type == 2) value = 0;

    for (int i = 0; i <= 2 * maskCenter; i++)
    {
        for (int j = 0; j <= 2 * maskCenter; j++)
        {
            if (mask[i][j] != 1) continue;

            if (((kx - maskCenter + i) < 0) || ((kz - maskCenter + j) < 0) || (kx - maskCenter + i) >= source->Width || (kz - maskCenter + j) >= source->Height)
                continue;

            Px = source->GetPixel(kx - maskCenter + i, kz - maskCenter + j);

            if ((type == 1) && (value > Px.R)) value = Px.R;         // erosion
            else if ((type == 2) && (value < Px.R)) value = Px.R;    // dilation
        }
    }
    return Px.FromArgb(value, value, value);
}

// zad 4
void labeling()
{
    //===========================================================
    // loading image
    //===========================================================

    string fileName;

    std::cout << "insert image name:\n(possible moves example: ../../file.png)\n";
    std::cin >> fileName;

    Bitmap^ bmp, ^ bmpTmp;
    int kx, kz;
    Color Px;

    bmp = gcnew Bitmap(gcnew String(fileName.c_str()), true);
    bmpTmp = gcnew Bitmap(bmp->Width, bmp->Height);

    //===========================================================
    // number of elements calculation
    //===========================================================

    int noElem = 0;

    for (kz = 0; kz < bmp->Height; kz++)
    {
        for (kx = 0; kx < bmp->Width; kx++)
        {
            if ((bmp->GetPixel(kx, kz).R != 0) && (bmpTmp->GetPixel(kx, kz).R == 0))
                recursiveFinder(kx, kz, ++noElem, bmp, bmpTmp);
        }
    }

    std::cout << "\nnumber of elements:\n" << noElem;

    //===========================================================
    // .txt file saving
    //===========================================================

    ofstream myfile;
    myfile.open("../no_elem.txt");
    myfile << noElem << "\n";
    myfile.close();

    return;
}

void recursiveFinder(int x, int z, int current_label, Bitmap^ bmp, Bitmap^ bmpTmp)
{
    if (x < 0 || x == bmpTmp->Width) return;
    if (z < 0 || z == bmpTmp->Height) return;
    if ((bmp->GetPixel(x, z).R == 0) || (bmpTmp->GetPixel(x, z).R != 0)) return;

    Color Px;
    const int dx[] = { +1, 0, -1, 0, -1, 1, 1, -1 };
    const int dz[] = { 0, +1, 0, -1, -1, 1, -1, 1 };

    bmpTmp->SetPixel(x, z, Px.FromArgb(current_label, current_label, current_label));

    for (int direction = 0; direction < 8; ++direction)
        recursiveFinder(x + dx[direction], z + dz[direction], current_label, bmp, bmpTmp);
}