//
// Created by joelh on 17/10/2023.
//

#include "WSFreeSpaceGenerator.h"
#include "../Utils/Utils.h"
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/Polygon_vertical_decomposition_2.h>
#include <CGAL/draw_polygon_set_2.h>
#include <CGAL/draw_polygon_with_holes_2.h>
#include <CGAL/minkowski_sum_2.h>

WSFreeSpaceGenerator::WSFreeSpaceGenerator()  = default;

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

//Pwh_list_2 WSFreeSpaceGenerator::getFreeSpacePolygon(const Polygon_2& boundingBox, const Polygon_wh_2& free_space_complement_polygon) {
//  Pwh_list_2 freeSpacePolygons;
//
//  CGAL::difference(boundingBox, free_space_complement_polygon,
//                   std::back_inserter(freeSpacePolygons));
//
//  return freeSpacePolygons;
//}
std::vector<Polygon_2> WSFreeSpaceGenerator::getFreeSpaceComponents(const Polygon_2& workspacePolygon)
{
    // Create a bounding box that covers the entire plane
    Polygon_2 boundingBox = Utils::generateBoundingBox(workspacePolygon.bbox());
    //CGAL::draw(boundingBox);

    //Calculate obstacle space
    Polygon_wh_2 obstaclePolygon = getObstaclePolygon(boundingBox, workspacePolygon);
    //CGAL::draw(obstaclePolygon);

    // Create robot workspacePolygon
    Polygon_2 robot = Utils::generateRobot();

    //Free space complement workspacePolygon
    CGAL::Polygon_vertical_decomposition_2<K, Container>  decomp;
    Polygon_wh_2 free_space_complement_polygon = CGAL::minkowski_sum_2(obstaclePolygon, robot, decomp);
    CGAL::draw(free_space_complement_polygon);

    //  Polygon_wh_2 freeSpacePolygon;
    //Pwh_list_2 freeSpacePolygons = getFreeSpacePolygon(boundingBox, free_space_complement_polygon);

    // Create a vector to hold the holes as Polygon_2 objects
    std::vector<Polygon_2> freeSpaces;

    // Iterate through the holes and convert them to Polygon_2
    for (auto hole_it = free_space_complement_polygon.holes_begin(); hole_it != free_space_complement_polygon.holes_end(); ++hole_it) {
        Polygon_2 hole = *hole_it;
        freeSpaces.push_back(hole);
        //CGAL::draw(hole);
    }



    return freeSpaces;
}
Polygon_set_2 WSFreeSpaceGenerator::getFStar(Polygon_set_2 freeSpaceSet, const std::vector<Point_2>& startConfs, const std::vector<Point_2>& targetConfs)
{
    Polygon_set_2 auraSet;
    Polygon_set_2 startSet;
    Polygon_set_2 targetSet;
    std::for_each(startConfs.begin(), startConfs.end(), [&auraSet, &startSet](const Point_2& startConf){
        Polygon_2 auraPolygon = Utils::generateRobotAura(startConf);
        auraSet.insert(auraPolygon);
        startSet.insert(auraPolygon);
    });
    std::for_each(targetConfs.begin(), targetConfs.end(), [&auraSet, &targetSet](const Point_2& targetConf){
        Polygon_2 auraPolygon = Utils::generateRobotAura(targetConf);
        auraSet.insert(auraPolygon);
        targetSet.insert(auraPolygon);
    });

    freeSpaceSet.difference(auraSet);
//    CGAL::draw(startSet);
//    CGAL::draw(targetSet);
    CGAL::draw(freeSpaceSet);
//    std::vector<Polygon_wh_2> individualPolygons;
//    std::vector<Polygon_wh_2> startPolygons;
//    std::vector<Polygon_wh_2> targetPolygons;
//    freeSpaceSet.polygons_with_holes(std::back_inserter(individualPolygons));
//    startSet.polygons_with_holes(std::back_inserter(startPolygons));
//    targetSet.polygons_with_holes(std::back_inserter(targetPolygons));
//    for (const auto& poly_with_holes : individualPolygons) {
//        for (const auto& startAura : startPolygons)
//        {
//            bool doesIntersect = true;//CGAL::do_intersect(poly_with_holes, startAura);
//            std::cout << doesIntersect << std::endl;
//        }
//        for (const auto& targetAura : targetPolygons)
//        {
//            bool doesIntersect = true;//CGAL::do_intersect(poly_with_holes, targetAura);
//            std::cout << doesIntersect << std::endl;
//        }
//    }


    return freeSpaceSet;
}
