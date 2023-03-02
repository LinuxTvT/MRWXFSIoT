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

    // SelfServiceObject interface
public:
    virtual bool loadConfig(const QJsonObject &config) override;
    void printerStatus(QJsonObject &joPayload);
    //    QString getFieldValue(XFSField *pField, //
    //                          const QJsonValue &jvValue, //
    //                          bool *pIsSuccess, //
    //                          QString &strFailure);
    //    bool getFieldValues(XFSField *pField, //
    //                        const QJsonObject &fields, //
    //                        QMap<int, QString> &strListValues, //
    //                        QString &strFailure);
    bool checkFieldValue(const XFSField *pField, QString &strValue);
    virtual void printField2BMPPrinter(const XFSField *pField, //
                                       const QJsonObject &joFieldsPayload, //
                                       BMPPrinter *pBMPPrinter, //
                                       bool isFollows);
    // virtual void printAll2BMPPrinter(XFSField *pField, const QJsonObject &joFieldsPayload, BMPPrinter *pBMPPrinter);

private:
    QJsonObject m_joPrinterStatus;
    QJsonObject m_joPaperStatus;
};

#endif // PRINTERWORKER_H
