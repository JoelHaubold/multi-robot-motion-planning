//
// Created by joelh on 13/11/2023.
//

#include "MGCreationHelper.h"


using namespace std;


void MGCreationHelper::insertVertices(Motion_Graph &motionGraph, const vector<STConf>& startConfs, const vector<STConf>& targetConfs, STConfId2MGVertex &stId2Vertex)
{
    insertVertices(motionGraph, startConfs, targetConfs, stId2Vertex, {});
}

void MGCreationHelper::insertVertices(Motion_Graph &motionGraph, const vector<STConf>& startConfs, const vector<STConf>& targetConfs, STConfId2MGVertex &stId2Vertex, const vector<Tq>& toIgnore)
{
    std::for_each(startConfs.begin(), startConfs.end(), [&motionGraph, &stId2Vertex](const auto& startConf) {
        //Motion_Graph::vertex_descriptor vd = boost::add_vertex(motionGraph);
        MGVertex v = boost::add_vertex(MGVertexProperty{startConf.id,startConf.location, true}, motionGraph);
        stId2Vertex[startConf.id] = v;
    });

    for(const auto& tConf : targetConfs) {
        bool matchedToIgnore = false;
        for(const auto& toIgn : toIgnore ) {
            if(toIgn.tConf.id == tConf.id) {
                matchedToIgnore = true;
                break;
            }
        }
        if(!matchedToIgnore) {
            MGVertex v = boost::add_vertex(MGVertexProperty{tConf.id,tConf.location, false}, motionGraph);
            stId2Vertex[tConf.id] = v;
        }
    }
//    std::for_each(targetConfs.begin(), targetConfs.end(), [&motionGraph, &stId2Vertex, &toIgnore](const auto& targetConf) {
//        //Motion_Graph::vertex_descriptor vd = boost::add_vertex(motionGraph);
//        for(const auto& toIgn : toIgnore ) {
//
//        }
//
//        MGVertex v = boost::add_vertex(MGVertexProperty{targetConf.id,targetConf.location, false}, motionGraph);
//        stId2Vertex[targetConf.id] = v;
//    });
}

