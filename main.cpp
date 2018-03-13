#include "MainWindow.hpp"
#include <QApplication>

#include <QVector>
#include "algorithms/triangle.hpp"

int main(int argc, char *argv[])
{
    qRegisterMetaType<Triangle>("Triangle");
    qRegisterMetaType<QVector<Triangle>>("QVector<Triangle>");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
