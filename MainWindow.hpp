#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

#include "algorithms/delaunay.hpp"
#include "algorithms/triangle.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    /// When clicked on button 'Exit'.
    void on_bExit_clicked();

    /// When clicked on button 'CreateNode'.
    void on_bCreateNode_clicked();

    /// When clicked on button 'ClearAll'
    void on_bClearGraphicsView_clicked();

    /// When clicked on button 'Delaunay trinagulation'.
    void on_bPerformDelaunay_clicked();

    /// When clicked on button 'Generate random'.
    void on_bGenerateRandom_clicked();

    /// When clicked on button 'Delaunay animation'.
    void on_bPerformDelaunayAnimation_clicked();

    /// When clicked on button 'Delete edges'.
    void on_bClearEdges_clicked();

    /// When clicked on button 'Clear log'.
    void on_bClearLog_clicked();

    /// Shows the about window.
    void showAboutWindow();

    /// Shows the help window.
    void showHelpWindow();

    /// Loads vertices from csv file.
    void loadData();

    /// Saves the constructed image onto csv file.
    void saveData();

    /// Shows the user a new window which tells more about algorithm.
    void showAboutAlgorithmWindow();

    /// Resumes the animation.
    void on_bPlay_clicked();

    /// Pauses the animation.
    void on_bPause_clicked();

    // Slots that communicate with the delunay triangulation algorithm.
    void handleAlgorithmMessage(QString);
    void handleSuperTriangle(const Triangle & t);
    void handleUpdatedTriangles(QVector<Triangle>);
    void handleAlgorithmEnd(QVector<Triangle>);
    void handleProgressEvent(unsigned percentage);
    void handleAlgoCircle(const Triangle & t, const QPoint & center, float radius, const QPoint & vertex);
    void handleCurrentPoint(const QPointF & p);
    void clearCircles();

    void handleBadTriangles(const Triangle & t);

    /// Threads connect to this slot to tell this class that it's safe to do animation again.
    void setSafeAnimation();


private:
    Ui::MainWindow *ui;
    void setupMenubarIcons();

    Delaunay* m_triangulationThread;
    bool m_safe_for_animation;

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent* event);
};

#endif // MAINWINDOW_HPP
