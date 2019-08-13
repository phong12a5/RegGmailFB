#ifndef HTTPREQUESTCONTROLLER_H
#define HTTPREQUESTCONTROLLER_H

#include <QObject>
#include "AppDefines.h"
#include <QTimer>
#include <QBuffer>
#include <QHttpPart>

class HttpRequestController : public QObject
{
    Q_OBJECT
public:
    explicit HttpRequestController(QObject *parent = nullptr);

public:
    ~HttpRequestController();

    QString sendHttpRequest(QString, QString);

private:
    QNetworkAccessManager *manager;
    QEventLoop eventLoop;

signals:
    void takeCaptcha(QString captcha);

public slots:
};

#endif // HTTPREQUESTCONTROLLER_H
