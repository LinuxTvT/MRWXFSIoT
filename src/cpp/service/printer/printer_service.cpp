#include "printer_service.h"
#include "qjsonobject.h"
#include "qjsonvalue.h"
#include "service/printer/form/xfs_field.h"
#include "service/printer/form/xfs_form.h"
#include "service/printer/masung/masung_printer_worker.h"
#include "xfs_iot_standard.h"

PrinterService::PrinterService(const QString &strName, //
                               const QString &strFileConfig)
    : AbstractPrinterService{ strName, //
                              strFileConfig }
{
    log(QString("Created PrinterService object, name [%1], configs file [%2]") //
                .arg(strName, strFileConfig));
}

PrinterService::~PrinterService()
{
    log(QString("Destroy PrinterService object, name [%1]").arg(name()));
}

const XFSForm *PrinterService::form(const QString &strFormName) const
{
    return m_pXFSFormRepository->form(strFormName);
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

void PrinterService::Printer_GetFormList(XFSIoTCommandEvent *pEvent)
{
    QJsonObject l_joPayload;
    QJsonArray l_jaForms;
    m_pXFSFormRepository->dumpFormsName2Json(l_jaForms);
    l_joPayload["formList"] = l_jaForms;
    notifyCompletion(pEvent, l_joPayload);
}

void PrinterService::Printer_GetMediaList(XFSIoTCommandEvent *pEvent)
{
    QJsonObject l_joPayload;
    QJsonArray l_jaMedias;
    m_pXFSFormRepository->dumpMediasName2Json(l_jaMedias);
    l_joPayload["formList"] = l_jaMedias;
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
        notifyCompletionErrorCode(pEvent, "formNotFound", l_strError);
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
            notifyCompletionErrorCode(pEvent, //
                                      "formInvalid", //
                                      QString("Field [%1] can't found").arg(l_jvFieldName.toString()));
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
    const XFSForm *l_pForm = m_pXFSFormRepository->form(l_strFormName);

    if (l_pForm == nullptr) {
        notifyCompletionErrorCode(pEvent, //
                                  "formNotFound", //
                                  QString("Form [%1] can't found").arg(l_strFormName));
    } else {
        if (checkForm(l_pForm, pEvent)) {
            queueCommand(pEvent);
        } else {
            notifyCompletionErrorCode(pEvent, //
                                      "fieldError", //
                                      QString("Field value error"));
        }
    }
}

void PrinterService::Printer_Reset(XFSIoTCommandEvent *pCommandEvent)
{
    queueCommand(pCommandEvent);
}

bool PrinterService::getFromList(QJsonArray &joListFromName)
{
    m_pXFSFormRepository->dumpFormsName2Json(joListFromName);
    return true;
}

PrinterWorker *PrinterService::printerWorker()
{
    return (PrinterWorker *)deviceWorker();
}

bool PrinterService::loadFormRepository()
{
    m_pXFSFormRepository = new XFSFormRepository(this);
    QDir l_formDir{ m_strFormDir };
    QStringList l_strListFile = l_formDir.entryList({ "*.wfm" }, QDir::Files);
    for (auto itr = l_strListFile.constBegin(); itr != l_strListFile.constEnd(); itr++) {
        log(QString("Load forms from file [%1]").arg(*itr));
        if (m_pXFSFormRepository->loadForms(l_formDir.filePath(*itr))) {
            log(QString("Load forms from file [%1] success").arg(*itr));
        } else {
            error(QString("Load forms from file [%1] ERROR").arg(*itr));
        }
    }
    return true;
}

bool PrinterService::checkForm(const XFSForm *pForm, XFSIoTCommandEvent *pCommandEvent)
{
    const QHash<QString, XFSField *> &l_hFields = pForm->fields();
    QJsonObject l_joFields = pCommandEvent->payLoad()["fields"].toObject();

    for (auto itr = l_hFields.constBegin(); itr != l_hFields.constEnd(); itr++) {
        QString l_strFail = (*itr)->checkFieldValue(l_joFields);
        if (!l_strFail.isEmpty()) {
            notifyFieldErrorEvent(pCommandEvent, (*itr), l_strFail);
            return false;
        }
    }
    return true;
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

AbstractDeviceWorker *PrinterService::loadDeviceWorker(const QJsonValue &joProperties)
{
    return new MasungPrinterWorker(joProperties["configs"].toString(), this);
}

void PrinterService::Common_Status(XFSIoTCommandEvent *pCommandEvent)
{
    if (printerWorker()->isBusy()) {
        QJsonObject l_joPayload;
        printerWorker()->commonStatus(l_joPayload);
        printerWorker()->printerStatus(l_joPayload);
        notifyCompletion(pCommandEvent, l_joPayload);
    } else {
        queueCommand(pCommandEvent);
    }
}
