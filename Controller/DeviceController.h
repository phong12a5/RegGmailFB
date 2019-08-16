#ifndef DEVICECONTROLLER_H
#define DEVICECONTROLLER_H

#include "Base/BaseControllertroller.h"
#include "AppDefines.h"
#include "AppEnums.h"
#include <QString>
#include <Processing/ADBCommand.h>
#include "Processing/HttpRequestController.h"
#include "Processing/CheckFbScreenWorker.h"
#include <QThread>

#ifdef USE_OPENCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#endif

class DeviceController : public QObject
{
    Q_OBJECT
public:
    DeviceController(QString deviceName);
    ~DeviceController();

    QString currentActivity() const;
    void setCurrentActivity(QString data);

    USER_INFOR &userInfo();
    void setUserInfo(USER_INFOR data);

private:
    QString deviceName() const;

    AppEnums::E_EXCUTE_STEPS currentExcuteStep();
    void setCurrentExcuteStep(AppEnums::E_EXCUTE_STEPS step);

    int fbScreenId() const;
    void setFbScreenId(int screenId);

    QString sendCaptcherScreen(QString screenPath);

    void connectSignalSlot();

    void changeDeviceInfoHandler();
    void regGmailHandler();
    void regFbHandler();

    void pressBoDKeyboard(int number);

private:
    QString m_deviceName;
    QString m_currentActivity;
    AppEnums::E_EXCUTE_STEPS m_currentExcuteStep;
    USER_INFOR m_userInfo;
    QTimer* m_checkNotificationTimer;
    QTimer* m_checkFBScreenTimer;
    QTimer* m_keepFbSrcCheckerTimer;
    QTimer* m_waitForSyncTimer;

    QThread* m_checkFbSCreenThread;
    CheckFbScreenWorker* m_checkFbScreenWorker;
    int m_fbScreenId;

signals:
    void currentActivityChanged();
    void processFinished(int step, int exitedCode);
    void fbScreenIdChanged(int screenId);
    void missionCompleted(int exitedCode, QString deviceName);
    void startCheckFbScreen();
    void stopCheckFbScreen();
    void operate();

public slots:
    void doWork();
    void onCheckNotification();
    void onProcessFinished(int step, int exitedCode);

    void onCurrentActivityChanged();
    void onUpdateCurrentActivity(QString activity);

    void onFbScreenIdChanged(int screenId);
    void onUpdateFbScreen(int screenId);
    void onCouldNotDectectScreen();

    void onWaitForSyncTimeout();
};

#endif // DEVICECONTROLLER_H
