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




typedef CGAL::Exact_predicates_exact_constructions_kernel K;
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
    //Necessary for boost library
    MGVertexProperty()
        : location(Point_2(0, 0)), isStartVertex(false), hasPebble(false) {
    }

    MGVertexProperty(const Point_2 loc, const bool isStart)
        : location(loc), isStartVertex(isStart) {
    }
    //const std::string id;
    const Point_2 location;
    const bool isStartVertex;
    bool hasPebble = isStartVertex;
};

// Define the MGEdgeProperty struct
struct MGEdgeProperty {
    //PathtoTake
    const std::vector<Segment_2> pathLineSegments;
};

typedef boost::adjacency_list<
    boost::vecS,               // OutEdgeList type
    boost::vecS,               // VertexList type
    boost::undirectedS,        // Directed or undirected graph
    MGVertexProperty,            // Property for vertices
    MGEdgeProperty               // Property for edges
    >                                                     Motion_Graph;
typedef boost::graph_traits<Motion_Graph>::vertex_descriptor    Vertex;

#endif//MRMP_IMPLEMENTATION_MYTYPEDEFS_H
