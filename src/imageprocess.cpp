#include "imageprocess.h"

ImageProcess::ImageProcess(QObject *parent) : QObject(parent)
{
    m_image_process_running = false;
    m_yolo_running = false;
    m_full_demo_running = false;
    infer = new CPUInfer();
}


ImageProcess::~ImageProcess(){

}
/**
 * @brief convert opencv cv::Mat to qt QImage
 *
 * @param src 
 * @return QImage 
 */
QImage ImageProcess::MatImageToQImage(const cv::Mat &src){
    QImage returnImage;
    int channel = src.channels();                                                       // get the channels
    switch (channel) {
        case 1:{
            QImage qImage(src.cols, src.rows, QImage::Format_Indexed8);                 // CV_8UC1 grey sacle, create QImage from the given Mat CV_8UC1
            qImage.setColorCount(256);                                                  // extend the colorlist to 256
            for(int i = 0; i < 256; i ++){                                              // set color to given index
                qImage.setColor(i,qRgb(i,i,i));                                         // generate grayscale pic
            }
            uchar *pSrc = src.data;                                                     // get the pointer of the src's data
            for(int row = 0; row < src.rows; row ++){                                   // use the pointer to scan the pixels
                uchar *pDest = qImage.scanLine(row);
                std::memcmp(pDest, pSrc, src.cols);                                       // copy the data
                pSrc += src.step;
            }
            returnImage = qImage;
        }

        break;

        case 4:{
            const uchar *pSrc = (const uchar*)src.data;                                     // return the created image, CV_8UC4
            QImage qImage(pSrc, src.cols, src.rows, src.step, QImage::Format_ARGB32);       // Format RGBA
            returnImage = qImage.copy();
        }

        break;
        default:{
            const uchar *pSrc = (const uchar*)src.data;                                     // get the pointer of src's data, CV_8UC3
            QImage qImage(pSrc,src.cols,src.rows,src.step,QImage::Format_RGB888);           // create QImage from the given src
            returnImage = qImage.rgbSwapped();                                              // exchange the channel sequence from OpenCV's BGR to RGB
        }

        break;
    }
    return returnImage;
}


void ImageProcess::initCapture(const int cameraIndex = 0, const double capWidth = 640, const double capHeight = 480){
#ifdef USE_VIDEO
    cap.open("../video/street480.mp4");
#else
    cap.open(cameraIndex, cv::CAP_DSHOW);                                                    // necessary on windows, Mircosoft DirectShow
    cap.set(cv::CAP_PROP_FRAME_WIDTH, capWidth);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, capHeight);
#endif

    if(!cap.isOpened()){
#ifdef _DEBUG
        qDebug() << "Failed to open the camera or video file. ";
#endif
        m_image_process_running = false;
        runtime_error = "Error, unable to open the camera.";
        cap.release();
        emit sendCameraError(runtime_error);
        return ;
    } else {
        m_image_process_running = true;
    }
}



void ImageProcess::readFrame(){
    initCapture(m_camera_index, m_capture_width,m_capture_height);
    while(m_image_process_running){
        cap.read(m_frame);
#ifdef USE_VIDEO
        cv::waitKey(33);
#endif
        if(m_frame.empty()){
#ifdef USB_VIDEO
            qDebug() << "Frame is empty, will break now";
#endif
            break;
        } else {
            if(m_yolo_running){
                cv::Mat yolo_frame = infer->startInfer(m_frame);
                m_q_image = MatImageToQImage(yolo_frame);
                emit sendImage(m_q_image);
            } else {
                m_q_image = MatImageToQImage(m_frame);
                emit sendImage(m_q_image);
            }
        }
    }
    cap.release();
}


void ImageProcess::startCapture(){
    m_image_process_running = true;
}

void ImageProcess::endCapture(){
    QMutexLocker locker(&mutex);
    m_image_process_running = false;
}

void ImageProcess::checkInferParameter(QVector<int> capturePara, QStringList inferPara){
#ifdef _DEBUG
    qDebug() << "Received signal sendToThread, checking parameters now, in function " << __FUNCTION__;
#endif
    m_camera_index = capturePara[0];
    m_capture_width = capturePara[1];
    m_capture_height = capturePara[2];

    QString m_infer_device = inferPara[0];
    QString m_yolo_network_dir = inferPara[1];
    QString m_yolo_model_size = inferPara[2];


    QStringList switchList;
    switchList << "nano" << "tiny" << "small" << "medium" << "large";
    // got to check the file
    // I hate if... so switch case is used.
    if(m_infer_device == "Intel CPU"){
        switch(switchList.indexOf(m_yolo_model_size)){
            case 1:
                m_full_network_path = m_yolo_network_dir + "/" + "yolox_tiny.xml";
#ifdef _DEBUG
                qDebug() << "yolox tiny file path: " << m_full_network_path;
#endif
                break;
            case 2:
                m_full_network_path = m_yolo_network_dir + "/" + "yolox_s.xml";
#ifdef _DEBUG
                qDebug() << "yolox small file path: " << m_full_network_path;
#endif
                break;
            case 3:
                m_full_network_path = m_yolo_network_dir + "/" + "yolox_m.xml";
#ifdef _DEBUG
                qDebug() << "yolox medium file path: " << m_full_network_path;
#endif
                break;
            case 4:
                m_full_network_path = m_yolo_network_dir + "/" + "yolox_l.xml";
#ifdef _DEBUG
                qDebug() << "yolox large file path: " << m_full_network_path;
#endif
                break;
            default:
                m_full_network_path = m_yolo_network_dir + "/" + "yolox_nano.xml";
#ifdef _DEBUG
                qDebug() << "default yolox nano file path: " << m_full_network_path;
#endif
                break;

             // check if the file exsits...
        }
        QFileInfo fileInfo(m_full_network_path);
        if(!fileInfo.exists()){
            m_yolo_running = false;
#ifdef _DEBUG
            QString m_error_infer_device = "Error, unable to locate yolo network file.\n";
            qDebug() << m_error_infer_device;
#endif
            runtime_error = "Error, unable to locate yolo network file.";
            emit sendInferDeviceError(runtime_error);
            return ;
        } else {
            // If nothing happened, our camera and engine will init here...
            infer->initEngine(m_full_network_path.toStdString(), "CPU");
            emit sendInferDeviceSuccess();
        }


    } else {
        // TODO: add yolov5 engine
    }

}




void ImageProcess::changeYoloDetectStatus(){
    m_yolo_running = !m_yolo_running;
}


