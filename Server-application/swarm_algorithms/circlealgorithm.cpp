#include "circlealgorithm.h"

#include <simulatedrobot.h>

CircleAlgorithm::CircleAlgorithm()
{
    algorithmDiscription.name = "circle algorithm";
    algorithmDiscription.discription = "bots drive in a circle defined by red objects";
    algorithmDiscription.minimalAmountOfBots = 1;
    algorithmDiscription.maximalAmountOfBots = 100;
}
void CircleAlgorithm::update()
{
    LinearMotionAlgorithms::generateRobotList();
    LinearMotionAlgorithms::clearDestinations();
    if(swarmAlgorithmsSettings.inputSource == SwarmAlgorithmsSettings::AlgorithmInputSource::NONE)
    {
        calculateDestinationsCenterOuter();
    }
    else if(swarmAlgorithmsSettings.inputSource == SwarmAlgorithmsSettings::AlgorithmInputSource::HAND_GESTURE)
    {
        calculateDestinationsCenterOuter();
    }
    else if(swarmAlgorithmsSettings.inputSource == SwarmAlgorithmsSettings::AlgorithmInputSource::REAL_OBJECTS_CENTER_OUTER)
    {
        calculateDestinationsCenterOuter();
    }
    else if(swarmAlgorithmsSettings.inputSource == SwarmAlgorithmsSettings::AlgorithmInputSource::REAL_OBJECTS_OUTER_OUTER)
    {
        calculateDestinationsOuterOuter();
    }
    else if(swarmAlgorithmsSettings.inputSource == SwarmAlgorithmsSettings::AlgorithmInputSource::ROBOT_MOVEMENT)
    {
        findRobotMovementInputs();
    }

    LinearMotionAlgorithms::update();
}
bool compereUserInputs(const RobotLocation* robot1, const RobotLocation* robot2)
{
    return robot1->lastDisplacement > robot2->lastDisplacement;
}
void CircleAlgorithm::findRobotMovementInputs()
{
    QList<RobotLocation*> userInputs;
    QListIterator<RobotLocation*> i(data.swarmRobots);
    while (i.hasNext())
    {
        RobotLocation *currentRobot = i.next();
        if(currentRobot->userInput)
        {
            userInputs.append(currentRobot);
        }
    }
    qSort(userInputs.begin(),userInputs.end(), compereUserInputs);
    if(userInputs.size() == 0)
    {
        calculateDestinationsCenterOuter();
    }
    else if(userInputs.size() == 1)
    {
        RobotLocation *currentRobot = userInputs.first();
        //first check if the robot is near the radius of the circle
        if(abs(distanceFromCenter(outer1->x(),outer1->y()) - distanceFromCenter(currentRobot->x,currentRobot->y)) < 100)
        {
            outer1->rx() = currentRobot->x;
            outer1->ry() = currentRobot->y;
        }

        calculateDestinationsCenterOuter();
    }
    else
    {
        RobotLocation *robot1 = userInputs.first();
        RobotLocation *robot2 = userInputs.at(1);
        outer1->rx() = robot1->x;
        outer1->ry() = robot1->y;
        outer2->rx() = robot2->x;
        outer2->ry() = robot2->y;
        calculateDestinationsOuterOuter();
    }
}
int CircleAlgorithm::distanceFromCenter(int x, int y)
{
    int deltaX = center->rx() - x;//pytagoras A
    int deltaY = center->ry() - y;//pytagoras b
    return  sqrt(deltaX*deltaX + deltaY*deltaY);
}
void CircleAlgorithm::calculateDestinationsOuterAngle()
{

}
void CircleAlgorithm::calculateDestinationsOuterOuter()
{
    //this method uses two outer places of the circle, it calculates the center so the center-outer method can do the rest
    int deltaX = outer2->x() - outer1->x();
    int deltaY = outer2->y() - outer1->y();
    center->rx() = outer1->x() + deltaX / 2;
    center->ry() = outer1->y() + deltaY / 2;
    calculateDestinationsCenterOuter();
}
void CircleAlgorithm::calculateDestinationsCenterOuter()
{
    int deltaX = outer1->rx() - center->rx();//pytagoras A
    int deltaY = outer1->ry() - center->ry();//pytagoras b
    double beginAngle = atan2(deltaY, deltaX);
    calculateDestinationsCenterOuter(beginAngle,2*M_PI);
}
void CircleAlgorithm::calculateDestinationsCenterOuter(double beginAngle, double endAngle)
{
    if(endAngle <= beginAngle)qFatal("calculateDestinationsCenterOuter endAngle <= beginAngle");

    //calculate distance between markers
    int deltaX = outer1->rx() - center->rx();//pytagoras A
    int deltaY = outer1->ry() - center->ry();//pytagoras b
    int c = sqrt(deltaX*deltaX + deltaY*deltaY);//pytagoras C, distance between points

    double circumference = (c *(endAngle - beginAngle));
    int amountOfRobotsFitting = circumference/swarmAlgorithmsSettings.distanceBetweenRobots;

    int amountOfRobotsUsing = std::min(amountOfRobotsFitting, data.swarmRobots.size());

    int distanceBetweenBots = circumference/amountOfRobotsUsing;
    double angleBetweenRobots = (endAngle - beginAngle)/amountOfRobotsUsing;
    if(swarmAlgorithmsSettings.debugLinearMotionSources)
    {
        qDebug("distance between points %d",c);
        qDebug("amount of bots fitting %d",amountOfRobotsFitting);
        qDebug("distance between bots %d",distanceBetweenBots);
        qDebug("angle between bots %f",angleBetweenRobots);
    }
    double angle = beginAngle;
    for(int i=0;i<amountOfRobotsUsing;i++)
    {
        Destination *newDestination = new Destination;
        newDestination->x = center->x() + cos(angle) * c;
        newDestination->y = center->y() + sin(angle) * c;
        newDestination->endAngle = angle;
        destinations.append(newDestination);
        //qDebug("new position %d, %d",newDestination->x,newDestination->y);

        angle+=angleBetweenRobots;
    }
}
