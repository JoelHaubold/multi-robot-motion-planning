//
// Created by joelh on 17/10/2023.
//

#ifndef MRMP_IMPLEMENTATION_MRMPINPUTGENERATOR_H
#define MRMP_IMPLEMENTATION_MRMPINPUTGENERATOR_H

#include "mytypedefs.h"

class MRMPInputGenerator
{
public:
    static Polygon_2 generateRandomPolygon(int workspaceComplexity, double workspaceSize);
    static STConfigurations getStartAndTargetConfigurations(const std::vector<FreeSpaceComponent>& containingPolygons, int nmbrStartConfs);



  private:
    static Point_2 getRandomPoint(double xmin, double xmax, double ymin, double ymax);
    static bool conformsToChromaticDistance(const Point_2& conf, const std::vector<Point_2>& sameTypeConfs, const std::vector<Point_2>& otherTypeConfs, bool singleFreeSpace);
    static bool conformsToChromaticDistance(const Point_2& conf, const std::vector<Point_2>& startConfs, const std::vector<Point_2>& targetConfs, bool pointIsStartConf, bool singleFreeSpace);
};


#endif//MRMP_IMPLEMENTATION_MRMPINPUTGENERATOR_H
