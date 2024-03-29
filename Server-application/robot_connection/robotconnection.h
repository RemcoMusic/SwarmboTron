#ifndef ROBOTCONNECTION_H
#define ROBOTCONNECTION_H
#include "swarmbotcommunicationsettings.h"
#include <QObject>
#include <QUdpSocket>
#include <QDebug>
#include "robotlocation.h" // for the UDP struct
#include "locationmanager.h"
#include <QList>
#include <QTime>
extern SwarmBotCommunicationSettings communicationSettings;
class RobotConnection : public QObject
{
    Q_OBJECT
public:
    RobotConnection();
    QList<QString> IpList;
    void sendUDP(QByteArray byteArray,QString ip, int port);
public slots:
    void readyRead();
    void connectionloop();

signals:
    void done();


private:
    QUdpSocket * socket;
    QString lastRequestedBotIP = "0.0.0.0";
    QTime myTimer;


    void processIP(QString ip, uint16_t voltage);
    void turnRobotOn(QString ip);
    void updateRobots();
    void turnOffAllRobots();


};

#endif // ROBOTCONNECTION_H
