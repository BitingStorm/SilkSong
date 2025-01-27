#include "Math.h"



std::random_device Math::rd;
std::mt19937 Math::gen(rd());

int Math::RandInt(int lower, int upper)
{
    if(lower > upper)
    {
        int temp = lower;
        lower = upper;
        upper = temp;
    }
    std::uniform_int_distribution<int> dis(lower, upper);
    return dis(gen);
}

double Math::RandReal(double lower, double upper)
{
    if(lower > upper)
    {
        double temp = lower;
        lower = upper;
        upper = temp;
    }
    std::uniform_real_distribution<double> dis(lower, upper);
    return dis(gen);
}

double Math::RandPerc()
{
    return std::generate_canonical<double, 10>(gen);
}
