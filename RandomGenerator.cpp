//
// Created by joelh on 17/10/2023.
//

#include "RandomGenerator.h"

RandomGenerator::RandomGenerator() : rand(CGAL::get_default_random()) {}

int RandomGenerator::getRandomInt(int min, int max) {
  return rand.get_int(min, max);
}

double RandomGenerator::getRandomDouble(double min, double max) {
  return rand.get_double(min, max);
}