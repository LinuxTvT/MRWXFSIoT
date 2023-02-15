#ifndef MASUNG_PRINTER_WORKER_H
#define MASUNG_PRINTER_WORKER_H

#include "device/abstract_device_worker.h"
#include "device/device_worker.h"
#include "qlibrary.h"
#include "runable_object.h"
#include "service/printer/form/xfs_field.h"
#include <QObject>

class PrinterService;

class MasungPrinterWorker : public DeviceWorker
{
    Q_OBJECT
public:
    explicit MasungPrinterWorker(const QString &strFileConfig, PrinterService *pParent);
    virtual ~MasungPrinterWorker();

protected:
    virtual bool init() override;

private:
    bool loadSDK();
    QLibrary *m_pSDKLib;
    QString m_strFileConfig;

    // SelfServiceObject interface
public:
    virtual bool loadConfig(const QJsonObject &config) override;

private:
    QString m_strSDKDllFileName = QStringLiteral("Msprintsdk");
    QString m_strSDKVersion;

    bool printString(const QString &str, int iImme);
    bool feedLine(int iLines);
    QString m_strCurrentLine;
    int m_iLineNo = 0;

    // AbstractDeviceWorker interface
protected:
    virtual bool doCommand(XFSIoTCommandEvent *pCommandEvent) override;
    void doPrintForm(XFSIoTCommandEvent *pCommandEvent);

    virtual void print(XFSField *pField, const QJsonObject &joPayload);
};

#endif // MASUNG_PRINTER_WORKER_H
