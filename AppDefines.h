#ifndef APPDEFINES_H
#define APPDEFINES_H

#include <QDebug>
#include <QTest>
#include <AppEnums.h>
#include <QDir>

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

    void clear(){
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

#define OUTPUT_FILE             QDir::currentPath()+"/outputData.txt"

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
#define INVALID_USERNAME        "com.google.android.gsf.login/com.google.android.gsf.login.SuggestUsernameActivity"

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

#define FIRSTNAME_FILE          "firstname.txt"
#define LASTNAME_FILE           "lastname.txt"

#define EXITCODE_FALSE          1
#define EXITCODE_TRUE           0

#define KEEP_FB_SCREEN_TIMEOUT  30000


// GOOGLE SETTING icons
#define AUTO_CHANGE_ICON        QDir::currentPath() + "/IconImage/autoChangeBtn.png"
#define GOOGLE_ACCOUNT_ICON     QDir::currentPath() + "/IconImage/googleAccIcon.png"
#define ADD_NEW_ACC_ICON        QDir::currentPath() + "/IconImage/addNewAccIcon.png"
#define NEXT_YOURNAME_ICON      QDir::currentPath() + "/IconImage/NextYourNameIcon.png"
#define NOT_NOW_ICON            QDir::currentPath() + "/IconImage/notNowIcon.png"
#define ACCEPT_BY_ME_ICON       QDir::currentPath() + "/IconImage/acceptByMe.png"
#define SKIP_PAYMENT_ICON       QDir::currentPath() + "/IconImage/skipPaymentIcon.png"

// FACEBOOK LITE icons
#define CREATE_NEW_FBACC_ICON   QDir::currentPath() + "/IconImage/createNewAccIcon.png"
#define JOIN_FB_TEXT            QDir::currentPath() + "/IconImage/JoinFBText.png"
#define NEXT_BUTTON             QDir::currentPath() + "/IconImage/nextBtn.png"
#define WHAT_YOUR_NAME_TEXT     QDir::currentPath() + "/IconImage/InputNameScr.png"
#define FIRSTNAME_FIELD         QDir::currentPath() + "/IconImage/FirstNameField.png"
#define LASTNAME_FIELD          QDir::currentPath() + "/IconImage/LastNameField.png"
#define ENTER_MOBILE_NUM_TEXT   QDir::currentPath() + "/IconImage/enterMobileNumberScr.png"
#define WHAT_YOUR_BIRTHDAY      QDir::currentPath() + "/IconImage/whatYourBirthdayScr.png"
#define WHAT_YOUR_GENDER        QDir::currentPath() + "/IconImage/whatYourGender.png"
#define FEMALE_BUTTON           QDir::currentPath() + "/IconImage/femaleIcon.png"
#define MALE_BUTTON             QDir::currentPath() + "/IconImage/maleIcon.png"
#define CREATE_PASS_SCREEN      QDir::currentPath() + "/IconImage/createPassWord.png"
#define PASSWORD_FIELD          QDir::currentPath() + "/IconImage/passwordField.png"
#define SIGN_UP_BUTTON          QDir::currentPath() + "/IconImage/signUpBtn.png"
#define SAVE_LOGIN_TEXT         QDir::currentPath() + "/IconImage/saveLoginInfo.png"
#define NOT_NOW_BUTTON          QDir::currentPath() + "/IconImage/notNowBtn.png"
#define ADD_YOUR_EMAIL          QDir::currentPath() + "/IconImage/addYourEmail.png"
#define GAMIL_RADIO_BTN         QDir::currentPath() + "/IconImage/gmailRadioButton.png"
#define YES_BTN                 QDir::currentPath() + "/IconImage/yesButton.png"
#define CONFIRM_EMAIL           QDir::currentPath() + "/IconImage/confirmEmail.png"
#define SKIP_BUTTON             QDir::currentPath() + "/IconImage/skipButton.png"
#define ADD_A_PICTURE           QDir::currentPath() + "/IconImage/chooseFromGallery.png"
#define TURN_ON_FIND_FRIEND     QDir::currentPath() + "/IconImage/turnOnFindFriendScr.png"
#define SIGN_UP_WITH_EMAIL      QDir::currentPath() + "/IconImage/setupWithEmail.png"
#define ENTER_YOUR_EMAIL        QDir::currentPath() + "/IconImage/enterYourEmail.png"
#define SKIP_FIND_FRIEND        QDir::currentPath() + "/IconImage/skipFindFriend.png"
#define PROFILE_ICON            QDir::currentPath() + "/IconImage/profileIcon.png"
#define CHECK_POINT_SCREEN      QDir::currentPath() + "/IconImage/checkPointScreen.png"
#define BIRTH_DAY_FIELD         QDir::currentPath() + "/IconImage/birdayInputField.png"
#define KEYBOARD                QDir::currentPath() + "/IconImage/keyboard.png"
#define WELCOME_SCREEN          QDir::currentPath() + "/IconImage/WelcomeScreen.png"
#define OK_BUTTON               QDir::currentPath() + "/IconImage/OkButton.png"

#endif // APPDEFINES_H

