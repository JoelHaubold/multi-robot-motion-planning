//
// Created by joelh on 07/11/2023.
//

#include "FreeSpaceHelper.h"
#include "../Utils/Utils.h"
#include <CGAL/Polygon_vertical_decomposition_2.h>
#include <CGAL/minkowski_sum_2.h>

std::vector<FreeSpaceComponent> FreeSpaceHelper::getFreeSpaceComponents(const Polygon_2 &workspacePolygon)
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
void FreeSpaceHelper::associateSTConfs(std::vector<FreeSpaceComponent>& freeSpaceComponents, const std::vector<STConf>& startConfs, const std::vector<STConf>& targetConfs)
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

Polygon_wh_2 FreeSpaceHelper::getObstaclePolygon(const Polygon_2& boundingBox, const Polygon_2& workspacePolygon) {
    Polygon_wh_2 uncheckedObstaclePolygon;
    Pwh_list_2 obstaclePolygons;
    CGAL::difference(boundingBox, workspacePolygon,
                     std::back_inserter(obstaclePolygons));
    uncheckedObstaclePolygon = *obstaclePolygons.begin();
    if(uncheckedObstaclePolygon.number_of_holes() != 1) {
        throw std::runtime_error("Obstacle polygon should have exactly one hole" );
    }

    return uncheckedObstaclePolygon;
}

//bool FreeSpaceHelper::haveOverlappingEdges(const Polygon_2& poly1, const Polygon_2& poly2)
//{
//    for (const auto& polyEdge : poly1.edges()) {
//        for(const auto& auraEdge : poly2.edges()) {
//            if(CGAL::do_intersect(polyEdge, auraEdge)) {
//                return true;
//            }
//        }
//    }
//    return false;
//}
void FreeSpaceHelper::associateEfficiently(const std::vector<Polygon_wh_2> &polygonOptions, const std::vector<STConf> &confLocations, std::unordered_map<int, std::vector<STConf>> &associationMap)
{
    for(const auto& stConf : confLocations) {
        const Polygon_2 aura = Utils::generateRobotAura(stConf.location);
        for (int i = 0; i < polygonOptions.size(); i++) {
            Bbox_2 bbox = polygonOptions[i].bbox();
            if(bbox.xmin() <= stConf.location.x() + ROBOT_SIZE && bbox.xmax() >= stConf.location.x() - ROBOT_SIZE && bbox.ymin() <= stConf.location.y() + ROBOT_SIZE && bbox.ymax() >= stConf.location.y() - ROBOT_SIZE){
                const bool isContainedIn = polygonOptions[i].outer_boundary().has_on_bounded_side(stConf.location);
                if(isContainedIn) {
                    associationMap[i].push_back(stConf);
                    break; //STConf aura is contained fully in polygon -> Can't intersect other polygons
                } else {
                    if(Utils::haveOverlappingEdges(polygonOptions[i].outer_boundary(), aura)) {
                        associationMap[i].push_back(stConf);
                    }
                }
            }
        }
    }
}

