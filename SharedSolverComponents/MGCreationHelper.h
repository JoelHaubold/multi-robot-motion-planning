//
// Created by joelh on 13/11/2023.
//

#ifndef MRMP_IMPLEMENTATION_MGCREATIONHELPER_H
#define MRMP_IMPLEMENTATION_MGCREATIONHELPER_H

#include "../mytypedefs.h"
#include "../chrTypedefs.h."

using namespace std;

class MGCreationHelper
{
public:
    static void insertVertices(Motion_Graph &motionGraph, const vector<STConf> &startConfs, const vector<STConf> &targetConfs, STConfId2MGVertex &stId2Vertex, const vector<Tq> &toIgnore);
    static void insertVertices(Motion_Graph &motionGraph, const vector<STConf> &startConfs, const vector<STConf> &targetConfs, STConfId2MGVertex &stId2Vertex);
};


#endif//MRMP_IMPLEMENTATION_MGCREATIONHELPER_H
