//
// Created by joelh on 26/10/2023.
//

#include "SFMLDrawUtils.h"
#include "../constants.h"
#include "../mytypedefs.h"
#include "Utils.h"
#include <QtMath>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <cmath>

static const int UPSCALE_FACTOR = 8;
static const double BORDER_FACTOR = 1.6;
static sf::Font font;
static bool loadedFont;

void SFMLDrawUtils::drawPolygon_2(const Polygon_2& toDraw, const std::string& name)
{
    Bbox_2 bbox = toDraw.bbox();
//    int width = bbox.x_span()*UPSCALE_FACTOR*BORDER_FACTOR;
//    int height = bbox.y_span()*UPSCALE_FACTOR*BORDER_FACTOR;
    int width = CGAL::to_double((toDraw.right_vertex()->x()- toDraw.left_vertex()->x())*UPSCALE_FACTOR*BORDER_FACTOR);
    int height = CGAL::to_double((toDraw.top_vertex()->y()- toDraw.bottom_vertex()->y())*UPSCALE_FACTOR*BORDER_FACTOR);

    sf::RenderWindow window(sf::VideoMode(width, height), name);

    sf::ConvexShape polygon = getSFPolygon(toDraw, width, height);

    window.clear(sf::Color::White);
    window.draw(polygon);
    window.display();

    displayWindow(window);
}

void SFMLDrawUtils::drawFreeSpace(const std::vector<FreeSpaceComponent> &toDraw, const std::string &name)
{
    double xmin = 0.0;
    double xmax = 0.0;
    double ymin = 0.0;
    double ymax = 0.0;
    for (const auto& fComponent : toDraw) {
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
    int width = (xmax-xmin)*UPSCALE_FACTOR*BORDER_FACTOR;
    int height = (ymax-ymin)*UPSCALE_FACTOR*BORDER_FACTOR;

    sf::RenderWindow window(sf::VideoMode(width, height), name);
    window.clear(sf::Color::White);

    for (const auto& fComponent : toDraw)
    {
        sf::ConvexShape polygon = getSFPolygon(fComponent.freeSpaceComponent, width, height);

        window.draw(polygon);
    }



    window.display();

    displayWindow(window);
}


//void SFMLDrawUtils::drawPolygon(sf::ConvexShape polygon, sf::RenderWindow window) {
//
//}
//
//sf::RenderWindow SFMLDrawUtils::getWindow(int height, int width) {
//    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Shapes Example");
//    return window;
//}

void SFMLDrawUtils::drawFStar(const std::vector<FStarComponent> &toDraw, const std::string &name)
{
    double xmin = 0.0;
    double xmax = 0.0;
    double ymin = 0.0;
    double ymax = 0.0;
    for (const auto& fComponent : toDraw) {
        Bbox_2 bbox = fComponent.fStarPolygon.bbox();
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
    int width = (xmax-xmin)*UPSCALE_FACTOR*BORDER_FACTOR;
    int height = (ymax-ymin)*UPSCALE_FACTOR*BORDER_FACTOR;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(width, height), name, sf::Style::Default, settings);
    window.clear(sf::Color::White);

    for (const auto& fComponent : toDraw)
    {
        sf::ConvexShape polygon = getSFPolygon(fComponent.fStarPolygon.outer_boundary(), width, height);
//        for(const Polygon_2& poly :fComponent.fStarPolygon.holes()) {
//            const K::FT xV = (poly.vertex(0).x() + poly.vertex(1).x() + poly.vertex(2).x() + poly.vertex(3).x())/4;
//            const K::FT yV = (poly.vertex(0).y() + poly.vertex(1).y() + poly.vertex(2).y() + poly.vertex(3).y())/4;
//            std::cout << "Poly: " << xV <<"," << yV << std::endl;
//        }
//        for(const STConf& stconf: fComponent.adjacentSConfs) {
//            std::cout << "S: " << stconf.location.x() <<"," << stconf.location.y() << std::endl;
//        }
//        for(const STConf& stconf: fComponent.adjacentTConfs) {
//            std::cout << "T: " << stconf.location.x() <<"," << stconf.location.y() << std::endl;
//        }

        window.draw(polygon);

        for(const auto& sConf: fComponent.adjacentSConfs) {
            window.draw(getCube(sConf, true, width, height));
            window.draw(getText(sConf.location, sConf.id, width, height));
            //window.draw(getText(sConf.location, sConf.id, width, height));
        }

        for(const auto& tConf: fComponent.adjacentTConfs) {
            window.draw(getCube(tConf, false, width, height));
            window.draw(getText(tConf.location, tConf.id, width, height));
        }

    }



    window.display();

    displayWindow(window);
}

void SFMLDrawUtils::drawPaths(const std::vector<FStarComponent> &toDraw, const std::vector<MGEdgeProperty> &edges, const std::string &name)
{
    double xmin = 0.0;
    double xmax = 0.0;
    double ymin = 0.0;
    double ymax = 0.0;
    for (const auto& fComponent : toDraw) {
        Bbox_2 bbox = fComponent.fStarPolygon.bbox();
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
    int width = (xmax-xmin)*UPSCALE_FACTOR*BORDER_FACTOR;
    int height = (ymax-ymin)*UPSCALE_FACTOR*BORDER_FACTOR;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(width, height), name, sf::Style::Default, settings);
    window.clear(sf::Color::White);


    for (const auto& fComponent : toDraw)
    {
        sf::ConvexShape polygon = getSFPolygon(fComponent.fStarPolygon.outer_boundary(), width, height);
        window.draw(polygon);

        for(const auto& sConf: fComponent.adjacentSConfs) {
            window.draw(getCube(sConf, true, width, height));
            window.draw(getText(sConf.location, sConf.id, width, height));
            //window.draw(getText(sConf.location, sConf.id, width, height));
        }

        for(const auto& tConf: fComponent.adjacentTConfs) {
            window.draw(getCube(tConf, false, width, height));
            window.draw(getText(tConf.location, tConf.id, width, height));
        }

    }

//    sf::VertexArray lines(sf::Lines);
//    // Iterate through the edges and access their properties.
//    for (; edgeIt != edgeEnd; ++edgeIt) {
//        // Access the edge descriptor.
//        auto edgeDescriptor = *edgeIt;
//
//        // Access the edge properties using the edge descriptor.
//        MGEdgeProperty edgeProps = mg[edgeDescriptor];
//        std::vector<Segment_2> edgePath = edgeProps.pathLineSegments;
//        for(const auto& pathSegment : edgePath) {
//            lines.append(sf::MGVertex(getUpscaledOffsetVector(pathSegment.source(), width/2, height/2), sf::Color::Red));
//            lines.append(sf::MGVertex(getUpscaledOffsetVector(pathSegment.target(), width/2, height/2), sf::Color::Red));
//
//
//        }
//    }
//    window.draw(lines);

    for (const auto& edgeProps : edges) {
        //std::cout << "Drawing edge for: " << edgeProps.pathStartId<< std::endl;
        std::vector<Segment_2> edgePath = edgeProps.pathLineSegments;
        for(const auto& pathSegment : edgePath) {
            sf::Vector2f point1 = getUpscaledVector(pathSegment.source());
            sf::Vector2f point2 = getUpscaledVector(pathSegment.target());

            sf::Vector2f direction = point2 - point1;
            float rotation = std::atan2(direction.y, direction.x) * (180.0f / M_PI);
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

            sf::RectangleShape line(sf::Vector2f(length, 2.0f));
            line.setRotation(rotation);
            line.setPosition(point1.x+width / 2, point1.y + height / 2);
            line.setFillColor(sf::Color::Blue);

            window.draw(line);
        }
    }

//    for (; edgeIt != edgeEnd; ++edgeIt) {
//        // Access the edge descriptor.
//        auto edgeDescriptor = *edgeIt;
//
//        // Access the edge properties using the edge descriptor.
//        MGEdgeProperty edgeProps = mg[edgeDescriptor];
//        std::vector<Segment_2> edgePath = edgeProps.pathLineSegments;
//        for(const auto& pathSegment : edgePath) {
//            sf::ConvexShape line;
//            line.setPointCount(2);
//            line.setPoint(0, getUpscaledVector(pathSegment.source()));
//            line.setPoint(1, getUpscaledVector(pathSegment.target()));
//            line.setPosition(width / 2, height / 2);
//            line.setOutlineThickness(5);
//            line.setOutlineColor(sf::Color::Red);
//            window.draw(line);
//        }
//    }




    window.display();

    displayWindow(window);
}

void SFMLDrawUtils::displayWindow(sf::RenderWindow& window) {
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
}
sf::ConvexShape SFMLDrawUtils::getSFPolygon(const Polygon_2 &toDraw, int width, int height)
{
    sf::ConvexShape polygon;
    polygon.setPosition(width / 2, height / 2);
    polygon.setPointCount(toDraw.size());
    int i = 0;
    for (const Point_2& point : toDraw.vertices()) {
        //sf::Vector2f sfmlPoint(CGAL::to_double(point.x()*UPSCALE_FACTOR), CGAL::to_double(point.y()*UPSCALE_FACTOR));
        polygon.setPoint(i++, getUpscaledVector(point));
    }

    polygon.setOutlineColor(sf::Color::Black);
    polygon.setOutlineThickness(2);
    //polygon.setRotation(-90.0);


    return polygon;
}
sf::ConvexShape SFMLDrawUtils::getCube(const STConf &stConf, bool isStart, int width, int height)
{
    sf::ConvexShape polygon;
    polygon.setPosition(width / 2, height / 2);
    Polygon_2 toDraw = Utils::generateRobotAura(stConf.location);
    polygon.setPointCount(4);
    int i = 0;
    for (const Point_2& point : toDraw.vertices()) {
        //sf::Vector2f sfmlPoint(CGAL::to_double(point.x()*UPSCALE_FACTOR), CGAL::to_double(point.y()*UPSCALE_FACTOR));
        polygon.setPoint(i++, getUpscaledVector(point));
    }

    polygon.setOutlineColor(sf::Color::Black);
    polygon.setOutlineThickness(2);
    polygon.setFillColor(isStart ? sf::Color::Green : sf::Color::Magenta);

    //polygon.setRotation(-90.0);
    //sf::Vector2f actV = polygon.getPosition()+ polygon.getLocalBounds().getPosition();
    const float xV = (polygon.getPoint(0).x + polygon.getPoint(1).x +polygon.getPoint(2).x +polygon.getPoint(3).x)/4;
    const float yV = (polygon.getPoint(0).y + polygon.getPoint(1).y +polygon.getPoint(2).y +polygon.getPoint(3).y)/4;
//    std::cout << "Poly:" + std::to_string(xV) + ","+ std::to_string(yV)<< std::endl;
//    std::cout << "Poly:" + std::to_string(polygon.getPosition().x) + ","+ std::to_string(polygon.getPosition().y)<< std::endl;
//    std::cout << "Poly:" + std::to_string(xV+polygon.getPosition().x) + ","+ std::to_string(yV+polygon.getPosition().y)<< std::endl;

    return polygon;
}
sf::Text SFMLDrawUtils::getText(const Point_2& location, const std::string& text, const int width, const int height)
{
    if(!loadedFont) {
        font.loadFromFile("arial.ttf");
        loadedFont = true;
    }
    sf::Text toDraw(text, font, 16);
    toDraw.setFillColor(sf::Color::Black);
    sf::Vector2f relativePos = getUpscaledVector(location);
    const float xCenter = width/2;
    const float yCenter = height/2;
    toDraw.setPosition(relativePos + sf::Vector2f{xCenter - ROBOT_SIZE*UPSCALE_FACTOR, yCenter - ROBOT_SIZE*UPSCALE_FACTOR});
    sf::Vector2f actV = toDraw.getPosition();
//    std::cout << "Text:" + std::to_string(relativePos.x) + ","+ std::to_string(relativePos.y)<< std::endl;
//    std::cout << "Text:" + std::to_string(xCenter) + ","+ std::to_string(yCenter)<< std::endl;
//    std::cout << "Text:" + std::to_string(actV.x) + ","+ std::to_string(actV.y)<< std::endl;
    return toDraw;
}

sf::Vector2f SFMLDrawUtils::getUpscaledVector(const Point_2& location) {
//    std::cout << "Translating:" << location.x()*UPSCALE_FACTOR << ","<< location.y()*UPSCALE_FACTOR<< std::endl;
    sf::Vector2f sfVector(CGAL::to_double(location.x()*UPSCALE_FACTOR), -CGAL::to_double(location.y()*UPSCALE_FACTOR)); //Invert y-axis (SFML y-axis is oriented downwards)
//    std::cout << "To:" << sfVector.x << "," << sfVector.y<< std::endl;
//    sf::Vector2f sfVector2(CGAL::to_double(location.x())*UPSCALE_FACTOR, CGAL::to_double(location.y())*UPSCALE_FACTOR);
//    std::cout << "ToAlt:" << sfVector2.x << "," << sfVector2.y<< std::endl;
    return sfVector;
}

sf::Vector2f SFMLDrawUtils::getUpscaledOffsetVector(const Point_2& location, const int xOffset, const int yOffset) {
    sf::Vector2f sfVector(CGAL::to_double(location.x()*UPSCALE_FACTOR+ xOffset), -CGAL::to_double(location.y()*UPSCALE_FACTOR)+ yOffset);
    return sfVector;
}

