//
// Created by joelh on 23/10/2023.
//

#ifndef MRMP_IMPLEMENTATION_WSMOTIONGRAPHGENERATOR_H
#define MRMP_IMPLEMENTATION_WSMOTIONGRAPHGENERATOR_H

#include "../mytypedefs.h"

class WSMotionGraphGenerator
{
public:
    static std::unordered_map<std::string, Motion_Graph> getMotionGraphs(const std::vector<FreeSpaceComponent>& fSpaceComps, const std::vector<FStarComponent>& fStarComps);

private:
    static void generateListEdges(Motion_Graph& motionGraph, const FStarComponent& fStarComp , const std::vector<RepPoint>& unsortedRepPoints);
    static void getMGForFStarComponent(Motion_Graph& motionGraph, const FStarComponent &fStarSet, const std::vector<MGVertex>& relevantVertices);
    static bool intersectsAuraPredicate(const Point_2& pointToCheck, const Point_2& referencePoint, const K::FT& yIntersection);
    static RepPoint getBoundaryRepPoint(const Polygon_2& fStarBoundary, const Polygon_2& freeSpaceBoundary, const Point_2& vertexLocation, const MGVertex & forVertex);
    static RepPoint getRayIntersectionWithFreeSpace(const Point_2& shooterLocation, const Polygon_2& outerBoundary, const K::FT& rayYLimit, const MGVertex & forVertex);
    static std::vector<Segment_2> getAuraPath(const Point_2& from, const Point_2& to, const Polygon_2& freeSpaceBoundary);
    static MGEdgeProperty getRayEdgeProperty(const Point_2& rayShooter, const std::string& hitVertexId, const Point_2& hitVertex, const Polygon_2& freeSpace);
    static MGEdgeProperty getListEdgeProperty(const Motion_Graph& mg, const RepPoint& vertex1Rep, const RepPoint& vertex2Rep, const Polygon_2& freeSpaceBoundary, const Polygon_2& fStarBoundary);
    static void insertEdges(Motion_Graph& motionGraph, const FStarComponent& fStarComponent, const STConfId2MGVertex & id2Vertex);
};


#endif//MRMP_IMPLEMENTATION_WSMOTIONGRAPHGENERATOR_H
