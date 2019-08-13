#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <iostream>
#include <stdio.h>
#include <QObject>
#ifdef USE_OPENCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#endif
#include "AppDefines.h"
#include "QPoint"
#include <QFile>

class ImageProcessing : public QObject
{
    Q_OBJECT
public:
    explicit ImageProcessing(QObject *parent = nullptr);


public:
    static QPoint findImageOnImage(const cv::Mat &_smallImage, const cv::Mat &_largeImage);
    static QString extractCaptchaImage(const QString& path, const QString deviceName);

signals:

public slots:
};

#endif // IMAGEPROCESSING_H
