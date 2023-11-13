//
// Created by joelh on 07/11/2023.
//

#ifndef MRMP_IMPLEMENTATION_CHRTYPEDEFS_H
#define MRMP_IMPLEMENTATION_CHRTYPEDEFS_H

#include <utility>

#include "mytypedefs.h"


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

struct BlockingArea {
    BlockingArea(std::string baId, Polygon_2 blockingArea, STConf blocker, const std::vector<std::string> &adjacentResComponentIds) : baId(std::move(baId)), blockingArea(std::move(blockingArea)), blocker(std::move(blocker)), adjacentResComponentIds(adjacentResComponentIds) {}

    const std::string baId;
    const Polygon_2 blockingArea;
    const STConf blocker;
    const std::vector<std::string> adjacentResComponentIds;
};

struct ResidualComponent {
    ResidualComponent(std::string id, Polygon_2 resComponentPolygon, const std::vector<STConf> &startConfigurations, const std::vector<STConf> &targetConfigurations) : id(std::move(id)), resComponentPolygon(std::move(resComponentPolygon)), startConfigurations(startConfigurations), targetConfigurations(targetConfigurations) {}

    const std::string id;
    const Polygon_2 resComponentPolygon;
    const std::vector<STConf> startConfigurations;
    const std::vector<STConf> targetConfigurations;
    //const std::vector<BlockingArea> adjacentBlockingAreas;
};

struct ChrFreeSpaceSubsets {
    std::vector<FStarComponent> fStarComponents;
    std::vector<BlockingArea> blockers;
    std::vector<ResidualComponent> residualComponents;
};

/*
 * Target configuration located in the aura of a start configuration
 */
struct Tq {
    Tq(STConf tConf, STConf includingSConf) : tConf(std::move(tConf)), includingSConf(std::move(includingSConf)) {}

    const STConf tConf;
    const STConf includingSConf;
};

struct RCGVertexProperty {
    ResidualComponent& residualComponent;
};
struct RCGEdgeProperty {
    BlockingArea& blockingArea;
};

typedef boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::undirectedS,
    RCGVertexProperty,
    RCGEdgeProperty>                                       ResidualComponentGraph;
typedef boost::graph_traits<DirectedInterferenceForest>::vertex_descriptor DIFVertex;



#endif//MRMP_IMPLEMENTATION_CHRTYPEDEFS_H
