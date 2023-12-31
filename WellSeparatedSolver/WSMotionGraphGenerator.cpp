//
// Created by joelh on 23/10/2023.
//

#include "WSMotionGraphGenerator.h"
#include "../Utils/Utils.h"
#include "../Utils/GraphvizDrawUtils.h"
#include "../constants.h"
#include <CGAL/Boolean_set_operations_2.h>
#include "../SharedSolverComponents/MGCreationHelper.h"
//#include "../Utils/SFMLDrawUtils.h"
//#include <CGAL/Boolean_set_operations_2/join.h>
#include <CGAL/intersections.h>
#include <algorithm>
//#include <boost/graph/adjacency_list.hpp>


std::unordered_map<std::string, Motion_Graph> WSMotionGraphGenerator::getMotionGraphs(const std::vector<FreeSpaceComponent>& fSpaceComps, const std::vector<FStarComponent>& fStarComps) {
    //Solving random WS Problem with params workspaceComplexity: 100; nmbrStartPositions: 15; maxWorkspaceSize 170
    //Seed = 1679184067
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

//void WSMotionGraphGenerator::insertVertices(Motion_Graph &motionGraph, const std::vector<STConf>& startConfs, const std::vector<STConf>& targetConfs, STConfId2MGVertex &stId2Vertex)
//{
//    std::for_each(startConfs.begin(), startConfs.end(), [&motionGraph, &stId2Vertex](const auto& startConf) {
//        //Motion_Graph::vertex_descriptor vd = boost::add_vertex(motionGraph);
//        MGVertex v = boost::add_vertex(MGVertexProperty{startConf.id,startConf.location, true}, motionGraph);
//        stId2Vertex[startConf.id] = v;
//    });
//    std::for_each(targetConfs.begin(), targetConfs.end(), [&motionGraph, &stId2Vertex](const auto& targetConf) {
//        //Motion_Graph::vertex_descriptor vd = boost::add_vertex(motionGraph);
//        MGVertex v = boost::add_vertex(MGVertexProperty{targetConf.id,targetConf.location, false}, motionGraph);
//        stId2Vertex[targetConf.id] = v;
//    });
//}

void WSMotionGraphGenerator::insertEdges(Motion_Graph& motionGraph, const FStarComponent& fStarComponent, const STConfId2MGVertex & id2Vertex)//, const Polygon_2& freeSpaceSet)
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

bool WSMotionGraphGenerator::intersectsAuraPredicate(const Point_2& pointToCheck, const Point_2& referencePoint, const K::FT& yIntersection) {
    // Define your filtering conditions here
    return CGAL::abs(pointToCheck.x() - referencePoint.x()) <= 2 && pointToCheck.y() > referencePoint.y() && pointToCheck.y() <= yIntersection + ROBOT_SIZE;
}

void WSMotionGraphGenerator::getMGForFStarComponent(Motion_Graph& motionGraph, const FStarComponent &fStarComponent, const std::vector<MGVertex>& relevantVertices) {

    std::vector<RepPoint> repPoints;

    const Polygon_2& outerBoundary = fStarComponent.fStarPolygon.outer_boundary();
    const auto boundaryYMax = outerBoundary.bbox().ymax();

    for(const MGVertex & relevantVertex : relevantVertices) {
        MGVertexProperty vertexProps = motionGraph[relevantVertex];
        Point_2 vertexLocation = vertexProps.location;
        if(outerBoundary.has_on_unbounded_side(vertexLocation)) {
            repPoints.push_back(getBoundaryRepPoint(outerBoundary, fStarComponent.parent.freeSpaceComponent,vertexLocation, relevantVertex));
        } else {
            //Shoot ray upwards
            RepPoint rayIntersection = getRayIntersectionWithFreeSpace(vertexLocation, outerBoundary, boundaryYMax, relevantVertex);
            std::vector<MGVertex> interceptingAuras;
            std::copy_if(relevantVertices.begin(), relevantVertices.end(), std::back_inserter(interceptingAuras),
                         [& vertexLocation, &rayIntersection, &motionGraph](const MGVertex & intersecCandidate){
                             Point_2 candidateInterceptorLocation = motionGraph[intersecCandidate].location;
                             //std::cout << "Empty!" << motionGraph[rayIntersection.associatedMGVertex].id << ","<< motionGraph[intersecCandidate].id <<std::endl;
                             //std::cout << "Empty!" <<candidateInterceptorLocation.x() <<","<<candidateInterceptorLocation.y()<<"," <<vertexLocation.x()<<","<<vertexLocation.y()<<","<<rayIntersection.locationOnPolyBoundary.y() << std::endl;
                             return intersectsAuraPredicate(candidateInterceptorLocation, vertexLocation, rayIntersection.locationOnPolyBoundary.y());
                         });
            if(interceptingAuras.empty()) {
                //std::cout << "Empty!" << rayIntersection.locationOnPolyBoundary.x() << "," << rayIntersection.locationOnPolyBoundary.y() << std::endl;
                //std::cout << "Empty!" << motionGraph[rayIntersection.associatedMGVertex].id << std::endl;
                //repPoints.emplace_back(rayIntersection, relevantVertex);
                repPoints.push_back(rayIntersection);
            } else if(interceptingAuras.size() == 1) {
                //std::cout << "NonEmpty1" << std::endl;
                MGEdgeProperty edgeProperty = getRayEdgeProperty(vertexLocation, vertexProps.id, motionGraph[interceptingAuras[0]].location, fStarComponent.parent.freeSpaceComponent);
                boost::add_edge(relevantVertex, interceptingAuras[0], edgeProperty,motionGraph);
            } else {
                //std::cout << "NonEmpty2" << std::endl;
                std::sort(interceptingAuras.begin(), interceptingAuras.end(), [&motionGraph](const MGVertex & v1, const MGVertex & v2) {
                    return motionGraph[v1].location.y() < motionGraph[v2].location.y();
                });
                MGEdgeProperty edgeProperty = getRayEdgeProperty(vertexLocation, vertexProps.id, motionGraph[interceptingAuras[0]].location, fStarComponent.parent.freeSpaceComponent);
                boost::add_edge(relevantVertex, interceptingAuras[0], edgeProperty ,motionGraph);
            }

        }
    }
    generateListEdges(motionGraph, fStarComponent, repPoints);

}

/*
 * Method to reorder repPoint list
 *
 * Iterate through vertices. If we find repPoint add to vector
 */

bool compareRepPoints(const RepPoint& rp1, const RepPoint& rp2) {
    // Compare by polySegmentIndex
    if (rp1.polySegmentIndex < rp2.polySegmentIndex)
        return true;
    else if (rp1.polySegmentIndex > rp2.polySegmentIndex)
        return false;

    // If polySegmentIndex is the same, compare by positionAlongSegment
    return rp1.positionAlongSegment < rp2.positionAlongSegment;
}

void WSMotionGraphGenerator::generateListEdges(Motion_Graph& motionGraph, const FStarComponent& fStarComp , const std::vector<RepPoint>& unsortedRepPoints) {


//    std::for_each(repPoints.begin(), repPoints.end(), [&motionGraph](const auto& repPoint) {
//        std::cout << motionGraph[get<1>(repPoint)].id << std::endl;
//    });
    std::vector<RepPoint> sortedRepPoints = unsortedRepPoints;

    std::sort(sortedRepPoints.begin(), sortedRepPoints.end(), compareRepPoints);

    if(sortedRepPoints.size() == 1) {
        return; // Vertices are already connected by ray segments
    }

    if(sortedRepPoints.size() == 2) {
        std::string vertex1Id = motionGraph[sortedRepPoints[0].associatedMGVertex].id;
        MGEdgeProperty edgeProperty = getListEdgeProperty(motionGraph, sortedRepPoints[0], sortedRepPoints[1], fStarComp.parent.freeSpaceComponent, fStarComp.fStarPolygon.outer_boundary());
        boost::add_edge(sortedRepPoints[0].associatedMGVertex, sortedRepPoints[1].associatedMGVertex, edgeProperty,motionGraph);
        return;
    }

    for(int i = 1; i < sortedRepPoints.size(); i++) {
        std::string vertex1Id = motionGraph[sortedRepPoints[i-1].associatedMGVertex].id;
        MGEdgeProperty edgeProperty = getListEdgeProperty(motionGraph, sortedRepPoints[i-1], sortedRepPoints[i], fStarComp.parent.freeSpaceComponent, fStarComp.fStarPolygon.outer_boundary());
        //std::cout << "Adding edge:" << vertex1Id <<"-"<< motionGraph[sortedRepPoints[i].associatedMGVertex].id << std::endl;
        boost::add_edge(sortedRepPoints[i-1].associatedMGVertex, sortedRepPoints[i].associatedMGVertex, edgeProperty, motionGraph);
    }
    std::string vertex1Id = motionGraph[sortedRepPoints[0].associatedMGVertex].id;
    //std::cout << "Problem? " << vertex1Id << std::endl;
    //MGEdgeProperty edgeProperty = getListEdgeProperty(motionGraph, sortedRepPoints[sortedRepPoints.size()-1], sortedRepPoints[0], fStarComp.parent.freeSpaceComponent, fStarComp.fStarPolygon.outer_boundary());
    //boost::add_edge(sortedRepPoints[sortedRepPoints.size()-1].associatedMGVertex, sortedRepPoints[0].associatedMGVertex, edgeProperty,motionGraph);

    //SFMLDrawUtils::drawPaths({fStarComp}, {edgeProperty}, "problemEdge");
    //std::vector<Segment_2> efgr = motionGraph.
}



RepPoint WSMotionGraphGenerator::getBoundaryRepPoint(const Polygon_2& fStarBoundary, const Polygon_2& freeSpaceBoundary, const Point_2& vertexLocation, const MGVertex & forVertex) {
    std::vector<Point_2> intersectionPoints;
    Polygon_2 aura = Utils::generateRobotAura(vertexLocation);
    const std::list<Point_2>& candidates = aura.vertices();

    //Iterate through vertices if contains edge of aura we take as rep

    int i = 0;
    for(const Point_2& polyCorner : fStarBoundary.vertices()) {
        //std::cout << vertex.x() << " grp; "<<vertex.y() << std::endl;
        auto it = std::find(candidates.begin(), candidates.end(), polyCorner);
        if (it != candidates.end()) {
            // The targetPoint is found in the list
            std::vector<Segment_2> auraPath = getAuraPath(vertexLocation, *it, freeSpaceBoundary);
            return {*it, i, 0.0, auraPath, forVertex};
        }
        i++;
    }
    i = 0;
    for(const Segment_2& polyEdge : fStarBoundary.edges()) {
        for(const Segment_2& auraEdge : aura.edges()) {
            const auto result = CGAL::intersection(polyEdge, auraEdge);
            if(result) {
                if (const Segment_2* s = boost::get<Segment_2>(&*result)) {
                    std::vector<Segment_2> auraPath = getAuraPath(vertexLocation, s->source(), freeSpaceBoundary);
                    double percentageAlongEdge = Utils::getPercentageAlongSegment(s->source(), polyEdge);
                    return {s->source(), i, percentageAlongEdge, auraPath, forVertex};
                } else {
                    const Point_2* p = boost::get<Point_2 >(&*result);
                    std::vector<Segment_2> auraPath = getAuraPath(vertexLocation, *p, freeSpaceBoundary);
                    double percentageAlongEdge = Utils::getPercentageAlongSegment(*p, polyEdge);
                    return {*p, i, percentageAlongEdge, auraPath, forVertex};
                }
            }
        }
        i++;
    }
    throw std::runtime_error("Could not find representative point for st configuration");
}

RepPoint WSMotionGraphGenerator::getRayIntersectionWithFreeSpace(const Point_2& shooterLocation, const Polygon_2& outerBoundary, const K::FT& rayYLimit, const MGVertex & forVertex) {
    Point_2 rayEndpoint(shooterLocation.x(), rayYLimit);
    std::vector<Point_2> intersectionPoints;
    Segment_2 rayToIntersect(shooterLocation, rayEndpoint);
     //TODO: Put in util?
    Point_2 lowestPoint = rayEndpoint;
    Segment_2 segmentContainingPoint = {};
    int indexOfLowestPoint = 0;
    int i = -1;
    for(const auto& edge : outerBoundary.edges()) {
        //std::cout << edge.source().x() << " gri; "<<edge.source().y() << std::endl;
        i++;
        const auto result = CGAL::intersection(rayToIntersect, edge);
        if(!result) {
            continue;
        }
        if (const Segment_2* s = boost::get<Segment_2>(&*result)) {
            if(s->source().y() < lowestPoint.y() || s->target().y() < lowestPoint.y()) {
                lowestPoint = s->source().y() < s->target().y() ? s->source() : s->target();
                segmentContainingPoint = edge;
                indexOfLowestPoint = i;
            }
        } else {
            const Point_2* p = boost::get<Point_2 >(&*result);
            if(p->y() < lowestPoint.y()) {
                lowestPoint = *p;
                segmentContainingPoint = edge;
                indexOfLowestPoint = i;
            }
        }
    }
    double percentageAlongEdge = Utils::getPercentageAlongSegment(lowestPoint, segmentContainingPoint);
    Segment_2 pathToRepPoint = {shooterLocation, lowestPoint};
    return {lowestPoint, indexOfLowestPoint, percentageAlongEdge, {pathToRepPoint}, forVertex};
}

/*
 * If directly added:
 * Need shooterLocation; RayIntersectPoint and HitVertex locationOnPolyBoundary [Path from hit vertex to ]
 * If both Bi:
 * Need both locations and their representative points + index of corner/ side containing rep points[Find path to rep points and go from there]
 * If one Hi:
 * Need both locations and their representative points + info of one being H_i and index of containing edg
 *
 * Need method which finds path from
 */


//Vertex1 needs to be earlier in the polygon boundary than vertex2
//MGEdgeProperty WSMotionGraphGenerator::getListEdgeProperty(const std::string& vertex1Id, const Point_2& vertex1, const Point_2& vertex2, const RepPoint& vertex1Rep, const RepPoint& vertex2Rep, const Polygon_2& freeSpaceBoundary, const Polygon_2& fStarBoundary) {
MGEdgeProperty WSMotionGraphGenerator::getListEdgeProperty(const Motion_Graph& mg, const RepPoint& vertex1Rep, const RepPoint& vertex2Rep, const Polygon_2& freeSpaceBoundary, const Polygon_2& fStarBoundary) {

    std::vector<Segment_2> path;
    const Point_2 vertex1 = mg[vertex1Rep.associatedMGVertex].location;
    const Point_2 vertex2 = mg[vertex2Rep.associatedMGVertex].location;
    const std::string vertex1Id = mg[vertex1Rep.associatedMGVertex].id;

    //Copy path to rep point 1
    path.insert(path.begin(), vertex1Rep.pathFromVertex.begin(), vertex1Rep.pathFromVertex.end());

    if(vertex1Rep.polySegmentIndex == vertex2Rep.polySegmentIndex) {
        //Both representative points are on the same polygon edge
        path.emplace_back(vertex1Rep.locationOnPolyBoundary, vertex2Rep.locationOnPolyBoundary);
    } else {
        int currentVertex;
        if(vertex1Rep.polySegmentIndex == fStarBoundary.size()-1) {
            //Reached end of polygon
            path.emplace_back(vertex1Rep.locationOnPolyBoundary, fStarBoundary.vertex(0));
            currentVertex = 0;
        } else {
            //Go to next vertex
            path.emplace_back(vertex1Rep.locationOnPolyBoundary, fStarBoundary.vertex(vertex1Rep.polySegmentIndex +1));
            currentVertex = vertex1Rep.polySegmentIndex +1;
        }
        //Loop until vertex/ edge is found containing the vertex2 representative
        while(currentVertex!=vertex2Rep.polySegmentIndex) {
            if(currentVertex == fStarBoundary.size()-1) {
                //Reached end of polygon
                path.emplace_back(fStarBoundary.vertex(currentVertex),fStarBoundary.vertex(0));
                currentVertex = 0;
            } else {
                path.emplace_back(fStarBoundary.vertex(currentVertex),fStarBoundary.vertex(currentVertex+1));
                currentVertex++;
            }
        }
        if(fStarBoundary.vertex(currentVertex)!= vertex2Rep.locationOnPolyBoundary) {
            path.emplace_back(fStarBoundary.vertex(currentVertex), vertex2Rep.locationOnPolyBoundary);
        }
    }

    //Copy path to rep point 2
    path.insert(path.begin(), vertex2Rep.pathFromVertex.rbegin(), vertex2Rep.pathFromVertex.rend());

    return MGEdgeProperty{path, vertex1Id};
}

std::vector<Segment_2> WSMotionGraphGenerator::getAuraPath(const Point_2& from, const Point_2& to, const Polygon_2& freeSpaceBoundary) {
    const Segment_2 directAuraPath(from, to);

    //Get lowest and highest index of polygon edges intersecting the direct path
    int lowestIntersectIndex = -1;
    int highestIntersectIndex = -1;
//    bool intersectionPathEdgeIndicesCrossZero = true;
//    for(const Segment_2& freeSpaceEdge: freeSpaceBoundary.edges()) {
//        if(CGAL::do_intersect(directAuraPath, freeSpaceEdge)) {
//            if(i<lowestIntersectIndex) {
//                lowestIntersectIndex = i;
//            }
//        } else if(i == 0 || i == static_cast<int>(freeSpaceBoundary.edges().size())){
//            intersectionPathEdgeIndicesCrossZero = false; // The intersecting edges path does not cross the zero index
//        }
//        i++;
//    }

    const bool firstEdgeIntersects = CGAL::do_intersect(directAuraPath, freeSpaceBoundary.edge(0));
    const bool lastEdgeIntersects = CGAL::do_intersect(directAuraPath, freeSpaceBoundary.edge(freeSpaceBoundary.edges().size()-1));
    // The segment of the polygon edges which intersect the direct aura path, consist of edges at the beginning and at the end of the polygon edge array.
    bool intersectionPathEdgeIndicesCrossZero = firstEdgeIntersects && lastEdgeIntersects;

    if(!intersectionPathEdgeIndicesCrossZero) {
        for(int i = 0; i < freeSpaceBoundary.edges().size(); i++) {
            if(CGAL::do_intersect(directAuraPath, freeSpaceBoundary.edge(i))) {
                if(i<lowestIntersectIndex) {
                    lowestIntersectIndex = i;
                }
                highestIntersectIndex = i;
            }
        }
    } else {
        for(int i = 0; i < freeSpaceBoundary.edges().size(); i++) {
            if(!CGAL::do_intersect(directAuraPath, freeSpaceBoundary.edge(i))) {
                if(lowestIntersectIndex == -1) {
                    lowestIntersectIndex = i-1;
                }
                highestIntersectIndex = i+1;
            }
        }
    }

    if(lowestIntersectIndex == -1) {
        //Direct path is not blocked
        return {directAuraPath};
    }
    std::cerr << "Direct aura path was intersected" << std::endl; //Todo: remove

    //Get polygon edges corresponding to indices
    const Segment_2& lowestIndexSegment = freeSpaceBoundary.edge(lowestIntersectIndex);
    const Segment_2& highestIndexSegment = freeSpaceBoundary.edge(highestIntersectIndex);

    //Get intersection with direct aura path
    //Get lowest distance intersect point <-- travel to it
    //Travel to highest distance intersec point
    // Travel to rep point

    //Get intersection Points with direct path
    Point_2 intersectionLowestIndex = Utils::getClosestIntersectionPointToOrigin(from, directAuraPath, lowestIndexSegment);
    Point_2 intersectionHighestIndex = Utils::getClosestIntersectionPointToOrigin(from, directAuraPath, highestIndexSegment);
    //Get distance from stconf to intersction points
    double distanceLowestIndex = CGAL::to_double(CGAL::squared_distance(from,intersectionLowestIndex));
    double distanceHighestIndex = CGAL::to_double(CGAL::squared_distance(from,intersectionHighestIndex));

    std::vector<Segment_2> path; //Construct path to, along and from intersecting polygon edges
    if(!intersectionPathEdgeIndicesCrossZero) {
        if (distanceLowestIndex < distanceHighestIndex) {
            //Go to lowest index intersection and in order of polygon edges
            path.emplace_back(from, intersectionLowestIndex);
            path.emplace_back(intersectionLowestIndex, freeSpaceBoundary.edge(lowestIntersectIndex).target());
            for(int j = lowestIntersectIndex + 1 ; j < highestIntersectIndex; j++) {
                path.push_back(freeSpaceBoundary.edge(j));
            }
            path.emplace_back(freeSpaceBoundary.edge(highestIntersectIndex).source(), intersectionHighestIndex);
            path.emplace_back(intersectionHighestIndex, to);
        } else {
            //Go to highest index intersection and in order of polygon edges
            path.emplace_back(from, intersectionHighestIndex);
            path.emplace_back(intersectionHighestIndex, freeSpaceBoundary.edge(highestIntersectIndex).source());
            for(int j = highestIntersectIndex - 1; j > lowestIntersectIndex; j++) {
                path.push_back(freeSpaceBoundary.edge(j).opposite());
            }
            path.emplace_back(freeSpaceBoundary.edge(lowestIntersectIndex).target(), intersectionLowestIndex);
            path.emplace_back(intersectionLowestIndex, to);
        }
    } else {
        if (distanceLowestIndex < distanceHighestIndex) {
            //Go to lowest index intersection and in reverse order of polygon edges
            path.emplace_back(from, intersectionLowestIndex);
            path.emplace_back(intersectionLowestIndex, freeSpaceBoundary.edge(lowestIntersectIndex).source());
            for(int j = lowestIntersectIndex -1 ; j >= 0; j--) {
                path.push_back(freeSpaceBoundary.edge(j).opposite());
            }
            for(int j = freeSpaceBoundary.edges().size() - 1; j > highestIntersectIndex; j--) {
                path.push_back(freeSpaceBoundary.edge(j).opposite());
            }
            path.emplace_back(freeSpaceBoundary.edge(highestIntersectIndex).target(), intersectionHighestIndex);
            path.emplace_back(intersectionHighestIndex, to);
        } else {
            //Go to highest index intersection and in reverse order of polygon edges
            path.emplace_back(from, intersectionHighestIndex);
            path.emplace_back(intersectionHighestIndex, freeSpaceBoundary.edge(highestIntersectIndex).target());
            for(int j = highestIntersectIndex + 1 ; j < freeSpaceBoundary.edges().size(); j++) {
                path.push_back(freeSpaceBoundary.edge(j));
            }
            for(int j = 0; j < lowestIntersectIndex; j++) {
                path.push_back(freeSpaceBoundary.edge(j));
            }
            path.emplace_back(freeSpaceBoundary.edge(highestIntersectIndex).source(), intersectionLowestIndex);
            path.emplace_back(intersectionLowestIndex, to);
        }
    }

    if (distanceLowestIndex > distanceHighestIndex) {
        path.emplace_back(from, freeSpaceBoundary.edge(lowestIntersectIndex).target());
        for(int j = lowestIntersectIndex + 1 ; j < highestIntersectIndex; j++) {
            path.push_back(freeSpaceBoundary.edge(j));
        }
        path.emplace_back(freeSpaceBoundary.edge(highestIntersectIndex).source(), to);
    } else {
        path.emplace_back(from, freeSpaceBoundary.edge(highestIntersectIndex).source());
        for(int j = lowestIntersectIndex + 1 ; j < highestIntersectIndex; j++) {
            path.push_back(freeSpaceBoundary.edge(j));
        }
        path.emplace_back(freeSpaceBoundary.edge(lowestIntersectIndex).target(), to);
    }
    return path;
}



MGEdgeProperty WSMotionGraphGenerator::getRayEdgeProperty(const Point_2& rayShooter, const std::string& hitVertexId, const Point_2& hitVertex, const Polygon_2& freeSpace) {
    const Point_2 hitPoint(rayShooter.x(), hitVertex.y()-ROBOT_SIZE);

    const Segment_2 rayPath(hitPoint, rayShooter);
    std::vector<Segment_2> path = getAuraPath(hitVertex, hitPoint, freeSpace);
    path.push_back(rayPath);

    return MGEdgeProperty{path ,hitVertexId};
}

