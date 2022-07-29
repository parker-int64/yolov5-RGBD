#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include <QObject>
#include <QImage>
#include <QMutex>
#include <QMutexLocker>
#include <iostream>
#include <cstring>
#include <opencv2/highgui.hpp>
#include <QImage>
#include <QThread>
#include <QFileInfo>
#include "utility.h"
#include "cpuinfer.h"
#ifdef _DEBUG
    #include <QDebug>
#endif

class ImageProcess : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ImageProcess)
public:
    explicit ImageProcess(QObject *parent = nullptr);
    ~ImageProcess();

    QImage MatImageToQImage(const cv::Mat &src);
    void initCapture(const int cameraIndex, const double capWidth, const double capHeight);


signals:
    void sendImage(const QImage &);

    void sendCameraError(const QString);

    void sendInferDeviceError(const QString);

    void sendInferDeviceSuccess();

public slots:
    void startCapture();
    void readFrame();
    void endCapture();

    void checkInferParameter(QVecInt capturePara, QStringList inferPara);

    void changeYoloDetectStatus();


private:
    QMutex mutex;
    cv::VideoCapture cap;
    cv::Mat m_frame;
    QImage m_q_image;
    QMutex image_process_mutex;
    bool m_image_process_running;
    bool m_yolo_running;
    bool m_full_demo_running;

    int m_camera_index;
    int m_capture_width;
    int m_capture_height;

    QString m_full_network_path;

    QString runtime_error;

    CPUInfer *infer;
};



#endif // IMAGEPROCESS_H
