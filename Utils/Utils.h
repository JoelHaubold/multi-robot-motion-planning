//
// Created by joelh on 20/10/2023.
//

#ifndef MRMP_IMPLEMENTATION_UTILS_H
#define MRMP_IMPLEMENTATION_UTILS_H

#include "../mytypedefs.h"

class Utils
{
    public:
      static Polygon_2 generateBoundingBox(const Bbox_2& bbox);
      static Polygon_2 generateRobot();
      static Polygon_2 generateRobotAura();
      static Polygon_2 generateRobotAura(const Point_2& center);
      static Polygon_2 generateRobotAura(const Point_2& center, double sizeOfAura);
};


#endif//MRMP_IMPLEMENTATION_UTILS_H
