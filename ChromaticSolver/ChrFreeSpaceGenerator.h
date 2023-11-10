//
// Created by joelh on 07/11/2023.
//

#ifndef MRMP_IMPLEMENTATION_CHRFREESPACEGENERATOR_H
#define MRMP_IMPLEMENTATION_CHRFREESPACEGENERATOR_H

#include "../mytypedefs.h"
class ChrFreeSpaceGenerator
{
public:
    static std::vector<FreeSpaceComponent> getFreeSpaceComponents(const Polygon_2 &workspacePolygon);
    static void associateSTConfs(std::vector<FreeSpaceComponent> &freeSpaceComponents, const std::vector<STConf> &startConfs, const std::vector<STConf> &targetConfs);
};


#endif//MRMP_IMPLEMENTATION_CHRFREESPACEGENERATOR_H
