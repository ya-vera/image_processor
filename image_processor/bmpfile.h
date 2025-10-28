#ifndef BMPFILE_H
#define BMPFILE_H

// Работа с файлами BMP

#include <fstream>
#include "image.h"

bool LoadBitmap(std::ifstream &file, Image &image);  // Прочитать BMP
bool SaveBitmap(std::ofstream &file, Image &image);  // Записать BMP

#endif // BMPFILE_H
