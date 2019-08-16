#ifndef APPMAIN_H
#define APPMAIN_H

#include <QObject>
#include "AppDefines.h"
//#include <QQmlContext>
//#include <QQuickView>
//#include "AppEnums.h"
#include "AppDefines.h"
//#include "Controller/RegMailController.h"
//#include "Controller/RegDeviceInfoController.h"
//#include "Controller/RegFBController.h"
#include "Processing/ADBCommand.h"
//#include <QJsonObject>
//#include <QJsonDocument>
//#include <QVariant>
//#include <QThread>
//#include <QMutex>
//#include "Controller/ThreadController.h"
#include "Controller/DeviceController.h"
#include <QThread>
#ifdef USE_OPENCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#endif

class AppMain : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList deviceList READ deviceList WRITE setDeviceList NOTIFY deviceListChanged)

private:
    explicit AppMain(QObject *parent = nullptr);
    ~AppMain();
public:
    static AppMain *instance();
    void initApplication();

    QStringList deviceList() const;
    void setDeviceList(QStringList _devices);

    USER_INFOR generateUserInfo();

    QString getMatchingImg2ScreenId(int screenID);

    Q_INVOKABLE void startProgram();
    Q_INVOKABLE void closeProgram();

private:
    void loadUserInfo();

private:
    static AppMain* m_instance;

    QStringList m_deviceList;
    QList<DeviceController *> m_deviceCtrlList;
    QList<QThread *> m_matchThreadList;

    QStringList m_lastNameList;
    QStringList m_firstNameList;
    QStringList m_middleNameList;

signals:
    void oparate();
    void deviceListChanged();

public slots:
    void onMissionCompleted(int exitCode, QString deviceName);

};

#endif // APPMAIN_H
