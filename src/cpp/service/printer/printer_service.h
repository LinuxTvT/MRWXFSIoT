#ifndef XFS_PRINTERSERVICE_H
#define XFS_PRINTERSERVICE_H

#include "service/abstract_service.h"
#include "service/printer/abstract_printer_service.h"
#include "service/printer/form/xfs_field.h"
#include "service/printer/form/xfs_form.h"
#include "service/printer/form/xfs_form_repository.h"
#include "service/xfs_iot/xfs_iot_service.h"
#include <QJsonArray>
#include <QObject>

class PrinterService : public AbstractPrinterService
{
    Q_OBJECT
    Q_PROPERTY(QString formDir MEMBER m_strFormDir)
public:
    inline static const QString RECEIPT_PRINTER_SERVICE_NAME = QStringLiteral("ReceiptPrinter");
    Q_INVOKABLE explicit PrinterService(const QString &strFileConfig);
    virtual ~PrinterService() { }

    inline const XFSFormRepository *formRepository() const { return m_pXFSFormRepository; }

protected:
    // void getCommonStatus(QJsonObject &joCommon) override;
    bool getPrinterStatus(QJsonObject &joPrinter);
    bool getPaperStatus(QJsonObject &joPaperStatus);

    void Update_Status_Printer(QJsonObject &joStatus) override;

    // Printer Interface implements
    void Printer_GetFormList(XFSIoTCommandEvent *pEvent) override;
    void Printer_GetQueryForm(XFSIoTCommandEvent *pEvent) override;
    void Printer_GetQueryField(XFSIoTCommandEvent *pEvent) override;
    void Printer_PrintForm(XFSIoTCommandEvent *pEvent) override;

    virtual bool getFromList(QJsonArray &joListFromName);

    // AbstractService interface
protected:
    virtual bool loadFormRepository();

private:
    QString m_strFormDir;
    XFSFormRepository *m_pXFSFormRepository = nullptr;
    QJsonObject m_joPrinterStatus;

    // AbstractService interface
protected:
    virtual bool init() override;

    // AbstractService interface
protected:
    virtual bool updateStatus() override;
};

#endif // XFS_PRINTERSERVICE_H
