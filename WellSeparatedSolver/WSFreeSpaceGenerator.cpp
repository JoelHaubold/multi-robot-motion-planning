//
// Created by joelh on 17/10/2023.
//

#include "WSFreeSpaceGenerator.h"
#include "../Utils/Utils.h"
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/Polygon_vertical_decomposition_2.h>
//#include <CGAL/draw_polygon_set_2.h>
//#include <CGAL/draw_polygon_with_holes_2.h>
#include <CGAL/minkowski_sum_2.h>
#include "../constants.h"

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
std::vector<FreeSpaceComponent> WSFreeSpaceGenerator::getFreeSpaceComponents(const Polygon_2& workspacePolygon)
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
    //CGAL::draw(free_space_complement_polygon);

    //  Polygon_wh_2 freeSpacePolygon;
    //Pwh_list_2 freeSpacePolygons = getFreeSpacePolygon(boundingBox, free_space_complement_polygon);

    // Create a vector to hold the holes as Polygon_2 objects
    std::vector<FreeSpaceComponent> freeSpaces;

    int nmbr_free_spaces = 0;
    //std::cout << "Iterating:" << std::endl;
    // Iterate through the holes and convert them to Polygon_2
    for (auto hole_it = free_space_complement_polygon.holes_begin(); hole_it != free_space_complement_polygon.holes_end(); ++hole_it) {
        //std::cout << "Iter" << std::endl;
        Polygon_2 hole = *hole_it;
        hole.reverse_orientation(); // Counter clockwise orientation for later set processing
        nmbr_free_spaces++;
        freeSpaces.emplace_back(hole, FREE_SPACE_PREFIX+std::to_string(nmbr_free_spaces));
        //freeSpaces[FREE_SPACE_PREFIX+std::to_string(nmbr_free_spaces)] = hole;
        //CGAL::draw(hole);
    }
    //std::cout << "Stoping iterating:" << std::endl;
    return freeSpaces;
}


//Ray Casting Algorithm has complexity O(n) per point
void WSFreeSpaceGenerator::associateSTConfs(std::vector<FreeSpaceComponent>& freeSpaceComponents, const std::vector<STConf>& startConfs, const std::vector<STConf>& targetConfs)
{
    for(const auto& sConf : startConfs) {
        for(auto& freeSpaceComponent : freeSpaceComponents) {
            if( freeSpaceComponent.freeSpaceComponent.bounded_side(sConf.location) == CGAL::ON_BOUNDED_SIDE) {
                freeSpaceComponent.startConfigurations.push_back(sConf);
            }
        }
    }
    for(const auto& tConf : targetConfs) {
        for(auto& freeSpaceComponent : freeSpaceComponents) {
            if( freeSpaceComponent.freeSpaceComponent.bounded_side(tConf.location) == CGAL::ON_BOUNDED_SIDE) {
                freeSpaceComponent.targetConfigurations.push_back(tConf);
            }
        }
    }

}


//std::vector<FStarComponent> WSFreeSpaceGenerator::getFStar(const std::vector<FreeSpaceComponent>& freeSpaces)//, Polygon_set_2 freeSpaceSet, const std::vector<Point_2>& startConfs, const std::vector<Point_2>& targetConfs)
//{ TODO: Calculating by one big difference
//
//    Polygon_set_2 auraSet;
////    Polygon_set_2 startSet;
////    Polygon_set_2 targetSet;
//    for(const auto& freeSpace : freeSpaces)
//    {
//        for(const auto& startLocation : freeSpace.startConfigurations) {
//            Polygon_2 auraPolygon = Utils::generateRobotAura(startLocation);
//            auraSet.insert(auraPolygon);
////            startSet.insert(auraPolygon);
//        }
//        for(const auto& targetLocation : freeSpace.targetConfigurations) {
//            Polygon_2 auraPolygon = Utils::generateRobotAura(targetLocation);
//            auraSet.insert(auraPolygon);
////            targetSet.insert(auraPolygon);
//        }
//    }
//
//    Polygon_set_2 freeSpaceSet;
//    for(const auto& fComponent : freeSpaces) {
//        freeSpaceSet.insert(fComponent.freeSpaceComponent);
//    }
//    freeSpaceSet.difference(auraSet);
////    CGAL::draw(startSet);
////    CGAL::draw(targetSet);
//    CGAL::draw(freeSpaceSet);
//
//    std::vector<Polygon_wh_2> fStarPolygons;
//    freeSpaceSet.polygons_with_holes(std::back_inserter(fStarPolygons));
//    std::vector<FStarComponent> fStarComponents;
//
//    int fStarNumber = 0;
//
//    for (const auto& fStarPoly : fStarPolygons) {
//        fStarNumber++;
//        fStarComponents.emplace_back(fStarPoly, F_STAR_PREFIX+ std::to_string(fStarNumber));
//    }
//
//    return fStarComponents;
//}

std::vector<FStarComponent> WSFreeSpaceGenerator::getFStar2(const std::vector<FreeSpaceComponent>& freeSpaces)//, Polygon_set_2 freeSpaceSet, const std::vector<Point_2>& startConfs, const std::vector<Point_2>& targetConfs)
{
//    Polygon_set_2 auraSet;
//    //    Polygon_set_2 startSet;
//    //    Polygon_set_2 targetSet;
//    for(const auto& freeSpace : freeSpaces)
//    {
//        for(const auto& startLocation : freeSpace.startConfigurations) {
//            Polygon_2 auraPolygon = Utils::generateRobotAura(startLocation);
//            auraSet.insert(auraPolygon);
//            //            startSet.insert(auraPolygon);
//        }
//        for(const auto& targetLocation : freeSpace.targetConfigurations) {
//            Polygon_2 auraPolygon = Utils::generateRobotAura(targetLocation);
//            auraSet.insert(auraPolygon);
//            //            targetSet.insert(auraPolygon);
//        }
//    }

    int fStarNumber = 0;
    std::vector<FStarComponent> fStarComponents;
    for(const auto& freeSpace : freeSpaces)
    {
        Polygon_set_2 auraSet;
        for(const auto& startLocation : freeSpace.startConfigurations) {
            Polygon_2 auraPolygon = Utils::generateRobotAura(startLocation.location);
            auraSet.insert(auraPolygon);
            //            startSet.insert(auraPolygon);
        }
        for(const auto& targetLocation : freeSpace.targetConfigurations) {
            Polygon_2 auraPolygon = Utils::generateRobotAura(targetLocation.location);
            auraSet.insert(auraPolygon);
            //            targetSet.insert(auraPolygon);
        }
        Polygon_set_2 freeSpaceSet;
        freeSpaceSet.insert(freeSpace.freeSpaceComponent);
        freeSpaceSet.difference(auraSet);

        std::vector<Polygon_wh_2> fStarPolygons;
        freeSpaceSet.polygons_with_holes(std::back_inserter(fStarPolygons));
        for (const auto& fStarPoly : fStarPolygons) {
            //std::cout << "Getting fstar:" << std::endl;
            std::vector<STConf> adjSConfs = getAdjacentConfigurations(fStarPoly, freeSpace.startConfigurations);
            std::vector<STConf> adjTConfs = getAdjacentConfigurations(fStarPoly, freeSpace.targetConfigurations);
            if(adjSConfs.size() + adjTConfs.size() <= 1) {
                continue; // FStar component is not relevant
            }
            fStarNumber++;
            fStarComponents.emplace_back(fStarPoly, adjSConfs, adjTConfs, F_STAR_PREFIX+ std::to_string(fStarNumber), freeSpace);

            //CGAL::draw(fStarPoly);
        }

    }

    std::cout << std::to_string(fStarComponents.size()) << std::endl;

    return fStarComponents;
}

//void WSFreeSpaceGenerator::associateFStars(const std::vector<FreeSpaceComponent> &freeSpaceComponents, std::vector<FStarComponent> &fStarComponents, const std::vector<Point_2> &startConfs, const std::vector<Point_2> &targetConfs) {
//    for(const auto& sConf : startConfs) {
//        for(const auto& fStarComponent : fStarComponents)
//        {
//            Polygon_wh_2 joinResult;
//            bool doesIntersect = CGAL::join(fStarComponent.fStarPolygon, Utils::generateRobotAura(sConf, ROBOT_SIZE), joinResult);
//            if (doesIntersect)
//            {
//                fStarComponent.adjacentSConfs.push_back(sConf);
//            }
//        }
//    }
//    for(const auto& tConf : targetConfs) {
//        for(auto& fStarComponent : fStarComponents)
//        {
//            Polygon_wh_2 joinResult;
//            bool doesIntersect = CGAL::join(fStarComponent.fStarPolygon, Utils::generateRobotAura(tConf, ROBOT_SIZE), joinResult);
//            if (doesIntersect)
//            {
//                fStarComponent.adjacentTConfs.push_back(tConf);
//            }
//        }
//    }
//    fStarComponents.erase(std::remove_if(fStarComponents.begin(), fStarComponents.end(), fStarCompIsEmpty), fStarComponents.end());
//    for(auto& fStarComponent : fStarComponents) {
//        Point_2 firstStartPos = fStarComponent.adjacentSConfs.front();
//        for(const auto& freeSpaceComp : freeSpaceComponents) {
//            auto it = std::find(freeSpaceComp.startConfigurations.begin(), freeSpaceComp.startConfigurations.end(), targetPoint);
//
//            if (it != pointVector.end()) {
//                fStarComponent.parent = &freeSpaceComp;
//                break;
//            }
//        }
//    }
//}
//
//bool fStarCompIsEmpty(const FStarComponent& fStarComp) {
//    return fStarComp.adjacentSConfs.size() + fStarComp.adjacentTConfs.size()<= 1;
//}

std::vector<STConf> WSFreeSpaceGenerator::getAdjacentConfigurations(const Polygon_wh_2& fStarComponent, const std::vector<STConf>& confLocations)
{
    std::vector<STConf> adjacentConfigurations;

    for (const auto& adjConf : confLocations) {
        Polygon_wh_2 joinResult;
        bool doesIntersect = CGAL::join(fStarComponent, Utils::generateRobotAura(adjConf.location), joinResult);
        if(doesIntersect) {
            adjacentConfigurations.push_back(adjConf);
        }
    }
    //std::cout << "ac: "+ std::to_string(adjacentConfigurations.size()) << std::endl;


    return adjacentConfigurations;
}

