#include "ImageProcessing.h"

ImageProcessing::ImageProcessing(QObject *parent) : QObject(parent)
{

}

QPoint ImageProcessing::findImageOnImage(const cv::Mat &_smallImage, const cv::Mat &_largeImage)
{
    //    LOG << "[ImageProcessing]"  << smallImagePath.split("/").last() << largeImagePath.split("/").last();

        QPoint retVal;

    #ifdef USE_OPENCV

        //kiểm tra kích cỡ của ảnh input & template
        if (_smallImage.rows > _largeImage.rows || _smallImage.cols > _largeImage.cols)
        {
            LOG << "[ImageProcessing]" << "Mat template must be smaller than matInput: [" << _smallImage.rows << "," << _smallImage.cols << "]";
            return retVal;
        }else if(_smallImage.rows <= 0 || _smallImage.cols <= 0){
            LOG << "[ImageProcessing]" << "_smallImage: Invalid Image";
            return retVal;
        }else if(_largeImage.rows <= 0 || _largeImage.cols <= 0){
            LOG << "[ImageProcessing]" << "_largeImage: Invalid Image";
            return retVal;
        }

        cv::Mat result;
        int result_cols = _largeImage.cols - _smallImage.cols + 1;
        int result_rows = _largeImage.rows - _smallImage.rows + 1;
        result.create(result_rows, result_cols, CV_32FC1);

        //tìm mẫu
        cv::matchTemplate(_largeImage, _smallImage, result, CV_TM_CCOEFF_NORMED);


        double threshold = 0.8;
        cv::threshold(result, result, threshold, 1., CV_THRESH_TOZERO);
        double minval, maxval;
//        double bestMaxval = 0;
        //ngưỡng chính xác, giảm xuống thì sẽ tìm được nhiều đối tượng hơn nhưng sai số nhiều hơn

//        while (true)
//        {
            cv::Point minloc, maxloc;
            cv::minMaxLoc(result, &minval, &maxval, &minloc, &maxloc);

//            matchLoc = maxLoc;
//            if (maxval > threshold)
//            {
                //vẽ hình chữ nhật lên đối tượng tìm được
//                if(maxval > bestMaxval){
//                    bestMaxval = maxval;
            if(maxval > threshold){
                retVal = QPoint(maxloc.x + _smallImage.cols/2, maxloc.y + _smallImage.rows/2);
            }
//                }
//                cv::floodFill(result, maxloc, cv::Scalar(0), 0, cv::Scalar(.1), cv::Scalar(1.));

//            }
//            else
//                break;
//        }

    //    LOG << "[ImageProcessing]" << "Return values: " << retVal << " --- bestMaxVal: " << bestMaxval;
    #endif
        return retVal;
}

QString ImageProcessing::extractCaptchaImage(const QString &path, const QString deviceName)
{
    LOG << "[ImageProcessing]" << "Path: " << path;

#ifndef USE_OPENCV
    return  QString("");
#else
    cv::Mat src = cv::imread(path.toUtf8().constData());
    cv::Rect crop(58 , 473, 715, 216);
    cv::Mat rez = src(crop);

    QString captImgPath = (QDir::currentPath() + QString("/captcha_%1.png").arg(deviceName));
    cv::imwrite(captImgPath.toUtf8().constData(),rez);
    cv::waitKey(100);

    if(QFile(captImgPath).exists() && !QImage(captImgPath).isNull()){
        return captImgPath;
    }else{
        LOG << "[ImageProcessing]" << "Couldn't extract captcha image";
        return QString("");
    }
#endif
}
