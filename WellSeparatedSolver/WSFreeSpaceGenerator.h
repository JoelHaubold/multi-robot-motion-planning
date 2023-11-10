//
// Created by joelh on 17/10/2023.
//

#ifndef MRMP_IMPLEMENTATION_WSFREESPACEGENERATOR_H
#define MRMP_IMPLEMENTATION_WSFREESPACEGENERATOR_H

#include "../mytypedefs.h"
#include <unordered_map>
//#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

class WSFreeSpaceGenerator
{
public:
    static std::vector<FreeSpaceComponent> getFreeSpaceComponents(const Polygon_2 &workspacePolygon);
    //static std::vector<FStarComponent> getFStar(const std::vector<FreeSpaceComponent>& freeSpaceSet);
    static std::vector<FStarComponent> getFStar2(const std::vector<FreeSpaceComponent> &freeSpaceSet);
    static void associateSTConfs(std::vector<FreeSpaceComponent> &freeSpaceComponents, const std::vector<STConf> &startConfs, const std::vector<STConf> &targetConfs);
    //static void associateFStars(const std::vector<FreeSpaceComponent> &freeSpaceComponents, std::vector<FStarComponent> &fStarComponents, const std::vector<Point_2> &startConfs, const std::vector<Point_2> &targetConfs);


private:
    //static Polygon_wh_2 getObstaclePolygon(const Polygon_2 &boundingBox, const Polygon_2 &workspacePolygon);
    static std::vector<STConf> getAdjacentConfigurations(const Polygon_wh_2 &fStarComponent, const std::vector<STConf> &confLocations);
    static void associateByJoining(const std::vector<Polygon_wh_2> &fStarPolys,  const std::vector<STConf> &confLocations, std::unordered_map<int, std::vector<STConf>> &associationMap);
    static void associateEfficiently(const std::vector<Polygon_wh_2> &fStarPolys,  const std::vector<STConf> &confLocations, std::unordered_map<int, std::vector<STConf>> &associationMap);
    static bool haveOverlappingEdges(const Polygon_2& poly1, const Polygon_2& poly2); //TODO: In Util/ Shared?
    //static Pwh_list_2 getFreeSpacePolygon(const Polygon_2& boundingBox, const Polygon_wh_2& free_space_complement_polygon);
};


#endif//MRMP_IMPLEMENTATION_WSFREESPACEGENERATOR_H
