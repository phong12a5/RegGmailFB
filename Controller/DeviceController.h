#ifndef DEVICECONTROLLER_H
#define DEVICECONTROLLER_H

#include "Base/BaseControllertroller.h"
#include "AppDefines.h"
#include "AppEnums.h"
#include <QString>
#include <Processing/ADBCommand.h>
#include "Processing/HttpRequestController.h"

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

private:
    QString deviceName() const;

    AppEnums::E_EXCUTE_STEPS currentExcuteStep();
    void setCurrentExcuteStep(AppEnums::E_EXCUTE_STEPS step);

    USER_INFOR &userInfo();
    void setUserInfo(USER_INFOR data);

    int fbScreenId() const;
    void setFbScreenId(int screenId);

    QString sendCaptcherScreen(QString screenPath);

    void connectSignalSlot();
    void saveOutput();

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
    int m_fbScreenId;
    QList<int> m_fbScreenStack;

signals:
    void currentActivityChanged();
    void processFinished(int step, int exitedCode);
    void fbScreenIdChanged();

public slots:
    void doWork();
    void onCurrentActivityChanged();
    void onProcessFinished(int step, int exitedCode);
    void onCheckNotification();
    void onCheckFBScreen();
    void onFbScreenIdChanged();
};

#endif // DEVICECONTROLLER_H
