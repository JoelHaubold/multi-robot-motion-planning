//
// Created by joelh on 17/10/2023.
//

#ifndef MRMP_IMPLEMENTATION_WORKSPACEGENARATOR_H
#define MRMP_IMPLEMENTATION_WORKSPACEGENARATOR_H


#include "RandomGenerator.h"
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/point_generators_2.h>
#include <list>
#include <vector>


class WorkspaceGenerator {
public:
  typedef CGAL::Exact_predicates_exact_constructions_kernel K;
  typedef K::Point_2 Point_2;
  typedef std::list<Point_2> Container;
  typedef CGAL::Polygon_2<K, Container> Polygon_2;
  typedef CGAL::Creator_uniform_2<int, Point_2>             Creator;
  typedef CGAL::Random_points_in_square_2<Point_2, Creator> Point_generator;

//  const double RADIUS;
//  const int MAX_POLY_SIZE;
//
//  const int OBSTACLE_OFFSET;
//  const int ROBOT_SIZE;
//
//  const int NMBR_START_POS;
//  const int WORKSPACE_COMPLEXITY;

  WorkspaceGenerator();

  static Polygon_2 generateRandomPolygon(int maxPoints, int maxDistanceFromOrigin);
  std::list<Point_2> getRandomPoints(const Polygon_2& containingPolygon, const int& nmbrPoints);
  std::vector<Point_2> getRandomPointDistribution(const int& nmbrOfSections, const int& nmbrPoints);

private:
  static RandomGenerator randomGenerator;
};


#endif//MRMP_IMPLEMENTATION_WORKSPACEGENARATOR_H
