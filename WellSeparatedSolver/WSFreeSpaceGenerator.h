//
// Created by joelh on 17/10/2023.
//

#ifndef MRMP_IMPLEMENTATION_WSFREESPACEGENERATOR_H
#define MRMP_IMPLEMENTATION_WSFREESPACEGENERATOR_H

#include "../mytypedefs.h"
//#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

class WSFreeSpaceGenerator
{
public:
  static std::vector<Polygon_2> getFreeSpaceComponents(const Polygon_2& workspacePolygon);
  static Polygon_set_2 getFStar(Polygon_set_2 freeSpaceSet, const std::vector<Point_2>& startConfs, const std::vector<Point_2>& targetConfs);

  WSFreeSpaceGenerator();

private:
  static Polygon_wh_2 getObstaclePolygon(const Polygon_2& boundingBox, const Polygon_2& workspacePolygon);
  //static Pwh_list_2 getFreeSpacePolygon(const Polygon_2& boundingBox, const Polygon_wh_2& free_space_complement_polygon);
};


#endif//MRMP_IMPLEMENTATION_WSFREESPACEGENERATOR_H
