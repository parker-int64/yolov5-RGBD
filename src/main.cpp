/**
 * @file main.cpp
 * @author parker
 * @brief qt main function
 * @version 0.1
 * @date 2022-03-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include "imageprocess.h"
#include "ocvimageprovider.h"
#include "threadcontroller.h"
#include "utility.h"


int main(int argc, char *argv[]){

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);  // Solve the problem of high DPI blurry
#endif

    /**
      * In order to use QtCharts,
      * you'll have to change 'QGuiApplication app(argc, argv)'
      * to 'QApplication app(argc, argv)'
      * also you have to change the header '#include <QGuiApplication>' to '#include <QApplication>'
      * since QtCharts was based on Widgets
    */
    QApplication app(argc, argv);
    qRegisterMetaType<QVecInt>("QVecInt");
    // Register some classes in qml
//    qmlRegisterType<Utility>("Qt.Utility", 1, 0, "Utility");

    // Initialize utility for some additional pasering, then transfet to ThreadController
    Utility *utility = new Utility();

    // Initialize the ImageProvider, and transfer to the ThreadController
    OCVImageProvider *ocvImageProvider = new OCVImageProvider();

    // Initialize statusMoinitor for monitoring usage of some hardwares, also pass to ThreadController
    StatusMonitor *statusMonitor = new StatusMonitor();

    // Initialize the thread to do our work
    ThreadController *controller = new ThreadController(nullptr, ocvImageProvider, statusMonitor, utility);



    // Instantiate the QML engine
    QQmlApplicationEngine engine;

    // QML entrance
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));

    // Connect the engine and the app
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    // There is another way of registering Qt Object in qml
    engine.rootContext()->setContextProperty("controller", controller);
    engine.rootContext()->setContextProperty("statusMonitor", statusMonitor);
    engine.rootContext()->setContextProperty("liveImageProvider",ocvImageProvider);
    engine.rootContext()->setContextProperty("utility", utility);
    engine.addImageProvider(QStringLiteral("live"), ocvImageProvider);

    // Load the url
    engine.load(url);

#ifdef _DEBUG
    qDebug() << __FUNCTION__ << "running on: " << QThread::currentThreadId();
#endif
    return app.exec();
}

