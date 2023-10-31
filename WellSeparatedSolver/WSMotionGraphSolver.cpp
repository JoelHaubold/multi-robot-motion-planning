//
// Created by joelh on 30/10/2023.
//

#include "WSMotionGraphSolver.h"
#include "../mytypedefs.h"
#include <exception>


//Use MGIdToVertex to get vertices belonging to each free space component
void WSMotionGraphSolver::solveMotionGraphComponent(Motion_Graph& mg, MotionSchedule& ms)
{
    Vertex startLeaf;
    bool foundStartLeaf = false;
//    for(const Vertex& vertex : mg.vertex_set()) {
//        Motion_Graph::in_edge_iterator in_edge_it, in_edge_end;
//        for (boost::tie(in_edge_it, in_edge_end) = in_edges(vertex, mg); in_edge_it != in_edge_end; ++in_edge_it) {
//            Edge edge = *in_edge_it;
//        }
    while(!mg.vertex_set().empty()) {
        for(const Vertex& vertex : mg.vertex_set())
        {
            if (in_degree(vertex, mg) != 1)
            {
                continue;// Not a leaf vertex
            }
            if (!mg[vertex].isStartVertex)
            {
                handleTargetLeaf(mg, ms, vertex);
                continue;
            } else {
                if(!foundStartLeaf) {
                    startLeaf = vertex;
                }
            }
        }
        handleStartLeaf(mg, ms, startLeaf);

    }
}

void WSMotionGraphSolver::handleStartLeaf(Motion_Graph &mg, MotionSchedule &ms, const Vertex &startLeaf)
{
    if(mg[startLeaf].occupyingRobot == "") {
        //Start leaf is empty -> Simply remove
        boost::clear_vertex(startLeaf, mg);
        boost::remove_vertex(startLeaf, mg);
        return;
    } else {
        std::vector<Vertex> pathToEmpty = findNearestVertex(mg, startLeaf, false);
        doChainMove(mg, ms, pathToEmpty);
        boost::clear_vertex(startLeaf, mg);
        boost::remove_vertex(startLeaf, mg);
    }
}

void WSMotionGraphSolver::handleTargetLeaf(Motion_Graph &mg, MotionSchedule &ms, const Vertex &targetLeaf)
{
    std::vector<Vertex> pathToRobot = findNearestVertex(mg, targetLeaf, true);
    doMoveFrom(mg, ms, pathToRobot);
    boost::clear_vertex(targetLeaf, mg);
    boost::remove_vertex(targetLeaf, mg);
}

std::vector<Vertex> WSMotionGraphSolver::findNearestVertex(Motion_Graph& mg, const Vertex& toVertex, bool containingRobot) {
    std::vector<bool> visited(boost::num_vertices(mg), false);
    std::queue<std::vector<Vertex>> q;
    q.push({toVertex});

    while (!q.empty()) {
        std::vector<Vertex> current = q.front();
        const Vertex& currentCandidate = current.back();
        q.pop();

        if (containingRobot ? mg[currentCandidate].occupyingRobot != "" : mg[currentCandidate].occupyingRobot == "") {
            return current;  // Found a vertex with (no) robot
        }

        if (!visited[currentCandidate]) {
            visited[currentCandidate] = true;

            // Enqueue unvisited neighbors.
            Motion_Graph::adjacency_iterator vi, vi_end;
            for (boost::tie(vi, vi_end) = boost::adjacent_vertices(currentCandidate, mg); vi != vi_end; ++vi) {
                if (!visited[*vi]) {
                    std::vector<Vertex> newPath;
                    std::copy(current.begin(), current.end(), newPath.begin());
                    newPath.push_back(*vi);
                    q.push(newPath);
                }
            }
        }
    }
    return {}; // Will never be reached
}

//Move one robot from start of path to end of path
void WSMotionGraphSolver::doMoveFrom(Motion_Graph& mg, MotionSchedule& ms, std::vector<Vertex> path) {
    int i = path.size()-1;
    while(i> 0) {
        doMove(ms, mg[path[i]], mg[path[i-1]], mg[boost::edge(path[i], path[i-1], mg).first]); //Edge is guaranteed to exist, since we traversed it in BFS
        i--;
    }
}

//Move a chain of robots on space each from start of path to end of path
void WSMotionGraphSolver::doChainMove(Motion_Graph& mg, MotionSchedule& ms, std::vector<Vertex> path) {
    int i = path.size()-1;
    while(i> 0) {
        doMove(ms, mg[path[i-1]], mg[path[i]], mg[boost::edge(path[i-1], path[i], mg).first]); //Edge is guaranteed to exist, since we traversed it in BFS
        i--;
    }
}

void WSMotionGraphSolver::doMove(MotionSchedule &ms, MGVertexProperty &from, MGVertexProperty &to, const MGEdgeProperty &over)
{
    if(to.occupyingRobot != "") {
        throw std::runtime_error("To-Vertex was occupied");
    }
    if(from.occupyingRobot == "") {
        throw std::runtime_error("From-Vertex was occupied");
    }

    ms.robots2VerticesTraveled[from.occupyingRobot] += ("; "+ to.id);
    if(over.pathStartId == from.id) {
        for(const Segment_2& pathSeg : over.pathLineSegments) {
            ms.motionSchedule += (from.occupyingRobot + "->" + std::to_string(CGAL::to_double(pathSeg.target().x())) + std::to_string(CGAL::to_double(pathSeg.target().y())));
        }
    }

    to.occupyingRobot = from.occupyingRobot;
    from.occupyingRobot = "";

}

