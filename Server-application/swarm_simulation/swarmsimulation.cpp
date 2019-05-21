#include "swarmsimulation.h"
#include "swarmsimulationsettings.h"


#define toRad(angleDegrees) ((angleDegrees) * M_PI / 180.0)
#define toDeg(angleRadians) ((angleRadians) * 180.0 / M_PI)
SwarmSimulation::SwarmSimulation()
{

}
double map(double x, double x1, double x2, double y1, double y2)
{
 return (x - x1) * (y2 - y1) / (x2 - x1) + y1;
}
void SwarmSimulation::moveRobot(RobotLocation *robot)
{
//    double deltaX = robot->destinationX - robot->x;
//    double deltaY = robot->destinationY - robot->y;

//    if((abs(deltaX) <= 1) && (abs(deltaY) <= 1))return;//already on location

//    double goalAngle = atan2(deltaY,deltaX);
//    while(goalAngle <0) goalAngle += M_PI * 2;

//    double currentAngle = (robot->angle - 90) * (M_PI / 180);
//    while(currentAngle < 0) currentAngle += M_PI * 2;

//    if(currentAngle - goalAngle > 0)
//    {
//        currentAngle -= std::min(currentAngle - goalAngle, 0.1);
//    }
//    else if(currentAngle - goalAngle < 0)
//    {
//        currentAngle += std::min(goalAngle - currentAngle, 0.1);
//    }
//    qDebug("%f , %f",currentAngle,goalAngle);

//    robot->x = robot->x + std::fmin(cos(goalAngle) * robot->speed * swarmSimulationSettings.maxSpeed, abs(deltaX));
//    robot->y = robot->y + std::fmin(sin(goalAngle) * robot->speed * swarmSimulationSettings.maxSpeed, abs(deltaY));
//    robot->angle = (int)(currentAngle * (180/M_PI) + 450) % 360;


    double deltaX = robot->destinationX - robot->x;
    double deltaY = robot->destinationY - robot->y;
    double distanceFromDestination = sqrt(deltaX*deltaX + deltaY*deltaY);//pythogoras
    if((abs(deltaX) <= 4) && (abs(deltaY) <= 4))return;//already on location

    double goalAngle = atan2(deltaY,deltaX);
    while(goalAngle <0) goalAngle += M_PI * 2;
    double currentAngle = robot->angle;
    while(currentAngle < 0) currentAngle += M_PI * 2;

    double difference = currentAngle - goalAngle;
    if(difference > M_PI)difference -= 2* M_PI;
    if(difference < -M_PI)difference += 2 * M_PI;
    if(abs(difference) > 0.6 * M_PI)//0.6 is added for hysteresis
    {
        robot->angle += M_PI;
    }
    while(currentAngle >= 2*M_PI) currentAngle -= M_PI * 2;

    int maxSpeed = robot->speed;
    if(distanceFromDestination < 100)
    {
        maxSpeed = map(distanceFromDestination,0,100,maxSpeed/2,maxSpeed);
    }
    double left = 0;
    double right = 0;
    if(abs(difference) < 0.1 * M_PI)
    {
        left = maxSpeed;
        right = maxSpeed;
    }
    else if(difference > 0)
    {
        left = maxSpeed;
        if(difference > 0.5 * M_PI)
        {
            if(distanceFromDestination > 100)
            {
                right = map(difference,0,0.5 * M_PI,-maxSpeed,0);
            }
            else {
                right = map(difference,0,0.5 * M_PI,-maxSpeed,-maxSpeed/2);
            }

        }
        else {
            right = -maxSpeed;
        }
    }
    else if(difference < 0)
    {
        right = maxSpeed;
        if(difference > -0.5 * M_PI)
        {
            if(distanceFromDestination > 100)
            {
                left = map(difference,-0.5 * M_PI,0,-maxSpeed, 0);
            }
            else {
                left = map(difference,-0.5 * M_PI,0,-maxSpeed, -maxSpeed/2);
            }
        }
        else {
            left = -maxSpeed;
        }
    }

    double acceleration= 5;
    if(left > robot->currentSpeedLeft)
    {
        robot->currentSpeedLeft+= acceleration;
    }
    if(left < robot->currentSpeedLeft)
    {
        robot->currentSpeedLeft-= acceleration;
    }
    if(right > robot->currentSpeedRight)
    {
        robot->currentSpeedRight+= acceleration;
    }
    if(right < robot->currentSpeedRight)
    {
        robot->currentSpeedRight-= acceleration;
    }
    moveWheels(robot->currentSpeedLeft, robot->currentSpeedRight, robot);
}
void SwarmSimulation::moveWheels(double Vl, double Vr, RobotLocation* robot)
{
    //calculations from https://www.robotc.net/wikiarchive/File:Differential_Steering_Graphic_2_wheels.png
    //https://www.robotc.net/wikiarchive/Tutorials/Arduino_Projects/Additional_Info/Turning_Calculations
    double l = globalSettings.botDistanceBetweenWheels;
    double currentAngle = robot->angle;
    if(abs(Vr - Vl) <0.5)//if there is no steering the formula doesn't work, adding 0.01 is solution to fix it, in real life the robots will never go perfectly straith
    {
        Vr += 0.01;
    }
    double r = (l/2) * (Vl + Vr) / (Vr - Vl);

    double wdt = (Vr - Vl) * deltaT * globalSettings.simulationSpeed / l;

    double ICCx = robot->x - r * sin(currentAngle);
    double ICCy = robot->y + r * cos(currentAngle);

    double values[] = {
        cos(wdt), -sin(wdt), 0,
        sin(wdt), cos(wdt), 0,
        0, 0, 1
    };
    QGenericMatrix<3,3,double> matrix(values);

    double values2[] = {
        robot->x - ICCx, robot->y - ICCy, currentAngle
    };

    QGenericMatrix<1,3,double> matrix2(values2);
    double values3[] = {
        ICCx, ICCy, wdt
    };

    QGenericMatrix<1,3,double> matrix3(values3);
    QGenericMatrix<1,3,double> result = matrix * matrix2 + matrix3;
    robot->x = result.data()[0];
    robot->y = result.data()[1];
    robot->angle = result.data()[2];
    while(robot->angle >= 2 * M_PI) robot->angle -=2 * M_PI;
    while(robot->angle < 0) robot->angle +=2 * M_PI;

}
void SwarmSimulation::startSimulation()
{
    qDebug() << "start simulation called" << endl;
    deltaT = (double)(clock() - lastTime)/CLOCKS_PER_SEC;
    deltaT = std::fmax(deltaT, 0.1);
    lastTime = clock();
    QListIterator<RobotLocation*> i(robotLocationManager.robots);
    while (i.hasNext())
    {
        RobotLocation *currentRobot = i.next();
        if(currentRobot->type == RobotLocation::RobotType::SIMULATED)
        {
             moveRobot(currentRobot);
        }
    }


    emit simulationFinished();
}

