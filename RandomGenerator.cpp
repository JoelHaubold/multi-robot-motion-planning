//
// Created by joelh on 17/10/2023.
//

#include "RandomGenerator.h"
#include <random>

//CGAL::Random RandomGenerator::cgalRandom = CGAL::get_default_random();

std::random_device rd;// Use a random device to get a seed.
unsigned int seed = rd();
std::mt19937 rng(seed);

unsigned int RandomGenerator::getSeed() {
    return seed;
}
void RandomGenerator::setSeed(unsigned int n_seed) {
    rng = std::mt19937(n_seed);
    seed = n_seed;
}

int RandomGenerator::getRandomInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

double RandomGenerator::getRandomDouble(double min, double max) {
    std::uniform_real_distribution<double> dist(min, max);
    return dist(rng);
}
bool RandomGenerator::getRandomBoolean()
{
    return static_cast<bool>(getRandomInt(0,1));
}
