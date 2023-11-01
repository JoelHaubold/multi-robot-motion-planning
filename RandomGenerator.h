//
// Created by joelh on 17/10/2023.
//

#ifndef MRMP_IMPLEMENTATION_RANDOMGENERATOR_H
#define MRMP_IMPLEMENTATION_RANDOMGENERATOR_H

#include <CGAL/Random.h>
#include <CGAL/Polygon_2.h>

class RandomGenerator {
public:
    static int getRandomInt(int min, int max);
    static double getRandomDouble(double min, double max);
    static bool getRandomBoolean();
    static unsigned int getSeed();
    static void setSeed(unsigned int seed);

private:

};


#endif//MRMP_IMPLEMENTATION_RANDOMGENERATOR_H
