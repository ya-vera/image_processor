#include "image.h"
#include <cmath>

/*
 * Элементы класса Pixel
 */

Pixel::Pixel()               // Создание класса
{
    for (int i=0;i<PIXEL_COMP;i++)
        Value[i] = 0;
}

void Pixel::saturate()      // Насыщение - значения < 0 приравниваем к 0, значения >MAX приравниваем к MAX
{
    for (int i=0;i<PIXEL_COMP;i++)
    {
        if (Value[i] < 0) Value[i] = 0;
        if (Value[i] > getMax()) Value[i] = getMax();
    }
}

int Pixel::getBmpVal(int Index)   // Получить значение 0-255 для записи в BMP
{
    int val = std::round(Value[Index]*255/getMax());
    if (val < 0) val = 0;
    if (val > 255) val = 255;
    return val;
}

void Pixel::setBmpVal(int Index, int Val)    // Установить значение 0-255 из BMP
{
    Value[Index] = static_cast<float>(Val) * getMax() / 255;
}

// Сложение двух пикселей
Pixel & operator +(Pixel &a1, const Pixel &a2)
{
    for (int i=0;i<PIXEL_COMP;i++)
        a1.Value[i] += a2.Value[i];
    return a1;
}

// Умножение пикселя на коэффициент
Pixel operator *(const Pixel &a1, const float Factor)
{
    Pixel a2;
    for (int i=0;i<PIXEL_COMP;i++)
        a2.Value[i] = a1.Value[i] * Factor;
    return a2;
}



/*
 *
 * Элементы класса Image
 *
 */

Image::Image()         // Создать пустую картинку
{
    SetSize(0,0);
}

Image::Image(int width, int height)     // Создать картинку размера width x height
{
    SetSize(width, height);
}

void Image::SetSize(int width, int height)    // Изменить размер картинки, установив его width x height
{
    if (width < 0) Width = 0;
    else           Width = width;
    if (height < 0) Height = 0;
    else           Height = height;
    Pixels.resize(Width);
    for (int i=0;i<Width;i++)
        Pixels[i].resize(Height);
}

