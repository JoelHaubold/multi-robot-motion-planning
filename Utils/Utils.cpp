//
// Created by joelh on 20/10/2023.
//

#include "Utils.h"
#include "../constants.h"

Polygon_2 Utils::generateBoundingBox(const CGAL::Bbox_2& bbox) {
    //Bbox_2 bbox = polygon.bbox();
    K::FT minX = bbox.xmin() - OBSTACLE_OFFSET;
    K::FT maxX = bbox.xmax() + OBSTACLE_OFFSET;
    K::FT minY = bbox.ymin() - OBSTACLE_OFFSET;
    K::FT maxY = bbox.ymax() + OBSTACLE_OFFSET;



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

Polygon_2 Utils::generateRobotAura(const Point_2& center, double sizeOfAura) {
    auto xc = center.x();
    auto yc = center.y();


    Polygon_2 robot;
    robot.push_back(Point_2(xc-sizeOfAura, yc-sizeOfAura));
    robot.push_back(Point_2(xc+sizeOfAura, yc-sizeOfAura));
    robot.push_back(Point_2(xc+sizeOfAura, yc+sizeOfAura));
    robot.push_back(Point_2(xc-sizeOfAura, yc+sizeOfAura));

    return robot;
}
