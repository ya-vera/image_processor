#ifndef FILTER_H
#define FILTER_H

#include <string>
#include "image.h"

// Фильтры

// Абстрактный класс для фильтра
class Filter {
public:
    virtual int  getParamCount() = 0;     // Число требуемых параметров для фильтра
    virtual bool paramIsFloat() = 0;      // Тип требуемых параметров (false - int, true - float)
    virtual bool doFilter(const Image &input, Image &output, std::vector<int> intParams, std::vector<float> floatParams) = 0;  // Запустить фильтр
    virtual ~Filter() { };    // Виртуальный деструктор
};

// Фильтр обрезки.
class FilterCrop : public Filter
{
public:
    static std::string getName() { return "crop"; }
    static std::string getDescription() { return "-crop width height     Crops image to specified width and height"; }
    int  getParamCount() { return 2; }
    bool paramIsFloat() { return false; }
    bool doFilter(const Image &input, Image &output, std::vector<int> intParams, std::vector<float> floatParams);
};

// Фильтр преобразования в оттенки серого
class FilterGrayscale : public Filter
{
public:
    static std::string getName() { return "gs"; }
    static std::string getDescription() { return "-gs                    Converts image to grayscale"; }
    int  getParamCount() { return 0; }
    bool paramIsFloat() { return false; }
    bool doFilter(const Image &input, Image &output, std::vector<int> intParams, std::vector<float> floatParams);
};

class FilterNegative : public Filter
{
public:
    static std::string getName() { return "neg"; }
    static std::string getDescription() { return "-neg                   Converts image to negative"; }
    int  getParamCount() { return 0; }
    bool paramIsFloat() { return false; }
    bool doFilter(const Image &input, Image &output, std::vector<int> intParams, std::vector<float> floatParams);
};

// Фильтр, основанный на матрице - абстрактный класс
class FilterMatrix : public Filter
{
public:
    std::vector<std::vector<int>> Matrix;
    virtual bool initMatrix(std::vector<int> intParams, std::vector<float> floatParams) = 0;
    virtual bool doFilter(const Image &input, Image &output, std::vector<int> intParams, std::vector<float> floatParams);
};

// Фильтр повышения резкости
class FilterSharpening : public FilterMatrix
{
public:
    static std::string getName() { return "sharp"; }
    static std::string getDescription() { return "-sharp                 Performs image sharpening"; }
    int  getParamCount() { return 0; }
    bool paramIsFloat() { return false; }
    bool initMatrix(std::vector<int> intParams, std::vector<float> floatParams);
};

// Фильт определения краев
class FilterEdge : public FilterMatrix
{
public:
    static std::string getName() { return "edge"; }
    static std::string getDescription() { return "-edge threshold        Performs edge detection"; }
    int  getParamCount() { return 1; }
    bool paramIsFloat() { return true; }
    bool initMatrix(std::vector<int> intParams, std::vector<float> floatParams);
    bool doFilter(const Image &input, Image &output, std::vector<int> intParams, std::vector<float> floatParams);
};

// Фильтр гауссов (не сделан)
class FilterGaussian : public Filter
{
public:
    static std::string getName() { return "blur"; }
    static std::string getDescription() { return "-blur sigma            Performs Gaussian blur"; }
    int  getParamCount() { return 1; }
    bool paramIsFloat() { return true; }
    bool doFilter(const Image &input, Image &output, std::vector<int> intParams, std::vector<float> floatParams);
};


#endif // FILTER_H
