//
// Created by joelh on 17/10/2023.
//

#ifndef MRMP_IMPLEMENTATION_RANDOMGENERATOR_H
#define MRMP_IMPLEMENTATION_RANDOMGENERATOR_H

#include <CGAL/Random.h>
#include <CGAL/Polygon_2.h>

class RandomGenerator {
public:
  RandomGenerator();
  int getRandomInt(int min, int max);
  double getRandomDouble(double min, double max);

private:
  CGAL::Random rand;
};


#endif//MRMP_IMPLEMENTATION_RANDOMGENERATOR_H
