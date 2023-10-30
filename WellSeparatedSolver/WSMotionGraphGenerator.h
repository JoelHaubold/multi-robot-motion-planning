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
    static void generateListEdges(Motion_Graph& motionGraph, const FStarComponent& fStarComp , const std::list<Point_2>& vertices, const std::map<int, RepPoint>& polyVertex2RepPoint, const std::map<int, std::vector<RepPoint>>& polyVertex2RayRepPoints);
    static void getMGForFStarComponent(Motion_Graph& motionGraph, const FStarComponent &fStarSet, const std::vector<Vertex>& relevantVertices);
    static bool intersectsAuraPredicate(const Point_2& pointToCheck, const Point_2& referencePoint, const K::FT& yIntersection);
    static RepPoint getRepPoint(const Polygon_2& outerBoundary, const Point_2& vertexLocation, const Vertex& forVertex);
    static RepPoint getRayIntersectionWithFreeSpace(const Point_2& shooterLocation, const Polygon_2& outerBoundary, const K::FT& rayYLimit, const Vertex& forVertex);
    static std::vector<Segment_2> getAuraPath(const Point_2& from, const Point_2& to, const Polygon_2& freeSpaceBoundary);
    static MGEdgeProperty getRayEdgeProperty(const Point_2& rayShooter, const std::string& hitVertexId, const Point_2& hitVertex, const Polygon_2& freeSpace);
    static MGEdgeProperty getListEdgeProperty(const Motion_Graph& mg, const RepPoint& vertex1Rep, const RepPoint& vertex2Rep, const Polygon_2& freeSpaceBoundary, const Polygon_2& fStarBoundary);

};


#endif//MRMP_IMPLEMENTATION_WSMOTIONGRAPHGENERATOR_H
