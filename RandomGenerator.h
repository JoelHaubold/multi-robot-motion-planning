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
  static int getRandomInt(int min, int max);
  static double getRandomDouble(double min, double max);

private:
  static CGAL::Random cgalRandom;
};


#endif//MRMP_IMPLEMENTATION_RANDOMGENERATOR_H
