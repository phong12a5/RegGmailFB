#include "ADBCommand.h"

QString ADBCommand::adb_command(QString cmd, QString deviceName, int timeout)
{
    QProcess process;
    if(cmd == ""){
        process.start(QString("adb shell %1").arg(cmd));
    }else{
        process.start(QString("adb -s %1 shell %2").arg(deviceName).arg(cmd));
    }
    process.waitForFinished(timeout);
    QString error = process.readAllStandardError();
    if(error != ""){
//        LOG << "Couldn't execute " << cmd << " : " << error;
        return error;
    }
    return process.readAllStandardOutput();
}

QString ADBCommand::screenShot(QString deviceName, QString fileName, QString path)
{
    fileName = fileName.arg(deviceName);
    ADBCommand::adb_command(QString("screencap -p %1%2").arg(IMAGE_FOLDER).arg(fileName),deviceName);
    QProcess::execute(QString("adb -s %1 pull %2%3 %4").arg(deviceName).arg(IMAGE_FOLDER).arg(fileName).arg(path));
    return path + "/" + fileName;
}

void ADBCommand::swipeScreen(QPoint sourcePoint, QPoint desPoint, QString deviceName)
{
    ADBCommand::adb_command(QString("input swipe %1 %2 %3 %4")\
               .arg(sourcePoint.x())\
               .arg(sourcePoint.y())\
               .arg(desPoint.x())\
               .arg(desPoint.y())
               ,deviceName);
}

void ADBCommand::requestShowApp(QString packageName, QString activity, QString deviceName)
{
    ADBCommand::adb_command(QString("am start -n %1/%2").arg(packageName).arg(activity),deviceName);
}

void ADBCommand::goHomeScreen(QString deviceName)
{
    ADBCommand::adb_command("input keyevent KEYCODE_HOME",deviceName);
}

void ADBCommand::pressBack(QString deviceName)
{
    ADBCommand::adb_command("input keyevent KEYCODE_BACK",deviceName);
}

void ADBCommand::requestShowAppDirectly(QString activity, QString deviceName)
{
    ADBCommand::adb_command(QString("am start -a %1").arg(activity),deviceName);
}

void ADBCommand::enterText(QString text, QString deviceName)
{
    LOG << text;
    ADBCommand::adb_command(QString("input text %1").arg(text),deviceName);
}

void ADBCommand::pressTap(QString deviceName)
{
    ADBCommand::adb_command("input keyevent KEYCODE_TAB",deviceName);
}

void ADBCommand::rebootDevice(QString deviceName)
{
    ADBCommand::adb_command("reboot", deviceName);
}

QString ADBCommand::currentActivity(QString deviceName)
{
    QString retVal = ADBCommand::adb_command("dumpsys window windows | grep -E 'mCurrentFocus'",deviceName);
    if(retVal.contains("mCurrentFocus=Window")){
        retVal = retVal.split(" ").last().remove('}').simplified();
    }else if(retVal.contains("mCurrentFocus=null")){
        retVal = NULL_FOCUSED_SCREEN;
    }else if(retVal.contains(QString("device '%1' not found").arg(deviceName))){
        retVal = DISCONNECTED_SCREEN;
    }else{
        retVal = "";
    }
    return retVal;
}

void ADBCommand::killSpecificApp(QString packageName,QString deviceName)
{
    ADBCommand::adb_command(QString("am force-stop %1").arg(packageName,deviceName));
}

void ADBCommand::tapScreen(QPoint point, QString deviceName)
{
    ADBCommand::adb_command(QString("input tap %1 %2").arg(point.x()).arg(point.y()),deviceName);
}

bool ADBCommand::findAndClick(QString iconImage, QString deviceName)
{
    LOG;
    cv::Mat screenImg = cv::imread(ADBCommand::screenShot(deviceName).toUtf8().constData(),1);
    cv::Mat iconMat = cv::imread(iconImage.toUtf8().constData(),1);

    QPoint point = ImageProcessing::findImageOnImage(iconMat,screenImg);
    screenImg.release();
    iconMat.release();

    if(!point.isNull()){
        ADBCommand::tapScreen(point,deviceName);
        return true;
    }else{
        return false;
    }
}

bool ADBCommand::findAnImageOnScreen(const cv::Mat &iconImg, QString deviceName)
{
    cv::Mat screenImg = cv::imread(ADBCommand::screenShot(deviceName).toUtf8().constData(),1);
    while (screenImg.empty()){
        screenImg = cv::imread(ADBCommand::screenShot(deviceName).toUtf8().constData(),1);
    }

    LOG << iconImg.rows;
    QPoint point = ImageProcessing::findImageOnImage(iconImg,screenImg);
    LOG << iconImg.rows;
    if(!point.isNull()){
        return true;
    }else{
        return false;
    }
}

void ADBCommand::wakeUpScreen(QString deviceName)
{
    ADBCommand::adb_command("input keyevent KEYCODE_POWER",deviceName);
}

void ADBCommand::clearCacheOfPackage(QString packagName, QString deviceName)
{
    ADBCommand::adb_command(QString("pm clear %1").arg(packagName),deviceName);
}

QStringList ADBCommand::deviceList()
{
    QStringList output;
    QString cmdResult = ADBCommand::adb_command("devices");
    foreach(QString device, cmdResult.split("\r\n")){
        if(device.contains("\tdevice"))
            output << device.split("\t").at(0);
    }
    return output;
}

void ADBCommand::enterKeyBoard(QString deviceName)
{
    ADBCommand::adb_command("input keyevent KEYCODE_ENTER",deviceName);
}

void ADBCommand::hideKeyboard(QString deviceName)
{
    ADBCommand::adb_command("input keyevent 111",deviceName);
}

void ADBCommand::collapseStatusBar(QString deviceName)
{
    ADBCommand::adb_command("service call statusbar 2",deviceName);
}

QString ADBCommand::readNotificationData(QString deviceName)
{
    return ADBCommand::adb_command("dumpsys notification",deviceName);
}

void ADBCommand::installPackage(QString apkPath, QString deviceName)
{
    LOG << ADBCommand::adb_command(QString("pm install %1").arg(apkPath),deviceName,-1);
}

void ADBCommand::uninstallPackage(QString packageName, QString deviceName)
{
    ADBCommand::adb_command(QString("pm uninstall %1").arg(packageName),deviceName);
}

void ADBCommand::requestSyncAccount(QString deviceName)
{
    LOG << "Sync google account";
    ADBCommand::adb_command("requestsync",deviceName);
}
