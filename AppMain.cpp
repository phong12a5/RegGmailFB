#include "AppMain.h"
#include "QFile"

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

AppMain::~AppMain()
{
    for(int i =0; i < m_matchThreadList.length(); i++){
        m_matchThreadList.at(i)->quit();
        m_matchThreadList.at(i)->wait();
    }
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
    data.middleName = m_middleNameList.at(rand() % (m_middleNameList.length()));

    data.userName = data.firstName + data.lastName +  QString::number(rand() % 1000000000 + 3000000);
    data.sex = data.middleName == (rand() % 2) == 1? "F" : "M";
    data.captcha = "";
    QStringList part1 = QStringList() << data.firstName << data.lastName;
    QStringList part2 = QStringList() << data.firstName << data.lastName;
    QStringList part3 = QStringList() << "@";//  << "!" << "#" << "$";
    QStringList part4 = QStringList() << QString("0" + QString::number(data.bodDate)).right(2) + QString("0" + QString::number(data.bodMonth)).right(2)
                                      << QString("0" + QString::number(data.bodMonth)).right(2) + QString("0" + QString::number(data.bodYear)).right(4);

    QString type0 = part1.at(rand()%part1.length()) +
            part2.at(rand()%part2.length()).toLower() +
            part3.at(rand()%part3.length()) +
            part4.at(rand()%part4.length());

    QString type1 = part1.at(rand()%part1.length()) +
            part2.at(rand()%part2.length()).toLower() +
            part4.at(rand()%part4.length()) +
            part3.at(rand()%part3.length());

    data.fbPassword = (rand() % 2) == 1? type1 : type0;
    data.gmailPassword = data.fbPassword;

    LOG << QString("Info: %1 | %2 | %3 | %4").arg(data.firstName).arg(data.lastName).arg(data.userName).arg(data.fbPassword);
    return data;
}



void AppMain::startProgram()
{
    LOG;
    emit this->oparate();
}

void AppMain::closeProgram()
{
    LOG;
//    QCoreApplication::quit();
    qApp->exit();
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
}

void AppMain::loadUserInfo()
{

    QFile firstNameFile(FIRSTNAME_FILE);
    if (!firstNameFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        LOG << "[RegMailController]" << "Couldn't read first name file";
        return;
    }

    QFile lastNameFile(LASTNAME_FILE);
    if (!lastNameFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        LOG << "[RegMailController]" << "Couldn't read last name file";
        return;
    }

    while (!firstNameFile.atEnd()) {
        QString tmp = firstNameFile.readLine().simplified();
        m_firstNameList.append(tmp);
    }

    while (!lastNameFile.atEnd()) {
        QString tmp = lastNameFile.readLine().simplified();
        m_lastNameList.append(tmp);
    }

    m_middleNameList = m_lastNameList;

    if(m_firstNameList.isEmpty() || m_firstNameList.isEmpty()){
        LOG << "Cann't start without user data";
        this->closeProgram();
    }

//    LOG << "m_lastNameList: " << m_lastNameList;
//    LOG << "firstNameFile: " << firstNameFile;
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
