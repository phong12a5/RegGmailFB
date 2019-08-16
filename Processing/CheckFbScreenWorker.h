#ifndef CHECKCURRSRCWORKER_H
#define CHECKCURRSRCWORKER_H

#include <QObject>
#include <AppDefines.h>
#include <QTimer>
#include "ADBCommand.h"
#include "AppEnums.h"

class CheckFbScreenWorker : public QObject
{
    Q_OBJECT
public:
    explicit CheckFbScreenWorker(QString deviceName);
    ~CheckFbScreenWorker();

private:
    QString getMatchingImg2ScreenId(int screenID);

private:
    QTimer* m_updateFbScreenTimer;
    QTimer* m_updateCurrActTimer;
    QTimer* m_keepScreenTimer;
    QString m_deviceName;
    int m_screenId;
    QList<int> m_fbScreenStack;

signals:
    void updateFbScreen(int screenId);
    void updateCurrentActivity(QString activity);
    void couldNotDectectScreen();

public slots:
    void doWork();
    void onUpdateCurrentActivity();
    void onUpdateCurrentFbScreen();
    void onStartCheckFbScreen();
    void onStopCheckFbScreen();
    void onSyncFbScreenId(int screenId);
    void onKeepScreenTimeout();
};

#endif // CHECKCURRSRCWORKER_H
