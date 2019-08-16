#include "DeviceController.h"
#include <QThread>
#include "AppMain.h"

#define APP_MAIN AppMain::instance()

DeviceController::DeviceController(QString deviceName):
    m_deviceName(deviceName),
    m_currentActivity(UNKNOW_SCREEN),
    m_currentExcuteStep(AppEnums::E_EXCUTE_CHANGE_INFO),
    m_checkNotificationTimer(nullptr),
    m_keepFbSrcCheckerTimer(nullptr),
    m_checkFbSCreenThread(nullptr),
    m_checkFbScreenWorker(nullptr),
    m_fbScreenId(-1)
{
    this->connectSignalSlot();
}

DeviceController::~DeviceController()
{

}

QString DeviceController::currentActivity() const
{
    return m_currentActivity;
}

void DeviceController::setCurrentActivity(QString data)
{
    if(m_currentActivity != data){
        m_currentActivity = data;
        emit currentActivityChanged();
    }
}

QString DeviceController::deviceName() const
{
    return m_deviceName;
}

AppEnums::E_EXCUTE_STEPS DeviceController::currentExcuteStep()
{
    return m_currentExcuteStep;
}

void DeviceController::setCurrentExcuteStep(AppEnums::E_EXCUTE_STEPS step)
{
    if(m_currentExcuteStep != step){
        m_currentExcuteStep = step;
        if(m_currentExcuteStep != AppEnums::E_EXCUTE_REG_FACBOOK){
            emit stopCheckFbScreen();
        }else{
            this->setFbScreenId(-1);
            emit startCheckFbScreen();
        }
    }
}

USER_INFOR &DeviceController::userInfo()
{
    return m_userInfo;
}

void DeviceController::setUserInfo(USER_INFOR data)
{
    m_userInfo = data;
}

int DeviceController::fbScreenId() const
{
    return m_fbScreenId;
}

void DeviceController::setFbScreenId(int screenId)
{
    if(m_fbScreenId != screenId){
        m_fbScreenId = screenId;
        emit fbScreenIdChanged(m_fbScreenId);
    }
}

QString DeviceController::sendCaptcherScreen(QString screenPath)
{
    QString captchaImg = ImageProcessing::extractCaptchaImage(screenPath,this->deviceName());
    LOG <<  captchaImg;
    delay(1000);
    if(captchaImg != QString("")){
        HttpRequestController httpRq;
        return httpRq.sendHttpRequest(QString("http://poster.de-captcher.com"),captchaImg);
    }else {
        return QString("");
    }
}

void DeviceController::connectSignalSlot()
{
    connect(this, SIGNAL(currentActivityChanged()),this, SLOT(onCurrentActivityChanged()));
    connect(this, SIGNAL(fbScreenIdChanged(int)),this, SLOT(onFbScreenIdChanged(int)));
    connect(this, SIGNAL(processFinished(int,int)),this, SLOT(onProcessFinished(int,int)));
}

void DeviceController::changeDeviceInfoHandler()
{
    LOG;
    delay(200);
    if(this->currentActivity() != XGAME_SCREEN){
        ADBCommand::requestShowApp(XGAME_PKG,XGAME_ACTIVITYMAIN,this->deviceName());
    }else{
        LOG << " ------------------ New mission --------------------";
        ADBCommand::uninstallPackage(FBLITE_PKG,this->deviceName());
        ADBCommand::tapScreen(QPoint(766,150),this->deviceName());
        delay(10000);
        ADBCommand::rebootDevice(this->deviceName());
        delay(2000);
        emit processFinished(this->currentExcuteStep(), EXITCODE_TRUE);
    }
}

void DeviceController::regGmailHandler()
{
    if(this->currentActivity() == HOME_SCREEN)
    {
        if(this->userInfo().captcha == ""){
            ADBCommand::requestShowAppDirectly(ACCOUNT_SETTING_ACT, this->deviceName());
        }else{
            ADBCommand::requestSyncAccount(this->deviceName());
                m_waitForSyncTimer->start();
        }
    }else if(this->currentActivity() == ACCOUNT_SETTING_SCREEN){
        if(this->userInfo().captcha != ""){
            ADBCommand::requestSyncAccount(this->deviceName());
                m_waitForSyncTimer->start();
        }else{
            LOG << "Click google account Icon";
            delay(1000);
            ADBCommand::findAndClick(GOOGLE_ACCOUNT_ICON,this->deviceName());
        }
    }else if(this->currentActivity() == ADD_A_GGACCOUNT_SCREEN){
        LOG << "Click add new account Icon";
        ADBCommand::tapScreen(QPoint(541,1658),this->deviceName());
    }else if(this->currentActivity() == INPUT_YOURNAME_SCREEN){
        ADBCommand::enterText(this->userInfo().firstName,this->deviceName());
        ADBCommand::pressTap(this->deviceName());
        ADBCommand::enterText(this->userInfo().lastName,this->deviceName());
        ADBCommand::enterKeyBoard(this->deviceName());
    }else if(this->currentActivity() == INPUT_USERNAME_SCREEN){
        ADBCommand::enterText(this->userInfo().userName,this->deviceName());
        ADBCommand::enterKeyBoard(this->deviceName());
    }else if(this->currentActivity() == INPUT_PASSWORD_SCREEN){
        ADBCommand::enterText(this->userInfo().gmailPassword,this->deviceName());
        ADBCommand::pressTap(this->deviceName());
        ADBCommand::enterText(this->userInfo().gmailPassword,this->deviceName());
        delay(200);
        ADBCommand::enterKeyBoard(this->deviceName());
    }else if(this->currentActivity() == RECOVERY_INTRO_SCREEN){
        LOG << "Click Not Now Icon";
        ADBCommand::tapScreen(QPoint(541,1658),this->deviceName());
    }else if(this->currentActivity() == GOOGLE_SERVICE_SCREEN){
        LOG << "Click Next Icon";
        ADBCommand::tapScreen(QPoint(942,1637),this->deviceName());
    }else if(this->currentActivity() == TERM_SERVICE_SCREEN){
        LOG << "Click Accept Icon";
        ADBCommand::tapScreen(QPoint(542,1470),this->deviceName());
    }else if(this->currentActivity() == AUTHENTICATING_SCREEN){
        LOG << "Getting captcher";
        this->userInfo().captcha = this->sendCaptcherScreen(ADBCommand::screenShot(this->deviceName()));
        if(this->userInfo().captcha != "" && this->userInfo().captcha != "ERROR"){
            ADBCommand::enterText(this->userInfo().captcha,this->deviceName());
            ADBCommand::enterKeyBoard(this->deviceName());
        }else{
            this->userInfo().captcha = "ERROR";
        }
        delay(1000);
        this->setCurrentActivity(NULL_FOCUSED_SCREEN);
    }else if(this->currentActivity() == SYNC_INTRO_SCREEN){
        delay(500);
        LOG << "Click Next Icon";
        ADBCommand::tapScreen(QPoint(942,1637),this->deviceName());
    }else if(this->currentActivity() == PAYMENT_SETTING_SCREEN){
        LOG << "Click Skip Icon";
        ADBCommand::tapScreen(QPoint(135,1615),this->deviceName());
    }else if(this->currentActivity() == WIFI_PICKER_SCREEN){
        LOG << "[RegMailController]" << "Back when current screen is wifi setting";
        ADBCommand::pressBack(this->deviceName());
    }else if(this->currentActivity() == COULD_NOT_SIGNIN){
        LOG << "[RegMailController]" << "Couldn't sign in";
        emit processFinished(this->currentExcuteStep(),1);
    }else if(this->currentActivity() == CREATE_ACC_TASK){
        LOG << "[RegMailController]" << "Do nothing when is creating acc task";
    }else if(this->currentActivity() == INVALID_USERNAME){
        emit processFinished(this->currentExcuteStep(),EXITCODE_FALSE);
    }
}

void DeviceController::regFbHandler()
{
    LOG;
    if(this->currentActivity() != FACEBOOK_LITE_SCREEN)
    {
        ADBCommand::installPackage(FBLITE_APK_PATH,this->deviceName());
        ADBCommand::requestShowApp(FBLITE_PKG,FBLITE_ACTIVITYMAIN,this->deviceName());
    }
}

void DeviceController::pressBoDKeyboard(int number)
{
        if(number <0){
            LOG << "[RegFBController]" << "Invalid number: " << number;
            return;
        }

        QList<int> listNumber;
        while (number) {
            int temp = number%10;
            listNumber.prepend(temp);
            number /= 10;
        }

        if(listNumber.length() == 1)
            listNumber.prepend(0);

        LOG << listNumber;
        while (!listNumber.isEmpty()) {
            switch (listNumber.takeFirst()) {
            case 1:{
                LOG << "Press 1";
                ADBCommand::tapScreen(QPoint(90,1600),this->deviceName());
            }
                break;
            case 2:{
                LOG << "Press 2";
                ADBCommand::tapScreen(QPoint(270,1600),this->deviceName());
            }
                break;
            case 3:{
                LOG << "Press 2";
                ADBCommand::tapScreen(QPoint(450,1600),this->deviceName());
            }
                break;
            case 4:{
                LOG << "Press 4";
                ADBCommand::tapScreen(QPoint(630,1600),this->deviceName());
            }
                break;
            case 5:{
                LOG << "Press 5";
                ADBCommand::tapScreen(QPoint(810,1600),this->deviceName());
            }
                break;
            case 6:{
                    LOG << "Press 6";
                ADBCommand::tapScreen(QPoint(90,1700),this->deviceName());
                }
                break;
            case 7:{
                    LOG << "Press 7";
                ADBCommand::tapScreen(QPoint(270,1700),this->deviceName());
                }
                break;
            case 8:{
                    LOG << "Press 8";
                ADBCommand::tapScreen(QPoint(450,1700),this->deviceName());
                }
                break;
            case 9:{
                    LOG << "Press 9";
                ADBCommand::tapScreen(QPoint(630,1700),this->deviceName());
                }
                break;
            case 0:{
                    LOG << "Press 0";
                ADBCommand::tapScreen(QPoint(810,1700),this->deviceName());
                }
                break;
            default:
                break;
            }
            delay(100);
        }
}

void DeviceController::doWork()
{
    m_checkNotificationTimer = new QTimer(this);
    m_checkNotificationTimer->setInterval(2000);
    m_checkNotificationTimer->setSingleShot(false);
    connect(m_checkNotificationTimer, SIGNAL(timeout()),this, SLOT(onCheckNotification()));

    m_waitForSyncTimer = new QTimer(this);
    m_waitForSyncTimer->setInterval(60000);
    m_waitForSyncTimer->setSingleShot(true);
    connect(m_waitForSyncTimer, SIGNAL(timeout()),this, SLOT(onWaitForSyncTimeout()));

    m_checkFbSCreenThread = new QThread(this);
    m_checkFbScreenWorker = new CheckFbScreenWorker(this->deviceName());
    m_checkFbScreenWorker->moveToThread(m_checkFbSCreenThread);
    QObject::connect(this,&DeviceController::operate,m_checkFbScreenWorker,&CheckFbScreenWorker::doWork);
    QObject::connect(this,&DeviceController::startCheckFbScreen,m_checkFbScreenWorker,&CheckFbScreenWorker::onStartCheckFbScreen);
    QObject::connect(this,&DeviceController::stopCheckFbScreen,m_checkFbScreenWorker,&CheckFbScreenWorker::onStopCheckFbScreen);
    QObject::connect(this,&DeviceController::fbScreenIdChanged,m_checkFbScreenWorker,&CheckFbScreenWorker::onSyncFbScreenId);
    QObject::connect(m_checkFbSCreenThread, &QThread::finished, m_checkFbScreenWorker, &QObject::deleteLater);
    QObject::connect(m_checkFbScreenWorker, &CheckFbScreenWorker::updateFbScreen, this, &DeviceController::onUpdateFbScreen);
    QObject::connect(m_checkFbScreenWorker, &CheckFbScreenWorker::updateCurrentActivity, this, &DeviceController::onUpdateCurrentActivity);
    QObject::connect(m_checkFbScreenWorker, &CheckFbScreenWorker::couldNotDectectScreen, this, &DeviceController::onCouldNotDectectScreen);

    m_checkNotificationTimer->start();
    m_checkFbSCreenThread->start();

    emit operate();
}

void DeviceController::onUpdateCurrentActivity(QString activity)
{
    this->setCurrentActivity(activity);
}

void DeviceController::onCurrentActivityChanged()
{
    LOG << m_deviceName << " : " << this->currentActivity() << " -- step: " << this->currentExcuteStep();

    if(this->currentActivity() == LOCK_SCREEN){
        ADBCommand::wakeUpScreen(this->deviceName());
        return;
    }

    if(this->currentActivity() == STATUSBAR_SCREEN){
        ADBCommand::collapseStatusBar(this->deviceName());
        return;
    }

    switch (static_cast<int>(this->currentExcuteStep())) {
    case AppEnums::E_EXCUTE_CHANGE_INFO:
        this->changeDeviceInfoHandler();
        break;
    case AppEnums::E_EXCUTE_REG_GMAIL:
        this->regGmailHandler();
        break;
    case AppEnums::E_EXCUTE_REG_FACBOOK:
        this->regFbHandler();
        break;
    }
}


void DeviceController::onProcessFinished(int step, int exitedCode)
{
    if(exitedCode == EXITCODE_FALSE){
        this->setCurrentExcuteStep(AppEnums::E_EXCUTE_CHANGE_INFO);
        emit missionCompleted(EXITCODE_FALSE, this->deviceName());
        emit currentActivityChanged();
    }else{
        if(step == static_cast<int>(AppEnums::E_EXCUTE_CHANGE_INFO)){
            this->setCurrentExcuteStep(AppEnums::E_EXCUTE_REG_GMAIL);
        }else if(step == static_cast<int>(AppEnums::E_EXCUTE_REG_GMAIL)){
            this->setCurrentExcuteStep(AppEnums::E_EXCUTE_REG_FACBOOK);
        }else if(step == static_cast<int>(AppEnums::E_EXCUTE_REG_FACBOOK)){
            this->setCurrentExcuteStep(AppEnums::E_EXCUTE_CHANGE_INFO);
            emit missionCompleted(EXITCODE_TRUE, this->deviceName());
        }
        emit currentActivityChanged();
    }
}

void DeviceController::onCheckNotification()
{
    QString notificationData = ADBCommand::readNotificationData(this->deviceName());
    if(this->currentExcuteStep() == AppEnums::E_EXCUTE_REG_GMAIL){
        if(notificationData.contains("Finish setting up your new Google Account")){
            LOG << "Reg Gmail completed";
            m_waitForSyncTimer->stop();
            emit processFinished(this->currentExcuteStep(),EXITCODE_TRUE);
        }else if(notificationData.contains("Account Action Required"))
        {
            LOG << "Reg Gmail failure";
            m_waitForSyncTimer->stop();
            emit processFinished(this->currentExcuteStep(),EXITCODE_FALSE);
        }
    }else if(this->currentExcuteStep() == AppEnums::E_EXCUTE_REG_FACBOOK){
        QStringList noti =  notificationData.split("\r\r\n");
        foreach(QString data, noti){
            if(data.contains("is your Facebook confirmation code")){
                foreach(QString text, data.simplified().split(" ")){
                    bool isNumber = false;
                    int code = text.toInt(&isNumber);
                    if(isNumber){
                        LOG << "Authen code: " << code;
                    }
                }
            }
        }
    }
}


void DeviceController::onUpdateFbScreen(int screenId)
{
    this->setFbScreenId(screenId);
}

void DeviceController::onCouldNotDectectScreen()
{
    LOG;
    if(this->currentExcuteStep() == AppEnums::E_EXCUTE_REG_FACBOOK){
        emit processFinished(this->currentExcuteStep(),EXITCODE_FALSE);
    }
}

void DeviceController::onWaitForSyncTimeout()
{
    LOG << "Syncing waiting timeout";
    emit processFinished(this->currentExcuteStep(),EXITCODE_TRUE);
}

void DeviceController::onFbScreenIdChanged(int screenId){
    LOG << screenId;
    switch (screenId) {
    case AppEnums::E_FBLITE_SCREEN_ID_LOGIN:
        LOG << ADBCommand::findAndClick(CREATE_NEW_FBACC_ICON,this->deviceName());
        break;
    case AppEnums::E_FBLITE_SCREEN_ID_JOIN_FB:
        ADBCommand::findAndClick(NEXT_BUTTON,this->deviceName());
        break;
    case AppEnums::E_FBLITE_SCREEN_ID_ENTER_NAME:{
        ADBCommand::findAndClick(FIRSTNAME_FIELD,this->deviceName());
        ADBCommand::enterText(this->userInfo().firstName,this->deviceName());

        /////////////////
        ADBCommand::findAndClick(LASTNAME_FIELD,this->deviceName());
        ADBCommand::enterText(this->userInfo().lastName,this->deviceName());

        ///////////////
        ADBCommand::findAndClick(NEXT_BUTTON,this->deviceName());
    }
        break;
    case AppEnums::E_FBLITE_SCREEN_ID_ENTER_MOBILE_NUM:
        ADBCommand::hideKeyboard(this->deviceName());
        delay(500);
        ADBCommand::findAndClick(SIGN_UP_WITH_EMAIL,this->deviceName());
        break;
    case AppEnums::E_FBLITE_SCREEN_ID_ENTER_EMAIL_ADDRESS:
        ADBCommand::findAndClick(NEXT_BUTTON,this->deviceName());
        break;
    case AppEnums::E_FBLITE_SCREEN_ID_ENTER_BIRTHDAY:
    {
        if(!ADBCommand::findAnImageOnScreen(cv::imread(QString(KEYBOARD).toUtf8().constData(),1),this->deviceName())){
        ADBCommand::hideKeyboard(this->deviceName());
        delay(500);
        }

        LOG << "[RegFBController]";
        // CLICK MONTH FIELD
        pressBoDKeyboard(this->userInfo().bodMonth);
        delay(100);
        // CLICK DAY FIELD
        pressBoDKeyboard(this->userInfo().bodDate);
        delay(100);
        // CLICK YEAR FIELD
        pressBoDKeyboard(this->userInfo().bodYear);
        delay(100);

        // Next
        ADBCommand::findAndClick(NEXT_BUTTON,this->deviceName());
    }
        break;
    case AppEnums::E_FBLITE_SCREEN_ID_ENTER_GENDER:
        if(this->userInfo().sex == "F")
            ADBCommand::findAndClick(FEMALE_BUTTON,this->deviceName());
        else
            ADBCommand::findAndClick(MALE_BUTTON,this->deviceName());
        break;
    case AppEnums::E_FBLITE_SCREEN_ID_ENTER_PASSWORD:{
        delay(100);
        ADBCommand::enterText(this->userInfo().fbPassword,this->deviceName());
        ADBCommand::findAndClick(SIGN_UP_BUTTON,this->deviceName());
    }
        break;
    case AppEnums::E_FBLITE_SCREEN_ID_SAVE_LOGIN_INFO:
        ADBCommand::findAndClick(NOT_NOW_BUTTON,this->deviceName());
        break;
    case AppEnums::E_FBLITE_SCREEN_ID_WELCOME_SCREEN:
        ADBCommand::findAndClick(OK_BUTTON,this->deviceName());
        LOG << "REGISTER COMPLETED";
        emit processFinished(this->currentExcuteStep(),EXITCODE_TRUE);
        break;
    default:
        break;
    }
}
