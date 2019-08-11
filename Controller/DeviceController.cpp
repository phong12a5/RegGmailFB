#include "DeviceController.h"
#include <QThread>
#include "AppMain.h"

#define APP_MAIN AppMain::instance()

DeviceController::DeviceController(QString deviceName):
    m_deviceName(deviceName),
    m_currentActivity(UNKNOW_SCREEN),
    m_currentExcuteStep(AppEnums::E_EXCUTE_CHANGE_INFO),
    m_checkNotificationTimer(nullptr),
    m_fbScreenId(-1)
{
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
            m_checkFBScreenTimer->stop();
        }else{
            m_checkFBScreenTimer->start();
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
        LOG << screenId;
        m_fbScreenId = screenId;
        emit fbScreenIdChanged();
    }
}

QString DeviceController::sendCaptcherScreen(QString screenPath)
{
    QString captchaImg = ImageProcessing::extractCaptchaImage(screenPath);
    delay(1000);
//    LOG << "[RegMailController]" << "captchaImg: " << captchaImg;
    if(captchaImg != QString("")){
        return HttpRequestController::instance()->sendHttpRequest(QString("http://poster.de-captcher.com"),\
                                                                  QDir::currentPath() + "/captcha.png");
    }else {
        return QString("");
    }
}

void DeviceController::connectSignalSlot()
{
    connect(this, SIGNAL(currentActivityChanged()),this, SLOT(onCurrentActivityChanged()));
    connect(this, SIGNAL(processFinished(int,int)),this, SLOT(onProcessFinished(int,int)));
    connect(this, SIGNAL(fbScreenIdChanged()),this, SLOT(onFbScreenIdChanged()));
}

void DeviceController::saveOutput()
{

}

void DeviceController::changeDeviceInfoHandler()
{
    LOG;
    if(this->currentActivity() != XGAME_SCREEN){
        ADBCommand::requestShowApp(XGAME_PKG,XGAME_ACTIVITYMAIN,this->deviceName());
    }else{
        LOG << " ------------------ New mission --------------------";
        this->setUserInfo(APP_MAIN->generateUserInfo());
        ADBCommand::uninstallPackage(FBLITE_PKG,this->deviceName());
        ADBCommand::tapScreen(QPoint(766,150),this->deviceName());
        delay(10000);
        ADBCommand::rebootDevice(this->deviceName());
        delay(2000);
        this->setCurrentExcuteStep(AppEnums::E_EXCUTE_REG_GMAIL);
    }
}

void DeviceController::regGmailHandler()
{
    LOG << this->currentActivity();
    if(this->currentActivity() == HOME_SCREEN)
    {
        if(this->userInfo().captcha == ""){
            ADBCommand::requestShowAppDirectly(ACCOUNT_SETTING_ACT, this->deviceName());
        }
    }else if(this->currentActivity() == ACCOUNT_SETTING_SCREEN){
        LOG << "Click google account Icon";
        ADBCommand::tapScreen(QPoint(296,1091),this->deviceName());
    }else if(this->currentActivity() == ADD_A_GGACCOUNT_SCREEN){
        LOG << "Click add new account Icon";
        ADBCommand::tapScreen(QPoint(541,1658),this->deviceName());
    }else if(this->currentActivity() == INPUT_YOURNAME_SCREEN){
        LOG << "Entering first name: " << this->userInfo().firstName;
        ADBCommand::enterText(this->userInfo().firstName,this->deviceName());
        ADBCommand::pressTap(this->deviceName());
        LOG << "Entering first name: " << this->userInfo().lastName;
        ADBCommand::enterText(this->userInfo().lastName,this->deviceName());
        ADBCommand::enterKeyBoard(this->deviceName());
    }else if(this->currentActivity() == INPUT_USERNAME_SCREEN){
        LOG << "Entering username: " << this->userInfo().userName;
        ADBCommand::enterText(this->userInfo().userName,this->deviceName());
        ADBCommand::enterKeyBoard(this->deviceName());
    }else if(this->currentActivity() == INPUT_PASSWORD_SCREEN){
        ADBCommand::enterText(this->userInfo().gmailPassword,this->deviceName());
        ADBCommand::pressTap(this->deviceName());
        ADBCommand::enterText(this->userInfo().gmailPassword,this->deviceName());
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

        while (this->userInfo().captcha == "" && this->currentActivity() == AUTHENTICATING_SCREEN) {
            this->userInfo().captcha = this->sendCaptcherScreen(ADBCommand::screenShot(this->deviceName()));
        }

        ADBCommand::enterText(this->userInfo().captcha,this->deviceName());
        ADBCommand::enterKeyBoard(this->deviceName());
    }else if(this->currentActivity() == SYNC_INTRO_SCREEN){
        delay(500);
        LOG << "Click Next Icon";
        ADBCommand::tapScreen(QPoint(942,1637),this->deviceName());
    }else if(this->currentActivity() == PAYMENT_SETTING_SCREEN){
        //        if(!ADBCommand::findAndClick(SKIP_PAYMENT_ICON)){
        //            LOG << "[RegMailController]" << "Couldn't click SKIP PAYMENT";
        //            this->userInfo().captcha = "";
        //            ADBCommand::goHomeScreen();
        //        }
    }else if(this->currentActivity() == WIFI_PICKER_SCREEN){
        LOG << "[RegMailController]" << "Back when current screen is wifi setting";
        ADBCommand::pressBack(this->deviceName());
    }else if(this->currentActivity() == COULD_NOT_SIGNIN){
        LOG << "[RegMailController]" << "Couldn't sign in";
        emit processFinished(this->currentExcuteStep(),1);
    }
}

void DeviceController::regFbHandler()
{
/*    if(this->currentActivity() == TURNON_PLAY_PROTECT){
            delay(500);
            LOG << "Closing turn on play protect popup";
            ADBCommand::tapScreen(QPoint(841,1202),this->deviceName());
            while(!ADBCommand::adb_command(QString("pm list packages | grep %1").arg(FBLITE_PKG),this->deviceName()).contains(FBLITE_PKG)){
                delay(1000);
            }
            ADBCommand::requestShowApp(FBLITE_PKG,FBLITE_ACTIVITYMAIN,this->deviceName());
        }
    else*/ if(this->currentActivity() != FACEBOOK_LITE_SCREEN)
    {
//        QProcess process;
//        process.start(QString("adb -s %1 shell pm install %2").arg(this->deviceName()).arg(FBLITE_APK_PATH));
        ADBCommand::installPackage(FBLITE_APK_PATH,this->deviceName());
        ADBCommand::requestShowApp(FBLITE_PKG,FBLITE_ACTIVITYMAIN,this->deviceName());
    }else {

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
    LOG << m_deviceName;

    m_checkFBScreenTimer = new QTimer(this);
    m_checkFBScreenTimer->setInterval(2000);
    m_checkFBScreenTimer->setSingleShot(false);
    connect(m_checkFBScreenTimer, SIGNAL(timeout()),this, SLOT(onCheckFBScreen()));

    m_checkNotificationTimer = new QTimer(this);
    m_checkNotificationTimer->setInterval(2000);
    m_checkNotificationTimer->setSingleShot(false);
    connect(m_checkNotificationTimer, SIGNAL(timeout()),this, SLOT(onCheckNotification()));
    m_checkNotificationTimer->start();

    /* For testing */
//    this->setUserInfo(APP_MAIN->generateUserInfo());
//    this->setCurrentExcuteStep(AppEnums::E_EXCUTE_REG_FACBOOK);
}

void DeviceController::onCurrentActivityChanged()
{
    LOG << m_deviceName << " : " << this->currentActivity();

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
        emit currentActivityChanged();
    }else{
        if(step == static_cast<int>(AppEnums::E_EXCUTE_CHANGE_INFO)){
            this->setCurrentExcuteStep(AppEnums::E_EXCUTE_REG_GMAIL);
        }else if(step == static_cast<int>(AppEnums::E_EXCUTE_REG_GMAIL)){
            this->setCurrentExcuteStep(AppEnums::E_EXCUTE_REG_FACBOOK);
        }else if(step == static_cast<int>(AppEnums::E_EXCUTE_REG_FACBOOK)){
            this->saveOutput();
            this->setCurrentExcuteStep(AppEnums::E_EXCUTE_CHANGE_INFO);
        }
        emit currentActivityChanged();
    }
}

void DeviceController::onCheckNotification()
{
    QString notificationData = ADBCommand::readNotificationData(this->deviceName());
    if(this->currentExcuteStep() == AppEnums::E_EXCUTE_REG_GMAIL){
        if(notificationData.contains("android.text=Finish setting up your new Google Account")){
            LOG << "Reg Gmail completed";
            emit processFinished(this->currentExcuteStep(),EXITCODE_TRUE);
        }else if(notificationData.contains("android.title=Account Action Required"))
        {
            LOG << "Reg Gmail failure";
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

void DeviceController::onCheckFBScreen()
{
    int need2CheckedSCreen;
    if(!m_fbScreenStack.contains(this->fbScreenId())){
        need2CheckedSCreen = m_fbScreenStack.first();
    }else if(this->fbScreenId() == m_fbScreenStack.last() ){
        // Do nothing
    }else{
        need2CheckedSCreen = m_fbScreenStack.at(m_fbScreenStack.indexOf(this->fbScreenId()) + 1);
    }

    cv::Mat screenImage = cv::imread(ADBCommand::screenShot(this->deviceName()).toUtf8().constData(),1);
    while (screenImage.empty()){
        screenImage = cv::imread(ADBCommand::screenShot(this->deviceName()).toUtf8().constData(),1);
    }

    QPoint point = ImageProcessing::findImageOnImage(APP_MAIN->SCREEN_MATHING_TABLE.value(static_cast<AppEnums::E_FBLITE_SCREEN_ID>(need2CheckedSCreen)),screenImage);
    if(!point.isNull()){
        this->setFbScreenId(need2CheckedSCreen);
    }
}

void DeviceController::onFbScreenIdChanged()
{
    LOG << this->fbScreenId();
    switch (this->fbScreenId()) {
    case AppEnums::E_FBLITE_SCREEN_ID_LOGIN:
        LOG << ADBCommand::findAndClick(APP_MAIN->CREATE_NEW_FBACC_ICON,this->deviceName());
        break;
    case AppEnums::E_FBLITE_SCREEN_ID_JOIN_FB:
        ADBCommand::findAndClick(APP_MAIN->NEXT_BUTTON,this->deviceName());
        break;
    case AppEnums::E_FBLITE_SCREEN_ID_ENTER_NAME:{
        ADBCommand::findAndClick(APP_MAIN->FIRSTNAME_FIELD,this->deviceName());
        ADBCommand::enterText(this->userInfo().firstName,this->deviceName());

        /////////////////
        ADBCommand::findAndClick(APP_MAIN->LASTNAME_FIELD,this->deviceName());
        ADBCommand::enterText(this->userInfo().lastName,this->deviceName());

        ///////////////
        ADBCommand::findAndClick(APP_MAIN->NEXT_BUTTON,this->deviceName());
    }
        break;
    case AppEnums::E_FBLITE_SCREEN_ID_ENTER_MOBILE_NUM:
        ADBCommand::hideKeyboard(this->deviceName());
        delay(500);
        ADBCommand::findAndClick(APP_MAIN->SIGN_UP_WITH_EMAIL,this->deviceName());
        break;
    case AppEnums::E_FBLITE_SCREEN_ID_ENTER_EMAIL_ADDRESS:
        ADBCommand::findAndClick(APP_MAIN->NEXT_BUTTON,this->deviceName());
        break;
    case AppEnums::E_FBLITE_SCREEN_ID_ENTER_BIRTHDAY:
    {
        if(!ADBCommand::findAnImageOnScreen(APP_MAIN->KEYBOARD,this->deviceName())){
        ADBCommand::hideKeyboard(this->deviceName());
        delay(500);
        }

        LOG << "[RegFBController]";
        // CLICK MONTH FIELD
        pressBoDKeyboard(this->userInfo().bodMonth);
        // CLICK DAY FIELD
        pressBoDKeyboard(this->userInfo().bodDate);
        // CLICK YEAR FIELD
        pressBoDKeyboard(this->userInfo().bodYear);

        // Next
        ADBCommand::findAndClick(APP_MAIN->NEXT_BUTTON,this->deviceName());
    }
        break;
    case AppEnums::E_FBLITE_SCREEN_ID_ENTER_GENDER:
        if(this->userInfo().sex == "F")
            ADBCommand::findAndClick(APP_MAIN->FEMALE_BUTTON,this->deviceName());
        else
            ADBCommand::findAndClick(APP_MAIN->MALE_BUTTON,this->deviceName());
        break;
    case AppEnums::E_FBLITE_SCREEN_ID_ENTER_PASSWORD:{
        delay(100);
        ADBCommand::enterText(this->userInfo().fbPassword,this->deviceName());
        ADBCommand::findAndClick(APP_MAIN->SIGN_UP_BUTTON,this->deviceName());
    }
        break;
    case AppEnums::E_FBLITE_SCREEN_ID_SAVE_LOGIN_INFO:
        ADBCommand::findAndClick(APP_MAIN->NOT_NOW_BUTTON,this->deviceName());
        break;
    case AppEnums::E_FBLITE_SCREEN_ID_WELCOME_SCREEN:
        ADBCommand::findAndClick(APP_MAIN->OK_BUTTON,this->deviceName());
        LOG << "REGISTER COMPLETED";
        emit processFinished(this->currentExcuteStep(),EXITCODE_TRUE);
        break;
    default:
        break;
    }
}
