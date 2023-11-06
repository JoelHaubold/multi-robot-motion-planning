//
// Created by joelh on 30/10/2023.
//

#include "WSMotionGraphSolver.h"
//#include "../Utils/GraphvizDrawUtils.h"
#include "../Utils/StringUtils.h"
#include "../mytypedefs.h"
#include <boost/graph/adjacency_list.hpp>
#include <exception>

/*
 * TODO:
 * Iterate through dif vertices
 * If no incoming edges -> resolve
 * If incoming edges put on stack and try connected vertex
 * Then resolve stack
 * Continue with iteration but ignore already resolved vertices [set of fSpaceId : resolved]
 */
MotionSchedule WSMotionGraphSolver::solveMotionGraphs(std::unordered_map<std::string, Motion_Graph> &mgs, const DirectedInterferenceForest& dif)
{
    std::vector<std::string> mgOrder = getOrderFromDIForest(dif);
    MotionSchedule ms;
    for(const auto& fSpaceId : mgOrder) {
        Motion_Graph mg = mgs[fSpaceId];
        solveMotionGraphComponent(mg, ms);
    }
//    for(const auto& pair : mgs) {
//        Motion_Graph mg = pair.second;
//        solveMotionGraphComponent(mg, ms);
//    }
    //std::cout << ms.motionSchedule << std::endl;
    return ms;
}

std::vector<std::string> WSMotionGraphSolver::getOrderFromDIForest(const DirectedInterferenceForest& dif)
{
    std::vector<std::string> resultVector;
    for(const DIFVertex& difVertex : dif.vertex_set()) {
        if(boost::in_degree(difVertex, dif) > 0) {
            //Has parent in directed-interference forest -> Resolve after parent
            continue;
        } else if(boost::degree(difVertex, dif) == 0) {
            //Is isolated -> Resolve whenever
            resultVector.push_back(dif[difVertex].fSpaceId);
            continue;
        } else {
            // Is parent of other vertices -> Resolve itself then resolve children
            getOrderFromDITree(resultVector, dif, difVertex);
        }

    }
    return resultVector;
}

void WSMotionGraphSolver::getOrderFromDITree(std::vector<std::string>& resultVector, const DirectedInterferenceForest& dif, const DIFVertex& difVertex)
{
    resultVector.push_back(dif[difVertex].fSpaceId);

    //Resolve children
    DirectedInterferenceForest::out_edge_iterator in_edge_it, in_edge_end;
    for (boost::tie(in_edge_it, in_edge_end) = boost::out_edges(difVertex, dif); in_edge_it != in_edge_end; ++in_edge_it) {
        DIFVertex targetVertex = boost::target(*in_edge_it, dif);
        getOrderFromDITree(resultVector, dif, targetVertex);
    }
}

//Use STConfId2MGVertex to get vertices belonging to each free space component
void WSMotionGraphSolver::solveMotionGraphComponent(Motion_Graph& mg, MotionSchedule& ms)
{

//    for(const MGVertex& vertex : mg.vertex_set()) {
//        Motion_Graph::in_edge_iterator in_edge_it, in_edge_end;
//        for (boost::tie(in_edge_it, in_edge_end) = in_edges(vertex, mg); in_edge_it != in_edge_end; ++in_edge_it) {
//            MGEdge edge = *in_edge_it;
//        }
    while(!mg.vertex_set().empty()) {
        findAndRemoveLeaf(mg, ms);
    }
}

void WSMotionGraphSolver::findAndRemoveLeaf(Motion_Graph& mg, MotionSchedule& ms)
{
    MGVertex startLeaf;
    bool foundStartLeaf = false;
    //GraphvizDrawUtils::drawMotionGraph(mg, "mgTest.dot");
    for(const MGVertex & vertex : mg.vertex_set())
    {
        if (in_degree(vertex, mg) > 1)
        {
            continue;// Not a leaf vertex
        }
        if (!mg[vertex].isStartVertex)
        {//We prefer to remove a target leaf
            handleTargetLeaf(mg, ms, vertex);
            return;
        } else { //Save in case there are no target leafs
            if(!foundStartLeaf) {
                foundStartLeaf = true;
                startLeaf = vertex;
            }
        }
    }
    //If there are not target leafs remove start leaf
    handleStartLeaf(mg, ms, startLeaf);
}

void WSMotionGraphSolver::handleStartLeaf(Motion_Graph &mg, MotionSchedule &ms, const MGVertex &startLeaf)
{
    if(mg[startLeaf].occupyingRobot.empty()) {
        //Start leaf is empty -> Simply remove
        boost::clear_vertex(startLeaf, mg);
        boost::remove_vertex(startLeaf, mg);
        return;
    } else {
        std::vector<MGVertex> pathToEmpty = findNearestVertex(mg, startLeaf, false);
        doChainMove(mg, ms, pathToEmpty);
        boost::clear_vertex(startLeaf, mg);
        boost::remove_vertex(startLeaf, mg);
    }
}

void WSMotionGraphSolver::handleTargetLeaf(Motion_Graph &mg, MotionSchedule &ms, const MGVertex &targetLeaf)
{
    std::vector<MGVertex> pathToRobot = findNearestVertex(mg, targetLeaf, true);
    doMoveFrom(mg, ms, pathToRobot);
    boost::clear_vertex(targetLeaf, mg);
    boost::remove_vertex(targetLeaf, mg);
}

std::vector<MGVertex> WSMotionGraphSolver::findNearestVertex(Motion_Graph& mg, const MGVertex & toVertex, bool containingRobot) {
    std::vector<bool> visited(boost::num_vertices(mg), false);
    std::queue<std::vector<MGVertex>> q;
    q.push({toVertex});

    while (!q.empty()) {
        std::vector<MGVertex> current = q.front();
        const MGVertex & currentCandidate = current.back();
        q.pop();

        if (containingRobot ? !mg[currentCandidate].occupyingRobot.empty() : mg[currentCandidate].occupyingRobot.empty()) {
            return current;  // Found a vertex with (no) robot
        }

        if (!visited[currentCandidate]) {
            visited[currentCandidate] = true;

            // Enqueue unvisited neighbors.
            Motion_Graph::adjacency_iterator vi, vi_end;
            for (boost::tie(vi, vi_end) = boost::adjacent_vertices(currentCandidate, mg); vi != vi_end; ++vi) {
                if (!visited[*vi]) {
                    std::vector<MGVertex> newPath;
                    std::copy(current.begin(), current.end(), std::back_inserter(newPath));
                    newPath.push_back(*vi);
                    q.push(newPath);
                }
            }
        }
    }
    return {}; // Will never be reached
}

//Move one robot from start of path to end of path
void WSMotionGraphSolver::doMoveFrom(Motion_Graph& mg, MotionSchedule& ms, std::vector<MGVertex> path) {
    int i = path.size()-1;
    while(i> 0) {
        doMove(ms, mg[path[i]], mg[path[i-1]], mg[boost::edge(path[i], path[i-1], mg).first]); //MGEdge is guaranteed to exist, since we traversed it in BFS
        i--;
    }
}

//Move a chain of robots on space each from start of path to end of path
void WSMotionGraphSolver::doChainMove(Motion_Graph& mg, MotionSchedule& ms, std::vector<MGVertex> path) {
    int i = path.size()-1;
    while(i> 0) {
        doMove(ms, mg[path[i-1]], mg[path[i]], mg[boost::edge(path[i-1], path[i], mg).first]); //MGEdge is guaranteed to exist, since we traversed it in BFS
        i--;
    }
}

void WSMotionGraphSolver::doMove(MotionSchedule &ms, MGVertexProperty &from, MGVertexProperty &to, const MGEdgeProperty &over)
{
    if(!to.occupyingRobot.empty()) {
        throw std::runtime_error("To-MGVertex was occupied");
    }
    if(from.occupyingRobot.empty()) {
        throw std::runtime_error("From-MGVertex was occupied");
    }

    ms.robots2VerticesTraveled[ROBOT_PREFIX + from.occupyingRobot] += ("; "+ to.id);
    if(over.pathStartId == from.id) { // Path is defined in the correct order
        for(const Segment_2& pathSeg : over.pathLineSegments) {
            ms.motionSchedule += (ROBOT_PREFIX + from.occupyingRobot + ":" + StringUtils::toCoordinateString(pathSeg.source()) + " -> " + StringUtils::toCoordinateString(pathSeg.target()) + "\n");
        }
    } else { //Path is defined in reverse order
        for(auto it = over.pathLineSegments.rbegin(); it != over.pathLineSegments.rend(); ++it) {
            const Segment_2& pathSeg = *it;
            ms.motionSchedule += (ROBOT_PREFIX + from.occupyingRobot + ":" + StringUtils::toCoordinateString(pathSeg.target()) + " -> " + StringUtils::toCoordinateString(pathSeg.source()) + "\n");
        }
    }

    to.occupyingRobot = from.occupyingRobot;
    from.occupyingRobot = "";

}

