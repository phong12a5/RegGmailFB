#ifndef APPDEFINES_H
#define APPDEFINES_H

#include <QDebug>
#include <QTest>
#include <AppEnums.h>
#include <QMap>

typedef struct user_infor{
    QString firstName;
    QString lastName;
    QString middleName;
    QString userName;
    int bodYear;
    int bodMonth;
    int bodDate;
    QString sex;
    QString gmailPassword;
    QString fbPassword;
    QString captcha;

    user_infor(){
       firstName = "";
       lastName = "";
       userName = "";
       middleName = "";
       bodYear = 1994;
       bodMonth = 7;
       bodDate = 1;
       sex = "M";
       gmailPassword = "";
       fbPassword = "";
       captcha = "";
    }

} USER_INFOR;

typedef struct birthday_struct{
    int month;
    int day;
    int year;

    birthday_struct(){
       month = 7;
       day = 1;
       year = 1994;
    }

} BIRTHDAY_STRUCT;


#define LOG                     qDebug()<<"["<<__FUNCTION__<<"][" << __LINE__ << "] "

#define delay(milescond)        QTest::qSleep(milescond)

#define IMAGE_FOLDER            "/storage/emulated/legacy/DCIM/"

#define FIRST_NAME_FILE         QDir::currentPath()+"/firstname.txt"
#define LAST_NAME_FILE          QDir::currentPath()+"/lastname.txt"
#define MIDDLE_NAME_FILE        QDir::currentPath()+"/middlename.txt"

#define OUTPUT_FILE             QDir::currentPath()+"/emailList.txt"

#define ACCOUNT_SETTING_ACT     "android.settings.ADD_ACCOUNT_SETTINGS"

#define XGAME_PKG               "app.haonam.xgame"
#define XGAME_ACTIVITYMAIN      ".MainActivity"

#define FBLITE_PKG              "com.facebook.lite"
#define FBLITE_ACTIVITYMAIN     ".MainActivity"
#define FBLITE_APK_PATH         "/storage/emulated/legacy/Download/com.facebook.lite.apk"


//HOME activities
#define HOME_SCREEN             "com.sonyericsson.home/com.sonymobile.home.HomeActivity"

//GOOGLE SETTING activities
#define ACCOUNT_SETTING_SCREEN  "com.android.settings/com.android.settings.accounts.ChooseAccountActivity"
#define ADD_A_GGACCOUNT_SCREEN  "com.google.android.gsf.login/com.google.android.gsf.login.AccountIntroUIActivity"
#define INPUT_YOURNAME_SCREEN   "com.google.android.gsf.login/com.google.android.gsf.login.NameActivity"
#define INPUT_USERNAME_SCREEN   "com.google.android.gsf.login/com.google.android.gsf.login.UsernameActivity"
#define INPUT_PASSWORD_SCREEN   "com.google.android.gsf.login/com.google.android.gsf.login.ChoosePasswordActivity"
#define RECOVERY_INTRO_SCREEN   "com.google.android.gsf.login/com.google.android.gsf.login.RecoveryIntroActivity"
#define GOOGLE_SERVICE_SCREEN   "com.google.android.gsf.login/com.google.android.gsf.login.GoogleServicesActivity"
#define TERM_SERVICE_SCREEN     "com.google.android.gsf.login/com.google.android.gsf.login.TermsOfServiceActivity"
#define AUTHENTICATING_SCREEN   "com.google.android.gsf.login/com.google.android.gsf.login.CaptchaActivity"
#define WIFI_PICKER_SCREEN      "com.android.settings/com.android.settings.wifi.WifiPickerActivity"
#define SYNC_INTRO_SCREEN       "com.google.android.gsf.login/com.google.android.gsf.login.SyncIntroActivity"
#define PAYMENT_SETTING_SCREEN  "com.android.vending/com.google.android.finsky.billing.promptforfop.SetupWizardPromptForFopActivity"
#define COULD_NOT_SIGNIN        "com.google.android.gsf.login/com.google.android.gsf.login.ShowErrorActivity"
#define TURNON_PLAY_PROTECT     "com.android.vending/com.google.android.finsky.verifier.impl.ConsentDialog"
#define CREATE_ACC_TASK         "com.google.android.gsf.login/com.google.android.gsf.login.CreateAccountTask"

#define UNKNOW_SCREEN           "UNKNOW_SCREEN"
#define LOCK_SCREEN             "Keyguard"
#define STATUSBAR_SCREEN        "StatusBar"
#define NULL_FOCUSED_SCREEN     "NULL"
#define DISCONNECTED_SCREEN     "DISCONNECTED_SCREEN"

// XGAME activities
#define XGAME_SCREEN            "app.haonam.xgame/app.haonam.xgame.MainActivity"

// FACEBOOK LITE activity
#define FACEBOOK_LITE_SCREEN    "com.facebook.lite/com.facebook.lite.MainActivity"

// UNKNOW_SCREEN

#define CONFIG_FILE_NAME        "config.json"
#define SAVE_LOCAL_FIELD        "save_local"
#define SAVE_SERVER_FIELD       "save_server"
#define USE_KEYBOARD_FIELD      "use_keyboard"
#define APP_DATA_FIELD          "app_data"
#define REG_FAEBOOK_FIELD       "reg_facebook"
#define RECOVERY_EMAIL_FIELD    "recovery_email"
#define NAME_LANG_FIELD         "name_language"

#define EXITCODE_FALSE          1
#define EXITCODE_TRUE           0

//extern QMap<AppEnums::E_FBLITE_SCREEN_ID, cv::Mat> SCREEN_MATHING_TABLE;/*{
//    {AppEnums::E_FBLITE_SCREEN_ID_LOGIN,                CREATE_NEW_FBACC_ICON},
//    {AppEnums::E_FBLITE_SCREEN_ID_JOIN_FB,              JOIN_FB_TEXT},
//    {AppEnums::E_FBLITE_SCREEN_ID_ENTER_NAME,           WHAT_YOUR_NAME_TEXT},
//    {AppEnums::E_FBLITE_SCREEN_ID_ENTER_MOBILE_NUM,     ENTER_MOBILE_NUM_TEXT},
//    {AppEnums::E_FBLITE_SCREEN_ID_ENTER_EMAIL_ADDRESS,  ENTER_YOUR_EMAIL},
//    {AppEnums::E_FBLITE_SCREEN_ID_ENTER_BIRTHDAY,       WHAT_YOUR_BIRTHDAY},
//    {AppEnums::E_FBLITE_SCREEN_ID_ENTER_GENDER,         WHAT_YOUR_GENDER},
//    {AppEnums::E_FBLITE_SCREEN_ID_ENTER_PASSWORD,       CREATE_PASS_SCREEN},
//    {AppEnums::E_FBLITE_SCREEN_ID_SAVE_LOGIN_INFO,      SAVE_LOGIN_TEXT},
//    {AppEnums::E_FBLITE_SCREEN_ID_WELCOME_SCREEN,       WELCOME_SCREEN},
//    {AppEnums::E_FBLITE_SCREEN_ID_CONFIRM_EMAIL,        "STR_SETUP_SYSTEM_INFO"},
//    {AppEnums::E_FBLITE_SCREEN_ID_ADD_PIC_PROFILE,      "STR_SETUP_SYSTEM_INFO"},
//    {AppEnums::E_FBLITE_SCREEN_ID_FIND_FRIENDS,         "STR_SETUP_SYSTEM_INFO"},
//    {AppEnums::E_FBLITE_SCREEN_ID_NEW_FEEDS,            "STR_SETUP_SYSTEM_INFO"},
//    {AppEnums::E_FBLITE_SCREEN_ID_CHECK_POINT,          "STR_SETUP_SYSTEM_INFO"}
//};*/

#endif // APPDEFINES_H

