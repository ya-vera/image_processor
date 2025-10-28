#include <iostream>
#include "filtermanager.h"

Filter *FilterManager::getFilter(std::string name)
{
    if (name == FilterCrop::getName())
        return new FilterCrop();
    else
    if (name == FilterGrayscale::getName())
        return new FilterGrayscale();
    else
    if (name == FilterNegative::getName())
        return new FilterNegative();
    else
    if (name == FilterSharpening::getName())
        return new FilterSharpening();
    else
    if (name == FilterEdge::getName())
        return new FilterEdge();
    else
    if (name == FilterGaussian::getName())
        return new FilterGaussian();
    else
        return NULL;
}


void FilterManager::listFilters()
{
    std::cout << FilterCrop::getDescription() << std::endl;
    std::cout << FilterGrayscale::getDescription() << std::endl;
    std::cout << FilterNegative::getDescription() << std::endl;
    std::cout << FilterSharpening::getDescription() << std::endl;
    std::cout << FilterEdge::getDescription() << std::endl;
    std::cout << FilterGaussian::getDescription() << std::endl;
}

bool FilterManager::doFilter(const Image &src, Image &dest, std::string name, std::vector<std::string> parameters)
{
    Filter *filter = getFilter(name);
    if (filter == NULL) return false;   // Filter name not found
    std::vector<int> intparams;
    std::vector<float> floatparams;
    bool result = false;
    if (parameters.size() >= (unsigned int)filter->getParamCount())
    {
        for (unsigned int i=0;i<parameters.size();i++)
        {
            if (filter->paramIsFloat())
                floatparams.push_back(atof(parameters[i].c_str()));
            else
                intparams.push_back(atoi(parameters[i].c_str()));
        }
        result = filter->doFilter(src, dest, intparams, floatparams);
    }
    delete filter;
    return result;
}
