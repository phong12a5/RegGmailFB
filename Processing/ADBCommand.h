#ifndef ADBCOMMAND_H
#define ADBCOMMAND_H

#include <QObject>
#include <QDir>
#include "AppDefines.h"
#include <QProcess>
#include "Processing/ImageProcessing.h"

class ADBCommand : public QObject
{
    Q_OBJECT

public:

    static QString adb_command(QString cmd, QString deviceName = "", int timeout = 3000);
    static QString screenShot( QString deviceName, QString fileName = "screen_%1.png",QString path = QDir::currentPath());
    static void tapScreen(QPoint point, QString deviceName);
    static void swipeScreen(QPoint sourcePoint, QPoint desPoint, QString deviceName);
    static void requestShowApp(QString packageName, QString activity, QString deviceName);
    static void goHomeScreen(QString deviceName);
    static void pressBack(QString deviceName);
    static void requestShowAppDirectly(QString activity, QString deviceName);
    static void enterText(QString text,QString deviceName);
    static void pressTap(QString deviceName);
    static void rebootDevice(QString deviceName);
    static QString currentActivity(QString deviceName);
    static void killSpecificApp(QString packageName,QString deviceName);
    static bool findAndClick(QString iconImage, QString deviceName);
    static bool findAnImageOnScreen(const cv::Mat &iconImg, QString deviceName);
    static void wakeUpScreen(QString deviceName);
    static void clearCacheOfPackage(QString packagName,QString deviceName);
    static QStringList deviceList();
    static void enterKeyBoard(QString deviceName);
    static void hideKeyboard(QString deviceName);
    static void collapseStatusBar(QString deviceName);
    static QString readNotificationData(QString deviceName);
    static void installPackage(QString apkPath,QString deviceName);
    static void uninstallPackage(QString packageName, QString deviceName);
    static void requestSyncAccount(QString deviceName);
};

#endif // ADBCOMMAND_H
