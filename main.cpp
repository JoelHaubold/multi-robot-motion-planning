#include <CGAL/draw_polygon_with_holes_2.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <stdexcept>
#include "WorkspaceGenarator.h"
#include "mytypedefs.h"
#include "constants.h"
#include "WSFreeSpaceGenerator.h"

//#ifdef CGAL_USE_GMP
//#include <CGAL/Gmpz.h>
//typedef CGAL::Gmpz RT;
//#else
// NOTE: the choice of double here for a number type may cause problems
//       for degenerate point sets
#include <CGAL/double.h>
//typedef double RT;
//#endif

#include <fstream>
#include <list>

//const double RADIUS = 100;
//const int MAX_POLY_SIZE = 100;
//
//const int OBSTACLE_OFFSET = 5;
//const int ROBOT_SIZE = 4;
//
//const int NMBR_START_POS = 10;
//const int WORKSPACE_COMPLEXITY = 50;

//Polygon_2 generateBoundingBox(const Bbox_2& bbox) {
//  //Bbox_2 bbox = polygon.bbox();
//  K::FT minX = bbox.xmin() - OBSTACLE_OFFSET;
//  K::FT maxX = bbox.xmax() + OBSTACLE_OFFSET;
//  K::FT minY = bbox.ymin() - OBSTACLE_OFFSET;
//  K::FT maxY = bbox.ymax() + OBSTACLE_OFFSET;
////  K::FT minX = polygon.left_vertex()->x() - OBSTACLE_OFFSET;
////  K::FT maxX = polygon.right_vertex()->x() + OBSTACLE_OFFSET;
////  K::FT minY = polygon.bottom_vertex()->y() - OBSTACLE_OFFSET;
////  K::FT maxY = polygon.top_vertex()->y() + OBSTACLE_OFFSET;
//
//
//  std::cout << maxY << std::endl;
//
//  // Create a bounding box that covers the entire plane
//  Polygon_2 boundingBox;
//  boundingBox.push_back(Point_2(minX, minY));
//  boundingBox.push_back(Point_2(maxX, minY));
//  boundingBox.push_back(Point_2(maxX, maxY));
//  boundingBox.push_back(Point_2(minX, maxY));
//
//  return boundingBox;
//}
//
//Polygon_2 generateRobot() {
//
//  Polygon_2 robot;
//  robot.push_back(Point_2(-ROBOT_SIZE / 2, -ROBOT_SIZE / 2));
//  robot.push_back(Point_2(ROBOT_SIZE / 2, - ROBOT_SIZE / 2));
//  robot.push_back(Point_2(ROBOT_SIZE / 2, ROBOT_SIZE / 2));
//  robot.push_back(Point_2(-ROBOT_SIZE / 2, ROBOT_SIZE / 2));
//
//  return robot;
//}
//
//Polygon_wh_2 getObstaclePolygon(const Polygon_2& boundingBox, const Polygon_2& workspacePolygon) {
//  Polygon_wh_2 uncheckedObstaclePolygon;
//  Pwh_list_2 obstaclePolygons;
//  CGAL::difference(boundingBox, workspacePolygon,
//                   std::back_inserter(obstaclePolygons));
//  std::cout << obstaclePolygons.size() << std::endl;
//  uncheckedObstaclePolygon = *obstaclePolygons.begin();
//  if(uncheckedObstaclePolygon.number_of_holes() != 1) {
//    throw std::runtime_error("Obstacle polygon should have exactly one hole" );
//  }
//
//  return uncheckedObstaclePolygon;
//}
//
//Pwh_list_2 getFreeSpacePolygon(const Polygon_2& boundingBox, const Polygon_wh_2& free_space_complement_polygon) {
//  Pwh_list_2 freeSpacePolygons;
//
//  CGAL::difference(boundingBox, free_space_complement_polygon,
//                   std::back_inserter(freeSpacePolygons));
//
//  return freeSpacePolygons;
//}

int main()
{
//  Polygon_2 testPolygon = WorkspaceGenerator::generateRandomPolygon();
//    std::vector<Polygon_2> holes;
//    Polygon_2 outer_polygon;
//    holes.push_back(testPolygon);
//  Polygon_wh_2 testPolygon2(outer_polygon, holes.begin(), holes.end());
//
//CGAL::draw(testPolygon2);

    Polygon_2 workspacePolygon = WorkspaceGenerator::generateRandomPolygon();



    Pwh_list_2 freeSpaceSegments = WSFreeSpaceGenerator::getFreeSpaceComponents(workspacePolygon);

    //CGAL::Holes_container freeSpaceComponents = free_space_complement_polygon.holes();

    //CGAL::draw(free_space_complement_polygon*);

    //WorkspaceGenerator::getRandomPoints(freeSpaceSegments);

    // Create random points for start and target configurations
    // Add as Sq1 to workspace as holes
    // Proceed to generate



    return 0;

    //std::cout << "Hello, World!" << std::endl;
    //return 0;
}
