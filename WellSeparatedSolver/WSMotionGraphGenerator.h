//
// Created by joelh on 23/10/2023.
//

#ifndef MRMP_IMPLEMENTATION_WSMOTIONGRAPHGENERATOR_H
#define MRMP_IMPLEMENTATION_WSMOTIONGRAPHGENERATOR_H

#include "../mytypedefs.h"

class WSMotionGraphGenerator
{
public:
    static void WSMotionGraphGenerator::fillMotionGraph(Motion_Graph& motionGraph, const Polygon_set_2 &fStarSet, const std::vector<Point_2>& startConfs, const std::vector<Point_2>& targetConfs);

private:
    static void getMGForFStarComponent(Motion_Graph& motionGraph, const Polygon_wh_2 &fStarSet, const std::vector<Vertex>& relevantVertices);
    static MGEdgeProperty getRayEdgeProperty(const Point_2& rayShooter, const Point_2& hitVertex, const Polygon_wh_2& fStarComponent);
    static bool intersectsAuraPredicate(const Point_2& pointToCheck, const Point_2& referencePoint, const K::FT& yIntersection);
    static Point_2 getRepPoint(const Polygon_2& outerBoundary, const Point_2& vertexLocation);
    static Point_2 getRayIntersectionWithFreeSpace(const Point_2& shooterLocation, const Polygon_2& outerBoundary, const K::FT& rayYLimit);
};


#endif//MRMP_IMPLEMENTATION_WSMOTIONGRAPHGENERATOR_H
