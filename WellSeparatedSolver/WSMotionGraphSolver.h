//
// Created by joelh on 30/10/2023.
//

#ifndef MRMP_IMPLEMENTATION_WSMOTIONGRAPHSOLVER_H
#define MRMP_IMPLEMENTATION_WSMOTIONGRAPHSOLVER_H

#include "../mytypedefs.h"

struct MotionSchedule{
    std::unordered_map<std::string, std::string> robots2VerticesTraveled;
    std::string motionSchedule;
};

class WSMotionGraphSolver
{
public:
    static MotionSchedule solveMotionGraphs();
    static void solveMotionGraphComponent(Motion_Graph& mg, MotionSchedule& ms); //const Directed-interference forest
private:
    static void handleTargetLeaf(Motion_Graph& mg, MotionSchedule& ms, const Vertex& targetLeaf);
    static void handleStartLeaf(Motion_Graph &mg, MotionSchedule &ms, const Vertex &startLeaf);
    static std::vector<Vertex> findNearestVertex(Motion_Graph& mg, const Vertex& toVertex, bool containingRobot);
    static void doMoveFrom(Motion_Graph& mg, MotionSchedule& ms, std::vector<Vertex> path);
    static void doMove(MotionSchedule& ms, MGVertexProperty& from, MGVertexProperty& to, const MGEdgeProperty& over);
    static void doChainMove(Motion_Graph &mg, MotionSchedule &ms, std::vector<Vertex> path);
};


#endif//MRMP_IMPLEMENTATION_WSMOTIONGRAPHSOLVER_H
