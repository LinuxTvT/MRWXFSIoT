#ifndef XFS_PRINTERSERVICE_H
#define XFS_PRINTERSERVICE_H

#include "device/device_worker.h"
#include "service/abstract_service.h"
#include "service/printer/abstract_printer_service.h"
#include "service/printer/form/xfs_field.h"
#include "service/printer/form/xfs_form.h"
#include "service/printer/form/xfs_form_repository.h"
#include "service/printer/printer_worker.h"
#include <QJsonArray>
#include <QObject>

class PrinterService : public AbstractPrinterService
{
    Q_OBJECT
    Q_PROPERTY(QString formDir MEMBER m_strFormDir)

public:
    Q_INVOKABLE explicit PrinterService(const QString &strName, const QString &strFileConfig);
    virtual ~PrinterService() { }

    SERVICE_FUNCTION(Common, Status) override;
    SERVICE_FUNCTION(Printer, GetFormList) override;
    SERVICE_FUNCTION(Printer, GetMediaList) override;
    SERVICE_FUNCTION(Printer, GetQueryForm) override;
    SERVICE_FUNCTION(Printer, GetQueryField) override;
    SERVICE_FUNCTION(Printer, PrintForm) override;
    SERVICE_FUNCTION(Printer, Reset) override;

    const XFSForm *form(const QString &strFormName) const;

protected:
    bool getPrinterStatus(QJsonObject &joPrinter);
    bool getPaperStatus(QJsonObject &joPaperStatus);

    virtual bool getFromList(QJsonArray &joListFromName);
    virtual bool loadFormRepository();
    virtual bool checkForm(const XFSForm *pForm, XFSIoTCommandEvent *pCommandEvent);
    virtual bool init() override;
    virtual bool updateStatus() override;
    virtual AbstractDeviceWorker *loadDeviceWorker(const QJsonValue &joProperties) override;

private:
    PrinterWorker *printerWorker();

private:
    QString m_strFormDir;
    XFSFormRepository *m_pXFSFormRepository = nullptr;
    QJsonObject m_joPrinterStatus;
};

#endif // XFS_PRINTERSERVICE_H
