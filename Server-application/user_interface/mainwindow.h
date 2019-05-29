#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <algrorithmvisualisation.h>
#include "robotlocation.h"
#include "locationmanager.h"
#include "ball.h"
#include <QTime>
#include <QDebug>
#include <robotdetectionsettings.h>
#include <QObject>
#include "swarmsimulationsettings.h"

extern QGraphicsScene *dataScene; //forward declaration (made in cpp)

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void colorSlidersChanged(int c);
    void on_pushButton_clicked();

    void on_colorComboBox_currentIndexChanged(int index);


    void on_sliderDilate_valueChanged(int value);


    void on_sliderDeviation_valueChanged(int value);

    void on_sliderErode_valueChanged(int value);

    void on_checkRealSimulation_stateChanged(int arg1);

    void on_checkAccelerationControl_stateChanged(int arg1);

    void on_checkInconsisentMotors_stateChanged(int arg1);

    void on_checkUADWLB_stateChanged(int arg1);

    void on_checkDynamicSpeed_stateChanged(int arg1);

    void on_checkRotationTime_stateChanged(int arg1);

    void on_SliderRobotSpeed_valueChanged(int value);

    void on_AddSimulatedRobotButton_clicked();

    void on_addSimulatedObjectButton_clicked();

public slots:
    void updateGui();
private:
    //QGraphicsScene *dataScene;
    //QGraphicsScene *cameraScene;
    void updateNumberOfRobots();
    QTime *fpsTimer;
    Ui::MainWindow *ui;
    bool flipFlop = true;
};

#endif // MAINWINDOW_H
