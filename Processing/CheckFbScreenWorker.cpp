#include "CheckFbScreenWorker.h"
#include "AppMain.h"

#define APP_MAIN    AppMain::instance()

CheckFbScreenWorker::CheckFbScreenWorker(QString deviceName)
{
    m_updateFbScreenTimer = nullptr;
    m_updateCurrActTimer = nullptr;
    m_deviceName = deviceName;
    m_screenId = -1;

    m_fbScreenStack.clear();
    m_fbScreenStack << AppEnums::E_FBLITE_SCREEN_ID_LOGIN\
                    << AppEnums::E_FBLITE_SCREEN_ID_JOIN_FB\
                    << AppEnums::E_FBLITE_SCREEN_ID_ENTER_NAME\
                    << AppEnums::E_FBLITE_SCREEN_ID_ENTER_MOBILE_NUM\
                    << AppEnums::E_FBLITE_SCREEN_ID_ENTER_EMAIL_ADDRESS\
                    << AppEnums::E_FBLITE_SCREEN_ID_ENTER_BIRTHDAY\
                    << AppEnums::E_FBLITE_SCREEN_ID_ENTER_GENDER\
                    << AppEnums::E_FBLITE_SCREEN_ID_ENTER_PASSWORD\
                    << AppEnums::E_FBLITE_SCREEN_ID_SAVE_LOGIN_INFO\
                    << AppEnums::E_FBLITE_SCREEN_ID_WELCOME_SCREEN;
}

CheckFbScreenWorker::~CheckFbScreenWorker()
{

}

QString CheckFbScreenWorker::getMatchingImg2ScreenId(int screenID)
{
    switch (screenID) {
    case AppEnums::E_FBLITE_SCREEN_ID_LOGIN:
        return CREATE_NEW_FBACC_ICON;
    case AppEnums::E_FBLITE_SCREEN_ID_JOIN_FB:
        return JOIN_FB_TEXT;
    case AppEnums::E_FBLITE_SCREEN_ID_ENTER_NAME:
        return WHAT_YOUR_NAME_TEXT;
    case AppEnums::E_FBLITE_SCREEN_ID_ENTER_MOBILE_NUM:
        return ENTER_MOBILE_NUM_TEXT;
    case AppEnums::E_FBLITE_SCREEN_ID_ENTER_EMAIL_ADDRESS:
        return ENTER_YOUR_EMAIL;
    case AppEnums::E_FBLITE_SCREEN_ID_ENTER_BIRTHDAY:
        return WHAT_YOUR_BIRTHDAY;
    case AppEnums::E_FBLITE_SCREEN_ID_ENTER_GENDER:
        return WHAT_YOUR_GENDER;
    case AppEnums::E_FBLITE_SCREEN_ID_ENTER_PASSWORD:
        return CREATE_PASS_SCREEN;
    case AppEnums::E_FBLITE_SCREEN_ID_SAVE_LOGIN_INFO:
        return SAVE_LOGIN_TEXT;
    case AppEnums::E_FBLITE_SCREEN_ID_WELCOME_SCREEN:
        return WELCOME_SCREEN;
    default:
        return "";
    }
}

void CheckFbScreenWorker::doWork()
{
    QString result;
    m_updateFbScreenTimer = new QTimer(this);
    m_updateFbScreenTimer->setInterval(1000);
    QObject::connect(m_updateFbScreenTimer, SIGNAL(timeout()), this, SLOT(onUpdateCurrentFbScreen()));

    m_updateCurrActTimer = new QTimer(this);
    m_updateCurrActTimer->setInterval(1000);
    m_updateCurrActTimer->setSingleShot(false);
    QObject::connect(m_updateCurrActTimer, SIGNAL(timeout()), this, SLOT(onUpdateCurrentActivity()));

    m_keepScreenTimer = new QTimer(this);
    m_keepScreenTimer->setInterval(20000);
    m_keepScreenTimer->setSingleShot(true);
    QObject::connect(m_keepScreenTimer, SIGNAL(timeout()), this, SLOT(onKeepScreenTimeout()));

    m_updateCurrActTimer->start();
}

void CheckFbScreenWorker::onUpdateCurrentActivity()
{
    emit updateCurrentActivity(ADBCommand::currentActivity(m_deviceName));
}

void CheckFbScreenWorker::onUpdateCurrentFbScreen()
{
    LOG;
    int need2CheckedSCreen;
    if(!m_fbScreenStack.contains(m_screenId)){
        need2CheckedSCreen = m_fbScreenStack.first();
    }else if(m_screenId == m_fbScreenStack.last() ){
        // Do nothing
    }else{
        need2CheckedSCreen = m_fbScreenStack.at(m_fbScreenStack.indexOf(m_screenId) + 1);
    }

    cv::Mat screenImage = cv::imread(ADBCommand::screenShot(m_deviceName).toUtf8().constData(),1);
    cv::Mat screenMat = cv::imread(this->getMatchingImg2ScreenId(need2CheckedSCreen).toUtf8().constData());

    QPoint point = ImageProcessing::findImageOnImage(screenMat,screenImage);
    if(!point.isNull()){
        emit updateFbScreen(need2CheckedSCreen);
    }
    m_updateFbScreenTimer->start();
}

void CheckFbScreenWorker::onStartCheckFbScreen()
{
    LOG;
    m_updateFbScreenTimer->start();
    m_keepScreenTimer->start();
}

void CheckFbScreenWorker::onStopCheckFbScreen()
{
    LOG;
    m_updateFbScreenTimer->stop();
    m_keepScreenTimer->stop();
}

void CheckFbScreenWorker::onSyncFbScreenId(int screenId)
{
    LOG << screenId;
    if(m_screenId != screenId){
        m_screenId = screenId;
        m_keepScreenTimer->start();
    }
}

void CheckFbScreenWorker::onKeepScreenTimeout()
{
    if(m_screenId != -1){
        emit couldNotDectectScreen();
    }
}
