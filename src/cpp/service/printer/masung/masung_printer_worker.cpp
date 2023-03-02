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

//#define PRINT_TO_CONSOLE

#ifdef PRINT_TO_CONSOLE
#    include "masung_printer_simulator.h"
#    define MASUNG_API(x) test##x
#else
#    define MASUNG_API(x) x
#endif

#define RESOLVE_FUNCTION(name, type)                                                                                   \
    name = (type)m_pSDKLib->resolve(#name);                                                                            \
    if (name == nullptr) {                                                                                             \
        return false;                                                                                                  \
    }

MasungPrinterWorker::MasungPrinterWorker(const QString &strFileConfig, PrinterService *pParent)
    : PrinterWorker{ "MasungPrinterWorker", strFileConfig, pParent }
{
}

MasungPrinterWorker::~MasungPrinterWorker() { }

bool MasungPrinterWorker::init()
{
    AbstractDeviceWorker::init();
    setDeviceStatus(ST_noDevice);
    log("Init device");
    int l_iResult = 0;
    if (m_strConnectType == "USB") {
        log("Connect by USB");
        l_iResult = SetUsbportauto();
    }
    if (l_iResult == MS_SUCCESS) {
        setDeviceStatus(ST_offline);
        log("Connect Success");
    } else {
        error("Connect ERROR");
        return false;
    }

    initDevice();

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
        RESOLVE_FUNCTION(SetUsbportauto, int (*)());
        RESOLVE_FUNCTION(GetProductinformation, int (*)(int, char *, int));
        RESOLVE_FUNCTION(GetStatusspecial, int (*)());
        RESOLVE_FUNCTION(PrintString, int (*)(const char *, int));
        RESOLVE_FUNCTION(PrintFeedline, int (*)(int));
        RESOLVE_FUNCTION(PrintChangeRow, int (*)());
        RESOLVE_FUNCTION(PrintCutpaper, int (*)(int));
        RESOLVE_FUNCTION(PrintDiskbmpfile, int (*)(const char *));

        char l_acVersionInfo[128];
        if (0 == GetSDKinformation(l_acVersionInfo)) {
            m_strSDKVersion = QString{ l_acVersionInfo };
            log(QString("Loading SDK success, SDK Version [%1]").arg(QString(l_acVersionInfo)));
        } else {
            warn("Loading SDK success, BUT get version information ERROR");
        }
        return true;
    } else {
        log(QString("Loading SDK DLL failed, error [%1]").arg(m_pSDKLib->errorString()));
        return false;
    }
}

bool MasungPrinterWorker::loadConfig(const QJsonObject &config)
{
    // Load configure of object
    if (PrinterWorker::loadConfig(config)) {
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

bool MasungPrinterWorker::printString(const QString &str)
{
    m_iCurX = str.length();
    return (0 == MASUNG_API(PrintString)(str.toUtf8().constData(), 1));
}

bool MasungPrinterWorker::goTo(int iX, int iY)
{
    if (m_iCurY < iY) {
        feedLine(iY - m_iCurY);
        m_iCurY = iY;
    }
    if (iX > m_iCurX) {
        printString(QString(iX - m_iCurX, '@'));
        m_iCurX = iX;
    }
    return true;
}

bool MasungPrinterWorker::feedLine(int iLines)
{
    // return (0 == MASUNG_API(PrintChangeRow)());
    return (0 == MASUNG_API(PrintFeedline)(iLines));
}

bool MasungPrinterWorker::executeCommand(XFSIoTCommandEvent *pCommandEvent)
{
    if (DeviceWorker::executeCommand(pCommandEvent)) {
        return true;
    } else {
        QString l_strCommandName = pCommandEvent->commandName();
        if (l_strCommandName == "Printer.PrintForm") {
            doPrintForm(pCommandEvent);
            return true;
        } else if (l_strCommandName == "Printer.Reset") {
            doReset(pCommandEvent);
            return true;
        } else if (l_strCommandName == "Common.Status") {
            doCommonStatus(pCommandEvent);
            return true;
        } else {
            service()->notifyCompletion(pCommandEvent, //
                                        XFSIoTStandard::JV_UNSUPPORTED_COMMAND, //
                                        QString("Device can not DO command [%1]").arg(l_strCommandName));
            return false;
        }
    }
}

void MasungPrinterWorker::doPrintForm(XFSIoTCommandEvent *pCommandEvent)
{
    QString l_strFormName = pCommandEvent->payLoad()["formName"].toString();
    const XFSForm *l_pForm = ((PrinterService *)service())->form(l_strFormName);

    BMPPrinter *l_pBMPPrinter = new BMPPrinter(l_pForm->unit(), l_pForm->size(), this);

    const QMap<Position, XFSField *> &field = l_pForm->fieldMapByPosition();
    const QJsonObject &l_joFieldsPayload = pCommandEvent->payLoad()["fields"].toObject();
    QSet<QString> l_setFieldsPrinted;
    for (auto itr = field.constBegin(); itr != field.constEnd(); itr++) {
        printField(*itr, l_joFieldsPayload, l_setFieldsPrinted, l_pBMPPrinter, false);
    }

    const QHash<QString, XFSFrame *> &l_refFrames = l_pForm->frames();

    for (auto itr = l_refFrames.constBegin(); itr != l_refFrames.constEnd(); itr++) {
        printFrame((*itr), l_pBMPPrinter);
    }
    l_pBMPPrinter->printToFile("outs/BMPPrinter.bmp");
    l_pBMPPrinter->deleteLater();
    // printBitmap("outs/BMPPrinter.bmp");
    // feedLine(3);

    // cutPaper();
}

void MasungPrinterWorker::doCommonStatus(XFSIoTCommandEvent *pCommandEvent)
{
    QJsonObject l_joPayload;
    updateDeviceStatus();
    commonStatus(l_joPayload);
    printerStatus(l_joPayload);

    service()->notifyCompletion(pCommandEvent, l_joPayload);
}

void MasungPrinterWorker::doReset(XFSIoTCommandEvent *pCommandEvent)
{
    if (isReady()) {
        log("Reset device");
        initDevice();
        if (ST_online == deviceStatus()) {
            service()->notifyCompletion(pCommandEvent, //
                                        XFSIoTStandard::JV_COMPLETION_CODE_success, //
                                        DEVICE_STATUS_MAP_STR.value(m_iHardwareStatus));
        } else {
            service()->notifyCompletion(pCommandEvent, //
                                        XFSIoTStandard::JV_COMPLETION_CODE_hardwareError, //
                                        DEVICE_STATUS_MAP_STR.value(m_iHardwareStatus));
        }
    } else {
        service()->notifyCompletion(pCommandEvent, //
                                    XFSIoTStandard::JV_COMPLETION_CODE_deviceNotReady, //
                                    DEVICE_STATUS_MAP_STR.value(m_iHardwareStatus));
    }
}

void MasungPrinterWorker::printField(XFSField *pField, const QJsonObject &joPayload)
{
    int l_iFieldRow = pField->position().y();
    int l_iFieldCol = pField->position().x();
    QString l_strValue;
    qDebug() << "Print field: " << pField->name() << pField->position().x() << " , " << pField->position().y();
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
    goTo(l_iFieldCol, l_iFieldRow);
    if (pField->type().is("text")) {
        printString(l_strValue);
    } else if (pField->type().is("graphic")) {
        printBitmap(l_strValue);
    } else {
        error(QString("Can not print field type [%1]").arg(pField->type().value()));
    }
}

void MasungPrinterWorker::printField(const XFSField *pField, //
                                     const QJsonObject &joFieldsPayload, //
                                     QSet<QString> &setPrintedList, //
                                     BMPPrinter *pBMPPrinter, //
                                     bool isFollows)
{
    if (setPrintedList.contains(pField->name())) {
        debug(QString("Field[%1] was printed").arg(pField->name()));
    } else {
        if (isFollows || pField->previousFollows() == nullptr) {
            printField2BMPPrinter(pField, joFieldsPayload, pBMPPrinter, isFollows);
            setPrintedList.insert(pField->name());
            if (pField->nextFollows() != nullptr) {
                printField(pField->nextFollows(), joFieldsPayload, setPrintedList, pBMPPrinter, true);
            }
        } else {
            debug(QString("Field[%1] Followed By Other Field").arg(pField->name()));
        }
    }
}

void MasungPrinterWorker::printField(XFSField *pField, const QString &strValue)
{
    goTo(pField->position().x(), pField->position().y());
    if (pField->type().is("text")) {
        printString(strValue);
    } else if (pField->type().is("graphic")) {
        printBitmap(strValue);
    } else {
        error(QString("Can not print field type [%1]").arg(pField->type().value()));
    }
}

void MasungPrinterWorker::printField(XFSField *pField, const QString &strValue, BMPPrinter *pBMPPrinter)
{
    pBMPPrinter->printField(pField, strValue);
}

void MasungPrinterWorker::printFrame(XFSFrame *pFrame, BMPPrinter *pBMPPrinter)
{
    pBMPPrinter->goTo(pFrame->position().x(), pFrame->position().y());
    pBMPPrinter->printFrame(pFrame->size().qSize());
}

void MasungPrinterWorker::updateDeviceInfo()
{
    char l_buff[128];
    int l_iLen = 0;
    if (MS_SUCCESS == MASUNG_API(GetProductinformation)(MS_INFO_MODEL, l_buff, l_iLen)) {
        m_strModel = QString(l_buff);
        log(QString("Printer Model: [%1]").arg(m_strModel));
    }
    if (MS_SUCCESS == MASUNG_API(GetProductinformation)(MS_INFO_MANUFACTURER, l_buff, l_iLen)) {
        m_strManufacturer = QString(l_buff);
        log(QString("Printer Manufacturer: [%1]").arg(m_strManufacturer));
    }
    if (MS_SUCCESS == MASUNG_API(GetProductinformation)(MS_INFO_FIRMWARE_VERSION, l_buff, l_iLen)) {
        m_strFirmwareVersion = QString(l_buff);
        log(QString("Firmware Version: [%1]").arg(m_strFirmwareVersion));
    }
}

void MasungPrinterWorker::updateDeviceStatus()
{
    debug("Update device status");
    m_iHardwareStatus = MASUNG_API(GetStatus)();
    setDeviceStatus(DEVICE_STATUS_MAP.value(m_iHardwareStatus, ST_offline));
}

void MasungPrinterWorker::cutPaper()
{
    if (MS_SUCCESS == MASUNG_API(PrintCutpaper)(0)) {
        debug("Cutting paper success");
    } else {
        error("Cutting paper ERROR");
    }
}

bool MasungPrinterWorker::printBitmap(const QString &filePath)
{
    return (MS_SUCCESS == MASUNG_API(PrintDiskbmpfile)(filePath.toUtf8().constData()));
}

void MasungPrinterWorker::initDevice()
{
    log("Init device ......................");
    int l_iResult = MASUNG_API(SetInit)();
    if (l_iResult == MS_SUCCESS) {
        log("Init device success");
        updateDeviceInfo();
        updateDeviceStatus();
        setReady(true);
    } else {
        log(QString("Init device ERROR [%1]").arg(l_iResult));
        setReady(false);
    }
}

void MasungPrinterWorker::idleProcess()
{
    debug("Idle processing ...");
    updateDeviceStatus();
    if (deviceStatus() == ST_offline) {
        initDevice();
    }
}
