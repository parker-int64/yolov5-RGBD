/**
 * @file threadcontroller.cpp
 * @author parker
 * @brief so typically, this controller is like a crossroad,
 *        many singals and slot funtions are mainly connected here
 * @version 0.1
 * @date 2022-03-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "threadcontroller.h"

ThreadController::ThreadController(QObject *parent, OCVImageProvider *imageProvider,
                                   StatusMonitor *statusMonitor, Utility *utility) :
    QObject(parent)
{   
    // Only reading intel opencl info is in main thread, because we don't use it as primary gpu



    imageProcess = new ImageProcess();

    statusMonitor = new StatusMonitor();

    statusMonitor->moveToThread(&monitorThread);

    imageProcess->moveToThread(&readingThread);

    connect(this, SIGNAL(startMonitorProcess()), statusMonitor, SLOT(monitorProcess()));
    connect(this, SIGNAL(monitorThreadExit()), statusMonitor, SLOT(endMonitorProcess()), Qt::ConnectionType::DirectConnection); // have to use 'Qt::ConnectionType::DirectConnection', otherwise connect wont work
    connect(&monitorThread, &QThread::finished, statusMonitor, &QObject::deleteLater);
    connect(statusMonitor, &StatusMonitor::resultReady, this, &ThreadController::handleResult);

    connect(this, SIGNAL(operateImageThread(int)), imageProcess, SLOT(readFrame()));
    connect(this, SIGNAL(imageProcessExit()), imageProcess, SLOT(endCapture()), Qt::ConnectionType::DirectConnection);
    connect(&readingThread, &QThread::finished, imageProcess, &QObject::deleteLater);

    connect(imageProcess, SIGNAL(sendImage(QImage)), imageProvider, SLOT(updateImage(QImage)));

    // check the parameters...
    connect(utility, &Utility::sendToThread, imageProcess, &ImageProcess::checkInferParameter, Qt::ConnectionType::DirectConnection);

    // start yolo
    connect(this, &ThreadController::sendYoloStart, imageProcess, &ImageProcess::changeYoloDetectStatus, Qt::ConnectionType::DirectConnection);

    // TODO: send error message when init engine
    // sender: imageProcess
    // signal: sendRuntimeError
    // receiver: utility
    // slot: handleError
    // handleError will write error message
    // In GUI, just call utility.getEngineStatus() to receive error message
    connect(imageProcess, &ImageProcess::sendCameraError, utility, &Utility::handleRuntimeError);
    connect(imageProcess, &ImageProcess::sendInferDeviceError, utility, &Utility::handleRuntimeError);

    // sender: imageProcess, signal: sendInferDeviceSuccess
    // receiver: utility, slot: handleRuntimeSuccess
    connect(imageProcess, &ImageProcess::sendInferDeviceSuccess, utility, &Utility::handleRuntimeSuccess);
    monitorThread.start();
    emit startMonitorProcess();
}


ThreadController::~ThreadController(){
    monitorThread.quit();
    monitorThread.wait();

    readingThread.quit();
    readingThread.wait();
}



void ThreadController::imageThreadStart(){
    readingThread.start();
    emit operateImageThread(0);
}

void ThreadController::monitorThreadFinished(){
#ifdef _DEBUG
    qDebug() << "Called " << __FUNCTION__ << ", exting thread... ";
#endif
    emit monitorThreadExit();
}

void ThreadController::imageThreadFinished(){
#ifdef _DEBUG
    qDebug() << "Called " << __FUNCTION__ << ", exting thread... ";
#endif
    emit imageProcessExit();
}


void ThreadController::handleResult(const int cpuUsage, const int memUsage, const int nvidiaGpuUsage){
    setMemUsage(memUsage);
    setNvidiaGpuUsage(nvidiaGpuUsage);
    setCpuUsage(cpuUsage);
}

int ThreadController::getCpuUsage() const{
    return m_cpu_usage;
}

void ThreadController::setCpuUsage(const int cpuUsage){
    if(cpuUsage != m_cpu_usage){
        m_cpu_usage = cpuUsage;
        emit cpuUsageChanged(m_cpu_usage);
    }
}


int ThreadController::getMemUsage() const{
    return m_mem_usage;
}


void ThreadController::setMemUsage(const int memUsage){
    if(memUsage != m_mem_usage){
        m_mem_usage = memUsage;
        emit memUsageChanged(m_mem_usage);
    }
}

int ThreadController::getNvidiaGpuUsage() const{
    return m_nvidia_gpu_usage;
}

void ThreadController::setNvidiaGpuUsage(const int nvidiaGpuUsage){
    if(nvidiaGpuUsage != m_nvidia_gpu_usage){
        m_nvidia_gpu_usage = nvidiaGpuUsage;
        emit nvidiaGpuUsageChanged(m_nvidia_gpu_usage);
    }
}

void ThreadController::startYoloDetect() {
    emit sendYoloStart();
}

