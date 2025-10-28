#ifndef IMAGE_H
#define IMAGE_H

// Работа с изображениями

#include <vector>
#include <cmath>

#define PIXEL_COMP 3    // Число компонентов в описании пикселя

#define IDX_R 0         // Индексы компонентов пикселя
#define IDX_G 1
#define IDX_B 2

// Хранение цветовых компонентов пикселя
class Pixel {
public:
    float Value[3];  // Значение

    Pixel();               // Создание класса
    void saturate();      // Насыщение - значения < 0 приравниваем к 0, значения >MAX приравниваем к MAX
    int getBmpVal(int Index);   // Получить значение 0-255 для записи в BMP
    void setBmpVal(int Index, int Val);    // Установить значение 0-255 из BMP
    static float getMax() { return 1.0f; }
};

Pixel & operator +(Pixel &a1, const Pixel &a2); // Сложение двух пикселей
Pixel operator *(const Pixel &a1, const float Factor); // Умножение пикселя на коэффициент

// Изображение размера Width x Height
class Image {
public:
    std::vector<std::vector<Pixel>> Pixels;
    int                             Width;
    int                             Height;
public:

    Image();         // Создать пустую картинку
    Image(int width, int height);     // Создать картинку размера width x height
    void SetSize(int width, int height);    // Изменить размер картинки, установив его width x height
};

#endif // IMAGE_H
