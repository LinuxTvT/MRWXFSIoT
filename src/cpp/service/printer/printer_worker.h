#ifndef PRINTERWORKER_H
#define PRINTERWORKER_H

#include "device/device_worker.h"
#include "qjsonvalue.h"
#include "service/printer/bmp_printer.h"
#include "service/printer/form/xfs_field.h"
#include <QObject>

class PrinterService;

class PrinterWorker : public DeviceWorker
{
    Q_OBJECT
public:
    explicit PrinterWorker(const QString &strName, const QString &strFileConfig, PrinterService *pParent);
    virtual ~PrinterWorker();

public:
    virtual bool loadConfig(const QJsonObject &config) override;
    void printerStatus(QJsonObject &joPayload);
    bool checkFieldValue(const XFSField *pField, QString &strValue);
    virtual void printField2BMPPrinter(const XFSField *pField, //
                                       const QJsonObject &joFieldsPayload, //
                                       BMPPrinter *pBMPPrinter, //
                                       bool isFollows);

private:
    QJsonObject m_joPrinterStatus;
    QJsonObject m_joPaperStatus;
};

#endif // PRINTERWORKER_H
