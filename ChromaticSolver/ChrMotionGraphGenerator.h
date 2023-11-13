//
// Created by joelh on 13/11/2023.
//

#ifndef MRMP_IMPLEMENTATION_CHRMOTIONGRAPHGENERATOR_H
#define MRMP_IMPLEMENTATION_CHRMOTIONGRAPHGENERATOR_H

/*
 * Dif to WS
 *
 */
#include "../chrTypedefs.h"
class ChrMotionGraphGenerator
{
public:
    static std::unordered_map<std::string, Motion_Graph> getMotionGraphs(const std::vector<FreeSpaceComponent>& fSpaceComps, const std::vector<FStarComponent>& fStarComps, const std::vector<Tq> tqs);
};


#endif//MRMP_IMPLEMENTATION_CHRMOTIONGRAPHGENERATOR_H
