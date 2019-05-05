#include <stdio.h>
#include <vector>

int main()
{
    int size = 10;
    std::vector<float> vec{0.0, 0.1, 0.2};

    for (int i = 0; i < vec.size(); i++)
    {
        vec.push_back(vec.at(i) + 0.1);
        size++;

        printf("i: %d size: %lu value: %.4f\n", i, vec.size(), vec.at(i));
    }
}