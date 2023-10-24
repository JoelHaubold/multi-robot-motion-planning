//
// Created by joelh on 17/10/2023.
//

#ifndef MRMP_IMPLEMENTATION_WORKSPACEGENARATOR_H
#define MRMP_IMPLEMENTATION_WORKSPACEGENARATOR_H


#include <CGAL/point_generators_2.h>
#include <list>
#include <vector>
#include "mytypedefs.h"

//TODO: Rename to ~RequirementsGenerator
class WorkspaceGenerator {
public:
    typedef CGAL::Random_points_in_square_2<Point_2, Creator> Point_generator;

    WorkspaceGenerator();

    static Polygon_2 generateRandomPolygon();
    static std::list<Point_2> getRandomPoints(const Polygon_2& containingPolygon, const int& nmbrPoints);
    static std::vector<int> getRandomPointDistribution(const int& nmbrOfSections, const int& nmbrPoints);

    static void getStartAndTargetConfigurations(const std::vector<Polygon_2>& containingPolygons, std::vector<Point_2>& startConfs, std::vector<Point_2>& targetConfs);
    static std::list<Point_2> getRandomPoints(const std::vector<Polygon_2>& containingPolygons);

  private:
    static Point_2 getRandomPoint(double xmin, double xmax, double ymin, double ymax);
    static bool conformsToChromaticDistance(const Point_2& conf, const std::vector<Point_2>& sameTypeConfs, const std::vector<Point_2>& otherTypeConfs);
    static bool conformsToChromaticDistance(const Point_2& conf, const std::vector<Point_2>& startConfs, const std::vector<Point_2>& targetConfs, bool pointIsStartConf);
};


#endif//MRMP_IMPLEMENTATION_WORKSPACEGENARATOR_H
