/**
 * @file statusmonitor.h
 * @author parker
 * @brief 
 * @version 0.1
 * @date 2022-03-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef STATUSMONITOR_H
#define STATUSMONITOR_H

#include <QObject>
#include <QThread>
#include <QMutexLocker>
#include <QProcess>
#include <QSettings>
#include <QHostInfo>
#include <windows.h>
#ifdef _DEBUG
    #include <QDebug>
#endif
class StatusMonitor : public QObject {
    Q_OBJECT

public:
    explicit StatusMonitor(QObject *parent = nullptr);
    ~StatusMonitor();
    // Get system infomation
    Q_INVOKABLE const QString localMachineName();
    Q_INVOKABLE const QString cpuType();
    Q_INVOKABLE const QString intelGPU();
    Q_INVOKABLE const QString nvidiaGPU();
//  Q_INVOKABLE const QString AMDGPU();
    Q_INVOKABLE const QString memory();
    Q_INVOKABLE const QString osVersion();
    Q_INVOKABLE const QString intelGpuInfo(); // \clinfo.exe -d 1:0


    const int getCpuUsageThread();
    const int getMemUsageThread();
    const int getNvidiaGpuUsageThread(const QString powershellScriptPath);



signals:
    void resultReady(const int cpuUsage, const int memUsage, const int nvidiaGpuUsage);


public slots:
    void monitorProcess();

    void endMonitorProcess();




protected:
    __int64 Filetime2Int64(const FILETIME* ftime);
    __int64 CompareFileTime(FILETIME previousTime, FILETIME currentTime);

private:

    QString m_localMachineName;
    QString m_cpuDescribe;
    unsigned long long m_totalMem;
    QString m_memDescribe;
    QString m_osDescirbe;

    int m_cpu_usage_thread;
    int m_mem_usage_thread;
    int m_nvidia_gpu_usage_thread;

    bool m_monitor_running;

    QString m_cl_info_path;

    QString m_intel_gpu_opencl_info;

    QMutex mutex;
    QString m_powershell_script_path;

    MEMORYSTATUSEX memsStat;

};



#endif // STATUSMONITOR_H
