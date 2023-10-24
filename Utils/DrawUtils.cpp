//
// Created by joelh on 24/10/2023.
//

#include "DrawUtils.h"
#include "../mytypedefs.h"
#include <CGAL/Qt/Basic_viewer_qt.h>
#include <CGAL/Qt/PainterOstream.h>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <boost/graph/graphviz.hpp>
#include <vector>

void DrawUtils::drawPolygon(const Polygon_2& polyToDraw) {


    // Define your polygon
    //Polygon_2 polygon;
    // Add points and edges to your polygon
    // ...

    // Define a vector of points
    std::vector<Point_2> points;
    // Add points to the vector
    // ...
//
//    CGAL::Qt::Basic_viewer_qt<> viewer;
//    Basic_viewer_qt::Basic_viewer_qt

    // Create a PainterOstream for drawing in the viewer
    //CGAL::Qt::PainterOstream<K> painter(viewer);
    QPixmap pixmap(200, 200);  // Create a pixmap with the desired dimensions
    QPainter qp(&pixmap);

    CGAL::Qt::PainterOstream<K> painter(&qp);//, QRectF(0, 0, 200, 200));

    // Draw the Polygon_2
    for (auto edge = polyToDraw.edges_begin(); edge != polyToDraw.edges_end(); ++edge) {
        Segment_2 segment = *edge;
        painter << segment;
    }

    QImage image = pixmap.toImage(); // Convert QPixmap to QImage
    QLabel label;
    label.setPixmap(QPixmap::fromImage(image)); // Set the QImage in a QLabel
    label.show(); // Display the QLabel

    // Draw the points in the vector
    for (const Point_2 &point : points) {
        painter << point;
    }
    //viewer.show();
    return;
    //return app.exec();
}
//
//void drawPolygonQT(Polygon_2 polyToDraw) {
//    // Initialize Qt application
//    QApplication app();//argc, argv);
//
//    // Define your polygon
//    Polygon_2 polygon;
//    // Add points and edges to your polygon
//    // ...
//
//    // Define a vector of points
//    std::vector<Point_2> points;
//    // Add points to the vector
//    // ...
//
//    // Create a QGraphicsScene and a QGraphicsView
//    QGraphicsScene scene;
//    QGraphicsView view(&scene);
//    view.setRenderHint(QPainter::Antialiasing);
//    view.show();
//
//    // Draw the Polygon_2
//    CGAL::Qt::GraphicsViewPolygonInput<K> ppg(scene);
//    ppg.generate_polygon(polygon);
//
//    // Draw the points in the vector
//    for (const Point_2 &point : points) {
//        scene.addItem(new QGraphicsEllipseItem(point.x(), point.y(), 0.05, 0.05)); // Adjust size as needed
//    }
//
//    return app.exec();
//}