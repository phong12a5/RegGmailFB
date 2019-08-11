#ifndef BASECONTROLLERTROLLER_H
#define BASECONTROLLERTROLLER_H

#include <QObject>

class BaseController : public QObject
{
    Q_OBJECT
public:
    explicit BaseController(QObject *parent = nullptr);

signals:

public slots:
};

#endif // BASECONTROLLERTROLLER_H