//
// Created by joelh on 07/11/2023.
//

#ifndef MRMP_IMPLEMENTATION_CHRFREESPACEGENERATOR_H
#define MRMP_IMPLEMENTATION_CHRFREESPACEGENERATOR_H

#include "../chrTypedefs.h"
#include "../mytypedefs.h"
class ChrFreeSpaceGenerator
{
public:
    static std::vector<FreeSpaceComponent> getFreeSpaceComponents(const Polygon_2 &workspacePolygon);
    static void associateSTConfs(std::vector<FreeSpaceComponent> &freeSpaceComponents, const std::vector<STConf> &startConfs, const std::vector<STConf> &targetConfs);
    static std::vector<Tq> getTq(const std::vector<FreeSpaceComponent> &freeSpaceComponents);
    static ChrFreeSpaceSubsets getResidualComponents(const std::vector<FreeSpaceComponent>& fSpaceComponents);
};


#endif//MRMP_IMPLEMENTATION_CHRFREESPACEGENERATOR_H
