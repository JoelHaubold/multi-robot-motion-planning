#include "WellSeparatedSolver/WSFreeSpaceGenerator.h"
#include "WellSeparatedSolver/WSMotionGraphGenerator.h"
#include "WorkspaceGenarator.h"
#include "mytypedefs.h"
//#include <CGAL/draw_polygon_2.h>
//#include <CGAL/draw_polygon_set_2.h>
#include "Utils/SFMLDrawUtils.h"
#include "Utils/GraphvizDrawUtils.h"
//#include "DrawUtils.h"
//#include <QApplication>

//#ifdef CGAL_USE_GMP
//#include <CGAL/Gmpz.h>
//typedef CGAL::Gmpz RT;
//#else
// NOTE: the choice of double here for a number type may cause problems
//       for degenerate point sets
#include <CGAL/double.h>
//typedef double RT;
//#endif

#include <fstream>
#include <list>

//const double RADIUS = 100;
//const int MAX_POLY_SIZE = 100;
//
//const int OBSTACLE_OFFSET = 5;
//const int ROBOT_SIZE = 4;
//
//const int NMBR_START_POS = 10;
//const int WORKSPACE_COMPLEXITY = 50;

/*
 * TODO: Draw combined pictures: https://doc.cgal.org/latest/GraphicsView/classCGAL_1_1Qt_1_1PainterOstream.html
 * TODO: Directed-interference forest from F
 * TODO: Motion Graph
 */

int main(int argc, char *argv[])
{
//    QApplication app(argc, argv);

    Polygon_2 workspacePolygon = WorkspaceGenerator::generateRandomPolygon();
//    CGAL::draw(workspacePolygon);
//    SFMLDrawUtils::drawPolygon_2(workspacePolygon, "workspace");

    //DrawUtils::drawPolygon(workspacePolygon);
//    std::cout << "FC:" << std::endl;
    std::vector<FreeSpaceComponent> freeSpaceComponents = WSFreeSpaceGenerator::getFreeSpaceComponents(workspacePolygon);
//    SFMLDrawUtils::drawFreeSpace(freeSpaceComponents, "freeSpace");
//    std::cout << "FCE:" << std::endl;
//    std::for_each(freeSpaceSegments.begin(), freeSpaceSegments.end(), [&freeSpaceSet](Polygon_2 poly){
//                      poly.reverse_orientation();
//                      freeSpaceSet.insert(poly);
//                  });

    //CGAL::Holes_container freeSpaceComponents = free_space_complement_polygon.holes();

    //CGAL::draw(free_space_complement_polygon*);

    STConfigurations stConfigurations = WorkspaceGenerator::getStartAndTargetConfigurations(freeSpaceComponents);
    WSFreeSpaceGenerator::associateSTConfs(freeSpaceComponents, stConfigurations.startConfigurations, stConfigurations.targetConfigurations);
    std::vector<FStarComponent> fStarSet = WSFreeSpaceGenerator::getFStar2(freeSpaceComponents);
//    Polygon_set_2 set;
//    for(const auto& fs : fStarSet) {
//        set.insert(fs.fStarPolygon);
//    }
//
//    CGAL::draw(set);

    SFMLDrawUtils::drawFStar(fStarSet, "fStar");

    //WSFreeSpaceGenerator::associateSTConfs(freeSpaceComponents, fStarSet, stConfigurations.startConfigurations, stConfigurations.targetConfigurations);
    //WSFreeSpaceGenerator::getMotionGraph(fStarSet, startConfs, targetConfs);

    Motion_Graph motionGraph;
    WSMotionGraphGenerator::insertVertices(motionGraph, stConfigurations);


    GraphvizDrawUtils::drawMotionGraph(motionGraph);

/*
 * Motion graph:
 * Struct stConf containing st-aura and id and pebble bool
 * Motion graph node has id
 * Look at one connected component of F*
 * Get B and create resulting list of stConfs
 * Get H and shoot rays up detect intersection point against whole connected component? Put in list add edge
 */

    return 0;

    //std::cout << "Hello, World!" << std::endl;
    //return 0;
}
