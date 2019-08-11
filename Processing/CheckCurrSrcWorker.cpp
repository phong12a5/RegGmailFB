#include "CheckCurrSrcWorker.h"
#include "AppMain.h"

#define APP_MAIN    AppMain::instance()

CheckCurrSrcWorker::CheckCurrSrcWorker(QObject *parent) : QObject(parent)
{
    m_currentActivity = "";
}

CheckCurrSrcWorker::~CheckCurrSrcWorker()
{
    LOG;
    m_updateCurrActTimer->stop();
    delete m_updateCurrActTimer;
}

void CheckCurrSrcWorker::doWork()
{
    QString result;
    m_updateCurrActTimer = new QTimer(this);
    m_updateCurrActTimer->setInterval(1000);
    m_updateCurrActTimer->setSingleShot(false);
    QObject::connect(m_updateCurrActTimer, SIGNAL(timeout()), this, SLOT(onUpdateCurrentActivity()));
    m_updateCurrActTimer->start();
}

void CheckCurrSrcWorker::onUpdateCurrentActivity()
{
    QStringList outputList;
    outputList.clear();
    foreach(QString device, APP_MAIN->deviceList()){
        outputList << ADBCommand::currentActivity(device);
    }
    emit updateCurrentActOnDevices(outputList);
}
