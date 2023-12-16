//
// Created by joelh on 13/11/2023.
//

#include "ChrMotionGraphGenerator.h"
#include "../SharedSolverComponents/MGCreationHelper.h"
#include "../Utils/Utils.h"

//TODO: Maybe don't subtract start auras from free space as well <-- Already handling this by potentially having multiple representatives per st conf
//TODO: Calculate intersections with aura by finding intersection with polygon sides [Iterate through sides * each aura sides] Add rep points on the go
std::unordered_map<std::string, Motion_Graph> ChrMotionGraphGenerator::getMotionGraphs(const std::vector<FreeSpaceComponent> &fSpaceComps, const std::vector<FStarComponent> &fStarComps, const std::vector<Tq> tqs)
{
    std::unordered_map<std::string, Motion_Graph> fSpaceId2motionGraph;
    STConfId2MGVertex stIds2Vertices;
    for(const auto& fSpaceComp : fSpaceComps) {
        if(fSpaceComp.startConfigurations.empty()) {
            continue; //Irrelevant free space component
        }
        //std::cout << "Vertices for" << fSpaceComp.freeSpaceId << std::endl;
        Motion_Graph& mg = fSpaceId2motionGraph[fSpaceComp.freeSpaceId];
        MGCreationHelper::insertVertices(mg, fSpaceComp.startConfigurations, fSpaceComp.targetConfigurations, stIds2Vertices);
    }

    for(const auto& fStarComp : fStarComps){
        //std::cout << "Edges for" << fStarComp.parent.freeSpaceId << std::endl;
        Motion_Graph& mg = fSpaceId2motionGraph[fStarComp.parent.freeSpaceId];
        insertEdges(mg, fStarComp, stIds2Vertices);
    }
    return fSpaceId2motionGraph;
}

void ChrMotionGraphGenerator::insertEdges(Motion_Graph& motionGraph, const FStarComponent& fStarComponent, const STConfId2MGVertex & id2Vertex)//, const Polygon_2& freeSpaceSet)
{
    std::vector<MGVertex> relevantVertices;
    for(const auto& sConf : fStarComponent.adjacentSConfs) {
        relevantVertices.push_back(id2Vertex.find(sConf.id)->second);
    }
    for(const auto& tConf : fStarComponent.adjacentTConfs) {
        relevantVertices.push_back(id2Vertex.find(tConf.id)->second);
    }
    getMGForFStarComponent(motionGraph, fStarComponent, relevantVertices);

}

void ChrMotionGraphGenerator::getMGForFStarComponent(Motion_Graph& motionGraph, const FStarComponent &fStarComponent, const std::vector<MGVertex>& relevantVertices)
{
    //TODO: Don't generate guaranteed edge if blocking edge should be generated [Filter if part of blocking area?]
    //std::vector<std::tuple<Point_2, MGVertex>> repPoints;

//    std::map<int, RepPoint> polyVertex2RepPoint;
//    std::map<int, std::vector<RepPoint>> polyVertex2RayRepPoints;
//
//    const Polygon_2& outerBoundary = fStarComponent.fStarPolygon.outer_boundary();
//    const auto boundaryYMax = outerBoundary.bbox().ymax();
//
//    for(const MGVertex & relevantVertex : relevantVertices) {
//        MGVertexProperty vertexProps = motionGraph[relevantVertex];
//        Point_2 vertexLocation = vertexProps.location;
//        if(outerBoundary.has_on_unbounded_side(vertexLocation)) { //StartConf on edge
//            //TODO: Check if sConf location is overlapped by target conf aura -> Direct edge to that conf
//            //If not: Check if we can construct path to non overlapped point on aura edge
//            //If yes insert rep point. If not: all possible rep point paths are overlapped. Find overlapping t closest to sConf location and construct path as direct edge (Always possible)
//            RepPoint repPoint = getRepPoint(outerBoundary, vertexLocation, relevantVertex);
//            polyVertex2RepPoint[repPoint.polySegmentIndex] = repPoint;
//        } else { //TODO: Also need to handle target conf on edge
//            //TODO: Pick set of representative points instead of single for start pos on edge
//            //TODO: Remove duplicates on repPoint edge forming
//            //Shoot ray upwards
//            RepPoint rayIntersection = getRayIntersectionWithFreeSpace(vertexLocation, outerBoundary, boundaryYMax, relevantVertex);
//            std::vector<MGVertex> interceptingAuras;
//            std::copy_if(relevantVertices.begin(), relevantVertices.end(), std::back_inserter(interceptingAuras),
//                         [& vertexLocation, &rayIntersection, &motionGraph](const MGVertex & intersecCandidate){
//                             Point_2 candidateInterceptorLocation = motionGraph[intersecCandidate].location;
//                             //std::cout << "Empty!" << motionGraph[rayIntersection.associatedMGVertex].id << ","<< motionGraph[intersecCandidate].id <<std::endl;
//                             //std::cout << "Empty!" <<candidateInterceptorLocation.x() <<","<<candidateInterceptorLocation.y()<<"," <<vertexLocation.x()<<","<<vertexLocation.y()<<","<<rayIntersection.locationOnPolyBoundary.y() << std::endl;
//                             return intersectsAuraPredicate(candidateInterceptorLocation, vertexLocation, rayIntersection.locationOnPolyBoundary.y());
//                         });
//            if(interceptingAuras.empty()) {
//                //std::cout << "Empty!" << rayIntersection.locationOnPolyBoundary.x() << "," << rayIntersection.locationOnPolyBoundary.y() << std::endl;
//                //std::cout << "Empty!" << motionGraph[rayIntersection.associatedMGVertex].id << std::endl;
//                //repPoints.emplace_back(rayIntersection, relevantVertex);
//                polyVertex2RayRepPoints[rayIntersection.polySegmentIndex].push_back(rayIntersection);
//            } else if(interceptingAuras.size() == 1) {
//                //std::cout << "NonEmpty1" << std::endl;
//                MGEdgeProperty edgeProperty = getRayEdgeProperty(vertexLocation, vertexProps.id, motionGraph[interceptingAuras[0]].location, fStarComponent.parent.freeSpaceComponent);
//                boost::add_edge(relevantVertex, interceptingAuras[0], edgeProperty,motionGraph);
//            } else {
//                //std::cout << "NonEmpty2" << std::endl;
//                std::sort(interceptingAuras.begin(), interceptingAuras.end(), [&motionGraph](const MGVertex & v1, const MGVertex & v2) {
//                    return motionGraph[v1].location.y() < motionGraph[v2].location.y();
//                });
//                MGEdgeProperty edgeProperty = getRayEdgeProperty(vertexLocation, vertexProps.id, motionGraph[interceptingAuras[0]].location, fStarComponent.parent.freeSpaceComponent);
//                boost::add_edge(relevantVertex, interceptingAuras[0], edgeProperty ,motionGraph);
//            }
//
//        }
//    }
//    const std::list<Point_2>& corners = outerBoundary.vertices();
//    generateListEdges(motionGraph, fStarComponent, corners, polyVertex2RepPoint, polyVertex2RayRepPoints);
//
//}
//
//void ChrMotionGraphGenerator::handleStartOnFStarEdge(Motion_Graph& mg, const FStarComponent& fStarComponent, MGVertex& relevantVertex, const std::vector<MGVertex>& fStarVertices) {
//
//    Point_2 rVertexLocation = mg[relevantVertex].location;
//
//    std::vector<MGVertex> overlappingVertices;
//    for(const auto& potIntersectorVertex : fStarVertices) {
//        if(potIntersectorVertex != relevantVertex && Utils::aurasOverlap(rVertexLocation, mg[potIntersectorVertex].location)) {
//            overlappingVertices.push_back(potIntersectorVertex);
//        }
//    }
//
//    if(overlappingVertices.empty()) {
//        getRepPoint();
//        return;
//    }
//
//    Polygon_2 aura = Utils::generateRobotAura(rVertexLocation);
//    std::vector<Segmen>
//    //Find aura
//    for(const Segment_2& polyEdge : fStarComponent.fStarPolygon.outer_boundary().edges()) {
//        for(const Segment_2& auraEdge : aura.edges()) {
//            const auto result = CGAL::intersection(polyEdge, auraEdge);
//            if(result) {
//                if (const Segment_2* s = boost::get<Segment_2>(&*result)) {
//                    return {s->source(), i, forVertex};
//                } else {
//                    const Point_2* p = boost::get<Point_2 >(&*result);
//                    return {*p, i, forVertex};
//                }
//            }
//        }
//        i++;
//    }
//



}