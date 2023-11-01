//
// Created by joelh on 30/10/2023.
//

#ifndef MRMP_IMPLEMENTATION_WSMOTIONGRAPHSOLVER_H
#define MRMP_IMPLEMENTATION_WSMOTIONGRAPHSOLVER_H

#include "../mytypedefs.h"



class WSMotionGraphSolver
{
public:
    static MotionSchedule solveMotionGraphs(std::unordered_map<std::string, Motion_Graph>& mgs);
    static void solveMotionGraphComponent(Motion_Graph& mg, MotionSchedule& ms); //const Directed-interference forest
private:
    static void handleTargetLeaf(Motion_Graph& mg, MotionSchedule& ms, const MGVertex & targetLeaf);
    static void handleStartLeaf(Motion_Graph &mg, MotionSchedule &ms, const MGVertex &startLeaf);
    static std::vector<MGVertex> findNearestVertex(Motion_Graph& mg, const MGVertex & toVertex, bool containingRobot);
    static void doMoveFrom(Motion_Graph& mg, MotionSchedule& ms, std::vector<MGVertex> path);
    static void doMove(MotionSchedule& ms, MGVertexProperty& from, MGVertexProperty& to, const MGEdgeProperty& over);
    static void doChainMove(Motion_Graph &mg, MotionSchedule &ms, std::vector<MGVertex> path);
    static void findAndRemoveLeaf(Motion_Graph &mg, MotionSchedule &ms);
};


#endif//MRMP_IMPLEMENTATION_WSMOTIONGRAPHSOLVER_H
