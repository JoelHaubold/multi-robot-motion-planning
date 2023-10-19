//
// Created by joelh on 17/10/2023.
//

#include "WorkspaceGenarator.h"
#include <CGAL/random_polygon_2.h>
#include <iostream>

WorkspaceGenerator::WorkspaceGenerator() = default;

Polygon_2 WorkspaceGenerator::generateRandomPolygon(const int& maxPoints, const int& maxDistanceFromOrigin) {
  Polygon_2            polygon;
  std::list<Point_2>   point_set;
  //CGAL::Random         rand;
  //std::cerr << "Seed = " <<  rand.get_seed() << std::endl;
  int size = RandomGenerator::getRandomInt(4, maxPoints);
  // copy size points from the generator, eliminating duplicates, so the
  // polygon will have <= size vertices
  CGAL::copy_n_unique(WorkspaceGenerator::Point_generator(maxDistanceFromOrigin), size,
                      std::back_inserter(point_set));
  std::ostream_iterator< Point_2 >  out( std::cout, " " );
  std::cout << "From the following " << point_set.size() << " points "
            << std::endl;
  std::copy(point_set.begin(), point_set.end(), out);
  std::cout << std::endl;
  CGAL::random_polygon_2(point_set.size(), std::back_inserter(polygon),
                         point_set.begin());
  std::cout << "The following simple polygon was made: " << std::endl;
  std::cout << polygon << std::endl;

  //CGAL::draw(polygon);
  return polygon;
}

std::list<Point_2> WorkspaceGenerator::getRandomPoints(const Polygon_2& containingPolygon, const int& nmbrPoints) {
  CGAL::Bbox_2 bbox = containingPolygon.bbox();
  std::list<Point_2>   point_set;
  for (int i = 0; i < nmbrPoints; i++)
  {
    //K::FT randomX = bbox.xmin() + static_cast<K::FT>(std::rand()) / RAND_MAX * (bbox.xmax() - bbox.xmin());
    //K::FT randomY = bbox.ymin() + static_cast<K::FT>(std::rand()) / RAND_MAX * (bbox.ymax() - bbox.ymin());

    K::FT randomX = RandomGenerator::getRandomDouble(bbox.xmin(), bbox.xmax());
    K::FT randomY = RandomGenerator::getRandomDouble(bbox.ymin(), bbox.ymax());

    Point_2 randomPoint = Point_2(randomX, randomY);
    if (CGAL::bounded_side_2(containingPolygon.vertices_begin(), containingPolygon.vertices_end(), randomPoint, K()) == CGAL::ON_BOUNDED_SIDE)
    {
      point_set.push_back(randomPoint);
    }
  }
  return point_set;
}

std::vector<int> WorkspaceGenerator::getRandomPointDistribution(const int& nmbrOfSections, const int& nmbrPoints) {
  std::vector<int> pointsInSections(nmbrOfSections, 0); // Initialize all baskets with 0 apples

  // Distribute apples randomly into baskets
  for (int i = 0; i < nmbrPoints; i++) {
    int randomBasket = RandomGenerator::getRandomInt(0, nmbrOfSections - 1);
    pointsInSections[randomBasket]++;
  }

  return pointsInSections;
}