//
// Created by joelh on 07/11/2023.
//

#include "ChrFreeSpaceGenerator.h"
#include "../SharedSolverComponents/FreeSpaceHelper.h"
#include "../Utils/Utils.h"
#include <CGAL/Boolean_set_operations_2/difference.h>
std::vector<FreeSpaceComponent> ChrFreeSpaceGenerator::getFreeSpaceComponents(const Polygon_2 &workspacePolygon)
{
    return FreeSpaceHelper::getFreeSpaceComponents(workspacePolygon);
}
void ChrFreeSpaceGenerator::associateSTConfs(std::vector<FreeSpaceComponent> &freeSpaceComponents, const std::vector<STConf> &startConfs, const std::vector<STConf> &targetConfs)
{
    return FreeSpaceHelper::associateSTConfs(freeSpaceComponents, startConfs, targetConfs);
}


/*
 * Remote components <- In t aura but only reachable through s aura
 * Blocking area <- Remote component which splits the free space
 * Blocker <- t which has a blocking area
 * Residual component <- Part of the free space split of by a blocking area
 * Residual component graph (Tree; Undirected)<- Edge if seperated by single blocking area whose blocker is in either residual component
 *
 * Need: F_Residuals for graph construction [Each residual comp knows its neighbouring blocking areas]
 * -> Support Struct blocking area: polygon and reference to t
 *
 * Need: F_Star
 *
 * Graph:
 * Vertices have residual component ID/ Maybe reference to residual component itself?
 * Edges have blocking area reference
 */
void getResidualComponents(const std::vector<FreeSpaceComponent> fSpaceComponents) {
    for(const auto& fSpaceComp : fSpaceComponents) {
        Polygon_set_2 sAuraSet;
        for(const auto& startLocation : fSpaceComp.startConfigurations) {
            Polygon_2 auraPolygon = Utils::generateRobotAura(startLocation.location);
            sAuraSet.insert(auraPolygon);
            //            startSet.insert(auraPolygon);
        }
        for(const auto& tConf : fSpaceComp.targetConfigurations) {
            Polygon_set_2 remoteComponents;
            remoteComponents.difference(sAuraSet);
        }
    }
    //Calculate remote components

    //TODO: Maybe create Res Component graph edges from perspective of blocking area (blocking area nows its adjacent Residual Components)
}