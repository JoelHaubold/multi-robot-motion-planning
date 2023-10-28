//
// Created by joelh on 23/10/2023.
//

#include "WSMotionGraphGenerator.h"
#include "../Utils/Utils.h"
#include "../constants.h"
#include <CGAL/Boolean_set_operations_2.h>
//#include <CGAL/Boolean_set_operations_2/join.h>
#include <CGAL/intersections.h>
#include <algorithm>
//#include <boost/graph/adjacency_list.hpp>

MGIdToVertex WSMotionGraphGenerator::insertVertices(Motion_Graph &motionGraph, const STConfigurations &stConfigurations)
{
    MGIdToVertex id2Vertex;
    std::for_each(stConfigurations.startConfigurations.begin(), stConfigurations.startConfigurations.end(), [&motionGraph, &id2Vertex](const auto& startConf) {
        //Motion_Graph::vertex_descriptor vd = boost::add_vertex(motionGraph);
        Vertex v = boost::add_vertex(MGVertexProperty{startConf.id,startConf.location, true}, motionGraph);
        id2Vertex[startConf.id] = v;
    });
    std::for_each(stConfigurations.targetConfigurations.begin(), stConfigurations.targetConfigurations.end(), [&motionGraph, &id2Vertex](const auto& targetConf) {
        //Motion_Graph::vertex_descriptor vd = boost::add_vertex(motionGraph);
        Vertex v = boost::add_vertex(MGVertexProperty{targetConf.id,targetConf.location, false}, motionGraph);
        id2Vertex[targetConf.id] = v;
    });
    return id2Vertex;
}

void WSMotionGraphGenerator::insertEdges(Motion_Graph& motionGraph, const std::vector<FStarComponent>& fStarComponents, const MGIdToVertex& id2Vertex)//, const Polygon_2& freeSpaceSet)
{
    for(const auto& fStarComponent : fStarComponents) {

        std::vector<Vertex> relevantVertices;
        for(const auto& sConf : fStarComponent.adjacentSConfs) {
            relevantVertices.push_back(id2Vertex.find(sConf.id)->second);
        }
        for(const auto& tConf : fStarComponent.adjacentTConfs) {
            relevantVertices.push_back(id2Vertex.find(tConf.id)->second);
        }
        getMGForFStarComponent(motionGraph, fStarComponent.fStarPolygon, relevantVertices);
    }

}

bool WSMotionGraphGenerator::intersectsAuraPredicate(const Point_2& pointToCheck, const Point_2& referencePoint, const K::FT& yIntersection) {
    // Define your filtering conditions here
    return CGAL::abs(pointToCheck.x() - referencePoint.x()) <= 2 && pointToCheck.y() > referencePoint.y() && pointToCheck.y() <= yIntersection + ROBOT_SIZE;
}

void WSMotionGraphGenerator::getMGForFStarComponent(Motion_Graph& motionGraph, const Polygon_wh_2 &fStarComponent, const std::vector<Vertex>& relevantVertices) {

    //TODO: Change to struct containing at least vertex; reference point; and shootByRay
    //std::vector<std::tuple<Point_2, Vertex>> repPoints;
    std::map<int, RepPoint> polyVertex2RepPoint;
    std::map<int, std::vector<RepPoint>> polyVertex2RayRepPoints;

    const Polygon_2& outerBoundary = fStarComponent.outer_boundary();
    const auto boundaryYMax = outerBoundary.bbox().ymax();

    for(const Vertex& relevantVertex : relevantVertices) {
        MGVertexProperty vertexProps = motionGraph[relevantVertex];
        Point_2 vertexLocation = vertexProps.location;
        bool isInside = outerBoundary.bounded_side(vertexLocation) == CGAL::ON_BOUNDED_SIDE;
        if(!isInside) {
            RepPoint repPoint = getRepPoint(outerBoundary, vertexLocation, relevantVertex);
            polyVertex2RepPoint[repPoint.polyVertexNmbr] = repPoint;
        } else {
            //Shoot ray upwards
            RepPoint rayIntersection = getRayIntersectionWithFreeSpace(vertexLocation, outerBoundary, boundaryYMax, relevantVertex);
            std::vector<Vertex> interceptingAuras;
            std::copy_if(relevantVertices.begin(), relevantVertices.end(), std::back_inserter(interceptingAuras),
                         [& vertexLocation, &rayIntersection, &motionGraph](const Vertex& intersecCandidate){
                             Point_2 candidateInterceptorLocation = motionGraph[intersecCandidate].location;
                             //std::cout << "Empty!" << motionGraph[rayIntersection.forVertex].id << ","<< motionGraph[intersecCandidate].id <<std::endl;
                             //std::cout << "Empty!" <<candidateInterceptorLocation.x() <<","<<candidateInterceptorLocation.y()<<"," <<vertexLocation.x()<<","<<vertexLocation.y()<<","<<rayIntersection.location.y() << std::endl;
                             return intersectsAuraPredicate(candidateInterceptorLocation, vertexLocation, rayIntersection.location.y());
                         });
            if(interceptingAuras.empty()) {
                //std::cout << "Empty!" << rayIntersection.location.x() << "," << rayIntersection.location.y() << std::endl;
                //std::cout << "Empty!" << motionGraph[rayIntersection.forVertex].id << std::endl;
                //TODO: Mark that this was created by ray
                //repPoints.emplace_back(rayIntersection, relevantVertex);
                polyVertex2RayRepPoints[rayIntersection.polyVertexNmbr].push_back(rayIntersection);
            } else if(interceptingAuras.size() == 1) {
                std::cout << "NonEmpty1" << std::endl;
                MGEdgeProperty edgeProperty = getRayEdgeProperty(vertexLocation, motionGraph[interceptingAuras[0]].location, fStarComponent);
                boost::add_edge(relevantVertex, interceptingAuras[0], MGEdgeProperty{std::vector<Segment_2>()},motionGraph);
            } else {
                std::cout << "NonEmpty2" << std::endl;
                std::sort(interceptingAuras.begin(), interceptingAuras.end(), [&motionGraph](const Vertex& v1, const Vertex& v2) {
                    return motionGraph[v1].location.y() < motionGraph[v2].location.y(); //TODO: Get VertexProperties
                });
                MGEdgeProperty edgeProperty = getRayEdgeProperty(vertexLocation, motionGraph[interceptingAuras[0]].location, fStarComponent);
                boost::add_edge(relevantVertex, interceptingAuras[0], edgeProperty ,motionGraph);
            }

        }
    }
    const std::list<Point_2>& vertices = outerBoundary.vertices();
    generateListEdges(motionGraph, vertices, polyVertex2RepPoint, polyVertex2RayRepPoints);

}

/*
 * Method to reorder repPoint list
 *
 * Iterate through vertices. If we find repPoint add to vector
 */

void WSMotionGraphGenerator::generateListEdges(Motion_Graph& motionGraph, const std::list<Point_2>& vertices, const std::map<int, RepPoint>& polyVertex2RepPoint, const std::map<int, std::vector<RepPoint>>& polyVertex2RayRepPoints) {


//    std::for_each(repPoints.begin(), repPoints.end(), [&motionGraph](const auto& repPoint) {
//        std::cout << motionGraph[get<1>(repPoint)].id << std::endl;
//    });
    int i = 0;
    std::vector<Vertex> sortedRepPoints;
    Point_2 lastVertex;
    for(const Point_2& vertex : vertices) {
        //std::cout << vertex.x() << " gle; "<<vertex.y() << std::endl;
        if(i!=0) {
            auto it2 =  polyVertex2RayRepPoints.find(i-1);
            if (it2 != polyVertex2RayRepPoints.end()) {
                if(it2->second.size() == 1) {
                    sortedRepPoints.push_back(it2->second.front().forVertex);
                } else { //TODO: Debug order in this this case
                    std::vector<RepPoint> rayRepPoints = it2->second;
                    if(lastVertex.x() < vertex.x()){
                        std::sort(rayRepPoints.begin(), rayRepPoints.end(), [](const RepPoint& p1, const RepPoint& p2) {
                            return p1.location.x() < p2.location.x();
                        });
                    } else {
                        std::sort(rayRepPoints.begin(), rayRepPoints.end(), [](const RepPoint& p1, const RepPoint& p2) {
                            return p1.location.x() > p2.location.x();
                        });
                    }

                    std::cout << vertex.x() << " ; "<<lastVertex.x() << std::endl;
                    for(const RepPoint& rayRepPoint : rayRepPoints) {
                        std::cout << motionGraph[rayRepPoint.forVertex].id << " ; "<<rayRepPoint.location.x()<<" ; "<< rayRepPoint.location.y() << std::endl;
                        sortedRepPoints.push_back(rayRepPoint.forVertex);
                    }
                }
            }
        }

        auto it =  polyVertex2RepPoint.find(i);
        if (it != polyVertex2RepPoint.end()) {
            sortedRepPoints.push_back(it->second.forVertex);
        }
        lastVertex = vertex;
        i++;
    }

    const Point_2& vertex = vertices.front();
    auto it2 =  polyVertex2RayRepPoints.find(i-1);
    if (it2 != polyVertex2RayRepPoints.end()) {
        if(it2->second.size() == 1) {
            sortedRepPoints.push_back(it2->second.front().forVertex);
        } else { //TODO: Debug order in this this case
            std::vector<RepPoint> rayRepPoints = it2->second;
            if(lastVertex.x() < vertex.x()){
                std::sort(rayRepPoints.begin(), rayRepPoints.end(), [](const RepPoint& p1, const RepPoint& p2) {
                    return p1.location.x() < p2.location.x();
                });
            } else {
                std::sort(rayRepPoints.begin(), rayRepPoints.end(), [](const RepPoint& p1, const RepPoint& p2) {
                    return p1.location.x() > p2.location.x();
                });
            }
            for(const RepPoint& rayRepPoint : rayRepPoints) {
                sortedRepPoints.push_back(rayRepPoint.forVertex);
            }
        }
    }



    if(sortedRepPoints.size() == 2) {
        boost::add_edge(sortedRepPoints[0], sortedRepPoints[1], MGEdgeProperty{std::vector<Segment_2>()},motionGraph);
        return;
    }

    for(int i = 1; i < sortedRepPoints.size(); i++) {
        boost::add_edge(sortedRepPoints[i-1], sortedRepPoints[i], MGEdgeProperty{std::vector<Segment_2>()},motionGraph);
    }
    boost::add_edge(sortedRepPoints[0], sortedRepPoints[sortedRepPoints.size()-1], MGEdgeProperty{std::vector<Segment_2>()},motionGraph);
}



RepPoint WSMotionGraphGenerator::getRepPoint(const Polygon_2& outerBoundary, const Point_2& vertexLocation, const Vertex& forVertex) {
    std::vector<Point_2> intersectionPoints;
    Polygon_2 aura = Utils::generateRobotAura(vertexLocation);
    const std::list<Point_2>& candidates = aura.vertices();

    //Iterate through vertices if contains edge of aura we take as rep

    int i = 0;
    for(const Point_2& vertex : outerBoundary.vertices()) { //TODO: Name Polygon vertices corners
        //std::cout << vertex.x() << " grp; "<<vertex.y() << std::endl;
        auto it = std::find(candidates.begin(), candidates.end(), vertex);
        if (it != candidates.end()) {
            // The targetPoint is found in the list
            return {*it, i, forVertex};
        }
        i++;
    }
    i = 0;
    for(const Segment_2& polyEdge : outerBoundary.edges()) {
        for(const Segment_2& auraEdge : aura.edges()) {
            const auto result = CGAL::intersection(polyEdge, auraEdge);
            if(result) {
                if (const Segment_2* s = boost::get<Segment_2>(&*result)) {
                    return {s->source(), i, forVertex};
                } else {
                    const Point_2* p = boost::get<Point_2 >(&*result);
                    return {*p, i, forVertex};
                }
            }
        }
        i++;
    }

    std::cout << "Point not found" << std::endl;
    return {};
}

RepPoint WSMotionGraphGenerator::getRayIntersectionWithFreeSpace(const Point_2& shooterLocation, const Polygon_2& outerBoundary, const K::FT& rayYLimit, const Vertex& forVertex) {
    Point_2 rayEndpoint(shooterLocation.x(), rayYLimit);
    std::vector<Point_2> intersectionPoints;
    Segment_2 rayToIntersect(shooterLocation, rayEndpoint);
     //TODO: Put in util?
    Point_2 lowestPoint = rayEndpoint;
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
                indexOfLowestPoint = i;
            }
        } else {
            const Point_2* p = boost::get<Point_2 >(&*result);
            if(p->y() < lowestPoint.y()) {
                lowestPoint = *p;
                indexOfLowestPoint = i;
            }
        }
    }
    return {lowestPoint, indexOfLowestPoint, forVertex};
}

MGEdgeProperty WSMotionGraphGenerator::getRayEdgeProperty(const Point_2& rayShooter, const Point_2& hitVertex, const Polygon_wh_2& fStarComponent) {

    std::vector<Segment_2> path;

    //    path.emplace_back(rayShooter.x(), hitVertex.y()- ROBOT_SIZE/2);
    //
    //    Segment_2 raySegment(rayShooter, hitPoint);
    //    Segment_2 directAuraSegment(hitPoint, hitVertex);
    //
    //    std::vector<Segment_2> intersectingEdges;
    //    for (Polygon_2::Edge_const_iterator edgeIt = fStarComponent.edges_begin(); edgeIt != fStarComponent.edges_end(); ++edgeIt)
    //    {
    //        if (CGAL::do_intersect(*edgeIt, directAuraSegment)) {
    //            intersectingEdges.push_back(*edgeIt);
    //        }
    //    }
    //
    //    if(intersectingEdges.empty()) {
    //        path.push_back(directAuraSegment);
    //    } else {
    //        path.push_back();
    //    }
    //
    //
    //
    //
    //
    //    Segment_2 raySegment();
    return MGEdgeProperty{std::vector<Segment_2>()};
}

