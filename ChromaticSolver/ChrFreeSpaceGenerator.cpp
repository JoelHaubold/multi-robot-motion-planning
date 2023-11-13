//
// Created by joelh on 07/11/2023.
//

#include "ChrFreeSpaceGenerator.h"
#include "../SharedSolverComponents/FreeSpaceHelper.h"
#include "../Utils/Utils.h"
#include "../chrTypedefs.h"
#include <CGAL/Boolean_set_operations_2/difference.h>
#include <CGAL/Boolean_set_operations_2/intersection.h>
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
ChrFreeSpaceSubsets ChrFreeSpaceGenerator::getResidualComponents(const std::vector<FreeSpaceComponent>& fSpaceComponents) {
//    std::vector<BlockingArea> blockingAreas;
//    std::vector<ResidualComponent> resComponents;
//    std::vector<FStarComponent> fStarComponents;
    ChrFreeSpaceSubsets subsets;
    int nmbrResComponents = 0;
    int nmbrBlockingAreas = 0;
    int nmbrFStarComps = 0;
    for(const auto& fSpaceComp : fSpaceComponents) {
        Polygon_set_2 sAuraSet;
        for(const auto& startLocation : fSpaceComp.startConfigurations) {
            Polygon_2 auraPolygon = Utils::generateRobotAura(startLocation.location);
            sAuraSet.insert(auraPolygon);
            //            startSet.insert(auraPolygon);
        }
        std::vector<std::tuple<Polygon_2, STConf>> remoteComponentsWithBlockers; //TODO: Decide form to save themm in
        Polygon_set_2 remoteComponentSet;

        for(const auto& tConf : fSpaceComp.targetConfigurations) {
            Polygon_set_2 remoteComponentsOfTConf;
            remoteComponentsOfTConf.insert(Utils::generateRobotAura(tConf.location));
            remoteComponentsOfTConf.intersection(fSpaceComp.freeSpaceComponent);
//            Polygon_2 auraInFreeSpace = CGAL::intersection(Utils::generateRobotAura(tConf.location), fSpaceComp.freeSpaceComponent);

            remoteComponentsOfTConf.difference(sAuraSet);
            if(remoteComponentsOfTConf.number_of_polygons_with_holes() == 1) {
                continue; // No remote components
            }
            std::vector<Polygon_wh_2> potRemoteComponents;
            remoteComponentsOfTConf.polygons_with_holes(std::back_inserter(potRemoteComponents));

            for(const Polygon_wh_2& remoteComponent: potRemoteComponents) {
                if(remoteComponent.has_holes()) {
                    throw std::runtime_error("Remote Components should not have any holes!");
                }
                Polygon_2 potRemoteComponent = remoteComponent.outer_boundary();
                if(potRemoteComponent.has_on_unbounded_side(tConf.location)) {
                    remoteComponentsWithBlockers.push_back(std::make_tuple(potRemoteComponent, tConf));
                    remoteComponentSet.insert(potRemoteComponent);
                }
            }
        }

        Polygon_set_2 fStar;
        fStar.insert(fSpaceComp.freeSpaceComponent);
        fStar.difference(remoteComponentSet);
        //Extract FMinuS
        std::vector<Polygon_wh_2> fMinusPolygons;
        fStar.polygons_with_holes(std::back_inserter(fMinusPolygons));

        std::unordered_map<int, std::vector<STConf>> startAssociations;
        std::unordered_map<int, std::vector<STConf>> targetAssociations;
        FreeSpaceHelper::associateEfficiently(fMinusPolygons, fSpaceComp.startConfigurations, startAssociations);
        FreeSpaceHelper::associateEfficiently(fMinusPolygons, fSpaceComp.targetConfigurations, targetAssociations);

        std::vector<int> indicesOfRelevantResComponents;
        for(int i = 0; i < fMinusPolygons.size(); i++) {
            nmbrResComponents++;
            if(fMinusPolygons[i].has_holes()) {
                throw std::runtime_error("Residual components should not have any holes!");
            }
            indicesOfRelevantResComponents.push_back(subsets.residualComponents.size());
            subsets.residualComponents.emplace_back(RESIDUAL_PREFIX + std::to_string(nmbrResComponents), fMinusPolygons[i].outer_boundary(), startAssociations[i], targetAssociations[i]);

        }

        for(const auto& remoteComp : remoteComponentsWithBlockers) {
            std::vector<int> borderingResComponents;
            Polygon_2 remoteCompPoly = std::get<0>(remoteComp);
            for(const int& index : indicesOfRelevantResComponents) {
                if(Utils::haveOverlappingEdges(subsets.residualComponents[index].resComponentPolygon, remoteCompPoly)) {
                    borderingResComponents.push_back(index);
                }
            }
            if(borderingResComponents.size() > 1) {
                std::vector<std::string> assocResComponentIds;
                for(const int& index : indicesOfRelevantResComponents) {
                    assocResComponentIds.push_back(subsets.residualComponents[index].id);
                }
                STConf blocker = std::get<1>(remoteComp);
                subsets.blockers.emplace_back(BLOCKER_PREFIX+ std::to_string(nmbrBlockingAreas++), remoteCompPoly, blocker, assocResComponentIds);
            }
        }


        fStar.difference(sAuraSet);
        std::vector<Polygon_wh_2> fStarPolygons;
        fStar.polygons_with_holes(std::back_inserter(fStarPolygons)); //TODO: Maybe generate fstar from fminus instead of free space (for runtime)

        std::unordered_map<int, std::vector<STConf>> adjSConfs;
        std::unordered_map<int, std::vector<STConf>> adjTConfs;
        FreeSpaceHelper::associateEfficiently(fStarPolygons, fSpaceComp.startConfigurations, adjSConfs);
        FreeSpaceHelper::associateEfficiently(fStarPolygons, fSpaceComp.targetConfigurations, adjTConfs);

        for (int i = 0; i < fStarPolygons.size(); i++) {
            nmbrFStarComps++;
            if(adjSConfs[i].size() + adjTConfs[i].size() <= 1) {
                continue; // FStar component is not relevant
            }
            subsets.fStarComponents.emplace_back(fStarPolygons[i], adjSConfs[i], adjTConfs[i], F_STAR_PREFIX+ std::to_string(nmbrFStarComps), fSpaceComp);
            //CGAL::draw(fStarPoly);
        }

    }
    //Calculate remote components

    return subsets;
}

//Worst case Runtime m^2 could be improved by first sorting into structure
std::vector<Tq> ChrFreeSpaceGenerator::getTq(const std::vector<FreeSpaceComponent> &freeSpaceComponents)
{
    std::vector<Tq> tqs;
    for(const auto& fSpace : freeSpaceComponents) {
        for(const auto& tConf : fSpace.targetConfigurations) {
            for(const auto& sConf : fSpace.startConfigurations) {
                if(tConf.location.x() < sConf.location.x() + ROBOT_SIZE && tConf.location.x() > sConf.location.x() - ROBOT_SIZE
                    && tConf.location.y() < sConf.location.y() + ROBOT_SIZE&& tConf.location.x() > sConf.location.x() - ROBOT_SIZE) {
                    tqs.emplace_back(tConf, sConf);
                    break; //Monochromatic separation allows only one including start configuration
                }

            }
        }
    }
    return tqs;
}
