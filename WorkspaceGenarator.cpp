//
// Created by joelh on 17/10/2023.
//

#include "WorkspaceGenarator.h"
#include <CGAL/random_polygon_2.h>
#include <iostream>
#include "constants.h"
#include "RandomGenerator.h"
#include <functional>

Polygon_2 WorkspaceGenerator::generateRandomPolygon() {
  Polygon_2            polygon;
  std::set<Point_2>   point_set;
  //CGAL::Random         rand;
  std::cerr << "Seed = " <<  RandomGenerator::getSeed() << std::endl;
  std::cerr << "Seed = " <<  RandomGenerator::getSeed() << std::endl;
  int size = RandomGenerator::getRandomInt(4, WORKSPACE_COMPLEXITY);
  std::cerr << "Generating workspace of size = " <<  size << std::endl;
  // copy size points from the generator, eliminating duplicates, so the
  // polygon will have <= size vertices
//  CGAL::copy_n_unique(WorkspaceGenerator::Point_generator(RADIUS), size,
//                      std::back_inserter(point_set));
  while(point_set.size() < size) {
      double x = RandomGenerator::getRandomDouble(- MAX_WORKSPACE_SIZE/2, MAX_WORKSPACE_SIZE/2);
      double y = RandomGenerator::getRandomDouble(- MAX_WORKSPACE_SIZE/2, MAX_WORKSPACE_SIZE/2);
      point_set.emplace(x,y);
  }
//  std::ostream_iterator< Point_2 >  out( std::cout, " " );
//  std::cout << "From the following " << point_set.size() << " points "
//            << std::endl;
//  std::copy(point_set.begin(), point_set.end(), out);
//  std::cout << std::endl;
  CGAL::random_polygon_2(point_set.size(), std::back_inserter(polygon),
                         point_set.begin());
//  std::cout << "The following simple polygon was made: " << std::endl;
//  std::cout << polygon << std::endl;

  //CGAL::draw(polygon);
  return polygon;
}

//std::list<Point_2> WorkspaceGenerator::getRandomPoints(const Polygon_2& containingPolygon, const int& nmbrPoints) {
//  CGAL::Bbox_2 bbox = containingPolygon.bbox();
//  std::list<Point_2>   point_set;
//  for (int i = 0; i < nmbrPoints; i++)
//  {
//    //K::FT randomX = bbox.xmin() + static_cast<K::FT>(std::rand()) / RAND_MAX * (bbox.xmax() - bbox.xmin());
//    //K::FT randomY = bbox.ymin() + static_cast<K::FT>(std::rand()) / RAND_MAX * (bbox.ymax() - bbox.ymin());
//
//    K::FT randomX = RandomGenerator::getRandomDouble(bbox.xmin(), bbox.xmax());
//    K::FT randomY = RandomGenerator::getRandomDouble(bbox.ymin(), bbox.ymax());
//
//    Point_2 randomPoint = Point_2(randomX, randomY);
//
//    if (CGAL::bounded_side_2(containingPolygon.vertices_begin(), containingPolygon.vertices_end(), randomPoint, K()) == CGAL::ON_BOUNDED_SIDE)
//    {
//      point_set.push_back(randomPoint);
//    }
//  }
//  return point_set;
//}

//std::list<Point_2> WorkspaceGenerator::getRandomPoints(const std::vector<Polygon_2>& containingPolygons) {
//    double xmin = 0.0;
//    double xmax = 0.0;
//    double ymin = 0.0;
//    double ymax = 0.0;
//    for (const Polygon_2& poly : containingPolygons) {
//        Bbox_2 bbox = poly.bbox();
//        if (bbox.xmin() < xmin) {
//            xmin = bbox.xmin();
//        }
//        if (bbox.xmax() < xmax) {
//            xmax = bbox.xmax();
//        }
//        if (bbox.ymin() < ymin) {
//            ymin = bbox.ymin();
//        }
//        if (bbox.ymax() < ymax) {
//            ymax = bbox.ymax();
//        }
//    }
//
//    std::list<Point_2>   point_set;
//    int pointsGenerated = 0;
//    while(point_set.size() < NMBR_START_POS && pointsGenerated < 10000)
//    {
//      //Ones polygon gets a point genarate second point in that polygon, if fails x times don't use the point.
//      //std::cout << "While iter" << std::endl;
//        K::FT randomX = RandomGenerator::getRandomDouble(xmin, xmax);
//        K::FT randomY = RandomGenerator::getRandomDouble(ymin, ymax);
//
//        Point_2 randomPoint = Point_2(randomX, randomY);
//        for (const Polygon_2& poly : containingPolygons) {
//          //std::cout << "For iter" << std::endl;
//            if (CGAL::bounded_side_2(poly.vertices_begin(), poly.vertices_end(), randomPoint,
//                                     K()) == CGAL::ON_BOUNDED_SIDE) {
//                point_set.push_back(randomPoint);
//                break;
//            }
//            pointsGenerated++;
//        }
//    }
//    std::cout << "Generated " << pointsGenerated << " points for " << NMBR_START_POS << " points"
//              << std::endl;
//
//    return point_set;
//}



STConfigurations WorkspaceGenerator::getStartAndTargetConfigurations(const std::vector<FreeSpaceComponent>& containingPolygons) {
    //  std::vector<std::vector<Point_2>> startConfsPerComponent;
    //  std::vector<std::vector<Point_2>> targetConfsPerComponent;
    //
    //  for (const Polygon_2& poly : containingPolygons) {
    //    startConfsPerComponent.emplace_back();
    //    targetConfsPerComponent.emplace_back();
    //  }

    double xmin = 0.0;
    double xmax = 0.0;
    double ymin = 0.0;
    double ymax = 0.0;
    for (const auto& fComponent : containingPolygons) {
        Bbox_2 bbox = fComponent.freeSpaceComponent.bbox();
        if (bbox.xmin() < xmin) {
            xmin = bbox.xmin();
        }
        if (bbox.xmax() > xmax) {
            xmax = bbox.xmax();
        }
        if (bbox.ymin() < ymin) {
            ymin = bbox.ymin();
        }
        if (bbox.ymax() > ymax) {
            ymax = bbox.ymax();
        }
    }

    int pointsGenerated = 0;
    int pointsAccepted = 0;
    std::vector<Point_2> startConfsGenerated;
    std::vector<Point_2> targetConfsGenerated;
    while(pointsAccepted < 2*NMBR_START_POS && pointsGenerated < NMBR_POINT_TRIES)
    {
        //Ones polygon gets a point, genarate a second point in that polygon, if that fails x times don't use the first point.
        Point_2 randomPoint = getRandomPoint(xmin, xmax, ymin, ymax);
        pointsGenerated++;
        bool isStartConf = RandomGenerator::getRandomBoolean();
        //Check if configuration conforms to chromatic separation
        if(!conformsToChromaticDistance(randomPoint, startConfsGenerated, targetConfsGenerated, isStartConf)){
            continue;
        }
        for(size_t i = 0; i < containingPolygons.size(); i++) {
            //std::cout << "For iter" << std::endl;
            const FreeSpaceComponent& fComponent = containingPolygons[i];
            const Polygon_2& poly = fComponent.freeSpaceComponent;
            if (CGAL::bounded_side_2(poly.vertices_begin(), poly.vertices_end(), randomPoint,K()) != CGAL::ON_BOUNDED_SIDE) {
                continue;
            }
            //std::cout << "For iter" << std::endl;
            Bbox_2 bbox = poly.bbox();
            //Try to generate accompanying target conf
            for (int j = 0; i < NMBR_PAIR_POINT_TRIES; j++) {
                Point_2 pairPoint = getRandomPoint(bbox.xmin(), bbox.xmax(), bbox.ymin(), bbox.ymax());
                if (CGAL::bounded_side_2(poly.vertices_begin(), poly.vertices_end(), pairPoint,K()) != CGAL::ON_BOUNDED_SIDE) { //TODO: Group ifs into one private method and replace with poly.boundedSide
                    continue;
                }
                if(!conformsToChromaticDistance(pairPoint, startConfsGenerated, targetConfsGenerated, !isStartConf) ||
                    !conformsToChromaticDistance(pairPoint, {}, {randomPoint})) {
                    continue;
                }
                //Insert first point
                isStartConf ? startConfsGenerated.push_back(randomPoint) : targetConfsGenerated.push_back(randomPoint);
                //Insert paired point
                isStartConf ? targetConfsGenerated.push_back(pairPoint) : startConfsGenerated.push_back(pairPoint);
                pointsAccepted += 2;
                break;
            }
        }
    }
    std::cout << "Generated " << pointsGenerated << " points for " << NMBR_START_POS*2 << " points"
              << std::endl;
    STConfigurations stconfs;

    int conf_nmbr = 1;
    std::vector<STConf> startConfs;
    std::vector<STConf> targetConfs;
    for(const Point_2& sConf : startConfsGenerated) {
        startConfs.emplace_back(sConf, START_CONF_PREFIX+ std::to_string(conf_nmbr++));
    }
    conf_nmbr = 1;
    for(const Point_2& tConf : targetConfsGenerated) {
        targetConfs.emplace_back(tConf, TARGET_CONF_PREFIX+ std::to_string(conf_nmbr++));
    }
    return STConfigurations{startConfs, targetConfs};
}

Point_2 WorkspaceGenerator::getRandomPoint(double xmin, double xmax, double ymin, double ymax) {
    K::FT randomX = RandomGenerator::getRandomDouble(xmin, xmax);
    K::FT randomY = RandomGenerator::getRandomDouble(ymin, ymax);

    return Point_2{randomX, randomY};
}

bool WorkspaceGenerator::conformsToChromaticDistance(const Point_2& conf, const std::vector<Point_2>& startConfs, const std::vector<Point_2>& targetConfs, bool pointIsStartConf) {
    if(pointIsStartConf) {
        return conformsToChromaticDistance(conf, startConfs, targetConfs);
    } else {
        return conformsToChromaticDistance(conf, targetConfs, startConfs);
    }
}

bool WorkspaceGenerator::conformsToChromaticDistance(const Point_2& conf, const std::vector<Point_2>& sameTypeConfs, const std::vector<Point_2>& differentTypeConfs) {
//    bool monochromaticlySeparated = std::all_of(sameTypeConfs.begin(), sameTypeConfs.end(), [&conf](const Point_2& stConf){
//        return CGAL::abs(conf.x() - stConf.x()) < MONOCHROMATIC_SEPARATION && CGAL::abs(conf.y() - stConf.y()) < MONOCHROMATIC_SEPARATION;
//    });
//
//    bool bichromaticlySeparated = std::all_of(sameTypeConfs.begin(), sameTypeConfs.end(), [&conf](const Point_2& stConf){
//        return CGAL::abs(conf.x() - stConf.x()) < MONOCHROMATIC_SEPARATION && CGAL::abs(conf.y() - stConf.y()) < MONOCHROMATIC_SEPARATION;
//    });

    for(const auto& stConf : sameTypeConfs) {
        if(CGAL::max(CGAL::abs(conf.x() - stConf.x()), CGAL::abs(conf.y() - stConf.y())) < MONOCHROMATIC_SEPARATION) {
            return false;
        }
    }
    for(const auto& dtConf : differentTypeConfs) {
        if(CGAL::max(CGAL::abs(conf.x() - dtConf.x()), CGAL::abs(conf.y() - dtConf.y())) < BICHROMATIC_SEPARATION) {
            return false;
        }
    }
    return true;
}

//std::vector<int> WorkspaceGenerator::getRandomPointDistribution(const int& nmbrOfSections, const int& nmbrPoints) {
//  std::vector<int> pointsInSections(nmbrOfSections, 0); // Initialize all baskets with 0 apples
//
//  // Distribute apples randomly into baskets
//  for (int i = 0; i < nmbrPoints; i++) {
//    int randomBasket = RandomGenerator::getRandomInt(0, nmbrOfSections - 1);
//    pointsInSections[randomBasket]++;
//  }
//
//  return pointsInSections;
//}