#include "AppMain.h"

AppMain* AppMain::m_instance = nullptr;
#include <QMutex>

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
    QMutex mutex;
    mutex.lock();
    if(m_instance == nullptr){
        m_instance = new AppMain();
    }
    mutex.unlock();
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
    USER_INFOR data;

    if(m_lastNameList.isEmpty() || m_firstNameList.isEmpty()){
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
//    data.middleName = m_middleNameList.at(rand() % (m_middleNameList.length()));

    data.userName = data.firstName + data.lastName +  QString::number(rand() % 1000000000 + 3000000);
    data.sex = data.middleName == "Thi"? "F" : "M";
    data.captcha = "";

    QString adcString = "abcdefghijklmnopqrstuvwxyz";

    data.gmailPassword = QString(adcString.at(rand()%adcString.length()).toUpper()) +\
            adcString.at(rand()%adcString.length()) +
            adcString.at(rand()%adcString.length()) +
            adcString.at(rand()%adcString.length()) +
            QString::number(rand()%10) +
            QString::number(rand()%10) +
            QString::number(rand()%10) +
            QString::number(rand()%10) +
            QString::number(rand()%10);

    data.fbPassword = data.gmailPassword;
    LOG << QString("Info: %1 | %2 | %3 | %4").arg(data.firstName).arg(data.lastName).arg(data.userName).arg(data.fbPassword);
    return data;
}

QString AppMain::getMatchingImg2ScreenId(int screenID)
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
        _ctrlPtr->setUserInfo(this->generateUserInfo());
        _ctrlPtr->moveToThread(_thread);
        QObject::connect(this,&AppMain::oparate,_ctrlPtr,&DeviceController::doWork);
        QObject::connect(_ctrlPtr,&DeviceController::missionCompleted,this,&AppMain::onMissionCompleted);
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
    m_firstNameList << "Trang" << "Chau" << "Thi" << "Ba" << "Hoang" << "Tung" << "Thi" << "Thi" << "Khanh" << "Ngoc" << "Dinh" << "Thanh";
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

void AppMain::onMissionCompleted(int exitCode, QString deviceName)
{
    LOG << deviceName << " : " << exitCode;

    if(exitCode == EXITCODE_TRUE){

        // ------------- Save output --------- //
        LOG << "[RegMailController]";
        QFile outputFile(OUTPUT_FILE);

        if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
            LOG << "[RegMailController]" << "Couldn't open output file";
            return;
        }

        QTextStream out(&outputFile);
        USER_INFOR data = m_deviceCtrlList.at(this->deviceList().indexOf(deviceName))->userInfo();
        out << data.userName + "@gmail.com|" + data.gmailPassword << "|" << data.fbPassword <<  "\n";
        outputFile.close();
    }

    m_deviceCtrlList.at(this->deviceList().indexOf(deviceName))->setUserInfo(this->generateUserInfo());
}
