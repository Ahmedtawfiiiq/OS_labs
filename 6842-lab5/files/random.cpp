#include "random.hpp"

float priceGenerator(float mean, float standard_deviation)
{
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    normal_distribution<float> distribution(mean, standard_deviation);

    return distribution(generator);
}