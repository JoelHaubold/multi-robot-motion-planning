//
// Created by joelh on 26/10/2023.
//

#ifndef MRMP_IMPLEMENTATION_SFMLDRAWUTILS_H
#define MRMP_IMPLEMENTATION_SFMLDRAWUTILS_H

#include "../mytypedefs.h"
#include <SFML/Graphics.hpp>

class SFMLDrawUtils
{
public:
    static void drawPolygon_2(const Polygon_2& toDraw, const std::string& name);
    static void drawFreeSpace(const std::vector<FreeSpaceComponent>& toDraw, const std::string& name);
    static void drawFStar(const std::vector<FStarComponent> &toDraw, const std::string &name);
    //static void drawPolygon_wh_2(Polygon_wh_2 toDraw, std::string name);
    //static void drawPolygonSet(Polygon_set_2 toDraw);
    //static void drawStartAndTargets(Polygon_set_2 toDraw, std::string name);

private:
//    static void drawPolygon(sf::ConvexShape polygon, sf::RenderWindow window);
//    static sf::RenderWindow getWindow(int height, int width);
    static void displayWindow(sf::RenderWindow& window);
    static sf::ConvexShape getSFPolygon(const Polygon_2& toDraw, int width, int height);
    static sf::ConvexShape getCube(const STConf& stConf, bool isStart, int width, int height);
    static sf::Text getText(const Point_2& location, const std::string& text, const int width, const int height);
    static sf::Vector2f getUpscaledVector(const Point_2& location);
};




#endif//MRMP_IMPLEMENTATION_SFMLDRAWUTILS_H
