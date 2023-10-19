//
// Created by joelh on 17/10/2023.
//

#include "WSFreeSpaceGenerator.h"
#include <CGAL/Boolean_set_operations_2.h>
#include "constants.h"
#include <CGAL/minkowski_sum_2.h>
#include <CGAL/Polygon_vertical_decomposition_2.h>
#include <CGAL/draw_polygon_with_holes_2.h>
#include <CGAL/draw_polygon_2.h>

WSFreeSpaceGenerator::WSFreeSpaceGenerator()  = default;

Polygon_2 WSFreeSpaceGenerator::generateBoundingBox(const CGAL::Bbox_2& bbox) {
  //Bbox_2 bbox = polygon.bbox();
  K::FT minX = bbox.xmin() - OBSTACLE_OFFSET;
  K::FT maxX = bbox.xmax() + OBSTACLE_OFFSET;
  K::FT minY = bbox.ymin() - OBSTACLE_OFFSET;
  K::FT maxY = bbox.ymax() + OBSTACLE_OFFSET;



  std::cout << maxY << std::endl;

  // Create a bounding box that covers the entire plane
  Polygon_2 boundingBox;
  boundingBox.push_back(Point_2(minX, minY));
  boundingBox.push_back(Point_2(maxX, minY));
  boundingBox.push_back(Point_2(maxX, maxY));
  boundingBox.push_back(Point_2(minX, maxY));

  return boundingBox;
}

Polygon_2 WSFreeSpaceGenerator::generateRobot() {

  Polygon_2 robot;
  robot.push_back(Point_2(-ROBOT_SIZE / 2, -ROBOT_SIZE / 2));
  robot.push_back(Point_2(ROBOT_SIZE / 2, - ROBOT_SIZE / 2));
  robot.push_back(Point_2(ROBOT_SIZE / 2, ROBOT_SIZE / 2));
  robot.push_back(Point_2(-ROBOT_SIZE / 2, ROBOT_SIZE / 2));

  return robot;
}

Polygon_wh_2 WSFreeSpaceGenerator::getObstaclePolygon(const Polygon_2& boundingBox, const Polygon_2& workspacePolygon) {
  Polygon_wh_2 uncheckedObstaclePolygon;
  Pwh_list_2 obstaclePolygons;
  CGAL::difference(boundingBox, workspacePolygon,
                   std::back_inserter(obstaclePolygons));
  std::cout << obstaclePolygons.size() << std::endl;
  uncheckedObstaclePolygon = *obstaclePolygons.begin();
  if(uncheckedObstaclePolygon.number_of_holes() != 1) {
    throw std::runtime_error("Obstacle polygon should have exactly one hole" );
  }

  return uncheckedObstaclePolygon;
}

Pwh_list_2 WSFreeSpaceGenerator::getFreeSpacePolygon(const Polygon_2& boundingBox, const Polygon_wh_2& free_space_complement_polygon) {
  Pwh_list_2 freeSpacePolygons;

  CGAL::difference(boundingBox, free_space_complement_polygon,
                   std::back_inserter(freeSpacePolygons));

  return freeSpacePolygons;
}
Pwh_list_2 WSFreeSpaceGenerator::getFreeSpaceComponents(const Polygon_2& workspacePolygon)
{
    // Create a bounding box that covers the entire plane
    Polygon_2 boundingBox = generateBoundingBox(workspacePolygon.bbox());
    CGAL::draw(boundingBox);

    //Calculate obstacle space
    Polygon_wh_2 obstaclePolygon = getObstaclePolygon(boundingBox, workspacePolygon);
    //CGAL::draw(obstaclePolygon);

    // Create robot workspacePolygon
    Polygon_2 robot = generateRobot();

    //Free space complement workspacePolygon
    CGAL::Polygon_vertical_decomposition_2<K, Container>  decomp;
    Polygon_wh_2 free_space_complement_polygon = CGAL::minkowski_sum_2(obstaclePolygon, robot, decomp);
    CGAL::draw(free_space_complement_polygon);

    //  Polygon_wh_2 freeSpacePolygon; TODO: maybe replace with simply getting holes
    Pwh_list_2 freeSpacePolygons = getFreeSpacePolygon(boundingBox, free_space_complement_polygon);

    // Create a vector to hold the holes as Polygon_2 objects
    std::vector<Polygon_2> holesVector;

    // Iterate through the holes and convert them to Polygon_2
    for (auto hole_it = free_space_complement_polygon.holes_begin();
         hole_it != free_space_complement_polygon.holes_end(); ++hole_it) {
        Polygon_2 hole = *hole_it;
        holesVector.push_back(hole);
        CGAL::draw(hole);
    }



    return freeSpacePolygons;
}
