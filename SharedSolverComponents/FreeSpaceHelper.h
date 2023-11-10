//
// Created by joelh on 07/11/2023.
//

#ifndef MRMP_IMPLEMENTATION_FREESPACEHELPER_H
#define MRMP_IMPLEMENTATION_FREESPACEHELPER_H

#include "../mytypedefs.h"

class FreeSpaceHelper
{
public:
    static std::vector<FreeSpaceComponent> getFreeSpaceComponents(const Polygon_2& workspacePolygon);
    static void associateSTConfs(std::vector<FreeSpaceComponent> &freeSpaceComponents, const std::vector<STConf> &startConfs, const std::vector<STConf> &targetConfs);
private:
    static Polygon_wh_2 getObstaclePolygon(const Polygon_2 &boundingBox, const Polygon_2 &workspacePolygon);
};


#endif//MRMP_IMPLEMENTATION_FREESPACEHELPER_H
