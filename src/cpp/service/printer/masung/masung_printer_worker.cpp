#include "masung_printer_worker.h"
#include "qdebug.h"
#include "qlibrary.h"
#include "qthread.h"
#include "service/printer/form/element/position.h"
#include "service/printer/form/xfs_form.h"
#include "service/printer/form/xfs_form_repository.h"
#include "service/printer/masung/sdk/Msprintsdk.h"
#include "service/printer/printer_service.h"
#include "xfs_iot_standard.h"

#define PRINT_TO_CONSOLE

#ifdef PRINT_TO_CONSOLE
#    define f(x) test##x
#else
#    define f(x) #    x
#endif

typedef int (*Test)(char *bInfodata);

QStringList g_strTestBuffer;
int g_iCurLine = 0;
int testPrintString(const char *strData, int iImme)
{
    if (iImme == 0) {
        qDebug().noquote() << strData;
    } else {
        g_strTestBuffer.append(strData);
    }
    return 0;
}

int testPrintFeedline(int iLine)
{
    for (int i = 0; i < iLine; i++) {
        qDebug().noquote() << "__";
    }
    return 0;
}

int testPrintChangeRow()
{
    while (g_strTestBuffer.isEmpty()) {
        qDebug().noquote() << g_strTestBuffer.takeFirst();
    }
    return 0;
}

#define RESOLVE_FUNCTION(name, type)                                                                                   \
    name = (type)m_pSDKLib->resolve(#name);                                                                            \
    if (name == nullptr) {                                                                                             \
        return false;                                                                                                  \
    }

MasungPrinterWorker::MasungPrinterWorker(const QString &strFileConfig, PrinterService *pParent)
    : DeviceWorker{ strFileConfig, pParent }
{
    this->setName("MasungPrinterWorker");
}

MasungPrinterWorker::~MasungPrinterWorker() { }

bool MasungPrinterWorker::init()
{
    AbstractDeviceWorker::init();
    log("Init device");
    SetPrintport("COM1", 115200);
    // QThread::sleep(20);
    int l_iRet = SetInit();
    if (l_iRet == 0) {
        log("Init device success");
        setDeviceStatus(ST_online);
    } else {
        log("Init device ERROR");
        setDeviceStatus(ST_hardwareError);
    }
    return true;
}

bool MasungPrinterWorker::loadSDK()
{
    log(QString("Loading SDK DLL, file name [%1.dll]").arg(m_strSDKDllFileName));
    m_pSDKLib = new QLibrary(m_strSDKDllFileName);

    if (m_pSDKLib->load()) {
        RESOLVE_FUNCTION(GetStatus, int (*)());
        RESOLVE_FUNCTION(GetSDKinformation, int (*)(char *));
        RESOLVE_FUNCTION(SetPrintport, int (*)(const char *, int));
        RESOLVE_FUNCTION(SetInit, int (*)());

        char l_acVersionInfo[128];
        if (0 == GetSDKinformation(l_acVersionInfo)) {
            m_strSDKVersion = QString{ l_acVersionInfo };
            // log(QString("Loading SDK success, SDK Version [%1]").arg(QString(l_acVersionInfo)));
        } else {
            // warn("Loading SDK success, BUT get version information ERROR");
        }
        return true;
    } else {
        // log(QString("Loading SDK DLL failed, error [%1]").arg(m_pSDKLib->errorString()));
        return false;
    }
}

bool MasungPrinterWorker::loadConfig(const QJsonObject &config)
{
    // Load configure of object
    if (SelfServiceObject::loadConfig(config)) {
        log("Load Configure success");
    } else {
        error("Load Configure ERROR");
        return false;
    }
    // Load Party SDK
    if (loadSDK()) {
        log("Load SDK success");
    } else {
        error("Load SDK ERROR");
        return false;
    }
    return true;
}

bool MasungPrinterWorker::printString(const QString &str, int iImme)
{
    QThread::sleep(1);
    return (0 == f(PrintString)(str.toUtf8().constData(), iImme));
}

bool MasungPrinterWorker::feedLine(int iLines)
{
    return (0 == f(PrintFeedline)(iLines));
}

bool MasungPrinterWorker::doCommand(XFSIoTCommandEvent *pCommandEvent)
{
    if (AbstractDeviceWorker::doCommand(pCommandEvent)) {
        return true;
    } else {
        QString l_strCommandName = pCommandEvent->commandName();
        if (l_strCommandName == "Printer.PrintForm") {
            doPrintForm(pCommandEvent);
            return true;
        } else {
            service()->notifyCompletionError(pCommandEvent, //
                                             XFSIoTStandard::JV_UNSUPPORTED_COMMAND, //
                                             QString("Device can not DO command [%1]").arg(l_strCommandName));
            return false;
        }
    }
}

void MasungPrinterWorker::doPrintForm(XFSIoTCommandEvent *pCommandEvent)
{
    m_iLineNo = 0;
    m_strCurrentLine.clear();
    QString l_strFormName = pCommandEvent->payLoad()["formName"].toString();
    PrinterService *l_pPrinterService = (PrinterService *)this->service();
    const XFSForm *l_pForm = l_pPrinterService->formRepository()->form(l_strFormName);

#ifdef PRINT_TO_CONSOLE
    int m_iFormWidth = l_pForm->size().width();
    QString m_strColNo;
    for (int i = 0; i < m_iFormWidth; i++) {
        m_strColNo.append('0' + (i % 10));
    }
    printString(m_strColNo, 0);
#endif

    const QMap<Position, XFSField *> &field = l_pForm->fieldMapByPosition();
    const QJsonObject &l_joFieldsPayload = pCommandEvent->payLoad()["fields"].toObject();
    for (auto itr = field.constBegin(); itr != field.constEnd(); itr++) {
        const QJsonValue l_jvFieldValue = l_joFieldsPayload[(*itr)->name()];
        if (l_jvFieldValue.isUndefined()) {
            if ((*itr)->fieldClass().is("required")) {
                l_pPrinterService->notifyFieldWarningEvent(pCommandEvent, l_pForm, (*itr), "required");
                continue;
            }
        } else {
            if (l_jvFieldValue.isString()) {
                QString l_strFieldValue = l_jvFieldValue.toString();
                int l_iFieldLen = (*itr)->size().width();
                if (l_strFieldValue.length() > l_iFieldLen) {
                    const StringElement &l_overflow = (*itr)->overflow();
                    if (l_overflow.is("terminate")) {
                        l_pPrinterService->notifyFieldErrorEvent(pCommandEvent, l_pForm, (*itr), "overflow");
                        return;
                    } else if (l_overflow.is("truncate")) {
                        l_strFieldValue.truncate(l_iFieldLen);
                        l_pPrinterService->notifyFieldWarningEvent(pCommandEvent, l_pForm, (*itr), "overflow");
                    } else if (l_overflow.is("bestFit")) {
                        l_strFieldValue.truncate(l_iFieldLen);
                        l_pPrinterService->notifyFieldWarningEvent(pCommandEvent, l_pForm, (*itr), "overflow");
                    } else if (l_overflow.is("overwrite")) {
                        l_pPrinterService->notifyFieldWarningEvent(pCommandEvent, l_pForm, (*itr), "overflow");
                    } else if (l_overflow.is("wordWrap")) {
                        l_pPrinterService->notifyFieldErrorEvent(pCommandEvent, l_pForm, (*itr), "notSupported");
                        return;
                    } else {
                        l_pPrinterService->notifyFieldErrorEvent(pCommandEvent, l_pForm, (*itr), "notSupported");
                        return;
                    }
                }
            } else {
                l_pPrinterService->notifyFieldWarningEvent(pCommandEvent, l_pForm, (*itr), "notSupported");
            }
        }
        print((*itr), pCommandEvent->payLoad()["fields"].toObject());
    }
}

void MasungPrinterWorker::print(XFSField *pField, const QJsonObject &joPayload)
{
    // qDebug() << "Print " << pField->name();
    int l_iFieldRow = pField->position().y();
    int l_iFieldCol = pField->position().x();
    QString l_strValue;
    if (pField->initialvalue().isEmpty()) {
        const QJsonValue l_jvFieldValue = joPayload[pField->name()];
        if (l_jvFieldValue.isUndefined()) {

        } else {
            if (l_jvFieldValue.isString()) {
                l_strValue = l_jvFieldValue.toString();
            } else {
            }
        }
        l_strValue = joPayload[pField->name()].toString();
    } else {
        l_strValue = pField->initialvalue();
    }
    if (m_iLineNo < l_iFieldRow) {
        printString(m_strCurrentLine, 0);
        feedLine(l_iFieldRow - m_iLineNo - 1);
        m_iLineNo = l_iFieldRow;
        m_strCurrentLine.clear();
    }
    if (l_iFieldCol > m_strCurrentLine.size()) {
        m_strCurrentLine.append(QString(l_iFieldCol - m_strCurrentLine.size(), ' '));
    }
    m_strCurrentLine.append(l_strValue);
}
