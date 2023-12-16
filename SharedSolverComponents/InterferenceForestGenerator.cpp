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

    //TODO: Check first if bounding boxes of free space components are close enough, then maybe if st loc is close enough.
    //TODO: Maybe check first if auras are on edge of free space
    for(const auto& fSpaceComp : fSpaceComponents)
    {
        if(fSpaceComp.startConfigurations.size() + fSpaceComp.startConfigurations.size() == 0) {
            continue;
        }
        for (const auto &fSpaceComp2: fSpaceComponents)
        {
            if(fSpaceComp.freeSpaceId == fSpaceComp2.freeSpaceId)
            {
                continue;
            }
            Bbox_2 bbox1 = fSpaceComp.freeSpaceComponent.bbox();
            Bbox_2 bbox2 = fSpaceComp2.freeSpaceComponent.bbox();
            if(bbox1.xmin() > bbox2.xmax() || bbox1.xmax() < bbox2.xmin() || bbox1.ymin() > bbox2.ymax() || bbox1.ymax() < bbox2.ymin()) {
                continue;
            }
            bool foundInterferingSTConf = false;
            for (const auto &sConf: fSpaceComp2.startConfigurations)
            {
                if(!foundInterferingSTConf) { // Only one STConf of fSpaceComp2 can interfere with fSpaceComp
                    Polygon_2 aura = Utils::generateRobotAura(sConf.location);
                    bbox2 = aura.bbox();
                    if(bbox1.xmin() > bbox2.xmax() || bbox1.xmax() < bbox2.xmin() || bbox1.ymin() > bbox2.ymax() || bbox1.ymax() < bbox2.ymin()) {
                        continue;
                    }
                    if(CGAL::do_intersect(aura, fSpaceComp.freeSpaceComponent))
                    {
                        boost::add_edge(fsId2Vertex[fSpaceComp2.freeSpaceId], fsId2Vertex[fSpaceComp.freeSpaceId], diForest);
                        foundInterferingSTConf = true;
                        break;
                    }
                }
            }
            if(foundInterferingSTConf)
            {
                continue;
            }
            for (const auto &sConf: fSpaceComp2.targetConfigurations)
            {
                if(!foundInterferingSTConf) { // Only one STConf of fSpaceComp2 can interfere with fSpaceComp
                    Polygon_2 aura = Utils::generateRobotAura(sConf.location);
                    bbox2 = aura.bbox();
                    if(bbox1.xmin() > bbox2.xmax() || bbox1.xmax() < bbox2.xmin() || bbox1.ymin() > bbox2.ymax() || bbox1.ymax() < bbox2.ymin()) {
                        continue;
                    }
                    if(CGAL::do_intersect(aura, fSpaceComp.freeSpaceComponent))
                    {
                        boost::add_edge(fsId2Vertex[fSpaceComp.freeSpaceId], fsId2Vertex[fSpaceComp2.freeSpaceId], diForest);
                        foundInterferingSTConf = true;
                        break;
                    }
                }
            }
        }
    }
    return diForest;
}
