#include "WellSeparatedSolver/WSFreeSpaceGenerator.h"
#include "WorkspaceGenarator.h"
#include "mytypedefs.h"
#include <CGAL/draw_polygon_set_2.h>
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
//  Polygon_2 testPolygon = WorkspaceGenerator::generateRandomPolygon();
//    std::vector<Polygon_2> holes;
//    Polygon_2 outer_polygon;
//    holes.push_back(testPolygon);
//  Polygon_wh_2 testPolygon2(outer_polygon, holes.begin(), holes.end());
//
//CGAL::draw(testPolygon2);




    Polygon_2 workspacePolygon = WorkspaceGenerator::generateRandomPolygon();

    //DrawUtils::drawPolygon(workspacePolygon);


    std::vector<Polygon_2> freeSpaceSegments = WSFreeSpaceGenerator::getFreeSpaceComponents(workspacePolygon);
    Polygon_set_2 freeSpaceSet;

    std::for_each(freeSpaceSegments.begin(), freeSpaceSegments.end(), [&freeSpaceSet](Polygon_2 poly){
                      poly.reverse_orientation();
                      freeSpaceSet.insert(poly);
                  });

    CGAL::draw(freeSpaceSet);

    //CGAL::Holes_container freeSpaceComponents = free_space_complement_polygon.holes();

    //CGAL::draw(free_space_complement_polygon*);

    std::vector<Point_2> startConfs;
    std::vector<Point_2> targetConfs;
    WorkspaceGenerator::getStartAndTargetConfigurations(freeSpaceSegments, startConfs, targetConfs);
    std::cout << "Start configurations: " << startConfs.size() << std::endl;
    std::cout << "Target configurations: " << targetConfs.size() << std::endl;

//    Polygon_set_2 auraSet;
//    Polygon_set_2 startSet;
//    Polygon_set_2 targetSet;
//    std::for_each(startConfs.begin(), startConfs.end(), [&auraSet, &startSet](const Point_2& startConf){
//        Polygon_2 auraPolygon = Utils::generateRobotAura(startConf);
//        auraSet.insert(auraPolygon);
//        startSet.insert(auraPolygon);
//    });
//    std::for_each(targetConfs.begin(), targetConfs.end(), [&auraSet, &targetSet](const Point_2& targetConf){
//        Polygon_2 auraPolygon = Utils::generateRobotAura(targetConf);
//        auraSet.insert(auraPolygon);
//        targetSet.insert(auraPolygon);
//    });
//
//    freeSpaceSet.difference(auraSet);
//    CGAL::draw(startSet);
//    CGAL::draw(targetSet);
//    CGAL::draw(freeSpaceSet);

    Polygon_set_2 fStarSet = WSFreeSpaceGenerator::getFStar(freeSpaceSet, startConfs, targetConfs);

    //WSFreeSpaceGenerator::getMotionGraph(fStarSet, startConfs, targetConfs);

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
