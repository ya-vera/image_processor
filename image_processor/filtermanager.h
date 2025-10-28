#ifndef FILTERMANAGER_H
#define FILTERMANAGER_H

// Менеджер фильтров
#include "filter.h"

class FilterManager
{
private:
    static Filter *getFilter(std::string name); //Вернуть фильтр по имени

public:
    static void listFilters();  // Распечатать подсказку по фильтрам
    static bool doFilter(const Image &src, Image &dest, std::string name, std::vector<std::string> parameters);  // Выполнить фильтрацию фильтром name с параметрами parameters
};

#endif // FILTERMANAGER_H
