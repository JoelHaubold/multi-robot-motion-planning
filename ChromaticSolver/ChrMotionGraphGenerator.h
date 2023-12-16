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
private:
    static void insertEdges(Motion_Graph &motionGraph, const FStarComponent &fStarComponent, const STConfId2MGVertex &id2Vertex);
    static void getMGForFStarComponent(Motion_Graph& motionGraph, const FStarComponent &fStarComponent, const std::vector<MGVertex>& relevantVertices);
};


#endif//MRMP_IMPLEMENTATION_CHRMOTIONGRAPHGENERATOR_H
