#ifndef CHECKCURRSRCWORKER_H
#define CHECKCURRSRCWORKER_H

#include <QObject>
#include <AppDefines.h>
#include <QTimer>
#include "ADBCommand.h"

class CheckCurrSrcWorker : public QObject
{
    Q_OBJECT
public:
    explicit CheckCurrSrcWorker(QObject *parent = nullptr);
    ~CheckCurrSrcWorker();

private:
    QTimer* m_updateCurrActTimer;
    QString m_currentActivity;

signals:
    void updateCurrentActOnDevices(QStringList result);

public slots:
    void doWork();
    void onUpdateCurrentActivity();
};

#endif // CHECKCURRSRCWORKER_H
