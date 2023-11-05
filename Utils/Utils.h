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
      //static Polygon_2 generateRobotAura(const Point_2& center, double sizeOfAura);
      static Point_2 getLowestIntersectionPoint(const Segment_2& verticalRay, const Polygon_2& polyToIntersect);
      static double getClosestIntersectionPointToOrigin(const Point_2& seg1Origin, const Segment_2 & seg1, const Segment_2 & seg2);

      static double getRuntimeBound(const int workspaceComplexity, const int nmbrStartConfs);
};


#endif//MRMP_IMPLEMENTATION_UTILS_H
