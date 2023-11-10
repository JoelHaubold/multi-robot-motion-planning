//
// Created by joelh on 17/10/2023.
//

#include "WSFreeSpaceGenerator.h"
#include "../Utils/Utils.h"
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/Polygon_vertical_decomposition_2.h>
//#include <CGAL/draw_polygon_set_2.h>
//#include <CGAL/draw_polygon_with_holes_2.h>
#include "../SharedSolverComponents/FreeSpaceHelper.h"
#include <CGAL/minkowski_sum_2.h>

//Polygon_wh_2 WSFreeSpaceGenerator::getObstaclePolygon(const Polygon_2& boundingBox, const Polygon_2& workspacePolygon) {
//  Polygon_wh_2 uncheckedObstaclePolygon;
//  Pwh_list_2 obstaclePolygons;
//  CGAL::difference(boundingBox, workspacePolygon,
//                   std::back_inserter(obstaclePolygons));
//  uncheckedObstaclePolygon = *obstaclePolygons.begin();
//  if(uncheckedObstaclePolygon.number_of_holes() != 1) {
//    throw std::runtime_error("Obstacle polygon should have exactly one hole" );
//  }
//
//  return uncheckedObstaclePolygon;
//}

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
    return FreeSpaceHelper::getFreeSpaceComponents(workspacePolygon);
}


//Ray Casting Algorithm has complexity O(n) per point
void WSFreeSpaceGenerator::associateSTConfs(std::vector<FreeSpaceComponent>& freeSpaceComponents, const std::vector<STConf>& startConfs, const std::vector<STConf>& targetConfs)
{
    FreeSpaceHelper::associateSTConfs(freeSpaceComponents, startConfs, targetConfs);
}


//std::vector<FStarComponent> WSFreeSpaceGenerator::getFStar(const std::vector<FreeSpaceComponent>& freeSpaces)//, Polygon_set_2 freeSpaceSet, const std::vector<Point_2>& startConfs, const std::vector<Point_2>& targetConfs)
//{
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

/**
 *
 * Only iterate over all st configurations once. Also check if included before joining and create aura only once.
 * @param freeSpaces
 * @return
 */
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
//    long long difTime = 0;
//    long long joinTime = 0;
//    long long effTime = 0;
//    long long creationTime = 0;
    for(const auto& freeSpace : freeSpaces)
    {
        //std::chrono::high_resolution_clock::time_point difStart = std::chrono::high_resolution_clock::now();
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
//        std::chrono::high_resolution_clock::time_point difEnd = std::chrono::high_resolution_clock::now();
//        difTime += std::chrono::duration_cast<std::chrono::microseconds >(difEnd - difStart).count();

        std::vector<Polygon_wh_2> fStarPolygons;
        freeSpaceSet.polygons_with_holes(std::back_inserter(fStarPolygons));

//        std::unordered_map<int, std::vector<STConf>> adjSConfs_;
//        std::unordered_map<int, std::vector<STConf>> adjTConfs_;

//        std::chrono::high_resolution_clock::time_point joinEnd = std::chrono::high_resolution_clock::now();
//        joinTime += std::chrono::duration_cast<std::chrono::microseconds >(joinEnd - difEnd).count();
        std::unordered_map<int, std::vector<STConf>> adjSConfs;
        std::unordered_map<int, std::vector<STConf>> adjTConfs;
//        associateByJoining(fStarPolygons, freeSpace.startConfigurations, adjSConfs);
//        associateByJoining(fStarPolygons, freeSpace.targetConfigurations, adjTConfs);
        associateEfficiently(fStarPolygons, freeSpace.startConfigurations, adjSConfs);
        associateEfficiently(fStarPolygons, freeSpace.targetConfigurations, adjTConfs);

//        for(const auto& sConf : freeSpace.startConfigurations) {
//            const Polygon_2 aura = Utils::generateRobotAura(sConf.location);
//            for (int i = 0; i < fStarPolygons.size(); i++) {
//                Bbox_2 bbox = fStarPolygons[i].bbox();
//                if(bbox.xmin() <= sConf.location.x() + ROBOT_SIZE && bbox.xmax() >= sConf.location.x() - ROBOT_SIZE && bbox.ymin() <= sConf.location.y() + ROBOT_SIZE && bbox.ymax() >= sConf.location.y() - ROBOT_SIZE)
//                {
//                    Polygon_wh_2 joinResult;
//                    const bool doesIntersect = CGAL::join(fStarPolygons[i], aura, joinResult);
//                    if (doesIntersect)
//                    {
//                        adjSConfs[i].push_back(sConf);
//                    }
//                }
//            }
//        }
//        for(const auto& tConf : freeSpace.targetConfigurations) {
//            const Polygon_2 aura = Utils::generateRobotAura(tConf.location);
//            for (int i = 0; i < fStarPolygons.size(); i++) {
//                Bbox_2 bbox = fStarPolygons[i].bbox();
//                if(bbox.xmin() <= tConf.location.x() + ROBOT_SIZE && bbox.xmax() >= tConf.location.x() - ROBOT_SIZE && bbox.ymin() <= tConf.location.y() + ROBOT_SIZE && bbox.ymax() >= tConf.location.y() - ROBOT_SIZE){
//                    Polygon_wh_2 joinResult;
//                    std::chrono::high_resolution_clock::time_point indJoinStart = std::chrono::high_resolution_clock::now();
//                    const bool doesIntersect = CGAL::join(fStarPolygons[i], aura, joinResult);
//                    std::chrono::high_resolution_clock::time_point indJoinEnd = std::chrono::high_resolution_clock::now();
//                    std::cout << "Ind join: " << std::chrono::duration_cast<std::chrono::microseconds >(indJoinEnd - indJoinStart).count() << std::endl;
//                    if(doesIntersect) {
//                        adjTConfs[i].push_back(tConf);
//                    }
//                }
//            }
//        }



//        std::chrono::high_resolution_clock::time_point effEnd = std::chrono::high_resolution_clock::now();
//        effTime += std::chrono::duration_cast<std::chrono::microseconds >(effEnd - joinEnd).count();

        for (int i = 0; i < fStarPolygons.size(); i++) {
            fStarNumber++;
            if(adjSConfs[i].size() + adjTConfs[i].size() <= 1) {
                continue; // FStar component is not relevant
            }
            fStarComponents.emplace_back(fStarPolygons[i], adjSConfs[i], adjTConfs[i], F_STAR_PREFIX+ std::to_string(fStarNumber), freeSpace);

            //CGAL::draw(fStarPoly);
        }
//        creationTime += std::chrono::duration_cast<std::chrono::microseconds >(std::chrono::high_resolution_clock::now() - effEnd).count();

    }
//    std::cout << "DifTime: " << difTime << std::endl; TODO: Make subchapter out of this
//    std::cout << "JoinTime: " << joinTime << std::endl;
//    std::cout << "EffTime: " << effTime << std::endl;
//    std::cout << "CreationTime: " << creationTime << std::endl;

    //std::cout << std::to_string(fStarComponents.size()) << std::endl;

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
void WSFreeSpaceGenerator::associateByJoining(const std::vector<Polygon_wh_2> &fStarPolys, const std::vector<STConf> &confLocations, std::unordered_map<int, std::vector<STConf>> &associationMap)
{
    for(const auto& stConf : confLocations) {
        const Polygon_2 aura = Utils::generateRobotAura(stConf.location);
        for (int i = 0; i < fStarPolys.size(); i++) {
            Bbox_2 bbox = fStarPolys[i].bbox();
            if(bbox.xmin() <= stConf.location.x() + ROBOT_SIZE && bbox.xmax() >= stConf.location.x() - ROBOT_SIZE && bbox.ymin() <= stConf.location.y() + ROBOT_SIZE && bbox.ymax() >= stConf.location.y() - ROBOT_SIZE)
            {
                Polygon_wh_2 joinResult;
                const bool doesIntersect = CGAL::join(fStarPolys[i], aura, joinResult);
                if (doesIntersect)
                {
                    associationMap[i].push_back(stConf);
                }
            }
        }
    }
}
bool WSFreeSpaceGenerator::haveOverlappingEdges(const Polygon_2& poly1, const Polygon_2& poly2)
{
    for (const auto& polyEdge : poly1.edges()) {
        for(const auto& auraEdge : poly2.edges()) {
            if(CGAL::do_intersect(polyEdge, auraEdge)) {
                return true;
            }
        }
    }
    return false;
}
void WSFreeSpaceGenerator::associateEfficiently(const std::vector<Polygon_wh_2> &fStarPolys, const std::vector<STConf> &confLocations, std::unordered_map<int, std::vector<STConf>> &associationMap)
{
    for(const auto& stConf : confLocations) {
        const Polygon_2 aura = Utils::generateRobotAura(stConf.location);
        for (int i = 0; i < fStarPolys.size(); i++) {
            Bbox_2 bbox = fStarPolys[i].bbox();
            if(bbox.xmin() <= stConf.location.x() + ROBOT_SIZE && bbox.xmax() >= stConf.location.x() - ROBOT_SIZE && bbox.ymin() <= stConf.location.y() + ROBOT_SIZE && bbox.ymax() >= stConf.location.y() - ROBOT_SIZE){
                const bool isContainedIn = fStarPolys[i].outer_boundary().has_on_bounded_side(stConf.location);
                if(isContainedIn) {
                    associationMap[i].push_back(stConf);
                    break;
                } else {
                    if(haveOverlappingEdges(fStarPolys[i].outer_boundary(), aura)) {
                        associationMap[i].push_back(stConf);
                    }
                }
            }
        }
    }
}
