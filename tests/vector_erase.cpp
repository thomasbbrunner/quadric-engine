#include <vector>
#include <stdio.h>
#include "print.h"

int main()
{
    std::vector<float> vec{0.1, 0.2, 0.3, 0.4, 0.5};

    // Test 1
    vec.erase(vec.begin() + 3);

    for (unsigned int i = 0; i < vec.size(); i++)
    {
        printf("%.4f ", vec.at(i));
    }

    printf("\n\n");

    vec.clear();

    // Test 2
    vec = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};

    for (unsigned int i = 0; i < vec.size(); i++)
    {
        for (unsigned int j = 0; j < vec.size(); j++)
        {
            if (j == vec.size() - 1)
            {
                vec.erase(vec.begin() + j);
                Print::array(vec);
            }
            printf("loo\n");
        }
    }

    return 0;
}