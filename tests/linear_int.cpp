#include <vector>
#include <algorithm>
#include "common.h"
#include "print.h"

int main()
{
    std::vector<float> range{0, 2, 4, 6, 8, 100};
    std::vector<float> data{1, 4, 10, 50, 100, -69.6969696969};

    Math::linear_interpolation(100, range, data);

    return 0;
}