#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <QGraphicsScene>
#include <QFileIconProvider>
#include <QMenuBar>
#include <QMessageBox>
#include <QTextBrowser>
#include <QFileDialog>
#include <QDebug>
#include <QDir>
#include <QSpinBox>
#include <QStringList>
#include <QCheckBox>
#include <QProgressBar>
#include <QTime>

#include <vector>
#include <iostream>

#include "algorithms/vector2.hpp"
#include "algorithms/delaunay.hpp"
#include "algorithms/triangle.hpp"
#include "algorithms/edge.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupMenubarIcons();
    m_safe_for_animation = true;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_bExit_clicked()
{
    close();
}

void MainWindow::on_bCreateNode_clicked()
{
    ui->progressBarAlgoStatus->setValue(0);
    ui->graphicsView->createNode();
}

void MainWindow::on_bClearGraphicsView_clicked()
{
    ui->progressBarAlgoStatus->setValue(0);
    ui->graphicsView->scene()->clear();
}

void MainWindow::showAboutWindow()
{
    QMessageBox::information(this, "About", "Created by Nemanja Micovic.\nContact at nmicovic@outlook.com");
}

void MainWindow::showHelpWindow()
{
    QString help = "Use the left mouse click to move vertices.\n";
    help += "While holding left mouse button, you can move around the canvas.\n\n";

    help += "DATA:\n";
    help += "* Create node - creates a node\n";
    help += "* Clear all - Clears created construction\n";
    help += "* Delete edges - Removes created edges\n";
    help += "* Generate random - Generates selected number of random vertices\n";
    help += "\n";

    help += "TRIANGULATION:\n";
    help += "* Delaunay triangulation - Performs the delaunay triangulation without animation)\n";
    help += "* Delaunay animation - Performs the delaunay triangulation with animation)\n";
    help += "\n";

    help += "Adittional:\n";
    help += "* Load data - Lets you load vertices for triangulation\n";
    help += "* Save data - Lets you save created image\n";
    help += "* Exit - Exists the application\n";

    QMessageBox::information(this, "Help", help);
}

void MainWindow::loadData()
{
    QString startPath = QDir::homePath();
    qDebug() << "home: " << QDir::homePath();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open data file"), "", tr("Text files (*.txt *.csv)"));
    if (fileName.isEmpty())
        qDebug() << "User hasn't selected a file...";
    else
        qDebug() << "User has selected path'" << fileName << "'";

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);

    std::vector<Vec2f> ps;
    std::vector<Triangle> ts;
    while (!in.atEnd()) {
        QString line = in.readLine();

        QStringList fields = line.split(",");
        qDebug() << "elements: " << fields.size();
        if (fields.size() == 2) {
            // point
            float x = fields.at(0).toFloat();
            float y = fields.at(1).toFloat();
            qDebug() << "read: " << x << y;
            ps.push_back(Vec2f(x, y));
        } else if (fields.size() == 6) {
            // edges
            float x1 = fields.at(0).toFloat();
            float y1 = fields.at(1).toFloat();
            float x2 = fields.at(2).toFloat();
            float y2 = fields.at(3).toFloat();
            float x3 = fields.at(4).toFloat();
            float y3 = fields.at(5).toFloat();
            qDebug() << "read: " << x1 << y1 << x2 << y2 << x3 << y3;
            ts.push_back(Triangle(Vec2f(x1, y1), Vec2f(x2, y2), Vec2f(x3, y3)));
        }
    }

    if (!ps.empty()) ui->graphicsView->setPoints(ps);

    // TODO: Not supported yet because of dependencies between nodes and edges.
    // if (!ts.empty()) ui->graphicsView->setTriangles(ts);

    file.close();
}

void MainWindow::saveData()
{
    QString startPath = QDir::homePath();
    qDebug() << "home: " << QDir::homePath();
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save Triangulation"), "",
            tr("CSV file (*.csv);"));
    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"), file.errorString());
            return;
        }
        QTextStream out(&file);
        //out.setVersion(QDataStream::Qt_4_5);
        auto xs = ui->graphicsView->items();
        for (auto & t : xs) {
            if (t->type() == gNode::TypeNode)
                out << static_cast<gNode*>(t)->str() << '\n';
            else if (t->type() == gEdge::TypeEdge)
                out << static_cast<gEdge*>(t)->str() << '\n';

        }
        file.close();
    }
}

void MainWindow::showAboutAlgorithmWindow()
{
    QString msg = "Algorithm that has been implemented is "
                  "Bowyer-Watson algorithm.\nlink: "
                  "https://en.wikipedia.org/wiki/Bowyer%E2%80%93Watson_algorithm\n\n\n";
    msg += "You can find implementation on github:\nlink: ";
    msg += "https://github.com/Bl4ckb0ne/delaunay-triangulation";

    QMessageBox::information(this, "About algorithm", msg);
}

void MainWindow::handleAlgorithmMessage(QString txt)
{
    if (ui->cbLogOn->isChecked())
        ui->tbLog->append(txt);
}

void MainWindow::handleSuperTriangle(const Triangle& t)
{
    ui->graphicsView->setSuperTriangle(t);
}

void MainWindow::handleUpdatedTriangles(QVector<Triangle> v)
{
    ui->graphicsView->setTriangles(v);
}

void MainWindow::handleAlgorithmEnd(QVector<Triangle> ts)
{
    ui->graphicsView->finalSetOfTheTriangles(ts);
}

void MainWindow::handleProgressEvent(unsigned percentage)
{
    ui->progressBarAlgoStatus->setValue(percentage);
}

void MainWindow::handleAlgoCircle(const Triangle& t, const QPoint& center, float radius, const QPoint& vertex)
{
    ui->graphicsView->addCircle(t, center, radius, vertex);
}

void MainWindow::handleCurrentPoint(const QPointF& p)
{
    ui->graphicsView->updateCurrentPoint(p);
}

void MainWindow::clearCircles()
{
    ui->graphicsView->removeCircles();
}

void MainWindow::handleBadTriangles(const Triangle& t)
{
    ui->graphicsView->setCurrentTriangle(t);
}

void MainWindow::setSafeAnimation()
{
    m_safe_for_animation = true;
    qDebug() << "Thread finished, it's safe for animation again!";
}

void MainWindow::setupMenubarIcons()
{
    QIcon icon= QIcon::fromTheme("file-new");
    ui->actionNew->setIcon(icon);
    icon= QIcon::fromTheme("file-load");
    ui->actionLoad->setIcon(icon);
    icon= QIcon::fromTheme("application-exit");
    ui->actionExit->setIcon(icon);
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    double scaleFactor = 1.15;
    switch (event->key()) {
    case Qt::Key_Plus:
        ui->graphicsView->scale(scaleFactor, scaleFactor);
        break;
    case Qt::Key_Minus:
        ui->graphicsView->scale(1/scaleFactor, 1/scaleFactor);
        break;
    }
}

void MainWindow::on_bPerformDelaunay_clicked()
{
    if (ui->graphicsView->scene()->items().isEmpty())
        return;
    ui->progressBarAlgoStatus->setValue(0);
    ui->tbLog->clear();

    // Remove all edges from previous triangulation if they exist.
    ui->graphicsView->removeEdges();


    if (! m_safe_for_animation) {
        qDebug() << "Refusing to start because last thread hasn't finished...";
        return;
    }

    // Collect all the nodes for triangulation
    std::vector<Vec2f> points;
    auto collection = ui->graphicsView->scene()->items();
    for (const auto & p : collection)
        if (p->type() == gNode::TypeNode)
            points.push_back(Vec2f(p->x(), p->y()));

    m_safe_for_animation = false;

    m_triangulationThread = new Delaunay(false, points, 0);
    connect(m_triangulationThread, SIGNAL(finished()), m_triangulationThread, SLOT(deleteLater()));
    connect(m_triangulationThread, SIGNAL(done(QVector<Triangle>)), this, SLOT(handleAlgorithmEnd(QVector<Triangle>)));
    connect(m_triangulationThread, SIGNAL(reportPercentage(uint)), this, SLOT(handleProgressEvent(uint)));
    connect(m_triangulationThread, SIGNAL(finished()), SLOT(setSafeAnimation()));

    qDebug() << "Triangluation thread without animation started!";
    m_triangulationThread->start();
}

void MainWindow::on_bGenerateRandom_clicked()
{
    ui->progressBarAlgoStatus->setValue(0);

    srand(time(NULL));

    float numberPoints = ui->sbNumNodes->value();

    std::vector<Vec2f> points;
    for(int i = 0; i < numberPoints; i++) {
        points.push_back(Vec2f(RandomFloat(0, ui->graphicsView->width()), RandomFloat(0, ui->graphicsView->height())));
    }

    ui->graphicsView->setPoints(points);
}


void MainWindow::on_bPerformDelaunayAnimation_clicked()
{
    if (ui->graphicsView->scene()->items().isEmpty()) {
        qDebug() << "Empty scene...triangulation is pointless...";
        return;
    }

    // Prepare everything for animation.
    on_bClearEdges_clicked();
    if (! m_safe_for_animation) {
        qDebug() << "Refusing to start because last thread hasn't finished...";
        return;
    }
    ui->progressBarAlgoStatus->setValue(0);
    ui->tbLog->clear();

    // Grab the vertices we wish to triangulate.
    std::vector<Vec2f> points;
    auto collection = ui->graphicsView->scene()->items();
    for (const auto & p : collection) {
        if (p->type() == gNode::TypeNode) {
            points.push_back(Vec2f(p->x(), p->y()));
        }
    }

    m_safe_for_animation = false;

    m_triangulationThread = new Delaunay(true, points, ui->sbAnimationSpeed->value());
    connect(m_triangulationThread, SIGNAL(updateMessage(QString)), this, SLOT(handleAlgorithmMessage(QString)));
    connect(m_triangulationThread, SIGNAL(finished()), m_triangulationThread, SLOT(deleteLater()));
    connect(m_triangulationThread, SIGNAL(superTriangle(Triangle)), this, SLOT(handleSuperTriangle(Triangle)));
    connect(m_triangulationThread, SIGNAL(updatedTriangles(QVector<Triangle>)), this, SLOT(handleUpdatedTriangles(QVector<Triangle>)));
    connect(m_triangulationThread, SIGNAL(done(QVector<Triangle>)), this, SLOT(handleAlgorithmEnd(QVector<Triangle>)));
    connect(m_triangulationThread, SIGNAL(reportPercentage(uint)), this, SLOT(handleProgressEvent(uint)));
    connect(m_triangulationThread, SIGNAL(showCircle(Triangle,QPoint,float,QPoint)), this, SLOT(handleAlgoCircle(Triangle,QPoint,float,QPoint)));
    connect(m_triangulationThread, SIGNAL(clearCircles()), this, SLOT(clearCircles()));
    connect(m_triangulationThread, SIGNAL(updateCurrentPoint(QPointF)), SLOT(handleCurrentPoint(QPointF)));
    connect(m_triangulationThread, SIGNAL(reportTriangle(Triangle)), SLOT(handleBadTriangles(Triangle)));
    connect(m_triangulationThread, SIGNAL(finished()), SLOT(setSafeAnimation()));

    qDebug() << "Triangluation thread with animation of speed" << ui->sbAnimationSpeed->value() << "started!";

    m_triangulationThread->start();
}

void MainWindow::on_bClearEdges_clicked()
{
    ui->graphicsView->removeEdges();
    ui->progressBarAlgoStatus->setValue(0);
}

void MainWindow::on_bClearLog_clicked()
{
    ui->tbLog->clear();
}

void MainWindow::on_bPlay_clicked()
{
    //if (m_triangulationThread != nullptr && m_triangulationThread->isRunning()) {
    if (!m_safe_for_animation) {
        m_triangulationThread->setThreadSleep(ui->sbAnimationSpeed->value());
        m_triangulationThread->play();
        qDebug() << "Setting play()";
    }
}

void MainWindow::on_bPause_clicked()
{
    //if (m_triangulationThread != nullptr && m_triangulationThread->isRunning())
    if (! m_safe_for_animation)
        m_triangulationThread->pause();
}
