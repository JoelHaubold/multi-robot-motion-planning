//
// Created by joelh on 17/10/2023.
//

#ifndef MRMP_IMPLEMENTATION_WORKSPACEGENARATOR_H
#define MRMP_IMPLEMENTATION_WORKSPACEGENARATOR_H

#include "mytypedefs.h"

//TODO: Rename to ~RequirementsGenerator
class WorkspaceGenerator {
public:
    static Polygon_2 generateRandomPolygon(int workspaceComplexity, double workspaceSize);
    static STConfigurations getStartAndTargetConfigurations(const std::vector<FreeSpaceComponent>& containingPolygons, int nmbrStartConfs);



  private:
    static Point_2 getRandomPoint(double xmin, double xmax, double ymin, double ymax);
    static bool conformsToChromaticDistance(const Point_2& conf, const std::vector<Point_2>& sameTypeConfs, const std::vector<Point_2>& otherTypeConfs);
    static bool conformsToChromaticDistance(const Point_2& conf, const std::vector<Point_2>& startConfs, const std::vector<Point_2>& targetConfs, bool pointIsStartConf);
};


#endif//MRMP_IMPLEMENTATION_WORKSPACEGENARATOR_H
