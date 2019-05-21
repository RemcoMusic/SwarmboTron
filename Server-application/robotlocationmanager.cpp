#include "robotlocationmanager.h"
RobotLocationManager robotLocationManager;
RobotLocationManager::RobotLocationManager()
{

}

void RobotLocationManager::printAllRobots()
{
    QListIterator<RobotLocation*> i(robotLocationManager.robots);
    while (i.hasNext())
    {
        i.next()->print();
    }
}
RobotLocation *RobotLocationManager::addSimulatedRobot(RobotGroup *group)
{
    RobotLocation *newRobot = new RobotLocation(group);
    newRobot->type = RobotLocation::RobotType::SIMULATED;
    robots.append(newRobot);
    return newRobot;
}
void RobotLocationManager::deleteAllSimulatedRobots()
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
void RobotLocationManager::deleteAllSimulatedRobotsFromGroup(RobotGroup *group)
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

void RobotLocationManager::makeNewRealRobot(int x, int y)
{
    RobotLocation *newRobot = new RobotLocation();
    newRobot->type = RobotLocation::RobotType::REAL;
    robots.append(newRobot);
}
