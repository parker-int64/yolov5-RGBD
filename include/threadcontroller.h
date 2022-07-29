/**
 * @file threadcontroller.h
 * @author parker
 * @brief 
 * @version 0.1
 * @date 2022-03-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef THREADCONTROLLER_H
#define THREADCONTROLLER_H

#include <QObject>
#include "statusmonitor.h"
#include "imageprocess.h"
#include "ocvimageprovider.h"
#include "utility.h"


class ThreadController : public QObject
{

    Q_OBJECT
    Q_PROPERTY(int cpuUsage READ getCpuUsage WRITE setCpuUsage NOTIFY cpuUsageChanged);

    Q_PROPERTY(int memUsage READ getMemUsage WRITE setMemUsage NOTIFY memUsageChanged);

    Q_PROPERTY(int nvidiaGpuUsage READ getNvidiaGpuUsage WRITE setNvidiaGpuUsage NOTIFY nvidiaGpuUsageChanged);

    QThread monitorThread;

    QThread readingThread;

    

public:
    explicit ThreadController(QObject *parent = nullptr, OCVImageProvider *imageProvider = nullptr,
                              StatusMonitor *statusMonitor = nullptr, Utility *utility = nullptr);

    ~ThreadController();

    Q_INVOKABLE int getCpuUsage() const;

    Q_INVOKABLE int getMemUsage() const;

    Q_INVOKABLE int getNvidiaGpuUsage() const;


signals:

    void startMonitorProcess();

    void operateImageThread(const int status);

    void imageProcessExit();

    void monitorThreadExit();

    void cpuUsageChanged(const int cpuUsage);

    void memUsageChanged(const int memUsage);

    void nvidiaGpuUsageChanged(const int nvidiaGpuUsage);

    void sendYoloStart();

public slots:

    void monitorThreadFinished();

    void imageThreadStart();

    void imageThreadFinished();

    void handleResult(const int cpuUsage, const int memUsage, const int nvidiaGpuUsage);

    void setCpuUsage(const int cpuUsage);

    void setMemUsage(const int memUsage);

    void setNvidiaGpuUsage(const int nvidiaGpuUsage);

    void startYoloDetect();

private:
    ImageProcess *imageProcess;

    int m_cpu_usage;

    int m_mem_usage;

    int m_nvidia_gpu_usage;


};

#endif // THREADCONTROLLER_H
