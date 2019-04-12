#ifndef MATH_H
#define MATH_H

#include <common.h>

class Math
{
  public:
    static void add_to_each(std::vector<unsigned int> *vec, unsigned int value)
    {
        for (unsigned int i = 0; i < vec->size(); i++)
        {
            vec->at(i) += value;
        }
    }
};

#endif