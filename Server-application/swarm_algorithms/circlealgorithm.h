#pragma once

#include "linearmotionalgorithms.h"
#include "math.h"
#include "swarmalgorithmssettings.h"

class CircleAlgorithm : public LinearMotionAlgorithms
{
public:
    CircleAlgorithm();
    virtual void update();
    QPoint *center = new QPoint(500,500);
    QPoint *outer1 = new QPoint(200,400);
    QPoint *outer2 = new QPoint(800,600);

private:
    void findRobotMovementInputs();
protected:
    int distanceFromCenter(int x, int y);
    void calculateDestinationsCenterOuter();
    void calculateDestinationsOuterOuter();
    void calculateDestinationsOuterAngle();
    void calculateDestinationsCenterOuter(double beginAngle, double endAngle);
};
