//
// Created by joelh on 20/10/2023.
//

#include "Utils.h"
#include "../constants.h"
#include <boost/optional.hpp>

Polygon_2 Utils::generateBoundingBox(const CGAL::Bbox_2& bbox) {
    //Bbox_2 bbox = polygon.bbox();
    K::FT minX = bbox.xmin() - OBSTACLE_SPACE_OFFSET;
    K::FT maxX = bbox.xmax() + OBSTACLE_SPACE_OFFSET;
    K::FT minY = bbox.ymin() - OBSTACLE_SPACE_OFFSET;
    K::FT maxY = bbox.ymax() + OBSTACLE_SPACE_OFFSET;



    std::cout << maxY << std::endl;

    // Create a bounding box that covers the entire plane
    Polygon_2 boundingBox;
    boundingBox.push_back(Point_2(minX, minY));
    boundingBox.push_back(Point_2(maxX, minY));
    boundingBox.push_back(Point_2(maxX, maxY));
    boundingBox.push_back(Point_2(minX, maxY));

    return boundingBox;
}

Polygon_2 Utils::generateRobot() {

    Polygon_2 robot;
    robot.push_back(Point_2(-ROBOT_SIZE / 2, -ROBOT_SIZE / 2));
    robot.push_back(Point_2(ROBOT_SIZE / 2, - ROBOT_SIZE / 2));
    robot.push_back(Point_2(ROBOT_SIZE / 2, ROBOT_SIZE / 2));
    robot.push_back(Point_2(-ROBOT_SIZE / 2, ROBOT_SIZE / 2));

    return robot;
}

Polygon_2 Utils::generateRobotAura() {

    Polygon_2 robot;
    robot.push_back(Point_2(-ROBOT_SIZE, -ROBOT_SIZE));
    robot.push_back(Point_2(ROBOT_SIZE, - ROBOT_SIZE));
    robot.push_back(Point_2(ROBOT_SIZE, ROBOT_SIZE));
    robot.push_back(Point_2(-ROBOT_SIZE, ROBOT_SIZE));

    return robot;
}

Polygon_2 Utils::generateRobotAura(const Point_2& center) {
    auto xc = center.x();
    auto yc = center.y();


    Polygon_2 robot;
    robot.push_back(Point_2(xc-ROBOT_SIZE, yc-ROBOT_SIZE));
    robot.push_back(Point_2(xc+ROBOT_SIZE, yc-ROBOT_SIZE));
    robot.push_back(Point_2(xc+ROBOT_SIZE, yc+ROBOT_SIZE));
    robot.push_back(Point_2(xc-ROBOT_SIZE, yc+ROBOT_SIZE));

    return robot;
}

Point_2 Utils::getLowestIntersectionPoint(const Segment_2& verticalRay, const Polygon_2& polyToIntersect) {
    Point_2 lowestPoint = verticalRay.target();
    for(const auto& edge : polyToIntersect.edges()) {
        Segment_2 es = edge;
        const auto result = CGAL::intersection(verticalRay, edge);
        if(!result) {
            continue;
        }
        if (const Segment_2* s = boost::get<Segment_2>(&*result)) {
            if(s->source().y() < lowestPoint.y() || s->target().y() < lowestPoint.y()) {
                lowestPoint = s->source().y() < s->target().y() ? s->source() : s->target();
            }
        } else {
            const Point_2* p = boost::get<Point_2 >(&*result);
            if(p->y() < lowestPoint.y()) {
                lowestPoint = *p;
            }
        }
    }
    return lowestPoint;
}

double Utils::getClosestIntersectionPointToOrigin(const Point_2& seg1Origin, const Segment_2 & seg1, const Segment_2 & seg2) {
    const auto result = CGAL::intersection(seg1, seg2);
    if (const Segment_2* s = boost::get<Segment_2>(&*result)) {
        double distSource = CGAL::to_double(CGAL::squared_distance(seg1Origin,s->source()));
        double distTarget = CGAL::to_double(CGAL::squared_distance(seg1Origin,s->target()));
        return distSource < distTarget ? distSource : distTarget;
    } else {
        const Point_2* p = boost::get<Point_2 >(&*result);
        return CGAL::to_double(CGAL::squared_distance(seg1Origin,*s));
    }
}
double Utils::getRuntimeBound(const int workspaceComplexity, const int nmbrStartConfs)
{
    return (workspaceComplexity+nmbrStartConfs)*std::log(workspaceComplexity+nmbrStartConfs) + std::pow(nmbrStartConfs,2) + nmbrStartConfs*workspaceComplexity;
}

//Polygon_2 Utils::generateRobotAura(const Point_2& center, double sizeOfAura) {
//    auto xc = center.x();
//    auto yc = center.y();
//
//
//    Polygon_2 robot;
//    robot.push_back(Point_2(xc-sizeOfAura, yc-sizeOfAura));
//    robot.push_back(Point_2(xc+sizeOfAura, yc-sizeOfAura));
//    robot.push_back(Point_2(xc+sizeOfAura, yc+sizeOfAura));
//    robot.push_back(Point_2(xc-sizeOfAura, yc+sizeOfAura));
//
//    return robot;
//}
