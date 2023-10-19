//
// Created by joelh on 17/10/2023.
//

#include "WSFreeSpaceGenerator.h"
#include <CGAL/Boolean_set_operations_2.h>
#include "constants.h"

WSFreeSpaceGenerator::WSFreeSpaceGenerator()  = default;

Polygon_2 WSFreeSpaceGenerator::generateBoundingBox(const CGAL::Bbox_2& bbox) {
  //Bbox_2 bbox = polygon.bbox();
  K::FT minX = bbox.xmin() - OBSTACLE_OFFSET;
  K::FT maxX = bbox.xmax() + obstacle_offset;
  K::FT minY = bbox.ymin() - obstacle_offset;
  K::FT maxY = bbox.ymax() + obstacle_offset;



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
  robot.push_back(Point_2(-robot_size / 2, -robot_size / 2));
  robot.push_back(Point_2(robot_size / 2, - robot_size / 2));
  robot.push_back(Point_2(robot_size / 2, robot_size / 2));
  robot.push_back(Point_2(-robot_size / 2, robot_size / 2));

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
Pwh_list_2 WSFreeSpaceGenerator::getFreeSpaceComponents()
{
  return Pwh_list_2();
}
