//
// Created by joelh on 17/10/2023.
//

#ifndef MRMP_IMPLEMENTATION_WSFREESPACEGENERATOR_H
#define MRMP_IMPLEMENTATION_WSFREESPACEGENERATOR_H

#include "mytypedefs.h"
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>

class WSFreeSpaceGenerator
{
public:
  static Pwh_list_2 getFreeSpaceComponents(const Polygon_2& workspacePolygon);

  WSFreeSpaceGenerator();

private:
  static Polygon_2 generateBoundingBox(const CGAL::Bbox_2& bbox);
  static Polygon_2 generateRobot();
  static Polygon_wh_2 getObstaclePolygon(const Polygon_2& boundingBox, const Polygon_2& workspacePolygon);
  Pwh_list_2 getFreeSpacePolygon(const Polygon_2& boundingBox, const Polygon_wh_2& free_space_complement_polygon);
};


#endif//MRMP_IMPLEMENTATION_WSFREESPACEGENERATOR_H
