//#include "WellSeparatedSolver/WSFreeSpaceGenerator.h"
//#include "WellSeparatedSolver/WSMotionGraphGenerator.h"
//#include "WorkspaceGenarator.h"
//#include "mytypedefs.h"
////#include <CGAL/draw_polygon_2.h>
//#include <CGAL/draw_polygon_set_2.h>
//#include "Utils/SFMLDrawUtils.h"
//#include "Utils/GraphvizDrawUtils.h"
//#include "Utils/StringUtils.h"
//#include "RandomGenerator.h"
//#include "WellSeparatedSolver/WSMotionGraphSolver.h"
//#include "DrawUtils.h"
//#include <QApplication>
#include "RandomGenerator.h"
#include "WellSeparatedSolver/WSSolver.h"
#include "testParameters.h"


//#ifdef CGAL_USE_GMP
//#include <CGAL/Gmpz.h>
//typedef CGAL::Gmpz RT;
//#else
// NOTE: the choice of double here for a number type may cause problems
//       for degenerate point sets
#include <CGAL/double.h>
//typedef double RT;
//#endif

//const double RADIUS = 100;
//const int WORKSPACE_COMPLEXITY = 100;
//
//const int OBSTACLE_SPACE_OFFSET = 5;
//const int ROBOT_SIZE = 4;
//
//const int NMBR_START_POS = 10;
//const int WORKSPACE_COMPLEXITY = 50;

/*
 * TODO: Test special cases as in disk shaped master thesis
 * TODO: Output validator
 * TODO: Generate new workspace polygon if point limit is reached
 * TODO: FSTAR Generation. Check number of generated stConfs. If one instant assign all STConfs. Else find smallest polygon https://web.archive.org/web/20100405070507/http://valis.cs.uiuc.edu/~sariel/research/CG/compgeom/msg00831.html check if locations are in bounding box+2 then join/ FIrst also check if . Repeat
 * TODO: Also use these methods in dif->stAssoc->(MGCrea)
 * TODO: Also iterate over fstar comps not m stconfs
 * TODO: Ask Carolim über nicht den Prozess beschreiben vs above provess beschreiben
 * TODO: Maybe focus on creating better paths instead of which paths to choose
 */

//try {
//    // Code that may throw an exception
//    throw std::runtime_error("This is an example exception.");
//} catch (const std::exception& e) {
//    // Catch the exception and handle it
//    std::cerr << "Caught an exception: " << e.what() << std::endl;
//}


int main(int argc, char *argv[])
{

//    std::vector<int> randomArray{1,2,3,4};
//    if(-1 > randomArray.size()) {
//        std::cout << "Wat" << std::endl;
//        return 1;
//    }

    if(PRE_SET_SEED) {
        RandomGenerator::setSeed(PRE_SET_SEED.get());
    }

    WSSolver::doMRMPRuntimeTest();

    //RandomGenerator::setSeed(900439196);
//    Polygon_2 workspacePolygon = MRMPInputGenerator::generateRandomPolygon();
////    CGAL::draw(workspacePolygon);
////    SFMLDrawUtils::drawPolygon_2(workspacePolygon, "workspace");
//
//    //DrawUtils::drawPolygon(workspacePolygon);
////    std::cout << "FC:" << std::endl;
//    std::vector<FreeSpaceComponent> freeSpaceComponents = WSFreeSpaceGenerator::getFreeSpaceComponents(workspacePolygon);
////    SFMLDrawUtils::drawFreeSpace(freeSpaceComponents, "freeSpace");
//
//
//    STConfigurations stConfigurations = MRMPInputGenerator::getStartAndTargetConfigurations(freeSpaceComponents);
//    WSFreeSpaceGenerator::associateSTConfs(freeSpaceComponents, stConfigurations.startConfigurations, stConfigurations.targetConfigurations);
//    std::vector<FStarComponent> fStarSet = WSFreeSpaceGenerator::getFStar2(freeSpaceComponents);
////    Polygon_set_2 set;
////    for(const auto& fs : fStarSet) {
////        set.insert(fs.fStarPolygon);
////    }
////
////    CGAL::draw(set);
//
//    SFMLDrawUtils::drawFStar(fStarSet, "fStar");
//
//    //WSFreeSpaceGenerator::associateSTConfs(freeSpaceComponents, fStarSet, stConfigurations.startConfigurations, stConfigurations.targetConfigurations);
//    //WSFreeSpaceGenerator::getMotionGraph(fStarSet, startConfs, targetConfs);
//
////    Motion_Graph motionGraph;
////
////    const STConfId2MGVertex id2Vertex = WSMotionGraphGenerator::insertVertices(motionGraph, stConfigurations);
////    WSMotionGraphGenerator::insertEdges(motionGraph, fStarSet, id2Vertex);
//
//    std::unordered_map<std::string, Motion_Graph> mgs= WSMotionGraphGenerator::getMotionGraphs(freeSpaceComponents,fStarSet);
//
//    GraphvizDrawUtils::drawMotionGraphs(mgs);
//
//    std::vector<MGEdgeProperty> edges;
//
//    for(const auto& fsMg : mgs) {
//        const auto& motionGraph = fsMg.second;
//        auto edgePair = boost::edges(motionGraph);
//        auto edgeIt = edgePair.first;
//        auto edgeEnd = edgePair.second;
//        auto edgePropertyMap = boost::get(boost::edge_bundle, motionGraph);
//        for (; edgeIt != edgeEnd; ++edgeIt) {
//            edges.push_back(motionGraph[*edgeIt]);
//            //        MGEdgeProperty edgeProperty = edgePropertyMap[edgePair.first];
//            //        edges.push_back(motionGraph[edge_descriptor]);
//        }
//    }
//
//    SFMLDrawUtils::drawPaths(fStarSet, edges, "Paths");
//    MotionSchedule ms =  WSMotionGraphSolver::solveMotionGraphs(mgs);
//
//    StringUtils::printMotionSteps(ms);

/*
 * Motion graph:
 * Struct stConf containing st-aura and id and pebble bool
 * Motion graph node has id
 * Look at one connected component of F*
 * Get B and create resulting list of stConfs
 * Get H and shoot rays up detect intersection point against whole connected component? Put in list add edge
 */

    /*
     * Check tree leaves
     * Contains target conf? Move pebble and remove
     * If not random start conf
     * If no pebble -> Remove
     * Else move pebble (by chain) to
     *
     * Outputs:
     * Chess notation of pebble moves w. opt removal
     * Translation of moves to robot moves (ends of segments)
     * Whole motion schedule
     */

    /*
     * Directed-intrference forest check intersection of each st with each FPoly
     */



    return 0;

    //std::cout << "Hello, World!" << std::endl;
    //return 0;
}
