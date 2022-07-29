/**
 * @file statusmonitor.cpp
 * @author parker
 * @brief this statusmonitor is for threading
 * @version 0.1
 * @date 2022-03-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "statusmonitor.h"

/**
 * @Func StatusMonitor
 * @brief Construct a new StatusMonitor object
 * 
 */
StatusMonitor::StatusMonitor(QObject *parent) :
    QObject(parent){

    m_monitor_running = true;
    m_cpu_usage_thread = 0;
    m_mem_usage_thread = 0;
    m_nvidia_gpu_usage_thread = 0;
    m_cl_info_path = "./clinfo.exe";
    m_powershell_script_path = "../script/nvidia_gpu_info.ps1";
}


/**
 * @brief Destroy the StatusMonitor object
 * 
 */
StatusMonitor::~StatusMonitor(){

}


/**
 * @brief this is where you run your heavy load function
 *
 */
void StatusMonitor::monitorProcess(){
    while(m_monitor_running){
        m_mem_usage_thread = getMemUsageThread();
        m_nvidia_gpu_usage_thread = getNvidiaGpuUsageThread(m_powershell_script_path);
        m_cpu_usage_thread = getCpuUsageThread();

        emit resultReady(m_cpu_usage_thread, m_mem_usage_thread, m_nvidia_gpu_usage_thread);
        if(!m_monitor_running) break;
    }
}   

__int64 StatusMonitor::Filetime2Int64(const FILETIME* ftime){
    LARGE_INTEGER li;
    li.LowPart = ftime->dwLowDateTime;
    li.HighPart = ftime->dwHighDateTime;
    return li.QuadPart;
}

__int64 StatusMonitor::CompareFileTime(FILETIME previousTime, FILETIME currentTime){
    return this->Filetime2Int64(&currentTime) - this->Filetime2Int64(&previousTime);
}

// Not so accurate
const int StatusMonitor::getCpuUsageThread(){
    HANDLE hEvent;
    bool res;
    static FILETIME preIdleTime;
    static FILETIME preKernelTime;
    static FILETIME preUserTime;
 
    FILETIME idleTime;
    FILETIME kernelTime;
    FILETIME userTime;
 
    res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
 
    preIdleTime = idleTime;
    preKernelTime = kernelTime;
    preUserTime = userTime;
 
    hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
 
    WaitForSingleObject(hEvent, 200);    // Wait for 200 ms
 
    res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
 
    int idle = CompareFileTime(preIdleTime, idleTime);
    int kernel = CompareFileTime(preKernelTime, kernelTime);
    int user = CompareFileTime(preUserTime, userTime);
 
    auto nCpuRate = (int)ceil(100.0 * (kernel + user - idle) / (kernel + user));
 
    return nCpuRate;
}

const int StatusMonitor::getMemUsageThread(){
    memsStat.dwLength = sizeof(memsStat);
    if(!GlobalMemoryStatusEx(&memsStat)){
        return -1;
    }
    double m_MemFree = memsStat.ullAvailPhys / ( 1024.0*1024.0 );
    double m_MemTotal = memsStat.ullTotalPhys / ( 1024.0*1024.0 );
    double m_MemUsed= m_MemTotal- m_MemFree;


    int usage = m_MemUsed / m_MemTotal * 100;
    return usage;
}

const int StatusMonitor::getNvidiaGpuUsageThread(const QString powershellScriptPath){

    QStringList parameters;
    QProcess process;

    parameters.append(powershellScriptPath);
    parameters.append("-ExecutionPolicy");
    parameters.append("Bypass");

    process.start("powershell", parameters); // something is wrong with cmd.exe, so running powershell script instead
    process.waitForStarted();
    process.waitForFinished();

    QString cmdResult = QString::fromLocal8Bit(process.readAllStandardOutput());
    QStringList list = cmdResult.split("\r\n");

    // list[0] gpu usage, list[1] gpu memory used / total
    return list[0].toInt();
}


void StatusMonitor::endMonitorProcess(){
    QMutexLocker locker(&mutex);
    m_monitor_running = !m_monitor_running;
}


/**
 * @brief Get the name of local machine
 * 
 * @return const QString 
 */
const QString StatusMonitor::localMachineName(){
    m_localMachineName = QHostInfo::localHostName();
    return m_localMachineName;
}



/**
 * @brief get cpu type
 * 
 * @return const QString& 
 */
const QString StatusMonitor::cpuType(){
    QSettings *CPU = new QSettings("HKEY_LOCAL_MACHINE\\HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",QSettings::NativeFormat);
    m_cpuDescribe = CPU->value("ProcessorNameString").toString();
    delete CPU;
 
    return m_cpuDescribe;
}



/**
 * @brief get the video / graphic card vendor and info
 * 
 * @return const QString
 */

const QString StatusMonitor::intelGPU(){
    QString IntelCard;
    QSettings *DCard = new QSettings("HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\Services\\igfx\\Video",QSettings::NativeFormat);
    QString type = DCard->value("DeviceDesc").toString();
    if(!type.isEmpty()){
        QStringList list2 = type.split(";");
        IntelCard = list2[1];
    }

    delete DCard;
    return IntelCard;
}


const QString StatusMonitor::nvidiaGPU(){
    QString nvidiaCard;
    QSettings *DCard = new QSettings("HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\Services\\nvlddmkm\\Video",QSettings::NativeFormat);
    QString type = DCard->value("DeviceDesc").toString();
    if(!type.isEmpty()){
        QStringList list2 = type.split(";");
        nvidiaCard = list2[1];
    }

    delete DCard;
    return nvidiaCard;
}

/**
 * @brief return the amount of total memory available
 * 
 * @return const QString& 
 */
const QString StatusMonitor::memory(){
    const unsigned long long MB = 1048576;
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof (statex);
    GlobalMemoryStatusEx(&statex);

    m_totalMem = statex.ullTotalPhys / MB;

    // The previous m_total is integer, we divided 1024 to get GB
    float m_menGB = m_totalMem / 1024 + 1;
    m_memDescribe = QString("%1").arg(QString::asprintf("%.1f", m_menGB));

    return m_memDescribe;
}

/**
 * @brief return the info of OS
 * 
 * @return const QString& 
 */
const QString StatusMonitor::osVersion(){
    typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
    LPFN_ISWOW64PROCESS fnIsWow64Process;
    BOOL bIsWow64 = FALSE;
    fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress( GetModuleHandle(TEXT("kernel32")),"IsWow64Process");
    if (NULL != fnIsWow64Process)
    {
        fnIsWow64Process(GetCurrentProcess(),&bIsWow64);
    }
    QString sysBit = "unknown";
    if(bIsWow64)
        sysBit = "64 Bit";
    else
        sysBit = "32 Bit";
 
    m_osDescirbe = QSysInfo::prettyProductName() + " " + sysBit;
    return m_osDescirbe;
}



const QString StatusMonitor::intelGpuInfo() {
    QProcess process;
    process.start(m_cl_info_path, QStringList() << "-c" << "--human");
    process.waitForStarted();
    process.waitForFinished();
    m_intel_gpu_opencl_info = QString::fromLocal8Bit(process.readAllStandardOutput());
    return m_intel_gpu_opencl_info;
}




