//code review rene: use right order in the h file, fist methodes than fields

#ifndef ROBOTCONNECTION_H
#define ROBOTCONNECTION_H
#include "swarmbotcommunicationsettings.h"
#include <QObject>
#include <QUdpSocket>
#include <QDebug>
#include "robotlocation.h" // for the UDP struct
#include "robotlocationmanager.h"
#include <QList>
#include <QTime>
//(rene) extern declaration not needed
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
        //(rene) give a better name
    QTime myTimer;


    void processIP(QString ip);
    void turnRobotOn(QString ip);
    void updateRobots();


};

#endif // ROBOTCONNECTION_H
