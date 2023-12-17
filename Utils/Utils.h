//
// Created by joelh on 20/10/2023.
//

#ifndef MRMP_IMPLEMENTATION_UTILS_H
#define MRMP_IMPLEMENTATION_UTILS_H

#include "../mytypedefs.h"

class Utils //TODO: Split into named utils
{
public:
    static Polygon_2 generateBoundingBox(const Bbox_2& bbox);
    static Polygon_2 generateRobot();
    static Polygon_2 generateRobotAura();
    static Polygon_2 generateRobotAura(const Point_2& center);
    //static Polygon_2 generateRobotAura(const Point_2& center, double sizeOfAura);
    static Point_2 getLowestIntersectionPoint(const Segment_2& verticalRay, const Polygon_2& polyToIntersect);
    static double getSmallestDistanceToOrigin(const Point_2& seg1Origin, const Segment_2 & seg1, const Segment_2 & seg2);
    static Point_2 getClosestIntersectionPointToOrigin(const Point_2& seg1Origin, const Segment_2 & seg1, const Segment_2 & seg2);

    static double getRuntimeBound(const int workspaceComplexity, const int nmbrStartConfs);

    static bool haveOverlappingEdges(const Polygon_2& poly1, const Polygon_2& poly2);
    static bool aurasOverlap(const Point_2& p1, const Point_2& p2);

    static double getPercentageAlongSegment(const Point_2 pointOnSegment, Segment_2 segment);
};


#endif//MRMP_IMPLEMENTATION_UTILS_H
