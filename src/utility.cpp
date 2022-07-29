/**
 * @file utility.cpp
 * @author parker
 * @brief some utilities to be used
 * @version 0.1
 * @date 2022-03-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "utility.h"


/**
 * @brief constructor
 *
*/
Utility::Utility(QObject *parent) :
    QObject(parent)
{
    m_engine_init_status = false;

}


bool Utility::getEngineStatus() const {
    return m_engine_init_status;
}

void Utility::setEngineStatus(bool engineStatus){
    if(m_engine_init_status != engineStatus){
        engineStatus = m_engine_init_status;
        emit engineStatusChanged(m_engine_init_status);
    }
}


void Utility::receivedEngineStatusCorrect(QString status){
    m_engine_init_status = true;
#ifdef _DEBUG
    qDebug() << "Engine status is correct.";
    qDebug() << status;
#endif
}

void Utility::receivedEngineStatusFalse(QString status){
    m_engine_init_status = false;
#ifdef _DEBUG
    qDebug() << "Engine init failed. The possibly reason may be: ";
    qDebug() << status;
#endif
}

void Utility::parseJSValue(QJSValue jsValue){
    // get the parameters in c++ using qjson
    QJsonObject jsonObject = jsValue.toVariant().toJsonObject();
    int cameraIndex = jsonObject.value("camera_index").toInt();
    QJsonObject resolutionObject = jsonObject.value("resolution").toObject();
    int captureWidth = resolutionObject.value("width").toInt();
    int captureHeight = resolutionObject.value("height").toInt();
    QString inferDevice = jsonObject.value("infer_device").toString();
    QString yoloNetworkDir = jsonObject.value("yolo_network_directory").toString();
    QString yoloModelSize = jsonObject.value("yolo_model_size").toString();

    // Maybe try to add more parameters in the future.
    QVector<int> capturePara;
//    std::vector<int> capturePara;
    capturePara.push_back(cameraIndex);
    capturePara.push_back(captureWidth);
    capturePara.push_back(captureHeight);

    QStringList inferPara;
    inferPara.push_back(inferDevice);
    inferPara.push_back(yoloNetworkDir);
    inferPara.push_back(yoloModelSize);

    emit sendToThread(capturePara, inferPara);

#ifdef _DEBUG
// uncomment the following debug to match the result if something went sideway
    qDebug() << "cameraIndex is: " << cameraIndex;
    qDebug() << "captureWidth is: " << captureWidth;
    qDebug() << "captureHeight is: " << captureHeight;
    qDebug() << "infer device is: " << inferDevice;
    qDebug() << "yoloNetworkDir is: " << yoloNetworkDir;
    qDebug() << "yoloModelSize is: " << yoloModelSize;
    qDebug() << "capturePara is: " << capturePara;
    qDebug() << "inferPara is: " << inferPara;
// In addition, in debug mode, a debug json file is generated.
    QString headerInfo = QString("// This json file is automatically generated for debug.\n%1 %2")
            .arg("// Generated time:")
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "\n");
    QString jsonString = QJsonDocument(jsonObject).toJson();
    qDebug() << "Received JSON string: " << jsonString;
    qDebug() << "Write JSON file to directory...";
    QFile *file = new QFile(QDir::currentPath() + "/conf/settings_debug.json");
    file->open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file->isOpen()){
        qDebug() << "QFile current path: " << QDir::currentPath() + "/conf/settings_debug.json" ;
        qDebug() << "Error: cannot open json file...";
    }
    file->write(headerInfo.toUtf8() + jsonString.toUtf8());
    file->close();
#endif

}


void Utility::handleRuntimeError(const QString error){
    m_engine_init_status = false;
    setErrorMessage(error);
    qDebug() << "In function: " << __FUNCTION__ << ", error message is: " << error;
}

QString Utility::getErrorMessage() const {
    return this->m_error;
}

void Utility::setErrorMessage(const QString errorMessage){
        this->m_error = errorMessage;
        emit errorMessageChanged(this->m_error);
    qDebug() << "In funtion " << __FUNCTION__ << "this->m_error is: " << this->m_error << ", errorMessage is " << errorMessage;
}


void Utility::handleRuntimeSuccess(){
    m_engine_init_status = true;
    setEngineStatus(m_engine_init_status);
}

/**
 * @brief deconstructor
 *
*/
Utility::~Utility(){

}


