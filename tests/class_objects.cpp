#include <stdio.h>
#include <vector>
#include <iostream>

class Print
{
  public:
    static void array(std::vector<unsigned int> arr)
    {
        for (unsigned int i = 0; i < arr.size(); i++)
        {
            printf("%u ", arr.at(i));
            printf("\n");
        }
        printf("\n");
    }
};

class Parent
{
  public:
    std::vector<unsigned int> vec1{88, 89, 90};
    std::vector<unsigned int> vec2;
};

class Child : public Parent
{
  public:
    Child(unsigned int i)
    {
        std::vector<unsigned int> vec1{i, i + 1, i + 2, i + 3};
        vec2 = {i, i + 1, i + 2, i + 3, i + 4};
    }
};

int main()
{
    Child child(9);

    Print::array(child.vec1);
    Print::array(child.vec2);

    return 0;
}