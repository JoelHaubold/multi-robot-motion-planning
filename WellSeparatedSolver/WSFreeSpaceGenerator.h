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
  static std::vector<FreeSpaceComponent> getFreeSpaceComponents(const Polygon_2& workspacePolygon);
  //static std::vector<FStarComponent> getFStar(const std::vector<FreeSpaceComponent>& freeSpaceSet);
  static std::vector<FStarComponent> getFStar2(const std::vector<FreeSpaceComponent>& freeSpaceSet);
  static void associateSTConfs(std::vector<FreeSpaceComponent> &freeSpaceComponents, const std::vector<Point_2> &startConfs, const std::vector<Point_2> &targetConfs);
  //static void associateFStars(const std::vector<FreeSpaceComponent> &freeSpaceComponents, std::vector<FStarComponent> &fStarComponents, const std::vector<Point_2> &startConfs, const std::vector<Point_2> &targetConfs);


  private:
  static Polygon_wh_2 getObstaclePolygon(const Polygon_2& boundingBox, const Polygon_2& workspacePolygon);
  static std::vector<Point_2> getAdjacentConfigurations(const Polygon_wh_2& fStarComponent, const std::vector<Point_2>& confLocations);
  //static Pwh_list_2 getFreeSpacePolygon(const Polygon_2& boundingBox, const Polygon_wh_2& free_space_complement_polygon);
};


#endif//MRMP_IMPLEMENTATION_WSFREESPACEGENERATOR_H
