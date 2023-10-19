//
// Created by joelh on 17/10/2023.
//

#include "RandomGenerator.h"

CGAL::Random RandomGenerator::cgalRandom = CGAL::get_default_random();

RandomGenerator::RandomGenerator()  = default;

int RandomGenerator::getRandomInt(int min, int max) {
  return RandomGenerator::cgalRandom.get_int(min, max);
}

double RandomGenerator::getRandomDouble(double min, double max) {
  return cgalRandom.get_double(min, max);
}