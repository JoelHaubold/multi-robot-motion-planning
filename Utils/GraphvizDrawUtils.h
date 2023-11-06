//
// Created by joelh on 27/10/2023.
//

#ifndef MRMP_IMPLEMENTATION_GRAPHVIZDRAWUTILS_H
#define MRMP_IMPLEMENTATION_GRAPHVIZDRAWUTILS_H

#include "../mytypedefs.h"
class GraphvizDrawUtils
{
public:
    static void drawMotionGraph(const Motion_Graph& motionGraph, const std::string& location);
    static void drawMotionGraphs(const std::unordered_map<std::string, Motion_Graph>& mgs);
    static void drawDIForest(const DirectedInterferenceForest &dif, const std::string &location);
};


#endif//MRMP_IMPLEMENTATION_GRAPHVIZDRAWUTILS_H
