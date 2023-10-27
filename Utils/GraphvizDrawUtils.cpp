//
// Created by joelh on 27/10/2023.
//

#include "GraphvizDrawUtils.h"
#include <boost/graph/graphviz.hpp>
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

    void operator()(std::ostream& out, const Vertex& v) const {
        MGVertexProperty vertexProps = motionGraph[v];
        out << "[label=\"" << vertexProps.id << "\"]";  // Use the label property.
    }
};

void GraphvizDrawUtils::drawMotionGraph(const Motion_Graph& motionGraph)
{
    std::ofstream dotFile("graph.dot");
    boost::dynamic_properties dp;
    //dp.property("label", VertexWriter(motionGraph));

    //boost::make_vertex_attributes_writer(motionGraph);
    VertexWriter<Motion_Graph> vw(motionGraph);
    boost::write_graphviz(dotFile, motionGraph, vw);
    dotFile.close();
}
