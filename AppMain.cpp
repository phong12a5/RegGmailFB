#include "AppMain.h"

AppMain* AppMain::m_instance = nullptr;

AppMain::AppMain(QObject *parent) : QObject(parent)
{
    m_deviceList.clear();
    m_deviceCtrlList.clear();
    m_matchThreadList.clear();

    m_lastNameList.clear();
    m_firstNameList.clear();
    m_middleNameList.clear();
}

AppMain *AppMain::instance()
{
    if(m_instance == nullptr){
        m_instance = new AppMain();
    }
    return m_instance;
}

QStringList AppMain::deviceList() const
{
    return m_deviceList;
}

void AppMain::setDeviceList(QStringList _devices)
{
    if(_devices != m_deviceList){
        m_deviceList = _devices;
        emit deviceListChanged();
    }
}

USER_INFOR AppMain::generateUserInfo()
{
    srand(time(nullptr));

    USER_INFOR data;
    if(m_lastNameList.isEmpty() || m_firstNameList.isEmpty() ||  m_middleNameList.isEmpty()){
        LOG << "There is no data";
        return data;
    }    

    /* Generate Birth of date */
    data.bodDate = rand()%28 + 0;
    data.bodMonth = rand() %12 + 0;
    data.bodYear = rand() % 45 + 1960;

    /* Generate full name*/

    data.firstName = m_firstNameList.at(rand() % (m_firstNameList.length()));
    data.lastName = m_lastNameList.at(rand() % (m_lastNameList.length()));
    data.middleName = m_middleNameList.at(rand() % (m_middleNameList.length()));

    data.userName = data.firstName + data.lastName +  QString::number(rand() % 1000000000 + 3000000);
    data.sex = data.middleName == "Thi"? "F" : "M";
    data.captcha = "";

    QStringList part1 = QStringList() << data.firstName << data.lastName
                                      << data.firstName.toLower() << data.lastName.toLower();

    QStringList part2 = QStringList() << data.firstName.toLower() + "@"
                                      << data.lastName.toLower() + "@"
                                      << data.firstName.toLower() + (data.bodDate < 10? ("0" + QString::number(data.bodDate)) : QString::number(data.bodDate))
                                      << data.lastName.toLower()  + (data.bodDate < 10? ("0" + QString::number(data.bodDate)) : QString::number(data.bodDate))
                                      << data.firstName.toLower() + QString::number(rand()%90 + 10)
                                      << data.lastName.toLower() + QString::number(rand()%90 + 10);

    QStringList part3 = QStringList() << QString::number(data.bodYear)
                                      << QString::number(data.bodYear%100) + QString::number(data.bodYear%100);

    data.gmailPassword = part1.at(rand()%part1.length()) + part2.at(rand()%part2.length()) + part3.at(rand()%part3.length());
    data.fbPassword = data.gmailPassword;
    LOG << QString("Info: %1 | %2 | %3 | %4").arg(data.firstName).arg(data.lastName).arg(data.middleName).arg(data.fbPassword);
    return data;
}

const cv::Mat &AppMain::getMatchingImg2ScreenId(int screenID)
{
    switch (screenID) {
    case AppEnums::E_FBLITE_SCREEN_ID_LOGIN:                return CREATE_NEW_FBACC_ICON;
    case AppEnums::E_FBLITE_SCREEN_ID_JOIN_FB:              return JOIN_FB_TEXT;
    case AppEnums::E_FBLITE_SCREEN_ID_ENTER_NAME:           return WHAT_YOUR_NAME_TEXT;
    case AppEnums::E_FBLITE_SCREEN_ID_ENTER_MOBILE_NUM:     return ENTER_MOBILE_NUM_TEXT;
    case AppEnums::E_FBLITE_SCREEN_ID_ENTER_EMAIL_ADDRESS:  return ENTER_YOUR_EMAIL;
    case AppEnums::E_FBLITE_SCREEN_ID_ENTER_BIRTHDAY:       return WHAT_YOUR_BIRTHDAY;
    case AppEnums::E_FBLITE_SCREEN_ID_ENTER_GENDER:         return WHAT_YOUR_GENDER;
    case AppEnums::E_FBLITE_SCREEN_ID_ENTER_PASSWORD:       return CREATE_PASS_SCREEN;
    case AppEnums::E_FBLITE_SCREEN_ID_SAVE_LOGIN_INFO:      return SAVE_LOGIN_TEXT;
    case AppEnums::E_FBLITE_SCREEN_ID_WELCOME_SCREEN:       return WELCOME_SCREEN;
    }
}

void AppMain::startProgram()
{
    LOG;
    emit this->oparate();
    emit this->startCheckActivity();
}

void AppMain::closeProgram()
{
    LOG;
    QCoreApplication::quit();
}

void AppMain::initApplication()
{
    LOG << "[AppMain]";

    /* Load user information for registering */
    this->loadUserInfo();

    /* Update connected devices */
    this->setDeviceList(ADBCommand::deviceList());

    /* Creating multi thread for controlling devices */
    for(int i = 0; i < this->deviceList().length(); i++){
        QString device = this->deviceList().at(i);
        DeviceController* _ctrlPtr = new DeviceController(device);
        QThread* _thread = new QThread(this);

        _ctrlPtr->moveToThread(_thread);
        QObject::connect(this,&AppMain::oparate,_ctrlPtr,&DeviceController::doWork);
        QObject::connect(_thread, &QThread::finished, _ctrlPtr, &QObject::deleteLater);

        m_deviceCtrlList << _ctrlPtr;
        m_matchThreadList << _thread;
        _thread->start();
    }

    /* Creating new thread for updating current activity to devices */
    CheckCurrSrcWorker* _ctrlPtr = new CheckCurrSrcWorker();
    QThread* _scheckSrcThread = new QThread(this);

    _ctrlPtr->moveToThread(_scheckSrcThread);
    QObject::connect(this,&AppMain::startCheckActivity,_ctrlPtr,&CheckCurrSrcWorker::doWork);
    QObject::connect(_scheckSrcThread, &QThread::finished, _ctrlPtr, &QObject::deleteLater);
    QObject::connect(_ctrlPtr, &CheckCurrSrcWorker::updateCurrentActOnDevices, this, &AppMain::onUpdateCurrentActOnDevices);
    _scheckSrcThread->start();
}

void AppMain::loadUserInfo()
{
    m_lastNameList << "Tran" << "Dang" << "Hoang" << "Nguyen" << "Vu" << "Cao" << "Phung" << "Le" << "Dao" << "Chau" << "Que" << "Vo";
    m_firstNameList << "Phong" << "Thuy" << "Linh" << "Ngoc" << "Khanh" << "Hung" << "Tuan" << "Thanh" << "Anh" << "Nguyet";
    m_middleNameList << "Thi" << "Ba" << "Hoang" << "Tung" << "Thi" << "Thi" << "Khanh" << "Thi" << "Ngoc" << "Dinh" << "Thanh" << "Thi";
}

void AppMain::onUpdateCurrentActOnDevices(QStringList activities)
{
    if(activities.length() == this->deviceList().length()){
        for (int i = 0; i < activities.length(); ++i) {
            m_deviceCtrlList.at(i)->setCurrentActivity(activities.at(i));
        }
    }else{
        LOG << "Not matching!";
    }
}

//void AppMain::onProcessFinished(int currentStep, int exitCode)
//{
//    LOG << "[AppMain]" << "currentStep: " << (currentStep == 0? "CHANGE DEVICE" : (currentStep == 1? "REG GMAIL" : "REG FACEBOOK")) << " --- exitCode: " << exitCode;
//    if(exitCode == 1){
//        LOG << "[AppMain]" << "Process incompleted! -> Restart process";
//        this->setCurrentExcuteStep(AppEnums::E_EXCUTE_CHANGE_INFO);
//        this->restartProgram();
//    }else if(exitCode == 0 ){
//        if(currentStep == AppEnums::E_EXCUTE_CHANGE_INFO){
//            this->setCurrentExcuteStep(AppEnums::E_EXCUTE_REG_GMAIL);
//            LOG << "[AppMain]" << "Change infor device completed -> Reboot device!!!";
//            ADBCommand::rebootDevice();
//        }else if(currentStep == AppEnums::E_EXCUTE_REG_GMAIL){
//            REG_FBACC_CTR->setUserInfo(REG_MAIL_CTR->getEmailInfor());
//            LOG << "[AppMain]" << "Reg gmail completed! -> Start reg facebook";
//            if(APP_MODEL->regFacebookOption()){
//                this->setCurrentExcuteStep(AppEnums::E_EXCUTE_REG_FACBOOK);
//                if(this->getCurrentActivity() == HOME_SCREEN){
//                    emit this-> currentActivityChanged();
//                }else{
//                    ADBCommand::goHomeScreen();
//                }
//            }else{
//                REG_MAIL_CTR->saveEmailToOutput();
//                this->setCurrentExcuteStep(AppEnums::E_EXCUTE_CHANGE_INFO);
//                this->restartProgram();
//            }
//        }else if(currentStep == AppEnums::E_EXCUTE_REG_FACBOOK){
//            REG_MAIL_CTR->saveEmailToOutput();
//            LOG << "[AppMain]" << "Process completed! -> Restart process";
//            this->setCurrentExcuteStep(AppEnums::E_EXCUTE_CHANGE_INFO);
//            this->restartProgram();
//        }
//    }
//}
