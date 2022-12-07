#include <iostream>
#include <random>
#include <chrono>

using namespace std;

int main()
{
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    normal_distribution<double> distribution(0.0, 1.0);

    float item = distribution(generator);
    cout << item;
    return 0;
}