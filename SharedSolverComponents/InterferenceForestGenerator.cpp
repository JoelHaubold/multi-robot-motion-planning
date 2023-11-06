//
// Created by joelh on 05/11/2023.
//

#include "InterferenceForestGenerator.h"
#include "../Utils/Utils.h"
#include <CGAL/Boolean_set_operations_2.h>
#include <boost/graph/adjacency_list.hpp>
DirectedInterferenceForest InterferenceForestGenerator::getInterferenceForest(const std::vector<FreeSpaceComponent>& fSpaceComponents)
{
    DirectedInterferenceForest diForest;
    std::map<std::string, DIFVertex> fsId2Vertex;
    for(const auto& fSpaceComp : fSpaceComponents) {
        fsId2Vertex[fSpaceComp.freeSpaceId] = boost::add_vertex({fSpaceComp.freeSpaceId}, diForest);
    }


    for(const auto& fSpaceComp : fSpaceComponents)
    {
        for (const auto &fSpaceComp2: fSpaceComponents)
        {
            if(fSpaceComp.freeSpaceId == fSpaceComp2.freeSpaceId)
            {
                continue;
            }
            bool foundInterferingSTConf = false;
            for (const auto &sConf: fSpaceComp2.startConfigurations)
            {
                if(!foundInterferingSTConf) { // Only one STConf of fSpaceComp2 can interfere with fSpaceComp
                    Polygon_2 aura = Utils::generateRobotAura(sConf.location);
                    if(CGAL::do_intersect(aura, fSpaceComp.freeSpaceComponent))
                    {
                        boost::add_edge(fsId2Vertex[fSpaceComp2.freeSpaceId], fsId2Vertex[fSpaceComp.freeSpaceId], diForest);
                        foundInterferingSTConf = true;
                    }
                }
            }
            for (const auto &sConf: fSpaceComp2.targetConfigurations)
            {
                if(!foundInterferingSTConf) { // Only one STConf of fSpaceComp2 can interfere with fSpaceComp
                    Polygon_2 aura = Utils::generateRobotAura(sConf.location);
                    if(CGAL::do_intersect(aura, fSpaceComp.freeSpaceComponent))
                    {
                        boost::add_edge(fsId2Vertex[fSpaceComp.freeSpaceId], fsId2Vertex[fSpaceComp2.freeSpaceId], diForest);
                        foundInterferingSTConf = true;
                    }
                }
            }
        }
    }
    return diForest;
}
