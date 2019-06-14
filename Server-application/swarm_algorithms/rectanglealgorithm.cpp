#include "rectanglealgorithm.h"

RectangleAlgorithm::RectangleAlgorithm()
{
    algorithmDiscription.name = "rectangle algorithm";
    algorithmDiscription.discription = "bots drive in the shape of a rectangle";
}

void RectangleAlgorithm::update()
{
    LinearMotionAlgorithms::generateRobotList();
    LinearMotionAlgorithms::clearDestinations();
    userInputs.clear();
    if(swarmAlgorithmsSettings.inputSource == SwarmAlgorithmsSettings::AlgorithmInputSource::NONE)
    {
        calculatePoints();
    }
    else if(swarmAlgorithmsSettings.inputSource == SwarmAlgorithmsSettings::AlgorithmInputSource::OBJECTS)
    {
        findObjectInputs(true);
        processUserInputs();
    }
    else if(swarmAlgorithmsSettings.inputSource == SwarmAlgorithmsSettings::AlgorithmInputSource::ROBOT_MOVEMENT)
    {
        findRobotMovementInputs(data.swarmRobots);
        processUserInputs();
    }
    LinearMotionAlgorithms::update();
}

void RectangleAlgorithm::processUserInputs()
{
    QListIterator<Object*> iterator(userInputs);
    while (iterator.hasNext())
    {
        Object *currentObject = iterator.next();
        if(distanceBetweenPoints(currentObject->x,currentObject->y,point1->x(),point1->y()) < 200)
        {
            point1->setX(currentObject->x);
            point1->setY(currentObject->y);
        }
        else if(distanceBetweenPoints(currentObject->x,currentObject->y,point2->x(),point2->y()) < 200)
        {
            point2->setX(currentObject->x);
            point2->setY(currentObject->y);
        }
        else if(distanceBetweenPoints(currentObject->x,currentObject->y,point1->x(),point2->y()) < 200)
        {
            point1->setX(currentObject->x);
            point2->setY(currentObject->y);
        }
        else if(distanceBetweenPoints(currentObject->x,currentObject->y,point2->x(),point1->y()) < 200)
        {
            point2->setX(currentObject->x);
            point1->setY(currentObject->y);
        }
    }
    calculatePoints();
}
static void constrainPoint(QPoint *point, int xMin, int yMin, int xMax, int yMax)
{
    point->rx() = std::max(xMin, point->x());
    point->ry() = std::max(yMin, point->y());
    point->rx() = std::min(xMax, point->x());
    point->ry() = std::min(yMax, point->y());
}
void RectangleAlgorithm::inputValidation()
{
    int clearance  = globalSettings.botDiameter/2;
    constrainPoint(point1, clearance, clearance, globalSettings.fieldSizeX-clearance, globalSettings.fieldSizeY-clearance);
    constrainPoint(point2, clearance, clearance, globalSettings.fieldSizeX-clearance, globalSettings.fieldSizeY-clearance);
}
void RectangleAlgorithm::calculatePoints()
{
    inputValidation();
    //calculate distance between markers
    //point1 is left top, point2 is right bottom
    int deltaX = abs(point2->rx() - point1->rx());
    int deltaY = abs(point2->ry() - point1->ry());

    //the rectangle have 4 lines, 2 x lines and 2 y lines
    int amountOfRobotsFittingXLines = deltaX/swarmAlgorithmsSettings.distanceBetweenRobots;
    int amountOfRobotsFittingYLines = deltaY/swarmAlgorithmsSettings.distanceBetweenRobots;

    //calculate the amount of robots fitting in each of the for lines and sum them
    int amountOfRobotsFitting = amountOfRobotsFittingXLines*2 + amountOfRobotsFittingYLines*2;

    int amountOfRobotsUsing = std::min(amountOfRobotsFitting, data.swarmRobots.size());
    if(amountOfRobotsUsing <= 3)
    {
        qDebug("amountOfRobotsUsing <= 3");
        return;
    }

    double xyRatio = ((double)amountOfRobotsUsing) / amountOfRobotsFitting;
    int amountOfRobotsUsingXLines = amountOfRobotsFittingXLines * xyRatio;
    int amountOfRobotsUsingYLines = (amountOfRobotsUsing - amountOfRobotsUsingXLines*2) / 2;

    int distanceBetweenRobotsXLines = (amountOfRobotsUsingXLines > 0) ? deltaX/(amountOfRobotsUsingXLines) : deltaX/2;
    int distanceBetweenRobotsYLines = (amountOfRobotsUsingYLines > 0) ? deltaY/(amountOfRobotsUsingYLines) : deltaY/2;

    for(int i=0;i<amountOfRobotsUsingXLines;i++)
    {
        Destination *newDestination = new Destination;
        newDestination->x = point1->rx()  + i * distanceBetweenRobotsXLines;
        newDestination->y = point1->ry();
        newDestination->endAngle = 0;
        destinations.append(newDestination);
    }
    for(int i=1;i<amountOfRobotsUsingXLines+1;i++)
    {
        Destination *newDestination = new Destination;
        newDestination->x = point1->rx()  + i * distanceBetweenRobotsXLines;
        newDestination->y = point2->ry();
        newDestination->endAngle = 0.5 * M_PI;
        destinations.append(newDestination);
    }
    for(int i=1;i<amountOfRobotsUsingYLines + 1;i++)
    {
        Destination *newDestination = new Destination;
        newDestination->x = point1->rx();
        newDestination->y = point1->ry() + i * distanceBetweenRobotsYLines;
        newDestination->endAngle = M_PI;
        destinations.append(newDestination);
    }
    for(int i=0;i<amountOfRobotsUsingYLines;i++)
    {
        Destination *newDestination = new Destination;
        newDestination->x = point2->rx();
        newDestination->y = point1->ry() + i * distanceBetweenRobotsYLines;
        newDestination->endAngle = 1.5 *  M_PI;
        destinations.append(newDestination);
    }
}

