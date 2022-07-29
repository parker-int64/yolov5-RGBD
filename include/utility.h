/**
 * @file utility.h
 * @author parker
 * @brief header file
 * @version 0.1
 * @date 2022-03-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef UTILITY_H
#define UTILITY_H

#include <QObject>
#include <QVariant>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJSValue>
#include <QFile>
#include <QDir>
#ifdef _DEBUG
    #include <QDebug>
#endif
typedef QVector<int> QVecInt;
class Utility : public QObject {

    Q_OBJECT
    Q_PROPERTY(bool engineInitStatus READ getEngineStatus WRITE setEngineStatus NOTIFY engineStatusChanged)
    Q_PROPERTY(QString errorMessage READ getErrorMessage WRITE setErrorMessage NOTIFY errorMessageChanged)

public:

    explicit Utility(QObject *parent = nullptr);
    ~Utility();

    Q_INVOKABLE bool getEngineStatus() const;
    Q_INVOKABLE QString getErrorMessage() const;
signals:
    void engineStatusChanged(const bool engineStatus);
    void errorMessageChanged(const QString errorMessage);
    void sendToThread(QVecInt capturePara, QStringList inferPara);

public slots:
    void setEngineStatus(const bool engineStatus);
    void setErrorMessage(const QString errorMessage);

    void parseJSValue(QJSValue jsValue);

    void receivedEngineStatusCorrect(QString status);
    void receivedEngineStatusFalse(QString status);

    void handleRuntimeSuccess();
    void handleRuntimeError(const QString);

private:
    bool m_engine_init_status;
    QString m_error;

};

#endif // UTILITY_H
