//
// Created by joelh on 19/10/2023.
//

#ifndef MRMP_IMPLEMENTATION_MYTYPEDEFS_H
#define MRMP_IMPLEMENTATION_MYTYPEDEFS_H

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
//typedef CGAL::Simple_cartesian<RT>                        K;
typedef K::Point_2                                        Point_2;
typedef std::list<Point_2>                                Container;
typedef CGAL::Polygon_2<K, Container>                     Polygon_2;
typedef CGAL::Polygon_with_holes_2<K, Container>          Polygon_wh_2;
typedef CGAL::Bbox_2                                      Bbox_2;
typedef CGAL::Creator_uniform_2<int, Point_2>             Creator;
typedef std::list<Polygon_wh_2>                           Pwh_list_2;

#endif//MRMP_IMPLEMENTATION_MYTYPEDEFS_H
