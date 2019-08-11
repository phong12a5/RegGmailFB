#ifndef APPMAIN_H
#define APPMAIN_H

#include <QObject>
#include "AppDefines.h"
#include <QQmlContext>
#include <QQuickView>
//#include "AppEnums.h"
#include "AppDefines.h"
//#include "Controller/RegMailController.h"
//#include "Controller/RegDeviceInfoController.h"
//#include "Controller/RegFBController.h"
#include "Processing/ADBCommand.h"
//#include <QJsonObject>
//#include <QJsonDocument>
//#include <QVariant>
//#include <QThread>
//#include <QMutex>
//#include "Controller/ThreadController.h"
#include "Processing/CheckCurrSrcWorker.h"
#include "Controller/DeviceController.h"
#include <QThread>
#ifdef USE_OPENCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#endif

class AppMain : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList deviceList READ deviceList WRITE setDeviceList NOTIFY deviceListChanged)

private:
    explicit AppMain(QObject *parent = nullptr);

public:
    static AppMain *instance();
    void initApplication();

    QStringList deviceList() const;
    void setDeviceList(QStringList _devices);

    USER_INFOR generateUserInfo();

    Q_INVOKABLE void startProgram();
    Q_INVOKABLE void closeProgram();

private:
    void loadUserInfo();

private:
    static AppMain* m_instance;

    QStringList m_deviceList;
    QList<DeviceController *> m_deviceCtrlList;
    QList<QThread *> m_matchThreadList;

    QStringList m_lastNameList;
    QStringList m_firstNameList;
    QStringList m_middleNameList;

signals:
    void oparate();
    void startCheckActivity();
    void deviceListChanged();
//    void processFinished(int stepId, int exitCode);

public slots:
    void onUpdateCurrentActOnDevices(QStringList activities);
//    void onProcessFinished(int currentStep, int exitCode);

public:
    // GOOGLE SETTING icons
    const cv::Mat AUTO_CHANGE_ICON        = cv::imread(QString(QDir::currentPath() + "/IconImage/autoChangeBtn.png").toUtf8().constData());
    const cv::Mat GOOGLE_ACCOUNT_ICON     = cv::imread(QString(QDir::currentPath() + "/IconImage/googleAccIcon.png").toUtf8().constData());
    const cv::Mat ADD_NEW_ACC_ICON        = cv::imread(QString(QDir::currentPath() + "/IconImage/addNewAccIcon.png").toUtf8().constData());
    const cv::Mat NEXT_YOURNAME_ICON      = cv::imread(QString(QDir::currentPath() + "/IconImage/NextYourNameIcon.png").toUtf8().constData());
    const cv::Mat NOT_NOW_ICON            = cv::imread(QString(QDir::currentPath() + "/IconImage/notNowIcon.png").toUtf8().constData());
    const cv::Mat ACCEPT_BY_ME_ICON       = cv::imread(QString(QDir::currentPath() + "/IconImage/acceptByMe.png").toUtf8().constData());
    const cv::Mat SKIP_PAYMENT_ICON       = cv::imread(QString(QDir::currentPath() + "/IconImage/skipPaymentIcon.png").toUtf8().constData());

    // FACEBOOK LITE icons
    const cv::Mat CREATE_NEW_FBACC_ICON   = cv::imread(QString(QDir::currentPath() + "/IconImage/createNewAccIcon.png").toUtf8().constData());
    const cv::Mat JOIN_FB_TEXT            = cv::imread(QString(QDir::currentPath() + "/IconImage/JoinFBText.png").toUtf8().constData());
    const cv::Mat NEXT_BUTTON             = cv::imread(QString(QDir::currentPath() + "/IconImage/nextBtn.png").toUtf8().constData());
    const cv::Mat WHAT_YOUR_NAME_TEXT     = cv::imread(QString(QDir::currentPath() + "/IconImage/InputNameScr.png").toUtf8().constData());
    const cv::Mat FIRSTNAME_FIELD         = cv::imread(QString(QDir::currentPath() + "/IconImage/FirstNameField.png").toUtf8().constData());
    const cv::Mat LASTNAME_FIELD          = cv::imread(QString(QDir::currentPath() + "/IconImage/LastNameField.png").toUtf8().constData());
    const cv::Mat ENTER_MOBILE_NUM_TEXT   = cv::imread(QString(QDir::currentPath() + "/IconImage/enterMobileNumberScr.png").toUtf8().constData());
    const cv::Mat WHAT_YOUR_BIRTHDAY      = cv::imread(QString(QDir::currentPath() + "/IconImage/whatYourBirthdayScr.png").toUtf8().constData());
    const cv::Mat WHAT_YOUR_GENDER        = cv::imread(QString(QDir::currentPath() + "/IconImage/whatYourGender.png").toUtf8().constData());
    const cv::Mat FEMALE_BUTTON           = cv::imread(QString(QDir::currentPath() + "/IconImage/femaleIcon.png").toUtf8().constData());
    const cv::Mat MALE_BUTTON             = cv::imread(QString(QDir::currentPath() + "/IconImage/maleIcon.png").toUtf8().constData());
    const cv::Mat CREATE_PASS_SCREEN      = cv::imread(QString(QDir::currentPath() + "/IconImage/createPassWord.png").toUtf8().constData());
    const cv::Mat PASSWORD_FIELD          = cv::imread(QString(QDir::currentPath() + "/IconImage/passwordField.png").toUtf8().constData());
    const cv::Mat SIGN_UP_BUTTON          = cv::imread(QString(QDir::currentPath() + "/IconImage/signUpBtn.png").toUtf8().constData());
    const cv::Mat SAVE_LOGIN_TEXT         = cv::imread(QString(QDir::currentPath() + "/IconImage/saveLoginInfo.png").toUtf8().constData());
    const cv::Mat NOT_NOW_BUTTON          = cv::imread(QString(QDir::currentPath() + "/IconImage/notNowBtn.png").toUtf8().constData());
    const cv::Mat ADD_YOUR_EMAIL          = cv::imread(QString(QDir::currentPath() + "/IconImage/addYourEmail.png").toUtf8().constData());
    const cv::Mat GAMIL_RADIO_BTN         = cv::imread(QString(QDir::currentPath() + "/IconImage/gmailRadioButton.png").toUtf8().constData());
    const cv::Mat YES_BTN                 = cv::imread(QString(QDir::currentPath() + "/IconImage/yesButton.png").toUtf8().constData());
    const cv::Mat CONFIRM_EMAIL           = cv::imread(QString(QDir::currentPath() + "/IconImage/confirmEmail.png").toUtf8().constData());
    const cv::Mat SKIP_BUTTON             = cv::imread(QString(QDir::currentPath() + "/IconImage/skipButton.png").toUtf8().constData());
    const cv::Mat ADD_A_PICTURE           = cv::imread(QString(QDir::currentPath() + "/IconImage/chooseFromGallery.png").toUtf8().constData());
    const cv::Mat TURN_ON_FIND_FRIEND     = cv::imread(QString(QDir::currentPath() + "/IconImage/turnOnFindFriendScr.png").toUtf8().constData());
    const cv::Mat SIGN_UP_WITH_EMAIL      = cv::imread(QString(QDir::currentPath() + "/IconImage/setupWithEmail.png").toUtf8().constData());
    const cv::Mat ENTER_YOUR_EMAIL        = cv::imread(QString(QDir::currentPath() + "/IconImage/enterYourEmail.png").toUtf8().constData());
    const cv::Mat SKIP_FIND_FRIEND        = cv::imread(QString(QDir::currentPath() + "/IconImage/skipFindFriend.png").toUtf8().constData());
    const cv::Mat PROFILE_ICON            = cv::imread(QString(QDir::currentPath() + "/IconImage/profileIcon.png").toUtf8().constData());
    const cv::Mat CHECK_POINT_SCREEN      = cv::imread(QString(QDir::currentPath() + "/IconImage/checkPointScreen.png").toUtf8().constData());
    const cv::Mat BIRTH_DAY_FIELD         = cv::imread(QString(QDir::currentPath() + "/IconImage/birdayInputField.png").toUtf8().constData());
    const cv::Mat KEYBOARD                = cv::imread(QString(QDir::currentPath() + "/IconImage/keyboard.png").toUtf8().constData());
    const cv::Mat WELCOME_SCREEN          = cv::imread(QString(QDir::currentPath() + "/IconImage/WelcomeScreen.png").toUtf8().constData());
    const cv::Mat OK_BUTTON               = cv::imread(QString(QDir::currentPath() + "/IconImage/OkButton.png").toUtf8().constData());

    const QMap<AppEnums::E_FBLITE_SCREEN_ID, cv::Mat> SCREEN_MATHING_TABLE{
        {AppEnums::E_FBLITE_SCREEN_ID_LOGIN,                CREATE_NEW_FBACC_ICON},
        {AppEnums::E_FBLITE_SCREEN_ID_JOIN_FB,              JOIN_FB_TEXT},
        {AppEnums::E_FBLITE_SCREEN_ID_ENTER_NAME,           WHAT_YOUR_NAME_TEXT},
        {AppEnums::E_FBLITE_SCREEN_ID_ENTER_MOBILE_NUM,     ENTER_MOBILE_NUM_TEXT},
        {AppEnums::E_FBLITE_SCREEN_ID_ENTER_EMAIL_ADDRESS,  ENTER_YOUR_EMAIL},
        {AppEnums::E_FBLITE_SCREEN_ID_ENTER_BIRTHDAY,       WHAT_YOUR_BIRTHDAY},
        {AppEnums::E_FBLITE_SCREEN_ID_ENTER_GENDER,         WHAT_YOUR_GENDER},
        {AppEnums::E_FBLITE_SCREEN_ID_ENTER_PASSWORD,       CREATE_PASS_SCREEN},
        {AppEnums::E_FBLITE_SCREEN_ID_SAVE_LOGIN_INFO,      SAVE_LOGIN_TEXT},
        {AppEnums::E_FBLITE_SCREEN_ID_WELCOME_SCREEN,       WELCOME_SCREEN},
    };
};

#endif // APPMAIN_H
