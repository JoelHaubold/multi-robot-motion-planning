//
// Created by joelh on 17/10/2023.
//

#include "WorkspaceGenarator.h"
#include <CGAL/random_polygon_2.h>
#include <iostream>
#include "constants.h"
#include "RandomGenerator.h"

WorkspaceGenerator::WorkspaceGenerator() = default;

Polygon_2 WorkspaceGenerator::generateRandomPolygon() {
  Polygon_2            polygon;
  std::list<Point_2>   point_set;
  //CGAL::Random         rand;
  //std::cerr << "Seed = " <<  rand.get_seed() << std::endl;
  int size = RandomGenerator::getRandomInt(4, MAX_POLY_SIZE);
  // copy size points from the generator, eliminating duplicates, so the
  // polygon will have <= size vertices
  CGAL::copy_n_unique(WorkspaceGenerator::Point_generator(RADIUS), size,
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

std::list<Point_2> WorkspaceGenerator::getRandomPoints(const std::vector<Polygon_2>& containingPolygons) {
    double xmin;
    double xmax;
    double ymin;
    double ymax;
    for (const Polygon_2& poly : containingPolygons) {
        Bbox_2 bbox = poly.bbox();
        if (bbox.xmin() < xmin) {
            xmin = bbox.xmin();
        }
        if (bbox.xmax() < xmax) {
            xmax = bbox.xmax();
        }
        if (bbox.ymin() < ymin) {
            ymin = bbox.ymin();
        }
        if (bbox.ymax() < ymax) {
            ymax = bbox.ymax();
        }
    }

    std::list<Point_2>   point_set;
    int pointsGenerated = 0;
    while(point_set.size() < NMBR_START_POS && pointsGenerated < 500)
    {
        K::FT randomX = RandomGenerator::getRandomDouble(xmin, xmax);
        K::FT randomY = RandomGenerator::getRandomDouble(ymin, ymax);

        Point_2 randomPoint = Point_2(randomX, randomY);
        for (const Polygon_2& poly : containingPolygons) {
            if (CGAL::bounded_side_2(poly.vertices_begin(), poly.vertices_end(), randomPoint,
                                     K()) == CGAL::ON_BOUNDED_SIDE) {
                point_set.push_back(randomPoint);
                break;
            }
            pointsGenerated++;
        }
    }
    std::cout << "Generated " << pointsGenerated << " points for" << NMBR_START_POS << "points"
              << std::endl;

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