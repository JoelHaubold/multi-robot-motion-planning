#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/random_polygon_2.h>
#include <CGAL/draw_polygon_2.h>
#include <CGAL/draw_polygon_with_holes_2.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/minkowski_sum_2.h>
#include <CGAL/Polygon_vertical_decomposition_2.h>
#include <stdexcept>
#include <vector>



//#ifdef CGAL_USE_GMP
//#include <CGAL/Gmpz.h>
//typedef CGAL::Gmpz RT;
//#else
// NOTE: the choice of double here for a number type may cause problems
//       for degenerate point sets
#include <CGAL/double.h>
typedef double RT;
//#endif

#include <fstream>
#include <list>
typedef CGAL::Exact_predicates_exact_constructions_kernel K;
//typedef CGAL::Simple_cartesian<RT>                        K;
typedef K::Point_2                                        Point_2;
typedef std::list<Point_2>                                Container;
typedef CGAL::Polygon_2<K, Container>                     Polygon_2;
typedef CGAL::Polygon_with_holes_2<K, Container>          Polygon_wh_2;
typedef CGAL::Bbox_2                                      Bbox_2;
typedef CGAL::Creator_uniform_2<int, Point_2>             Creator;
typedef CGAL::Random_points_in_square_2<Point_2, Creator> Point_generator;
typedef std::list<Polygon_wh_2>                           Pwh_list_2;

const double RADIUS = 100;
const int MAX_POLY_SIZE = 100;

const int OBSTACLE_OFFSET = 5;
const int ROBOT_SIZE = 4;

const int NMBR_START_POS = 10;
const int WORKSPACE_COMPLEXITY = 50;

Polygon_2 generateRandomPolygon() {
  Polygon_2            polygon;
  std::list<Point_2>   point_set;
  CGAL::Random         rand;
  std::cerr << "Seed = " <<  rand.get_seed() << std::endl;
  int size = rand.get_int(4, MAX_POLY_SIZE);
  // copy size points from the generator, eliminating duplicates, so the
  // polygon will have <= size vertices
  CGAL::copy_n_unique(Point_generator(RADIUS), size,
                      std::back_inserter(point_set));
  std::ostream_iterator< Point_2 >  out( std::cout, " " );
  std::cout << "From the following " << point_set.size() << " points "
            << std::endl;
  std::copy(point_set.begin(), point_set.end(), out);
  std::cout << std::endl;
  CGAL::random_polygon_2(point_set.size(), std::back_inserter(polygon),
                         point_set.begin());
  std::cout << "The following simple polygon was made: " << std::endl;
  std::cout << polygon << std::endl;

  //CGAL::draw(polygon);
  return polygon;
}

std::list<Point_2> getRandomPoints(const Polygon_2& containingPolygon, const int& nmbrPoints) {
  Bbox_2 bbox = containingPolygon.bbox();
  std::list<Point_2>   point_set;
  CGAL::Random         rand;
  for (int i = 0; i < nmbrPoints; i++)
  {
    //K::FT randomX = bbox.xmin() + static_cast<K::FT>(std::rand()) / RAND_MAX * (bbox.xmax() - bbox.xmin());
    //K::FT randomY = bbox.ymin() + static_cast<K::FT>(std::rand()) / RAND_MAX * (bbox.ymax() - bbox.ymin());

    K::FT randomX = rand.get_double(bbox.xmin(), bbox.xmax());
    K::FT randomY = rand.get_double(bbox.ymin(), bbox.ymax());

    Point_2 randomPoint = Point_2(randomX, randomY);
    if (CGAL::bounded_side_2(containingPolygon.vertices_begin(), containingPolygon.vertices_end(), randomPoint, K()) == CGAL::ON_BOUNDED_SIDE)
    {
      point_set.push_back(randomPoint);
    }
  }
  return point_set;
}

std::vector<Point_2> getRandomPointDistribution(const int& nmbrOfSections, const int& nmbrPoints) {

}

Polygon_2 generateBoundingBox(const Bbox_2& bbox) {
  //Bbox_2 bbox = polygon.bbox();
  K::FT minX = bbox.xmin() - OBSTACLE_OFFSET;
  K::FT maxX = bbox.xmax() + OBSTACLE_OFFSET;
  K::FT minY = bbox.ymin() - OBSTACLE_OFFSET;
  K::FT maxY = bbox.ymax() + OBSTACLE_OFFSET;
//  K::FT minX = polygon.left_vertex()->x() - OBSTACLE_OFFSET;
//  K::FT maxX = polygon.right_vertex()->x() + OBSTACLE_OFFSET;
//  K::FT minY = polygon.bottom_vertex()->y() - OBSTACLE_OFFSET;
//  K::FT maxY = polygon.top_vertex()->y() + OBSTACLE_OFFSET;


  std::cout << maxY << std::endl;

  // Create a bounding box that covers the entire plane
  Polygon_2 boundingBox;
  boundingBox.push_back(Point_2(minX, minY));
  boundingBox.push_back(Point_2(maxX, minY));
  boundingBox.push_back(Point_2(maxX, maxY));
  boundingBox.push_back(Point_2(minX, maxY));

  return boundingBox;
}

Polygon_2 generateRobot() {

  Polygon_2 robot;
  robot.push_back(Point_2(-ROBOT_SIZE / 2, -ROBOT_SIZE / 2));
  robot.push_back(Point_2(ROBOT_SIZE / 2, - ROBOT_SIZE / 2));
  robot.push_back(Point_2(ROBOT_SIZE / 2, ROBOT_SIZE / 2));
  robot.push_back(Point_2(-ROBOT_SIZE / 2, ROBOT_SIZE / 2));

  return robot;
}

Polygon_wh_2 getObstaclePolygon(const Polygon_2& boundingBox, const Polygon_2& workspacePolygon) {
  Polygon_wh_2 uncheckedObstaclePolygon;
  Pwh_list_2 obstaclePolygons;
  CGAL::difference(boundingBox, workspacePolygon,
                   std::back_inserter(obstaclePolygons));
  std::cout << obstaclePolygons.size() << std::endl;
  uncheckedObstaclePolygon = *obstaclePolygons.begin();
  if(uncheckedObstaclePolygon.number_of_holes() != 1) {
    throw std::runtime_error("Obstacle polygon should have exactly one hole" );
  }

  return uncheckedObstaclePolygon;
}

Pwh_list_2 getFreeSpacePolygon(const Polygon_2& boundingBox, const Polygon_wh_2& free_space_complement_polygon) {
  Pwh_list_2 freeSpacePolygons;

  CGAL::difference(boundingBox, free_space_complement_polygon,
                   std::back_inserter(freeSpacePolygons));

  return freeSpacePolygons;
}

int main()
{

  Polygon_2 workspacePolygon = generateRandomPolygon();

  // Create a bounding box that covers the entire plane
  Polygon_2 boundingBox = generateBoundingBox(workspacePolygon.bbox());

  //Calculate obstacle space
  Polygon_wh_2 obstaclePolygon = getObstaclePolygon(boundingBox, workspacePolygon);

  // Create robot workspacePolygon
  Polygon_2 robot = generateRobot();

  //Free space complement workspacePolygon
  CGAL::Polygon_vertical_decomposition_2<K, Container>  decomp;
  Polygon_wh_2 free_space_complement_polygon = CGAL::minkowski_sum_by_decomposition_2(obstaclePolygon, robot, decomp, decomp);

  //CGAL::Holes_container freeSpaceComponents = free_space_complement_polygon.holes();

  CGAL::draw(free_space_complement_polygon);

//  Polygon_wh_2 freeSpacePolygon; TODO: maybe replace with simply getting holes
  Pwh_list_2 freeSpacePolygons = getFreeSpacePolygon(boundingBox, free_space_complement_polygon);

  Bbox_2 bbox = workspacePolygon.bbox();

  // Create random points for start and target configurations
  // Add as Sq1 to workspace as holes
  // Proceed to generate



  return 0;

  //std::cout << "Hello, World!" << std::endl;
  //return 0;
}
