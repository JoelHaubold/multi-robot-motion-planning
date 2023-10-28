//
// Created by joelh on 23/10/2023.
//

#ifndef MRMP_IMPLEMENTATION_WSMOTIONGRAPHGENERATOR_H
#define MRMP_IMPLEMENTATION_WSMOTIONGRAPHGENERATOR_H

#include "../mytypedefs.h"

struct RepPoint{
    Point_2 location;
    int polyVertexNmbr;
    Vertex forVertex;
};

class WSMotionGraphGenerator
{
public:
    static void insertEdges(Motion_Graph& motionGraph, const std::vector<FStarComponent>& fStarComponents, const MGIdToVertex& id2Vertex);
    static MGIdToVertex insertVertices(Motion_Graph& motionGraph, const STConfigurations& stConfigurations);

private:
    static void generateListEdges(Motion_Graph& motionGraph, const std::list<Point_2>& vertices, const std::map<int, RepPoint>& polyVertex2RepPoint, const std::map<int, std::vector<RepPoint>>& polyVertex2RayRepPoints);
    static void getMGForFStarComponent(Motion_Graph& motionGraph, const Polygon_wh_2 &fStarSet, const std::vector<Vertex>& relevantVertices);
    static MGEdgeProperty getRayEdgeProperty(const Point_2& rayShooter, const Point_2& hitVertex, const Polygon_wh_2& fStarComponent);
    static bool intersectsAuraPredicate(const Point_2& pointToCheck, const Point_2& referencePoint, const K::FT& yIntersection);
    static RepPoint getRepPoint(const Polygon_2& outerBoundary, const Point_2& vertexLocation, const Vertex& forVertex);
    static RepPoint getRayIntersectionWithFreeSpace(const Point_2& shooterLocation, const Polygon_2& outerBoundary, const K::FT& rayYLimit, const Vertex& forVertex);
};


#endif//MRMP_IMPLEMENTATION_WSMOTIONGRAPHGENERATOR_H
