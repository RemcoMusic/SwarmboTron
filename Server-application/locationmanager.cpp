#include "locationmanager.h"
LocationManager locationManager;
LocationManager::LocationManager()
{

}

void LocationManager::printAllRobots()
{
    QListIterator<RobotLocation*> i(locationManager.robots);
    while (i.hasNext())
    {
        i.next()->print();
    }
}
RobotLocation *LocationManager::addSimulatedRobot(RobotGroup *group)
{
    RobotLocation *newRobot = new RobotLocation(group);
    newRobot->type = RobotLocation::RobotType::SIMULATED;
    robots.append(newRobot);
    return newRobot;
}
void LocationManager::deleteAllSimulatedRobots()
{
    QMutableListIterator<RobotLocation*> i(robots);
    while (i.hasNext())
    {
        if(i.next()->type == RobotLocation::RobotType::SIMULATED)
        {
            i.remove();
        }
    }
}
void LocationManager::deleteAllSimulatedRobotsFromGroup(RobotGroup *group)
{
    QMutableListIterator<RobotLocation*> i(robots);
    while (i.hasNext())
    {
        auto val = i.next();
        if(val->type == RobotLocation::RobotType::SIMULATED)
        {
            if(val->group == group)
            {
                i.remove();
            }
        }
    }
}

void LocationManager::addObject(Object *object)
{
    objects.append(object);
}

void LocationManager::makeNewRealRobot(int x, int y)
{
    RobotLocation *newRobot = new RobotLocation();
    newRobot->type = RobotLocation::RobotType::REAL;
    newRobot->x = x;
    newRobot->y = y;
    newRobot->myColor = Qt::red;
    newRobot->sharedData.status = robotStatus::STARTUP;
    robots.append(newRobot);
    //dataScene.addItem(newRobot);
    //Ui::ui->dataSene->addItem(newRobot);
    dataScene->addItem(newRobot);
    qDebug() << "MADE A NEW ROBOT";
}