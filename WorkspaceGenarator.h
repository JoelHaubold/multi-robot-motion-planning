//
// Created by joelh on 17/10/2023.
//

#ifndef MRMP_IMPLEMENTATION_WORKSPACEGENARATOR_H
#define MRMP_IMPLEMENTATION_WORKSPACEGENARATOR_H


#include "RandomGenerator.h"
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/point_generators_2.h>
#include <list>
#include <vector>
#include "mytypedefs.h"


class WorkspaceGenerator {
public:
  typedef CGAL::Random_points_in_square_2<Point_2, Creator> Point_generator;

  WorkspaceGenerator();

  static Polygon_2 generateRandomPolygon(const int& maxPoints, const int& maxDistanceFromOrigin);
  static std::list<Point_2> getRandomPoints(const Polygon_2& containingPolygon, const int& nmbrPoints);
  static std::vector<int> getRandomPointDistribution(const int& nmbrOfSections, const int& nmbrPoints);

};


#endif//MRMP_IMPLEMENTATION_WORKSPACEGENARATOR_H
