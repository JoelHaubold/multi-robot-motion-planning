//
// Created by joelh on 27/10/2023.
//

#include "GraphvizDrawUtils.h"
#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include "../mytypedefs.h"

template <class MGraph>
class VertexWriter {
public:
    explicit VertexWriter(const MGraph& _mGraph) : mGraph(_mGraph) {}
    template <class Vertex>
    void operator()(std::ostream& out, const Vertex& v) const {
        out << "[label=\"" << mGraph[v].id << "\",style=filled, fillcolor="<< (mGraph[v].isStartVertex ? "chartreuse4" : "darkorchid2") <<  "]";
    }
private:
    const MGraph& mGraph;
};

struct VertexLabelWriter {
    const Motion_Graph& motionGraph;  // The graph to write properties from.

    VertexLabelWriter(const Motion_Graph& g) : motionGraph(g) {}

    void operator()(std::ostream& out, const MGVertex & v) const {
        MGVertexProperty vertexProps = motionGraph[v];
        out << "[label=\"" << vertexProps.id << "\"]";  // Use the label property.
    }
};

void GraphvizDrawUtils::drawMotionGraph(const Motion_Graph& motionGraph, std::string location)
{
    std::ofstream dotFile(location);
    boost::dynamic_properties dp;
    //dp.property("label", VertexWriter(motionGraph));

    //boost::make_vertex_attributes_writer(motionGraph);
    VertexWriter<Motion_Graph> vw(motionGraph);
    boost::write_graphviz(dotFile, motionGraph, vw);
    dotFile.close();
}

void GraphvizDrawUtils::drawMotionGraphs(const std::unordered_map<std::string, Motion_Graph> &mgs)
{
    Motion_Graph mergedGraph;
    int addedVertices = 0;
    for(const auto& pair : mgs) {
        const Motion_Graph& mg = pair.second;
        drawMotionGraph(mg, "mg_"+pair.first + ".dot");
        for(const MGVertex & vertex : mg.vertex_set())
        {
            MGVertexProperty vProp = mg[vertex];
            boost::add_vertex(mg[vertex], mergedGraph);
        }

        for (auto it = boost::edges(mg).first; it != boost::edges(mg).second; ++it) {
            auto edge = *it; // Get the current edge descriptor
            MGEdgeProperty eProp = mg[edge];
            boost::add_edge(boost::source(edge, mg) + addedVertices, boost::target(edge, mg) + addedVertices, eProp, mergedGraph);
            //std::cout << "MGEdge: (" << boost::source(edge, g) << ", " << boost::target(edge, g) << ")\n";
        }
        addedVertices += mg.vertex_set().size();
    }
    drawMotionGraph(mergedGraph, "mg.dot");

}