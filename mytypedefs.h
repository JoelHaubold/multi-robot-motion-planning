//
// Created by joelh on 19/10/2023.
//

#ifndef MRMP_IMPLEMENTATION_MYTYPEDEFS_H
#define MRMP_IMPLEMENTATION_MYTYPEDEFS_H

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Polygon_set_2.h>
#include <boost/graph/adjacency_list.hpp>
#include <utility>
#include "constants.h"




typedef CGAL::Exact_predicates_exact_constructions_kernel K; // CGAL::Simple_cartesian<double>
//typedef CGAL::Simple_cartesian<RT>                        K;
typedef K::Point_2                                        Point_2;
typedef K::Segment_2                                      Segment_2;
typedef std::list<Point_2>                                Container;
typedef CGAL::Polygon_2<K, Container>                     Polygon_2;
typedef CGAL::Polygon_with_holes_2<K, Container>          Polygon_wh_2;
typedef CGAL::Bbox_2                                      Bbox_2;
typedef CGAL::Creator_uniform_2<int, Point_2>             Creator;
typedef std::list<Polygon_wh_2>                           Pwh_list_2;
typedef CGAL::Polygon_set_2<K, Container>                 Polygon_set_2;

// Define the MGVertexProperty struct
struct MGVertexProperty {
    //Necessary for boost library to add edges to the motion graph
    MGVertexProperty()
        : location(Point_2(0, 0)), isStartVertex(false) {
    }

    MGVertexProperty(const std::string id, const Point_2 loc, const bool isStart)
        : id(id), location(loc), isStartVertex(isStart) {
    }
    std::string id;
    Point_2 location;
    bool isStartVertex;
    std::string occupyingRobot = isStartVertex ? id.substr(START_CONF_PREFIX.length()) : "";
};

// Define the MGEdgeProperty struct
struct MGEdgeProperty {
    //PathtoTake
    std::vector<Segment_2> pathLineSegments;
    std::string pathStartId;
    bool isBlockable = false;
};

typedef boost::adjacency_list<
    boost::vecS,               // OutEdgeList type
    boost::vecS,               // VertexList type
    boost::undirectedS,        // Directed or undirected graph
    MGVertexProperty,            // Property for vertices
    MGEdgeProperty               // Property for edges
    >                                                     Motion_Graph;
typedef boost::graph_traits<Motion_Graph>::vertex_descriptor MGVertex;
typedef boost::graph_traits<Motion_Graph>::edge_descriptor MGEdge;
typedef std::unordered_map<std::string, MGVertex>           MGIdToVertex;


struct STConf {
    STConf(Point_2 loc, std::string id)
        : location(std::move(loc)), id(std::move(id)) {
    }

    const Point_2 location;
    const std::string id;
};

struct STConfigurations {
    const std::vector<STConf> startConfigurations;
    const std::vector<STConf> targetConfigurations;
};

struct FreeSpaceComponent {
    FreeSpaceComponent(Polygon_2  fsComp, std::string  id)
        : freeSpaceComponent(std::move(fsComp)), freeSpaceId(std::move(id)) {
    }

    const Polygon_2 freeSpaceComponent;
    std::vector<STConf> startConfigurations;
    std::vector<STConf> targetConfigurations;
    const std::string freeSpaceId;
};

struct FStarComponent {
    FStarComponent(Polygon_wh_2 fStarPoly, std::vector<STConf> adjSConfs, std::vector<STConf> adjTConfs, std::string fStarId, const FreeSpaceComponent &parent)
        : fStarPolygon(std::move(fStarPoly)), adjacentSConfs(std::move(adjSConfs)), adjacentTConfs(std::move(adjTConfs)), fStarId(std::move(fStarId)), parent(parent) {}

//    FStarComponent(Polygon_wh_2 fStarPoly, std::string fStarId)
//        : fStarPolygon(std::move(fStarPoly)), fStarId(std::move(fStarId))) {}

    const Polygon_wh_2 fStarPolygon;
    const std::string fStarId;

    const FreeSpaceComponent& parent;
    const std::vector<STConf> adjacentSConfs;
    const std::vector<STConf> adjacentTConfs;
};

struct MotionSchedule{
    std::unordered_map<std::string, std::string> robots2VerticesTraveled;
    std::string motionSchedule;
};

#endif//MRMP_IMPLEMENTATION_MYTYPEDEFS_H
