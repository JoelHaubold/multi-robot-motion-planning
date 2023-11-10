//
// Created by joelh on 07/11/2023.
//

#ifndef MRMP_IMPLEMENTATION_CHRTYPEDEFS_H
#define MRMP_IMPLEMENTATION_CHRTYPEDEFS_H

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
    std::string baId;
    Polygon_2& blockingArea;
    STConf& blocker;
};

struct ResidualComponent {
    const std::string id;
    const Polygon_2& resComponentPolygon;
    const std::vector<BlockingArea> adjacentBlockingAreas;
};

struct ChrFComponents {
    const std::vector<FStarComponent> fStarComponents;
    const std::vector<ResidualComponent> fResidualComponents;
};





#endif//MRMP_IMPLEMENTATION_CHRTYPEDEFS_H
