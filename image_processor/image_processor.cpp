#include <iostream>
#include <fstream>

#include "image.h"
#include "filter.h"
#include "bmpfile.h"
#include "filtermanager.h"

//#define VERBOSE    // Вывод дополнительной информации



void print_usage()
{
    std::cout << "image_processor is an utility for filtering bitmap images" << std::endl;
    std::cout << std::endl;
    std::cout << "Usage: image_processor {input bitmap path} {output bitmap path} [-{filter name} [param 1] [param 2] ...] [-{filter name} [param 1] [param 2] ...] ..." << std::endl;
    std::cout << "Available filters" << std::endl;
    FilterManager::listFilters();
    std::cout << std::endl;
}


int main(int argc, char** argv)
{
    if (argc < 3)
    {
        print_usage();
        return 0;
    }
    std::string input_file = argv[1];
    std::string output_file = argv[2];

    std::string filter_name;
    std::vector<std::string> filter_params;

    Image input;                  // Входное изображение
    Image output;                 // Выходное изображение
    bool  currSource = false;     // False - input - входное, output - выходное; True - output - входное, input - выходное

#ifdef VERBOSE
    std::cout << "Loading image " << input_file << "...";
#endif
    std::ifstream file_in(input_file, std::ios::in | std::ios::binary);
    bool result = LoadBitmap(file_in, input);
    file_in.close();
    if (!result)
    {
#ifdef VERBOSE
        std::cout << "FAIL" << std::endl;
#endif
        return 0;
    }
#ifdef VERBOSE
    std::cout << "OK" << std::endl;
#endif

    for (int i=3; i<=argc;i++)
    {
        std::string curr;
        if (i == argc) curr = "-";
        else           curr = argv[i];
        if (!curr.size()) continue;
        if (curr[0] == '-')   // Filter
        {
            if (!filter_name.empty())
            {
#ifdef VERBOSE
                std::cout << "Using filter " << filter_name << "...";
#endif
                bool result = FilterManager::doFilter(currSource ? output : input, currSource ? input : output, filter_name, filter_params);
#ifdef VERBOSE
                std::cout << (result ? "OK" : "FAIL") << std::endl;
#endif
                if (result) currSource = !currSource;
            }
                filter_params.clear();
                filter_name = curr.substr(1);
        }
        else                 // Parameters
        {
            if (!filter_name.empty())
                filter_params.push_back(curr);
        }
    }

#ifdef VERBOSE
    std::cout << "Saving image " << output_file << "...";
#endif
    std::ofstream file_out(output_file, std::ios::out | std::ios::binary);
    result = SaveBitmap(file_out, currSource ? output : input);
    file_out.close();
    if (!result)
    {
#ifdef VERBOSE
        std::cout << "FAIL" << std::endl;
#endif
        return 0;
    }
#ifdef VERBOSE
    std::cout << "OK" << std::endl;
#endif
    return 0;
}
