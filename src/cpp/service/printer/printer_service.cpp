#include "printer_service.h"
#include "qjsonvalue.h"
#include "service/printer/form/xfs_field.h"
#include "service/printer/form/xfs_form.h"
#include "service/printer/masung/masung_printer_worker.h"
#include "xfs_iot_standard.h"

PrinterService::PrinterService(const QString &strFileConfig) : AbstractPrinterService{ strFileConfig }
{
    setWorker(new MasungPrinterWorker(QString{}, this));
}

bool PrinterService::getPrinterStatus(QJsonObject &joPrinter)
{
    joPrinter["media"] = "present";
    const QJsonValue l_jvPaperStatus = this->payloadItem("Status.printer.paper");
    if (l_jvPaperStatus.isUndefined()) {
        return false;
    } else {
        QJsonObject l_joPaperStatus = l_jvPaperStatus.toObject();
        if (getPaperStatus(l_joPaperStatus)) {
            joPrinter["paper"] = l_joPaperStatus;
            return true;
        } else {
            return false;
        }
    }
}

bool PrinterService::getPaperStatus(QJsonObject &joPaperStatus)
{
    joPaperStatus["park"] = "jammed";
    return true;
}

void PrinterService::Update_Status_Printer(QJsonObject &joStatus)
{
    qDebug() << "joStatus = m_joPrinterStatus";
}

void PrinterService::Printer_GetFormList(XFSIoTCommandEvent *pEvent)
{
    QStringList listDir = QDir(this->m_strFormDir).entryList();
    QJsonObject l_joPayload;
    QJsonArray l_jaForms;
    m_pXFSFormRepository->dumpFormsName2Json(l_jaForms);
    l_joPayload["formList"] = l_jaForms;
    notifyCompletion(pEvent, l_joPayload);
}

void PrinterService::Printer_GetQueryForm(XFSIoTCommandEvent *pEvent)
{
    QString l_strFormName = pEvent->payLoad()["formName"].toString();
    QJsonObject l_joPayload;
    QJsonArray l_jaFields;
    const XFSForm *l_pForm = m_pXFSFormRepository->form(l_strFormName);
    if (l_pForm == nullptr) {
        QString l_strError = QString("Form [%1] can't found").arg(l_strFormName);
        error(l_strError);
        notifyCompletionError(pEvent, "commandErrorCode", l_strError);
    } else {
        XFSIoTStandard::buildPayloadCompletion(l_joPayload);
        l_pForm->dumpFieldsName2Json(l_jaFields);
        l_pForm->dump2Json(l_joPayload);
        l_joPayload["fields"] = l_jaFields;
        notifyCompletion(pEvent, l_joPayload);
    }
}

void PrinterService::Printer_GetQueryField(XFSIoTCommandEvent *pEvent)
{
    QString l_strFormName = pEvent->payLoad()["formName"].toString();
    QJsonValue l_jvFieldName = pEvent->payLoad()["fieldName"];
    QJsonObject l_joPayload;
    QJsonObject l_joFields;
    const XFSForm *l_pForm = m_pXFSFormRepository->form(l_strFormName);
    if (l_jvFieldName.isUndefined()) {
        l_pForm->dumpFileds2Json(l_joFields);
    } else {
        const XFSField *l_pField = l_pForm->field(l_jvFieldName.toString());
        if (l_pField == nullptr) {
            QString l_strError = QString("Field [%1] can't found").arg(l_jvFieldName.toString());
            error(l_strError);
            notifyCompletionError(pEvent, "commandErrorCode", l_strError);
            return;
        } else {
            QJsonObject l_joField;
            l_pField->dump2Json(l_joField);
            l_joFields[l_pField->name()] = l_joField;
        }
    }
    l_joPayload["fields"] = l_joFields;
    notifyCompletion(pEvent, l_joPayload);
}

void PrinterService::Printer_PrintForm(XFSIoTCommandEvent *pEvent)
{
    QString l_strFormName = pEvent->payLoad()["formName"].toString();

    if (m_pXFSFormRepository->form(l_strFormName) == nullptr) {
        notifyCompletionError(pEvent, "commandErrorCode", QString("Form [%1] can't found").arg(l_strFormName));
    } else {
        deviceWorker()->doCommand(new XFSIoTCommandEvent(pEvent));
        log(QString("Print From [%1] is queued to device worker").arg(l_strFormName));
    }
}

bool PrinterService::getFromList(QJsonArray &joListFromName)
{
    m_pXFSFormRepository->dumpFormsName2Json(joListFromName);
    return true;
}

bool PrinterService::loadFormRepository()
{
    m_pXFSFormRepository = new XFSFormRepository(this);
    return m_pXFSFormRepository->loadForms("C:/xfs/Form/RPTR/ReceiptFormEN.wfm");
}

bool PrinterService::init()
{
    if (AbstractService::init()) {
        return loadFormRepository();
    } else {
        return false;
    }
}

bool PrinterService::updateStatus()
{
    AbstractPrinterService::updateStatus();
    m_joPrinterStatus["media"] = "present";
    const QJsonValue l_jvPaperStatus = this->payloadItem("Status.printer.paper");
    if (l_jvPaperStatus.isUndefined()) {
        return false;
    } else {
        QJsonObject l_joPaperStatus = l_jvPaperStatus.toObject();
        if (getPaperStatus(l_joPaperStatus)) {
            m_joPrinterStatus["paper"] = l_joPaperStatus;
            return true;
        } else {
            return false;
        }
    }
    m_joPrinterStatus["test"] = "Test";
    return true;
}
