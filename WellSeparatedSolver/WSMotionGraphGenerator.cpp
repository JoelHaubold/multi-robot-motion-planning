//
// Created by joelh on 23/10/2023.
//

#include "WSMotionGraphGenerator.h"
#include "../Utils/Utils.h"
#include "../constants.h"
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/Boolean_set_operations_2/join.h>
#include <CGAL/intersections.h>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>

void WSMotionGraphGenerator::insertVertices(Motion_Graph &motionGraph, const STConfigurations &stConfigurations)
{
    std::for_each(stConfigurations.startConfigurations.begin(), stConfigurations.startConfigurations.end(), [&motionGraph](const auto& startConf) {
        //Motion_Graph::vertex_descriptor vd = boost::add_vertex(motionGraph);
        boost::add_vertex(MGVertexProperty{startConf.id,startConf.location, true}, motionGraph);
    });
    std::for_each(stConfigurations.targetConfigurations.begin(), stConfigurations.targetConfigurations.end(), [&motionGraph](const auto& targetConf) {
        //Motion_Graph::vertex_descriptor vd = boost::add_vertex(motionGraph);
        boost::add_vertex(MGVertexProperty{targetConf.id,targetConf.location, false}, motionGraph);
    });
}

//TODO: Take List of struct of F component and list of associated F* components
void WSMotionGraphGenerator::fillMotionGraph(Motion_Graph& motionGraph, const std::vector<FStarComponent>& fStarComponents)//, const Polygon_2& freeSpaceSet)
{
//    for(const auto& fStarComponent : fStarComponents) {
//
//        std::vector<Vertex> relevantVertices;
//        for (boost::tie(vi, vi_end) = vertices(motionGraph); vi != vi_end; ++vi) {
//            Vertex currentVertex = *vi;
//            MGVertexProperty vertex = motionGraph[currentVertex];
//            Polygon_wh_2 result;
//            bool doesIntersect = CGAL::join(poly_with_holes, Utils::generateRobotAura(vertex.location), result);
//            if(doesIntersect) {
//                relevantVertices.push_back(currentVertex);
//            }
//        }
//
//
//        //        for (const auto& startAura : startConfs)
//        //        {
//        //            //bool doesIntersect = CGAL::do_intersect(poly_with_holes, Utils::generateRobotAura(startAura, 2.1));
//        //            Polygon_wh_2 result;
//        //            bool doesIntersect = CGAL::join(poly_with_holes, Utils::generateRobotAura(startAura, 2), result);
//        //            std::cout << doesIntersect << std::endl;
//        //        }
//        //        for (const auto& targetAura : targetConfs)
//        //        {
//        //            //bool doesIntersect = CGAL::do_intersect(poly_with_holes, Utils::generateRobotAura(targetAura, 2.1));
//        //            Polygon_wh_2 result;
//        //            bool doesIntersect = CGAL::join(poly_with_holes, Utils::generateRobotAura(targetAura, 2), result);
//        //            std::cout << doesIntersect << std::endl;
//        //        }
//        getMGForFStarComponent(motionGraph, poly_with_holes, relevantVertices);
//    }

}

bool WSMotionGraphGenerator::intersectsAuraPredicate(const Point_2& pointToCheck, const Point_2& referencePoint, const K::FT& yIntersection) {
    // Define your filtering conditions here
    return CGAL::abs(pointToCheck.x() - referencePoint.x()) <= 2 && pointToCheck.y() >= referencePoint.y() && pointToCheck.y() <= yIntersection + ROBOT_SIZE/2;
}

void WSMotionGraphGenerator::getMGForFStarComponent(Motion_Graph& motionGraph, const Polygon_wh_2 &fStarComponent, const std::vector<Vertex>& relevantVertices) {

    std::list<std::tuple<Point_2, Vertex>> repPoints;

    const Polygon_2& outerBoundary = fStarComponent.outer_boundary();
    const auto boundaryYMax = outerBoundary.bbox().ymax();

    for(const Vertex& relevantVertex : relevantVertices) {
        MGVertexProperty vertexProps = motionGraph[relevantVertex];
        Point_2 vertexLocation = vertexProps.location;
        bool isInside = outerBoundary.bounded_side(vertexLocation) == CGAL::ON_BOUNDED_SIDE;
        if(!isInside) {
            repPoints.emplace_back(getRepPoint(outerBoundary, vertexLocation), relevantVertex);
        } else {
            //Shoot ray upwards
            Point_2 rayIntersection = getRayIntersectionWithFreeSpace(vertexLocation, outerBoundary, boundaryYMax);
            std::vector<Vertex> interceptingAuras;
            std::copy_if(relevantVertices.begin(), relevantVertices.end(), std::back_inserter(interceptingAuras),
                         [& vertexLocation, &rayIntersection, &motionGraph](const Vertex& intersecCandidate){
                             Point_2 candidateInterceptorLocation = motionGraph[intersecCandidate].location;
                             return intersectsAuraPredicate(candidateInterceptorLocation, vertexLocation, rayIntersection.y());
                         });
            if(interceptingAuras.empty()) {
                //TODO: Mark that this was created by ray
                repPoints.emplace_back(rayIntersection, relevantVertex);
            } else if(interceptingAuras.size() == 1) {
                MGEdgeProperty edgeProperty = getRayEdgeProperty(vertexLocation, motionGraph[interceptingAuras[0]].location, fStarComponent);
                boost::add_edge(relevantVertex, interceptingAuras[0], MGEdgeProperty{std::vector<Segment_2>()},motionGraph);
            } else {
                std::sort(interceptingAuras.begin(), interceptingAuras.end(), [&motionGraph](const Vertex& v1, const Vertex& v2) {
                    return motionGraph[v1].location.y() < motionGraph[v2].location.y(); //TODO: Get VertexProperties
                });
                MGEdgeProperty edgeProperty = getRayEdgeProperty(vertexLocation, motionGraph[interceptingAuras[0]].location, fStarComponent);
                boost::add_edge(relevantVertex, interceptingAuras[0], edgeProperty ,motionGraph);
            }

        }
    }

    //Turn rep confs into edges


    //CHeck B
    //Shoot H rays

}



Point_2 WSMotionGraphGenerator::getRepPoint(const Polygon_2& outerBoundary, const Point_2& vertexLocation) {
//    std::vector<Point_2> intersectionPoints;
//    Polygon_2 aura = Utils::generateRobotAura(vertexLocation);
//
//
//
//    CGAL::intersection(aura,outerBoundary,std::back_inserter(intersectionPoints));
//
//    if (!intersectionPoints.empty())
//    {
//        // Take the first intersection point as an arbitrary point on both boundaries
//        Point_2 commonPoint = intersectionPoints.front();
//        std::cout << "Common Point: (" << commonPoint.x() << ", " << commonPoint.y() << ")" << std::endl;
//        return commonPoint;
//    } else {
//        throw std::runtime_error("Aura of relevant start/ target configuration cannot be disjunct from their fstar component" );
//    }
    return Point_2();
}

Point_2 WSMotionGraphGenerator::getRayIntersectionWithFreeSpace(const Point_2& shooterLocation, const Polygon_2& outerBoundary, const K::FT& rayYLimit) {
//    Point_2 rayEndpoint(shooterLocation.x(), rayYLimit);
//    std::vector<Point_2> intersectionPoints;
//    CGAL::intersection(outerBoundary.edges_begin(), outerBoundary.edges_end(), shooterLocation, rayEndpoint, std::back_inserter(intersectionPoints));
//    std::sort(intersectionPoints.begin(), intersectionPoints.end(), [](const Point_2& p1, const Point_2& p2) {
//        return p1.y() < p2.y();
//    });
    return Point_2();
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

