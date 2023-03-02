#include "service/printer/printer_service_v3x.h"
#include "XFSAPI.H"
#include "XFSPTR.H"
#include "device/v3x/device_worker_v3x.h"
#include "qjsonvalue.h"
#include "service/printer/form/xfs_field.h"
#include "service/printer/form/xfs_form.h"
#include "service/printer/masung/masung_printer_worker.h"
#include "utils/xfs_utils.h"
#include "xfs_iot_standard.h"
#include <QJsonArray>
#include <QJsonObject>
#include <winnt.h>

PrinterServiceV3x::PrinterServiceV3x(const QString &strName, //
                                     const QString &strFileConfig)
    : AbstractPrinterService{ strName, strFileConfig }
{
}

PrinterServiceV3x::~PrinterServiceV3x() { }

void PrinterServiceV3x::dumpField(LPWFSFRMFIELD lpWfsFrmField, QJsonObject &jvFields)
{
    if (lpWfsFrmField != NULL) {
        QJsonObject l_joFieldInfo;
        l_joFieldInfo["type"] = FIELD_TYPE_HASH.value(lpWfsFrmField->fwType);
        l_joFieldInfo["indexCount"] = lpWfsFrmField->wIndexCount;
        l_joFieldInfo["class"] = FIELD_CLASS_HASH.value(lpWfsFrmField->fwClass);
        l_joFieldInfo["access"] = FIELD_ACCESS_HASH.value(lpWfsFrmField->fwAccess);
        l_joFieldInfo["overflow"] = FIELD_OVERFLOW_HASH.value(lpWfsFrmField->fwOverflow);
        l_joFieldInfo["initialValue"] = QString(lpWfsFrmField->lpszInitialValue);
        jvFields[lpWfsFrmField->lpszFieldName] = l_joFieldInfo;
    }
}

void PrinterServiceV3x::Printer_GetFormList(XFSIoTCommandEvent *pCommandEvent)
{
    DWORD l_dwTimeout = pCommandEvent->payLoad()["timeout"].toInt();
    LPWFSRESULT l_lpWfsResult;
    HRESULT l_hResult = m_pDeviceWorkerWrap->wfsGetInfo(WFS_INF_PTR_FORM_LIST, //
                                                        NULL, //
                                                        l_dwTimeout, //
                                                        &l_lpWfsResult);
    if (l_hResult == WFS_SUCCESS) {
        LPSTR l_lpszFormList = (LPSTR)l_lpWfsResult->lpBuffer;
        QJsonObject l_joPayload;
        QJsonArray l_jaForms;
        XfsUtils::parseLPSTR(l_lpszFormList, l_jaForms);
        l_joPayload["formList"] = l_jaForms;
        notifyCompletion(pCommandEvent, l_joPayload);
    } else {
        notifyCompletion(pCommandEvent, //
                         XFSIoTStandard::JV_UNSUPPORTED_COMMAND, //
                         QString("Do command ERROR  [%1]").arg(pCommandEvent->commandName()));
    }
    WFSFreeResult(l_lpWfsResult);
}

void PrinterServiceV3x::Printer_GetMediaList(XFSIoTCommandEvent *pCommandEvent) { }

void PrinterServiceV3x::Printer_GetQueryForm(XFSIoTCommandEvent *pCommandEvent)
{
    QString l_strFormName = pCommandEvent->payLoad()["formName"].toString();
    DWORD l_dwTimeout = pCommandEvent->payLoad()["timeout"].toInt();
    LPWFSRESULT l_lpWfsResult;
    HRESULT l_hResult = m_pDeviceWorkerWrap->wfsGetInfo(WFS_INF_PTR_QUERY_FORM, //
                                                        (LPVOID)l_strFormName.toUtf8().constData(), //
                                                        l_dwTimeout, //
                                                        &l_lpWfsResult);
    if (l_hResult == WFS_SUCCESS) {
        LPWFSFRMHEADER l_lpWfsForm = (LPWFSFRMHEADER)l_lpWfsResult->lpBuffer;
        QJsonObject l_joPayload;
        QJsonArray l_jaForms;
        XfsUtils::parseLPSTR(l_lpWfsForm->lpszFields, l_jaForms);
        l_joPayload["formList"] = l_jaForms;
        notifyCompletion(pCommandEvent, l_joPayload);
    } else {
        notifyCompletion(pCommandEvent, //
                         XFSIoTStandard::JV_UNSUPPORTED_COMMAND, //
                         QString("Do command ERROR  [%1]").arg(pCommandEvent->commandName()));
    }
    WFSFreeResult(l_lpWfsResult);
}

void PrinterServiceV3x::Printer_GetQueryField(XFSIoTCommandEvent *pCommandEvent)
{
    WFSPTRQUERYFIELD l_wfsPtrQueryField;
    QString l_strFormName = pCommandEvent->payLoad()["formName"].toString();
    QJsonValue l_jvFieldName = pCommandEvent->payLoad()["fieldName"];

    l_wfsPtrQueryField.lpszFormName = SelfServiceObject::stringDup(l_strFormName);
    if (l_jvFieldName.isUndefined()) {
        l_wfsPtrQueryField.lpszFieldName = NULL;
    } else {
        l_wfsPtrQueryField.lpszFieldName = SelfServiceObject::stringDup(l_jvFieldName.toString());
    }

    QJsonObject l_joPayload;
    QJsonObject l_joFields;
    DWORD l_dwTimeout = pCommandEvent->payLoad()["timeout"].toInt();
    LPWFSRESULT l_lpWfsResult;
    HRESULT l_hResult = m_pDeviceWorkerWrap->wfsGetInfo(WFS_INF_PTR_QUERY_FIELD, //
                                                        (LPVOID)&l_wfsPtrQueryField, //
                                                        l_dwTimeout, //
                                                        &l_lpWfsResult);
    if (l_hResult == WFS_SUCCESS) {
        LPWFSFRMFIELD *l_ppFields = (LPWFSFRMFIELD *)l_lpWfsResult->lpBuffer;
        while ((*l_ppFields) != NULL) {
            dumpField(*l_ppFields, l_joFields);
            l_ppFields++;
        }
        l_joPayload["fields"] = l_joFields;
        notifyCompletion(pCommandEvent, l_joPayload);
    } else {
        notifyCompletion(pCommandEvent, //
                         XFSIoTStandard::JV_UNSUPPORTED_COMMAND, //
                         QString("Do command ERROR  [%1]").arg(pCommandEvent->commandName()));
    }
    WFSFreeResult(l_lpWfsResult);
}

void PrinterServiceV3x::Printer_PrintForm(XFSIoTCommandEvent *pEvent)
{
    WFSPTRPRINTFORM l_wfsPtrPrintForm;
    REQUESTID l_requestId;
    wchar_t l_doubleNull[2] = { 0, 0 };
    char meida[] = "ReceiptMedia";

    QString l_strFormName = pEvent->payLoad()["formName"].toString();
    l_wfsPtrPrintForm.lpszFormName = XfsUtils::stringDup(l_strFormName);
    l_wfsPtrPrintForm.lpszMediaName = meida;
    l_wfsPtrPrintForm.wAlignment = WFS_PTR_ALNUSEFORMDEFN;
    l_wfsPtrPrintForm.wOffsetX = 0;
    l_wfsPtrPrintForm.wOffsetY = 0;
    l_wfsPtrPrintForm.wResolution = WFS_PTR_RESLOW;
    l_wfsPtrPrintForm.dwMediaControl = WFS_PTR_CTRLEJECT;
    l_wfsPtrPrintForm.lpszUNICODEFields = l_doubleNull;
    l_wfsPtrPrintForm.wPaperSource = 0;
    QJsonObject l_joFields = pEvent->payLoad()["fields"].toObject();
    QByteArray l_baFields;
    for (auto itr = l_joFields.constBegin(); itr != l_joFields.constEnd(); itr++) {
        l_baFields.append(itr.key().toUtf8());
        l_baFields.append('=');
        l_baFields.append(itr.value().toString().toUtf8());
        l_baFields.append('\0');
    }
    l_baFields.append('\0');
    l_wfsPtrPrintForm.lpszFields = l_baFields.data();
    HRESULT l_hResult = m_pDeviceWorkerWrap->wfsAsyncExecute(WFS_CMD_PTR_PRINT_FORM, //
                                                             &l_wfsPtrPrintForm, //
                                                             30000, //
                                                             &l_requestId);
}

void PrinterServiceV3x::Printer_Reset(XFSIoTCommandEvent *pEvent) { }

AbstractDeviceWorker *PrinterServiceV3x::loadDeviceWorker(const QJsonValue &joProperties)
{
    m_pDeviceWorkerWrap = new DeviceWorkerV3x(joProperties["logicalName"].toString(), //
                                              joProperties["deviceClass"].toString(), //
                                              joProperties["configs"].toString(), //
                                              this);
    return m_pDeviceWorkerWrap;
}

void PrinterServiceV3x::Common_Status(XFSIoTCommandEvent *pEvent)
{
    error(QString("Unimplement command [%1]").arg(pEvent->commandName()));
}
